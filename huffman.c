#include <stdio.h>
#include <stdlib.h>

#define arr_size 255

typedef struct Node Node;

struct Node {

    char *value;
    int   weight;
    Node *left, *right;
};

int is_present_arr(char *arr, char target, int length) {

    int i = 0;
    while (i < length) {
        if (arr[i] == target)
            return 1;
        i++;
    }
    return 0;
}

void store_unique(char *arr, char *input) {

    int i      = 0;
    int length = 0;
    while (*input != '\0') {

        if (is_present_arr(arr, *input, length)) {
            input++;
            continue;
        }

        arr[i] = *input;
        length++;
        input++;
    }
}

int get_unique(char *input) {

    int   result  = 0;
    char *arr     = malloc(arr_size + 1);
    arr[arr_size] = '\0';
    while (*input != '\0') {

        if (arr[*input] == 0) {
            result++;
            arr[*input]++;
        }

        input++;
    }
    return result;
}

char *get_unique_value_arr(char *input) {

    char *arr;
    int   size;
    size      = get_unique(input);
    arr       = malloc(size + 1);
    arr[size] = '\0';
    store_unique(arr, input);
    return arr;
}

char *file_process(void) {

    int   size;
    char *input;

    FILE *ptr = fopen("normal", "r");
    fseek(ptr, 0L, SEEK_END);
    size = ftell(ptr);
    fseek(ptr, 0L, SEEK_SET);
    input = malloc(size + 1);
    fread(input, 1, size, ptr);
    input[size] = '\0';

    return input
}

int main(void) {

    int   size;
    char *input;
    char *arr;

    input = file_process();
    arr   = get_unique_value_arr(input);

    free(input);
    free(arr);
    return 0;
}
