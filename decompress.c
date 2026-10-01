//================================//
// Header Files
//================================//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//================================//
// Macros, Constants, Typedefs
//================================//

#define GVN(Var)  (#Var)
#define GFN(func) (#func)

#define handle_failure(var, var_b) handle_func_failure(var, var_b, #var, __func__)
#define memory_alloc(name, var_a, var_b) memory_allocator(#name, __func__, var_a, var_b)

typedef unsigned char u_char;
typedef const char    C_C;
typedef struct Node Node;

//================================//
// Structures
//================================//

struct Node {

    u_char chr;
    Node   *left, *right;
    Node   *parent;

};

//================================//
// Forward Declarations
//================================//

// Tree Functions
Node *new_node(void);
Node *create_node(Node *root, Node *left, Node *right);
Node *construct_tree(u_char *path, u_char seperator);

// Memory & Error Handling
void *memory_allocator(C_C *var_name, C_C *func_name, int data_type, int size);
void  handle_func_failure(void *ptr, C_C *fail_func, C_C *var_name,
                          C_C *scope_func);

// I/O Functions
u_char *handle_file_operation(void);

// Data Process Functions
int     get_sizeof_string(u_char *input);
u_char *binary_to_u_char(u_char *input);
u_char  get_path(u_char *input);

//================================//
// Tree Functions
//================================//

Node *new_node(void) {

    Node *node = memory_alloc(node, sizeof(Node), 1);
    return node;
}

//================================//
// Error Handling / Debug Functions
//================================//

void handle_func_failure(void *ptr, C_C *fail_func, C_C *var_name,
                         C_C *scope_func) {

    if (ptr == NULL) {
        printf(
            "ERROR : In \"%s\" function "
            "\"%s\" function Failed "
            "for the variable \"%s\" \n",
            scope_func, fail_func, var_name);
        exit(1);
    }
}

//================================//
// Memory Functions
//================================//

void *memory_allocator(C_C *var_name, C_C *func_name, int data_type, int size) {

    void *memory = malloc(data_type * size);
    handle_failure(memory, GFN(malloc));
    memset(memory, 0, data_type * size);
    return memory;
}

//================================//
// I/O Functions
//================================//

u_char *handle_file_operation(void) {

    u_char *input;
    int     size;
    FILE   *ptr;

    ptr = fopen("compressed", "rb");
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
    count *= 8;
    return count;
}

u_char *binary_to_u_char(u_char *input) {

    u_char *path;
    int     size, temp;

    size = get_sizeof_string(input) + 1;
    path = memory_allocator(GVN(path), __func__, sizeof(u_char), size);

    for (int i = 0, j = 0; input[j] != '\0'; j++) {
        
        if (input[j] == '#') {
            path[i] = '#';
            i++;
            continue;
        }

        for (int k = 0; k < 8; i++, k++) {
            temp    = (input[j] >> (7 - k) & 0X1);
            path[i] = (temp) ? '1' : '0';
        }
        path[i] = '\0';
    }

    return path;
}

u_char get_path(u_char *input) {

    u_char start;

    while (*input != '#') {
        input++;
    }

    return *input;
}

u_char get_byte_value(u_char *path, int i) {

    u_char value;

    value = 0x0;

    for (int j = 0; j < 8; j++) {
        value = value << j;
        value = value | (path[i] - '0');
        i++;
    }

    return value;
}

int get_number_of_nodes(u_char *path) {

    u_char byte_value;
    int    i, count;

    count = i = 0;
    byte_value = get_byte_value(path, i);

    while (byte_value != '#') {
        for (int j = 0; j < 8; j++) {
            if (path[i] == '0')
                count++;
            i++;
        }
        byte_value = get_byte_value(path, i);
    }

    return count;
}

Node *construct_tree(u_char *path, u_char seperator) {
    
    Node  *root;
    Node **nodes;
    int    i, size;

    size  = get_number_of_nodes(path);
    nodes = memory_alloc(nodes, sizeof(Node *), size);
    i = 0;

    while (*path != '#') {

        for (; *path != '1'; i++) {
            nodes[i] = new_node();
            path++;
        }

    }
    return root;
}

int main(void) {

    Node   *root;
    u_char *input, *paths;
    u_char  break_p;

    input   = handle_file_operation();
    paths   = binary_to_u_char(input);
    /* break_p = get_path(input); */
    /* root    = construct_tree(paths, break_p); */

    free(input);
    free(paths);
    return 0;
}
