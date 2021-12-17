#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>

char *readCommandLine(FILE * f)
{
    size_t size = 0;
    size_t len  = 0;
    size_t last = 0;
    char *buf  = NULL;

    do {
        size += BUFSIZ;
        buf = (char*) realloc(buf, size);
        fgets(buf+last,size,f);
        len = strlen(buf);
        last = len - 1;
    } while (!feof(f) && buf[last]!='\n');
    return buf;
}

void executeCommand(char *path, char** args) {
    pid_t   pid;
    int     status;

    if ((pid = fork()) < 0) {
        fprintf(stderr, "fork error");
    } 
    else if (pid == 0) { // child
        execv(path, args);
        kill(getpid(), SIGKILL);
    }
    // parent
    waitpid(pid, &status, 0);
}

void redirectOuput(char *progname, char** args, char* dst) {
    FILE *stream, *dst_file;
    char *buf = (char*) malloc(BUFSIZ);
    char *action = (char*) malloc(BUFSIZ);

    strcpy(action, progname);
    strcat(action, " ");
    
    for (int i = 1; i < strlen(*args); i++) {
        if (args[i] == NULL) {
            break;
        }
        strcat(action, args[i]);
        strcat(action, " ");
    }

    // Attach to stream
    stream = popen(action, "r");
    if (stream == NULL) {
        printf("Failed to run action\n" );
        exit(1);
    }
    dst_file = fopen(dst, "w");

    // For each line ouput it to dst_file
    while (fgets(buf, sizeof(buf), stream) != NULL) {
        fprintf(dst_file, buf);
    }
    pclose(stream);
    fclose(dst_file);
}

