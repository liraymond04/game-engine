#include "raylib.h"

#define RRES_IMPLEMENTATION
#include "rres.h"

#include <json-c/json.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#if defined(_WIN32)
#define NOGDI  // All GDI defines and routines
#define NOUSER // All USER defines and routines
#endif
#include "windows/dirent.h"
#include "windows/getopt.h"
#if defined(_WIN32) // raylib uses these names as function parameters
#undef near
#undef far
#endif
#else
#include <dirent.h>
#include <getopt.h>
#endif

#include <limits.h>

#include "filetype.h"

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#define DEFAULT_OUTPUT_FILE "data.rres"
#define DEFAULT_OUTPUT_JSON_FILE "data.rres.json"

static int PackTEXT(const char *filepath);
static int PackIMGE(const char *filepath);
static int PackWAVE(const char *filepath);

const FileTypeEntry file_types[] = {
    { FILETYPE_IMGE, ".png", PackIMGE },  //
    { FILETYPE_IMGE, ".jpg", PackIMGE },  //
    { FILETYPE_IMGE, ".jpeg", PackIMGE }, //
    { FILETYPE_TEXT, ".txt", PackTEXT },  //
    { FILETYPE_TEXT, ".wav", PackWAVE },  //
    // Add more file types as needed
};

json_object *json_root;

FILE *rres_file;

rresResourceChunkInfo chunk_info = { 0 }; // Chunk info
rresResourceChunkData chunk_data = { 0 }; // Chunk data

unsigned char *buffer = NULL;

static unsigned char *LoadDataBuffer(rresResourceChunkData data,
                                     unsigned int raw_size);
static void UnloadDataBuffer(unsigned char *buffer);

static int PackTEXT(const char *filepath) {
    char *text = LoadFileText(filepath);
    unsigned int raw_size = strlen(text);

    // Define chunk info: TEXT
    chunk_info.type[0] = 'T'; // Resource chunk type (FourCC)
    chunk_info.type[1] = 'E'; // Resource chunk type (FourCC)
    chunk_info.type[2] = 'X'; // Resource chunk type (FourCC)
    chunk_info.type[3] = 'T'; // Resource chunk type (FourCC)

    // Resource chunk identifier (generated from filename CRC32 hash)
    chunk_info.id =
        rresComputeCRC32((unsigned char *)filepath, strlen(filepath));
    int id = chunk_info.id;

    chunk_info.compType = RRES_COMP_NONE,         // Data compression algorithm
        chunk_info.cipherType = RRES_CIPHER_NONE, // Data encription algorithm
        chunk_info.flags = 0,                     // Data flags (if required)
        chunk_info.baseSize =
            5 * sizeof(unsigned int) +
            raw_size; // Data base size (uncompressed/unencrypted)
    chunk_info.packedSize =
        chunk_info.baseSize;   // Data chunk size (compressed/encrypted + custom
                               // data appended)
    chunk_info.nextOffset = 0, // Next resource chunk global offset (if resource
                               // has multiple chunks)
        chunk_info.reserved = 0, // <reserved>

        // Define chunk data: TEXT
        chunk_data.propCount = 4;
    chunk_data.props =
        (unsigned int *)RRES_CALLOC(chunk_data.propCount, sizeof(unsigned int));
    chunk_data.props[0] = raw_size; // props[0]:size (bytes)
    chunk_data.props[1] =
        RRES_TEXT_ENCODING_UNDEFINED;               // props[1]:rresTextEncoding
    chunk_data.props[2] = RRES_CODE_LANG_UNDEFINED; // props[2]:rresCodeLang
    chunk_data.props[3] =
        0x0409; // props[3]:cultureCode: en-US: English - United States
    chunk_data.raw = text;

    // Get a continuous data buffer from chunk_data
    buffer = LoadDataBuffer(chunk_data, raw_size);

    // Compute data chunk CRC32 (propCount + props[] + data)
    chunk_info.crc32 = rresComputeCRC32(buffer, chunk_info.packedSize);

    // Write resource chunk into rres file
    fwrite(&chunk_info, sizeof(rresResourceChunkInfo), 1, rres_file);
    fwrite(buffer, 1, chunk_info.packedSize, rres_file);

    // Free required memory
    memset(&chunk_info, 0, sizeof(rresResourceChunkInfo));
    RRES_FREE(chunk_data.props);
    UnloadDataBuffer(buffer);
    UnloadFileText(text);
    return id;
}

