#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define GVN(Var) (#Var)
#define GFN(func) (#func)

typedef unsigned char u_char;
typedef const char C_C;

void handle_func_failure(void *ptr, C_C *fail_func, C_C* var_name,  C_C *scope_func) {

    if (ptr == NULL) {
        printf("ERROR : In \"%s\" function "
            "\"%s\" function Failed "
            "for the variable \"%s\" \n", scope_func, fail_func, var_name);
        exit(1);
    }
}

void *memory_allocator(C_C *var_name, C_C *func_name, int data_type, int size) {

    void* memory = malloc(data_type * size);
    handle_func_failure(memory, GFN(malloc), var_name, func_name);
    memset(memory, 0, data_type * size);
    return memory;
}

u_char *handle_file_operation(void) {

    u_char *input;
    int  size;

    FILE *ptr = fopen("compressed","rb");
    handle_func_failure(ptr, GFN(fopen), GVN(ptr), __func__);

    fseek(ptr, 0L, SEEK_END);
    size = ftell(ptr);

    while (size == -1L)
        size = ftell(ptr);

    input = memory_allocator(GVN(input), __func__, sizeof(u_char), size + 1);
    fseek(ptr, 0L, SEEK_SET);
    fread(input, 1, size, ptr);
    input[size] = '\0';

    fclose(ptr);
    return input;
}

int get_sizeof_string(u_char *input) {

    int count = 0;
    while (*input != '\0') {
        count++;
        input++;
    }
    printf("%d", count);
    count *= 8;
    return count;
}

u_char *binary_to_u_char(u_char *input) {

    u_char *path;
    int     size, temp;

    size = get_sizeof_string(input) + 1;
    path = memory_allocator(GVN(path), __func__, sizeof(u_char), size);

    for (int i = 0, j = 0; input[j] != '\0'; j++) {
        for (int k = 0; k < 8; i++, k++) {
            temp = (input[j] >> (7 - k) & 0x1);
            path[i] = (temp) ? '1' : '0';
        }
        path[i] = '\0';
    }

    return path;
}

int main(void) {

    u_char *input, *paths;

    input = handle_file_operation();
    paths = binary_to_u_char(input);
    
    free(input);
    free(paths);
    return 0;
}