int main(int argc, char const *argv[]) {
    if (argc == 1) {
        for (;;) { // ask usr for input forever
            fprintf(stderr, "smash> ");
            char *input = readCommandLine(stdin);

            char *expression = (char*) malloc(BUFSIZ);
            int ei = 0;
            // parse input
            // ;
            for (input; *input != '\0'; input++) {
                char c = input[0];
                if (';' == input[0] || 
                    '\n' == input[0] || 
                    '\0' == input[0]) {

                    char *action = (char*) malloc(BUFSIZ);
                    int ai = 0;
                    // parse input
                    // &
                    for (expression; ; expression++) {
                        char c = expression[0];
                        if ('&' == expression[0] || 
                            ';' == expression[0] || 
                            '\n' == expression[0] || 
                            '\0' == expression[0]) {

                            // first word in action is command
                            // next are flags divided by ' ' or '>'
                            char *output = strchr(action, '>'); // find output else NULL
                            if (output != NULL) {
                                output = strdup(output); // copy string
                                output = strtok(output, " |>"); // extract output name
                            }
                            char *progname = strtok(action, " |>");
                            char** args = (char**) malloc(BUFSIZ);

                            if (progname == NULL) { // if user did not write anything exit
                                exit(EXIT_SUCCESS);
                            }

                            args[0] = (char*) malloc(strlen(progname)*sizeof(char));
                            args[0] = progname;

                            char *arg = strtok(NULL, " "); // get args
                            int ei = 1;                    // 
                            while (arg != NULL &&          //
                                   strcmp(">", arg) != 0 && 
                                   strcmp("&", arg) != 0 && 
                                   strcmp(";", arg) != 0 && 
                                   strcmp("\n", arg) != 0 && 
                                   strcmp("\0", arg) != 0) {

                                args[ei] = (char*) malloc(strlen(arg)*sizeof(char));
                                args[ei] = arg;
                                ei++;
                                arg = strtok(NULL, " ");
                            }
 
                            ai = 0;

                            FILE * PATH_FILE;
                            char * line = NULL;
                            size_t len = 0;
                            ssize_t n_chars;

                            PATH_FILE = fopen("path", "r");
                            if (PATH_FILE == NULL)
                                exit(EXIT_FAILURE);

                            while ((n_chars = getline(&line, &len, PATH_FILE)) != -1) {
                                if (strstr(line, progname) != NULL) { // search for progname in line
                                    if (line[strlen(line)-1] == '\n') {
                                        line[strlen(line)-1] = '\0'; // realpath can't accept whitespace so modify line
                                    }
                                    char *exec_path = realpath(line, NULL);

                                    if (exec_path != NULL) { // if exec_path really exists
                                        if (output != NULL) {
                                            redirectOuput(exec_path, args, output);
                                        }
                                        else {
                                            executeCommand(exec_path, args);
                                        }
                                        fprintf(stderr, "\n");
                                    }
                                    break;
                                }
                            }
                            fclose(PATH_FILE);

                            if (strcmp(args[0], "exit") == 0) {
                                n_chars = 0;
                                if (strlen(*args) != 1) {
                                    exit(EXIT_FAILURE);
                                }
                                exit(EXIT_SUCCESS);
                            }
                            else if (strcmp(args[0], "cd") == 0) {
                                n_chars = 0;
                                if (strlen(*args) != 2 ||
                                    chdir(args[1]) == -1 // try to cd
                                ) {
                                    fprintf(stderr, "Specified %s action not found in PATH.\n", args[0]);
                                }
                            }
                            else if (strcmp(args[0], "path") == 0) {
                                n_chars = 0;
                                if (strcmp(args[1], "add") == 0) {
                                    PATH_FILE = fopen("path", "a");
                                    fprintf(PATH_FILE, args[2]);    // append to file
                                    fprintf(PATH_FILE, "\n");       //
                                    fclose(PATH_FILE);
                                }
                                else if (strcmp(args[1], "remove") == 0) {
                                    PATH_FILE = fopen("path", "r+");
                                    line = NULL;
                                    size_t line_cappacity = 0;
                                    ssize_t n_chars;
                                    int line_counter = 0;

                                    while ((n_chars = getline(&line, &line_cappacity, PATH_FILE)) != -1) {
                                        if(strstr(line, args[2]) != NULL) { // if strings equal
                                            break;
                                        }
                                        line_counter++;
                                    }
                                    fclose(PATH_FILE);                  // restart pointer to pos
                                    PATH_FILE = fopen("path", "r+");    //

                                    int counter = 0;
                                    char c;
                                    int position_in_file;

                                    while(counter != line_counter) {    // move to searched line
                                        if((c = fgetc(PATH_FILE)) == '\n') 
                                            counter++;
                                    }

                                    position_in_file = ftell(PATH_FILE);
                                    fseek(PATH_FILE, position_in_file, SEEK_SET);

                                    for (int i = 0; i < strlen(args[2]); i++) {
                                        fprintf(PATH_FILE, "\0");   // replace line with null
                                    }
                                    fclose(PATH_FILE);
                                }
                                else if (strcmp(args[1], "clear") == 0) {
                                    PATH_FILE = fopen("path", "w"); // clear entire file
                                    fclose(PATH_FILE);
                                }
                                else { // error
                                    n_chars = -1;
                                }
                            }
                            if (n_chars == -1) {
                                fprintf(stderr, "Specified %s action not found in PATH.\n", progname);
                            }

                            if (*expression == '\0') {
                                break;
                            }
                            continue;
                        }
                        action[ai] = c;
                        ai++;
                    }
                    ei = 0;
                    continue;
                }
                expression[ei] = c;
                ei++;
            }
        }
    }
    else {
        char *path = realpath(argv[1], NULL); // search for commands in batch file

        if (path == NULL) {
            fprintf(stderr, "smash: The action does not exist\n");
            exit(EXIT_FAILURE);
        }
        FILE * file_dst;
        char * line = NULL;
        size_t len = 0;
        ssize_t n_chars;

        file_dst = fopen(path, "r");
        if (file_dst == NULL)
            exit(EXIT_FAILURE);

        while ((n_chars = getline(&line, &len, file_dst)) != -1) { // iterate through commands
            char *expression = strtok(line, " ");
            while (expression != NULL) {
                fprintf(stderr, expression);
                expression = strtok(NULL, " ");
            }
        }
        fclose(file_dst);
        if (line)
            free(line);
        exit(EXIT_SUCCESS);
    }
    return 0;
}