#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

void rand_str(char *dest, size_t length) {
    char charset[] = "0123456789"
                     "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (length-- > 0) {
        size_t index = (double) rand() / RAND_MAX * (sizeof charset - 1);
        *dest++ = charset[index];
    }
    *dest = '\0';
}

int bintodec(const char *s) {
    return (int) strtol(s, NULL, 2);
}

int main(int argc, char const *argv[]) {
    if (argc == 1) {
        fprintf(stderr, "wis-untar: untar-file file […]\n");
        exit(EXIT_FAILURE);
    }
    FILE *tar_file = fopen(argv[1], "r+");

    if (tar_file == NULL) {
        fprintf(stderr, "Can't open a file.\n");
        exit(1);
    }
    char path_to_tar[100];
    while (fread(path_to_tar, 100, 1, tar_file) != NULL) {
        char *buffer_size_in_char = (char*) malloc(31);
        /* char c;
        for(int i=0; i < 32 && fread(&c,1,1,tar_file) > 0 ; i++) {
            buffer_size_in_char[i] = c;
        } */
        fread(buffer_size_in_char, sizeof(char), 32, tar_file);

        const char **err;
        int buffer_size = bintodec(buffer_size_in_char);

        char *buffer = (char*) malloc(buffer_size);
        fread(buffer, buffer_size, 1, tar_file);
        fprintf(stderr, buffer); /* TODO problems with reading next number and buffer */

        char filename[100];
        rand_str(filename, 100);

        FILE *output = fopen(filename, "wb");
        fprintf(output, buffer);

        memset(path_to_tar, 0, sizeof(path_to_tar));
        fclose(output);
    }
    return 0;
}