static int PackIMGE(const char *filepath) {
    Image image = LoadImage(filepath);
    unsigned int raw_size =
        GetPixelDataSize(image.width, image.height, image.format);

    // Define chunk info: IMGE
    chunk_info.type[0] = 'I'; // Resource chunk type (FourCC)
    chunk_info.type[1] = 'M'; // Resource chunk type (FourCC)
    chunk_info.type[2] = 'G'; // Resource chunk type (FourCC)
    chunk_info.type[3] = 'E'; // Resource chunk type (FourCC)

    // Resource chunk identifier (generated from filename CRC32 hash)
    chunk_info.id =
        rresComputeCRC32((unsigned char *)filepath, strlen(filepath));
    int id = chunk_info.id;

    chunk_info.compType = RRES_COMP_NONE,         // Data compression algorithm
        chunk_info.cipherType = RRES_CIPHER_NONE, // Data encription algorithm
        chunk_info.flags = 0,                     // Data flags (if required)
        chunk_info.baseSize =
            5 * sizeof(unsigned int) +
            raw_size; // Data base size (uncompressed/unencrypted)
    chunk_info.packedSize =
        chunk_info.baseSize;   // Data chunk size (compressed/encrypted + custom
                               // data appended)
    chunk_info.nextOffset = 0, // Next resource chunk global offset (if resource
                               // has multiple chunks)
        chunk_info.reserved = 0, // <reserved>

        // Define chunk data: IMGE
        chunk_data.propCount = 4;
    chunk_data.props =
        (unsigned int *)RRES_CALLOC(chunk_data.propCount, sizeof(unsigned int));
    chunk_data.props[0] = image.width;  // props[0]:width
    chunk_data.props[1] = image.height; // props[1]:height
    chunk_data.props[2] =
        image.format; // props[2]:rresPixelFormat
                      // NOTE: rresPixelFormat matches raylib PixelFormat enum,
    chunk_data.props[3] = image.mipmaps; // props[3]:mipmaps
    chunk_data.raw = image.data;

    // Get a continuous data buffer from chunk_data
    buffer = LoadDataBuffer(chunk_data, raw_size);

    // Compute data chunk CRC32 (propCount + props[] + data)
    chunk_info.crc32 = rresComputeCRC32(buffer, chunk_info.packedSize);

    // Write resource chunk into rres file
    fwrite(&chunk_info, sizeof(rresResourceChunkInfo), 1, rres_file);
    fwrite(buffer, 1, chunk_info.packedSize, rres_file);

    // Free required memory
    memset(&chunk_info, 0, sizeof(rresResourceChunkInfo));
    RRES_FREE(chunk_data.props);
    UnloadDataBuffer(buffer);
    UnloadImage(image);
    return id;
}

