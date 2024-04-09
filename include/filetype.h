#ifndef UTILS_FILETYPE
#define UTILS_FILETYPE

typedef enum FileType {
    FILETYPE_TEXT, //
    FILETYPE_IMGE, //
    FILETYPE_WAVE, //
    FILETYPE_COUNT
} FileType;

typedef struct {
    FileType type;
    const char *extension;
    int (*process_func)(const char *);
} FileTypeEntry;

#endif
