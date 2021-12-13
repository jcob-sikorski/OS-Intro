#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<err.h>

/* Finds lines in source (file or stdin) containing 
given search pattern and prints them out. */

char *concat(const char *dst, const char *src)
{
    int dst_len = strlen(dst);
    int src_len = strlen(src);
    int len = dst_len + src_len;
    char *buffer = new char[len+1];
    memcpy(buffer, dst, dst_len);
    memcpy(buffer+dst_len, src, src_len);
    buffer[len] = 0;
    return buffer;
}


int main(int argc, char const *argv[])
{
    /* [0] /. wisgrep [1] searchterm [2] path_to_file */
    if (argc == 3) {
        FILE *file;
        char *line = NULL;
        size_t line_cappacity = 0;
        ssize_t read;

        file = fopen(argv[2], "r");
        if (file == NULL) {
            printf("wis-grep: cannot open file");
            exit(EXIT_FAILURE);
        }

        while ((read = getline(&line, &line_cappacity, file)) != -1) {
            if(strstr(line, argv[1]) != NULL) {
                printf("%s", line);
            }
        }
        fclose(file);

        if (line) {
            free(line);
        }
    }
    /* [0] /. wisgrep [1] searchterm */
    else if (argc == 2) {
        char *result = "";
        char *line = NULL;
        size_t line_cappacity = 0;
        ssize_t read;

        while ((read = getline(&line, &line_cappacity, stdin)) != -1) {
            if(strstr(line, argv[1]) != NULL) {
                result = concat(result, line);
            }
        }
        fclose(stdin);

        printf("%s", result);

        if (result) {
            free(result);
        }
        if (line) {
            free(line);
        }
    }
    /* [0] /. wisgrep */
    else if (argc == 1) {
        printf("wis-grep: searchterm [file …]\n");
        exit(EXIT_FAILURE);
    }
    return 0;
} 