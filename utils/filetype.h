#ifndef UTILS_FILETYPE
#define UTILS_FILETYPE

typedef enum FileType {
    TEXT, //
    IMGE, //
    WAVE, //
    FILE_TYPE_COUNT
} FileType;

typedef struct {
    FileType type;
    const char *extension;
    int (*process_func)(const char *);
} FileTypeEntry;

#endif
