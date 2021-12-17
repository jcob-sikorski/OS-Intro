#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>

char* dectobin(int n) {
    char* res = (char*) malloc(32);
    int k;
    for (int c = 31; c >= 0; c--) {
        k = n >> c;

        if (k & 1) {
            res[32-c-1] = '1';
        }
        else {
            res[32-c-1] = '0';
        }
    }
    return res;
}

int main(int argc, char const *argv[])
{
    if (argc == 1) {
        fprintf(stderr, "wis-tar: tar-file file […]\n");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 2; i < argc; i++) {
        char *path = realpath(argv[i], NULL);

        if (path == NULL) {
            fprintf(stderr, "wis-tar: cannot open file\n");
            exit(EXIT_FAILURE);
        }
    }
    FILE *dst_file = fopen(argv[1], "wb");

    if(dst_file == NULL) {
        fprintf(stderr, "File creation failed.\n");
        exit(1);
    }
    for (size_t i = 2; i < argc; i++) {
        struct stat src_info;

        char *path;
        memcpy(&path, &argv[i], strlen(argv[i]));

        stat(path, &src_info);

        FILE *stream = fopen(path, "r");

        char serialized_path[100];
        int path_len = strlen(path);

        if (path_len > 100) {
            strncpy(serialized_path, path, 100);
        }
        else {
            for (int j = 0; j < path_len; j++) {
                fputc(path[j], dst_file);
            }

            for (int j = path_len; j < 100; j++) {
                fputc('\0', dst_file);
            }
        }
        char *file_size_bin = (char*) malloc(31);
        file_size_bin = dectobin(src_info.st_size);

        fprintf(dst_file, file_size_bin);
        
        char *buffer = (char*) malloc(src_info.st_size);
        fread(buffer, src_info.st_size, 1, stream);

        fprintf(dst_file, buffer);

        /* free(buffer); */
        fclose(stream);
    }
    fclose(dst_file);
    return 0;
}
