#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define arr_size 255

typedef struct Node Node;

struct Node {

    char *value;
    int   weight;
    Node *left, *right;
};

int get_value(char *arr, char target) {
    
    int i = 0;
    while (arr[i] != '\0') {
        if (arr[i] == target)
            return i;
        i++;
    }
    return -1;
}

void get_unique_vals(char* input, int *vals, char *chars) {

    int index, length;
    length = 0;
    while (*input != '\0') {
        index = get_value(chars, *input);

        if (index == -1) {
            chars[length] = *input;
            vals[length]++;
            length++;
        }

        else {
            vals[index]++;
        }
        input++;
    }
}

int get_unique_size(char *input) {

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

    free(arr);
    return result;
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

    return input;
}

int main(void) {

    int   size;
    char *input, *chars;
    int  *vals;

    input       = file_process();
    size        = get_unique_size(input);
    vals        = malloc(size);
    chars       = malloc(size + 1);
    memset(vals, 0, size);
    memset(chars, '\0', size);
    chars[size] = '\0';

    get_unique_vals(input, vals, chars);

    free(input);
    return 0;
}
