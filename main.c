#include <stdio.h>

#include "interpreter.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("input error, must enter a single argument (filename.py)\n");
        exit(-1);
    }
    FILE* py_program = fopen(argv[1], "r");
    if (!py_program) {
        printf("input error, program cannot find the python file specified\n");
    }
    String* program_str = str_create();
    int c;
    do {
        c = fgetc(py_program);
        if (c > 255) {
            printf("error, character outside of ASCII\n");
            fclose(py_program);
            exit(-1);
        }
        if(feof(py_program)) {
            break;
        }
        str_append(program_str, (char)c);
    } while(true);
    fclose(py_program);
    program = program_str->internal;
    run_program();
    printf("\nEvaluation is done.\n");
    return 0;
}