#include "raylib.h"

#define RRES_IMPLEMENTATION
#include "rres.h"

#define RRES_RAYLIB_IMPLEMENTATION
#define RRES_SUPPORT_COMPRESSION_LZ4
#define RRES_SUPPORT_ENCRYPTION_AES
#define RRES_SUPPORT_ENCRYPTION_XCHACHA20
#include "rres-raylib.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include <json-c/json.h>

#include "filetype.h"

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include "windows/getopt.h"
#else
#include <getopt.h>
#endif

#define DEFAULT_OUTPUT_DIR "assets"

const char *get_filename(const char *path) {
    const char *filename = strrchr(path, '/'); // For Unix-like paths
    if (!filename) {
        filename = strrchr(path, '\\'); // For Windows paths
    }
    if (!filename) {
        filename = path; // If no directory separator found, use the whole path
                         // as filename
    } else {
        filename++; // Move past the directory separator
    }
    return filename;
}

void make_directory(const char *name) {
#ifdef __linux__
    mkdir(name, 777);
    if (chmod(name, 0777) != 0) {
        printf("Failed to set permissions for directory: %s\n", name);
    }
#else
    _mkdir(name);
#endif
}

char *text = NULL; // Store RRES_DATA_TEXT loaded data

// Load content from rres file
rresResourceChunk chunk = { 0 }; // Single resource chunk
rresResourceMulti multi = { 0 }; // Multiple resource chunks

int result = 0; // Result of data unpacking

void DumpTEXT(const char *rres_file, const char *filename, const char *fullpath,
              int resource_id) {
    chunk = rresLoadResourceChunk(rres_file, resource_id);
    result = UnpackResourceChunk(
        &chunk); // Decompres/decipher resource data (if required)

    if (result == 0) // Data decompressed/decrypted successfully
    {
        text = LoadTextFromResource(
            chunk); // Load text data, must be freed at the end
        char filepath[PATH_MAX];
        snprintf(filepath, sizeof(filepath), "%s/%s", fullpath, filename);
        SaveFileText(filepath, text);
        free(text);
    }

    rresUnloadResourceChunk(chunk);
}

void DumpIMGE(const char *rres_file, const char *filename, const char *fullpath,
              int resource_id) {
    chunk = rresLoadResourceChunk(rres_file, resource_id);
    result = UnpackResourceChunk(
        &chunk); // Decompres/decipher resource data (if required)

    if (result == 0) // Data decompressed/decrypted successfully
    {
        Image image = LoadImageFromResource(chunk);
        if (image.data != NULL) {
            char filepath[PATH_MAX];
            snprintf(filepath, sizeof(filepath), "%s/%s", fullpath, filename);
            ExportImage(image, filepath);
            UnloadImage(image);
        }
    }

    rresUnloadResourceChunk(chunk);
}

void DumpWAVE(const char *rres_file, const char *filename, const char *fullpath,
              int resource_id) {
    chunk = rresLoadResourceChunk(rres_file, resource_id);
    result = UnpackResourceChunk(
        &chunk); // Decompres/decipher resource data (if required)

    if (result == 0) // Data decompressed/decrypted successfully
    {
        Wave wave = LoadWaveFromResource(chunk);
        char filepath[PATH_MAX];
        snprintf(filepath, sizeof(filepath), "%s/%s", fullpath, filename);
        // TODO - more file formats
        // Raylib only supports exporting .wav, .qoa, and .raw files
        // Possible to extend raudio to support exporting wave data 
        // to .ogg, .mp3, and .flac (which can already be loaded)
        ExportWave(wave, filepath);
        UnloadWave(wave);
    }

    rresUnloadResourceChunk(chunk);
}

void DumpFile(const char *rres_file, const char *filename, const char *fullpath,
              int type, int resource_id) {
    switch (type) {
    case TEXT:
        DumpTEXT(rres_file, filename, fullpath, resource_id);
        break;
    case IMGE:
        DumpIMGE(rres_file, filename, fullpath, resource_id);
        break;
    case WAVE:
        DumpWAVE(rres_file, filename, fullpath, resource_id);
        break;
    default:
        fprintf(stderr, "Invalid file type provided.\n");
    }
}

char *input_file = NULL;

void doit(json_object *obj, const char *path) {
    char fullpath[PATH_MAX];
    json_object_object_foreach(obj, key, val) {
        switch (json_object_get_type(val)) {
        case json_type_array:
            // File dump
            DumpFile(input_file, key, path,
                     json_object_get_int(json_object_array_get_idx(val, 0)),
                     json_object_get_int(json_object_array_get_idx(val, 1)));
            break;
        case json_type_object:
            // Make directory if it doesn't exist, and continue recurse
            snprintf(fullpath, sizeof(fullpath), "%s/%s", path, key);
            make_directory(fullpath);
            doit(val, fullpath);
            break;
        default:
            printf("%s: %s\n", key, json_object_get_string(val));
        }
    }
}

const char *get_file_extension(const char *filename) {
    const char *dot = strrchr(filename, '.'); // Find the last occurrence of '.'
    if (dot && dot != filename) { // Ensure '.' is not the first character
        return dot + 1;           // Return the substring after '.'
    }
    return ""; // No extension found
}

void remove_last_forward_slash(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '/') {
        str[len - 1] = '\0'; // Replace the last character with null terminator
    }
}

int main(int argc, char *argv[]) {
    int opt;
    char *output_directory = NULL;

    // Parse command-line options
    while ((opt = getopt(argc, argv, "o:")) != -1) {
        switch (opt) {
        case 'o':
            output_directory = optarg;
            break;
        default:
            fprintf(stderr, "Usage: %s [-o output_directory] input_file\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // The next argument (after the options) should be the input file
    if (optind < argc) {
        input_file = argv[optind];
    } else {
        fprintf(stderr, "Input file must be provided.\n");
        fprintf(stderr, "Usage: %s [-o output_directory] input_file\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(get_file_extension(input_file), "rres") != 0) {
        fprintf(stderr, "Input file must a .rres file.\n");
        exit(EXIT_FAILURE);
    }

    // Set default output directory if not provided
    if (output_directory == NULL) {
        output_directory = DEFAULT_OUTPUT_DIR;
    }
    remove_last_forward_slash(output_directory);

    // Your logic goes here
    printf("Output directory: %s\n", output_directory);
    printf("Input file: %s\n", input_file);

    // Read data JSON
    const char *filename = get_filename(input_file);
    char json_file[PATH_MAX];
    sprintf(json_file, "%s.json", filename);
    json_object *json_root = json_object_from_file(json_file);

    // Get root directory key name in JSON object
    char root_name[PATH_MAX];
    json_object *original_root;
    json_object_object_foreach(json_root, key, val) {
        bool done = false;

        switch (json_object_get_type(val)) {
        case json_type_object:
            strcpy(root_name, key);
            original_root = val;
            done = true;
            break;
        default:
            fprintf(stderr, "Malformed data JSON.\n");
            exit(EXIT_FAILURE);
        }

        if (done)
            break;
    }

    // Change key name if output_directory is specified
    if (output_directory != NULL) {
        json_object *copy_root;
        json_object_deep_copy(original_root, &copy_root, NULL);
        json_object_object_del(json_root, root_name);
        json_object_object_add(json_root, output_directory, copy_root);
    }

    // Start recursive dump
    doit(json_root, ".");

    //------------------------------------------------------------------------------------------------------

    return 0;
}
