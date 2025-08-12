#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        puts("usage: xorer [key] <path to file>");
        return 0;
    }

    long  FileSize                           = 0l;
    int   PathArgIndex                       = argc == 3? 2 : 1;
    int   FilenameLength                     = strlen(argv[PathArgIndex]);
    int   MaxKeyLength                       = 16;
    int   KeyLength                          = 0;
    FILE* InputFile                          = NULL;
    FILE* OutputFile                         = NULL;
    char* buf                                = NULL;
    char* path                               = NULL;
    char  XorKey[MaxKeyLength + 2]           = {};
    char  OutputFilename[FilenameLength + 1] = {};

    path = argv[PathArgIndex];
    if ((InputFile = fopen(path, "rb")) == NULL) {
        printf("unable to open file '%s'\n", path);
        return 1;
    }

    if (argc == 3) {
        strncpy(XorKey, argv[1], MaxKeyLength);
        if (XorKey[MaxKeyLength] != '\0')
            XorKey[MaxKeyLength]  = '\0';
        KeyLength = strlen(XorKey);
    }
    else {
        fputs("enter xor key [max 16 characters]: ", stdout);
        fgets(XorKey, MaxKeyLength + 2, stdin);
        KeyLength = strlen(XorKey) - 1;
        XorKey[KeyLength] = '\0';
    }

    strncpy(OutputFilename, path, FilenameLength);
    OutputFilename[FilenameLength]     = '2';
    OutputFilename[FilenameLength + 1] = '\0';
    OutputFile = fopen(OutputFilename, "wb");

    fseek(InputFile, 0, SEEK_END);
    FileSize = ftell(InputFile);
    rewind(InputFile);
    buf = (char*)malloc(FileSize);

    for (long index = 0l, KeyIndex = 0l; index < FileSize; index++) {
        buf[index] = fgetc(InputFile);
        fprintf(OutputFile, "%c", buf[index] ^ XorKey[KeyIndex++]);
        if (KeyIndex >= KeyLength)
            KeyIndex = 0;
    }

    fclose(InputFile);
    fclose(OutputFile);
    free(buf);
    return 0;
}
