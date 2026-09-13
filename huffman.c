#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;

struct Node {

    char *value;
    int   weight;
    Node *left, *right;
};


int is_present(char *input, int index) {

    int  i      = 0;
    char target = input[index];
    while (i < index) {

        if (input[i] == input[index])
            return 1;

        i++;
    }
    return 0;
}


int get_unique_values(char *input) {

    int   i, result;
    char *string;

    result = i = 0;
    string = input;
    while (input[i] != '\0') {

        if (input[i] == input[i + 1])
            i++;

        else if (is_present(string, i)) 
             result++;
        i++;
    }
    return result;
}


char *get_unique_value_arr(char *input) {

    int size;
    char *arr;
    size = get_unique_values(input);
    arr = malloc(size + 1);
    arr[size] = '\0';
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

    char *input;
    char *arr;

    input = file_process();
    arr   = get_unique_value_arr(input);

    free(input);
    free(arr);
    return 0;
}