static int PackWAVE(const char *filepath) {
    Wave wave = LoadWave(filepath);
    unsigned int raw_size =
        wave.frameCount * wave.channels * (wave.sampleSize / 8);

    // Define chunk info: WAVE
    chunk_info.type[0] = 'W'; // Resource chunk type (FourCC)
    chunk_info.type[1] = 'A'; // Resource chunk type (FourCC)
    chunk_info.type[2] = 'V'; // Resource chunk type (FourCC)
    chunk_info.type[3] = 'E'; // Resource chunk type (FourCC)

    // Resource chunk identifier (generated from filename CRC32 hash)
    chunk_info.id =
        rresComputeCRC32((unsigned char *)filepath, strlen(filepath));
    int id = chunk_info.id;

    chunk_info.compType = RRES_COMP_NONE,         // Data compression algorithm
        chunk_info.cipherType = RRES_CIPHER_NONE, // Data encription algorithm
        chunk_info.flags = 0,                     // Data flags (if required)
        chunk_info.baseSize =
            5 * sizeof(unsigned int) +
            raw_size; // Data base size (uncompressed/unencrypted)
    chunk_info.packedSize =
        chunk_info.baseSize;   // Data chunk size (compressed/encrypted + custom
                               // data appended)
    chunk_info.nextOffset = 0, // Next resource chunk global offset (if resource
                               // has multiple chunks)
        chunk_info.reserved = 0, // <reserved>

        // Define chunk data: WAVE
        chunk_data.propCount = 4;
    chunk_data.props =
        (unsigned int *)RRES_CALLOC(chunk_data.propCount, sizeof(unsigned int));
    chunk_data.props[0] = wave.frameCount; // props[0]:frameCount
    chunk_data.props[1] = wave.sampleRate; // props[1]:sampleRate
    chunk_data.props[2] = wave.sampleSize; // props[2]:sampleSize
    chunk_data.props[3] = wave.channels;   // props[3]:channels
    chunk_data.raw = wave.data;

    // Get a continuous data buffer from chunk_data
    buffer = LoadDataBuffer(chunk_data, raw_size);

    // Compute data chunk CRC32 (propCount + props[] + data)
    chunk_info.crc32 = rresComputeCRC32(buffer, chunk_info.packedSize);

    // Write resource chunk into rres file
    fwrite(&chunk_info, sizeof(rresResourceChunkInfo), 1, rres_file);
    fwrite(buffer, 1, chunk_info.packedSize, rres_file);

    // Free required memory
    memset(&chunk_info, 0, sizeof(rresResourceChunkInfo));
    RRES_FREE(chunk_data.props);
    UnloadDataBuffer(buffer);
    UnloadWave(wave);
    return id;
}

const char *get_dirname(const char *path) {
    static char dirname[PATH_MAX]; // Assuming PATH_MAX is defined
    strcpy(dirname, path);

    char *last_slash = strrchr(dirname, '/');
    char *last_backslash = strrchr(dirname, '\\');

    char *last_separator = NULL;
    if (last_slash != NULL && last_backslash != NULL) {
        last_separator =
            (last_slash > last_backslash) ? last_slash : last_backslash;
    } else if (last_slash != NULL) {
        last_separator = last_slash;
    } else if (last_backslash != NULL) {
        last_separator = last_backslash;
    }

    if (last_separator != NULL) {
        *last_separator = '\0';
    }

    return dirname;
}

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

// Function to create objects in the JSON structure based on the path
static json_object *create_json_objects(json_object *root, const char *path) {
    char *path_copy = strdup(path); // Duplicate the path for tokenization
    char *token = strtok(path_copy, "/");
    json_object *current_obj = root; // Start from the root object

    while (token != NULL) {
        json_object *obj = NULL;
        if (!json_object_object_get_ex(current_obj, token, &obj)) {
            // If the object doesn't exist, create a new one
            obj = json_object_new_object();
            json_object_object_add(current_obj, token, obj);
        }
        current_obj = obj; // Move to the next level
        token = strtok(NULL, "/");
    }

    free(path_copy); // Free the duplicated path
    return current_obj;
}

