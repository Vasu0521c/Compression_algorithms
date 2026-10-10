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

#define handle_failure(var, var_b)                  \
    handle_func_failure(var, var_b, #var, __func__)

#define memory_alloc(name, var_a, var_b)            \
    memory_allocator(#name, __func__, var_a, var_b)

typedef unsigned char u_char;
typedef const char    C_C;
typedef struct Node   Node;

//================================//
// Structures
//================================//

struct Node {

    u_char chr;
    _Bool  visited;
    Node  *left, *right;
    Node  *parent;
};

//================================//
// Forward Declarations
//================================//

// Tree Functions
Node *new_node(void);
Node *create_node(Node *root, Node *left, Node *right);
Node *construct_tree(u_char *path);

// Memory & Error Handling
void *memory_allocator(C_C *var_name, C_C *func_name, int data_type, int size);
void  handle_func_failure(void *ptr, C_C *fail_func, C_C *var_name,
                          C_C *scope_func);
void  free_tree(Node *root);

// I/O Functions
u_char *handle_file_operation(void);

// Data Process Functions
int     get_sizeof_string(u_char *input);
u_char *binary_to_u_char(u_char *input);

//================================//
// Tree Functions
//================================//

Node *new_node(void) {

    Node *node   = memory_alloc(node, sizeof(Node), 1);
    node->chr    = '0';
    node->left   = NULL;
    node->right  = NULL;
    node->parent = NULL;
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

void free_tree(Node *root) {

    if (root == NULL)
        return;

    free_tree(root->left);
    free_tree(root->right);
    free(root);
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
    return count;
}

u_char *binary_to_u_char(u_char *input) {

    u_char *path;
    int     size, temp;

    size = (get_sizeof_string(input) * 8) + 1;
    path = memory_allocator(GVN(path), __func__, sizeof(u_char), size);

    for (int i = 0, j = 0; input[j] != '\0'; j++) {

        if (input[j] == '#') {
            path[i] = '#';
            i++;
            continue;
        }

        for (int k = 0; k < 8; i++, k++) {
            temp    = (input[j] >> (7 - k)) & 0X1;
            path[i] = (temp) ? '1' : '0';
        }

        path[i] = '\0';
    }

    free(input);
    return path;
}

u_char get_byte_value(u_char *path, int i) {

    u_char value;

    value = 0X0;

    for (int j = 0; j < 8; j++, i++) {
        value = (value << 1) | (path[i] - '0');
    }

    return value;
}

u_char get_char(u_char *path, int *i) {

    u_char value = 0X0;
    (*i)++;

    for (int j = 0; j < 8; j++, (*i)++) {
        value = (value << 1) | (path[*i] - '0');
    }

    (*i)--;
    return value;
}

Node *handle_prev(Node *parent, Node *child) {

    if (parent->left == NULL) {
        parent->left = child;
        return parent;
    }

    while (parent->right != NULL)
        parent = parent->parent;

    parent->right = child;
    return parent;
}

_Bool check_hashtag(u_char *path, int i) {

    for (int j = 0; j < 8; i++, j++) {
        if (path[i] == '#')
            return 1;
    }

    return 0;
}

Node *construct_tree(u_char *path) {

    Node  *root;
    Node  *curr, *prev;
    u_char chr;
    _Bool  breaker;

    root    = new_node();
    curr    = root;
    breaker = 0;

    for (int i = 1; path[i] != '#'; i++) {

        breaker = check_hashtag(path, i);
        if (breaker)
            break;

        chr  = (path[i] == '1') ? get_char(path, &i) : '0';
        prev = curr;
        curr = new_node();
        prev = handle_prev(prev, curr);

        curr->parent = prev;
        curr->chr    = chr;

        if (chr != '0')
            curr = prev;
    }
    return root;
}

void push_(Node **arr, Node *elem) {

    int i = 0;

    while (arr[i]->visited) {
        i++;
    }
    arr[i] = elem;
    (arr[i]->visited)++;
}

int get_value_count(Node *root) {

    int result = 0;

    if (root == NULL)
        return result;

    if (root->chr != '0')
        result++;

    result += get_value_count(root->left);
    result += get_value_count(root->right);

    return result;
}

void assign_values(Node *root, Node **nodes, int *i) {

    if (root == NULL)
        return;

    if (root->chr != '0') {
        nodes[*i] = root;
        (*i)++;
    }

    assign_values(root->left, nodes, i);
    assign_values(root->right, nodes, i);
}

Node **get_chrs(Node *root, int count) {

    Node **nodes;
    Node **tree;
    int    size;

    nodes = memory_alloc(nodes, sizeof(Node *), count);
    size  = 0;
    assign_values(root, nodes, &size);

    return nodes;
}

u_char *get_path(Node *root, Node *node) {

    u_char *path;
    Node   *parent;
    Node   *curr;
    int     size, i;

    curr = node;
    size = i = 0;

    while (curr != root) {
        curr = curr->parent;
        size++;
    }

    path = memory_alloc(path, sizeof(u_char), size + 1);

    for (curr = node; curr != root; i++) {
        parent  = curr->parent;
        path[i] = (parent->left == curr) ? '0' : '1';
        curr    = parent;
    }

    path[i] = '\0';
    return path;
}

u_char **get_dictionary(Node *root, Node **nodes, int size) {

    u_char **path;

    path = memory_alloc(path, sizeof(u_char *), size);

    for (int i = 0; i < size; i++) {
        path[i] = get_path(root, nodes[i]);
    }

    return path;
}

int get_actual_value(u_char *paths) {

    int count = 0;
    while (*paths != '#') {
        paths++;
        count++;
    }
    paths++;
    count++;
    return count;
}

int is_same(u_char *paths, u_char **path, int s_pos, int path_index) {

    int size = get_sizeof_string(path[path_index]);
    for (int i = s_pos, j = 0; j < size; i++, j++) {
        if (paths[i] != path[path_index][j])
            return -1;
    }
    return 1;
}

int get_value(u_char *paths, u_char **path, int i, int size) {

    int index, dummy;

    index = 0;
    
    for (int j = 0; j < size; j++) {
        dummy = is_same(paths, path, i, j);
        if (dummy != -1) {
            index = j;
        }
    }
    return index;
}

u_char *get_extracted_values(u_char *paths, Node **nodes, u_char **path,
                             int *size) {

    u_char *bytes;
    int     sep, index, length, m_p_s, dummy;
    int     value;

    value  = 0;
    m_p_s  = *size;
    sep    = get_actual_value(paths);
    length = (*size) * 8;
    *size  = sep + get_sizeof_string(&(paths[sep]));
    bytes  = memory_alloc(bytes, sizeof(u_char), length);

    for (int i = sep; i < (*size);) {
        index = get_value(paths, path, i, m_p_s);
        i    += get_sizeof_string(path[index]);
        bytes[value] = nodes[index] -> chr;
        value++;
    }
    printf("%s\n", bytes);
    return bytes;
}

void reverse_sort_dictionary(u_char **path, int size) {

    u_char *dummy;
    int     len_a, len_b;

    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            len_a = get_sizeof_string(path[i]);
            len_b = get_sizeof_string(path[j]);
            if (len_a < len_b) {
                dummy   = path[i];
                path[i] = path[j];
                path[j] = dummy;
            }
        }
    }
}

void write_decompressed_data(u_char *bytes, int size) {}

int  main(void) {

    Node   **nodes;
    Node    *root;
    u_char **path;
    u_char  *input, *paths, *bytes;
    int      size;

    input = handle_file_operation();
    paths = binary_to_u_char(input);
    root  = construct_tree(paths);
    size  = get_value_count(root);
    nodes = get_chrs(root, size);
    path  = get_dictionary(root, nodes, size);

    bytes = get_extracted_values(paths, nodes, path, &size);

    write_decompressed_data(bytes, size);

    free(paths);
    free(nodes);
    free(path);
    free_tree(root);
    return 0;
}
