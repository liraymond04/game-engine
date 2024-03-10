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

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include "windows/getopt.h"
#else
#include <getopt.h>
#endif

#define DEFAULT_OUTPUT_DIR "assets"

int main(int argc, char *argv[]) {
    int opt;
    char *output_directory = NULL;
    char *input_file = NULL;

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

    // Set default output directory if not provided
    if (output_directory == NULL) {
        output_directory = DEFAULT_OUTPUT_DIR;
    }

    // Your logic goes here
    printf("Output directory: %s\n", output_directory);
    printf("Input file: %s\n", input_file);

    void *data = NULL; // Store RRES_DATA_RAW loaded data
    char *text = NULL; // Store RRES_DATA_TEXT loaded data
    Texture2D texture = {
        0
    }; // Store RRES_DATA_IMAGE loaded data -> LoadTextureFromImage()
    Sound sound = {
        0
    }; // Store RRES_DATA_WAVE loaded data -> LoadSoundFromWave()
    Font font = { 0 }; // Store RRES_DATA_FONT_GLYPHS + RRES_DATA_IMAGE

    // Load content from rres file
    rresResourceChunk chunk = { 0 }; // Single resource chunk
    rresResourceMulti multi = { 0 }; // Multiple resource chunks

    int result = 0; // Result of data unpacking

    //------------------------------------------------------------------------------------------------------

    return 0;
}