static void RecurseDirectory(const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;

    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        char fullpath[PATH_MAX];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        if (stat(fullpath, &statbuf) == -1) {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        if (S_ISDIR(statbuf.st_mode)) {
            // Skip "." and ".." directories
            if (strcmp(entry->d_name, ".") == 0 ||
                strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            // Recursively list files in subdirectory
            RecurseDirectory(fullpath);
        } else {
            const char *ext = strrchr(fullpath, '.');
            if (ext != NULL) {
                for (int i = 0; i < sizeof(file_types) / sizeof(FileTypeEntry);
                     i++) {
                    if (strcmp(ext, file_types[i].extension) == 0) {
                        // Pack into rres file
                        int id = file_types[i].process_func(fullpath);

                        // Add entry to data JSON
                        json_object *last_object = create_json_objects(
                            json_root, get_dirname(fullpath));
                        json_object *result = json_object_new_array();
                        json_object_array_add(
                            result, json_object_new_int(file_types[i].type));
                        json_object_array_add(result, json_object_new_int(id));
                        json_object_object_add(last_object,
                                               get_filename(fullpath), result);
                        break;
                    }
                }
            } else {
                // If the file type is not found, handle it here
                printf("Unknown file type: %s\n", fullpath);
            }
        }
    }

    closedir(dir);
}

// Load a continuous data buffer from rresResourceChunkData struct
static unsigned char *LoadDataBuffer(rresResourceChunkData data,
                                     unsigned int raw_size) {
    unsigned char *buffer = (unsigned char *)RRES_CALLOC(
        (data.propCount + 1) * sizeof(unsigned int) + raw_size, 1);

    memcpy(buffer, &data.propCount, sizeof(unsigned int));
    for (int i = 0; i < data.propCount; i++)
        memcpy(buffer + (i + 1) * sizeof(unsigned int), &data.props[i],
               sizeof(unsigned int));
    memcpy(buffer + (data.propCount + 1) * sizeof(unsigned int), data.raw,
           raw_size);

    return buffer;
}

// Unload data buffer
static void UnloadDataBuffer(unsigned char *buffer) { RRES_FREE(buffer); }

int main(int argc, char *argv[]) {
    int opt;
    char *input_directory = NULL;
    char *output_file = NULL;
    char output_json_file[PATH_MAX];
    bool json_prettify = false;

    const char *help_format_str =
        "Usage: %s [-o output_file] [--json-prettify] input_directory\n";

    // Parse command-line options
    while ((opt = getopt(argc, argv, "o:-:h")) != -1) {
        switch (opt) {
        case 'o':
            output_file = optarg;
            strcpy(output_json_file, output_file);
            strcat(output_json_file, ".json");
            break;
        case '-':
            if (strcmp(optarg, "help") == 0) {
                fprintf(stdout, help_format_str, argv[0]);
                exit(EXIT_SUCCESS);
            }
            if (strcmp(optarg, "json-prettify") == 0) {
                json_prettify = true;
            }
            break;
        case 'h':
            fprintf(stdout, help_format_str, argv[0]);
            exit(EXIT_SUCCESS);
        default:
            fprintf(stderr, help_format_str, argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // The next argument (after the options) should be the input file
    if (optind < argc) {
        input_directory = argv[optind];
    } else {
        fprintf(stderr, "Input directory must be provided.\n");
        fprintf(stderr, help_format_str, argv[0]);
        exit(EXIT_FAILURE);
    }

    // Set default output file if not provided
    if (output_file == NULL) {
        output_file = DEFAULT_OUTPUT_FILE;
        strcpy(output_json_file, DEFAULT_OUTPUT_JSON_FILE);
    }

    // Your logic goes here
    printf("Output file: %s\n", output_file);
    printf("Input directory: %s\n", input_directory);

    rres_file = fopen(output_file, "wb");

    // Define rres file header
    // NOTE: We are loading 4 files that generate 5 resource chunks to save in
    // rres
    rresFileHeader header = {
        .id[0] = 'r',    // File identifier: rres
        .id[1] = 'r',    // File identifier: rres
        .id[2] = 'e',    // File identifier: rres
        .id[3] = 's',    // File identifier: rres
        .version = 100,  // File version: 100 for version 1.0
        .chunkCount = 5, // Number of resource chunks in the file (MAX: 65535)
        .cdOffset = 0, // Central Directory offset in file (0 if not available)
        .reserved = 0  // <reserved>
    };

    // Write rres file header
    fwrite(&header, sizeof(rresFileHeader), 1, rres_file);

    rresResourceChunkInfo chunk_info = { 0 }; // Chunk info
    rresResourceChunkData chunk_data = { 0 }; // Chunk data

    unsigned char *buffer = NULL;

    json_root = json_object_new_object();

    // recurse directory
    RecurseDirectory(input_directory);

    int json_flags = JSON_C_TO_STRING_PLAIN;

    if (json_prettify) {
        json_flags |= JSON_C_TO_STRING_SPACED;
        json_flags |= JSON_C_TO_STRING_PRETTY;
    }

    // save json
    if (json_object_to_file_ext(output_json_file, json_root, json_flags))
        printf("Error: failed to save rres IDs JSON %s.\n", output_json_file);
    else
        printf("rres IDs JSON %s saved.\n", output_json_file);

    json_object_put(json_root);
    fclose(rres_file);

    return 0;
}
