#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define GVN(Var) (#Var)
#define GFN(func) (#func)
#define var = memory_allocator(data_type, size) memory_allocator(#var, __func__, data_type, size)

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

void *memory_allocator(C_C *var_name, C_C *func_name, size_t data_type, size_t size) {

    void* memory = malloc(data_type * size);
    handle_func_failure(memory, GFN(malloc), var_name, func_name);
    memset(memory, 0, data_type * size);
    return memory;
}

u_char *handle_file_operation(void) {

    u_char *input;
    int  size;

    FILE *ptr = fopen("compressed","r");
    handle_func_failure(ptr, GFN(fopen), GVN(ptr), __func__);

    fseek(ptr, 0L, SEEK_END);
    size = ftell(ptr);

    while (size == -1L)
        size = ftell(ptr);

    input = memory_allocator(sizeof(u_char), size);
    fseek(ptr, 0L, SEEK_SET);
    fread(input, 1, size, ptr);
    input[size] = '\0';

    fclose(ptr);
    return input;
}

u_char *binary_to_u_char(char *input) {

    u_char *path;
}

int main(void) {

    u_char *input, *paths;
    int  size;

    input = handle_file_operation();
    paths = binary_to_u_char(input);

    
    free(input);
    return 0;
}
