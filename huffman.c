#include "../../custom_header_files/C_headers/charVectar.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;

struct Node {

    char  *value;
    int    weight;
    Node  *left, *right;
};

int is_present(vectar *vec, int size, char target);
void process_input(char* input, int size);


int is_present(vectar *vec, int size, char target) {

    char* arr = vec -> data;
    int i = 0;
    while (i < size) {

        if (target == arr[i])
            return 1;

        i++;
    }
    return 0;
}

void process_input(char *input, int size) {

    vectar *char_lit, *count_lit;
    int     i, vec_length;

    i = vec_length = 0;
    while (i < size) {

        _Bool duplicate = is_present(char_lit, vec_length, input[i]);

        if (duplicate) {
            i++;
            continue;
        }

        i++;
    }
}

int main(void) {

    int   size;
    char* input;

    FILE* ptr = fopen("normal", "r");
    fseek(ptr, 0L, SEEK_END);
    size = ftell(ptr);
    fseek(ptr, 0L, SEEK_SET);
    input       = malloc(size + 1);
    input[size] = '\0';
    process_input(input, size);
    free(input);

    return 0;
}
