//================================================
// Header Files
// ===============================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==============================================
// Macros, Constants and Typedefs
// ==============================================

#define ARR_SIZE 255
#define MAX_VAL  10000000

typedef struct Node   Node;
typedef unsigned char u_char;

//==============================================
// Structures
// =============================================

struct Node {

    u_char chr;
    int    freq;
    Node  *left, *right, *parent;
};

typedef struct {

    u_char *chrs;
    int    *vals, *min;

} params;

//===========================================
// Forward declaratoins
// ==========================================

// Memory allocation and Free functions

void memory_allocator(params **pars, u_char ***path, Node ***nodes, int size);
void free_memory(params **pars, u_char ***path, Node ***nodes, int size);

// Tree and Dictionary operations

Node   *new_node(void);
Node   *node_assign(Node *node, int val, char chr);
Node   *create_subtree(Node *left, Node *right, params *pars);
Node   *construct_tree(Node **nods, params *pars, int size);
Node   *find_node(Node *root, char target);
u_char *get_path(Node *curr, char target, u_char *path);
void    get_dictionary(u_char *chrs, Node *root, u_char **path, int size);

// Data Processing

u_char *file_process(void);
int     get_min_vals(int *arr, int val, int size);
void    get_unique_vals(u_char *input, params *pars);
int     get_unique_size(u_char *input);
int     get_value(u_char *arr, u_char target, int size);

// =========================================
// Memory Functions
// =========================================

void memory_allocator(params **pars, u_char ***path, Node ***nodes, int size) {

    *pars  = malloc(sizeof(params));
    *path  = malloc(size * sizeof(u_char *));
    *nodes = malloc(size * sizeof(Node *));

    (*pars) -> vals = malloc(size * sizeof(int));
    (*pars) -> chrs = malloc(size + 1);

    for (int i = 0; i < size; i++) {
        (*path)[i]  = malloc(size + 1);
        (*nodes)[i] = NULL;
    }

    (*pars) -> chrs[size] = '\0';
    memset((*pars) -> vals, 0, size * sizeof(int));
}

void free_memory(params **pars, u_char ***path, Node ***nodes, int size) {

    for (int i = 0; i < size; i++) {
        free((*path)[i]);
    }

    free((*pars) -> vals);
    free((*pars) -> chrs);
    free(*pars);
    free(*path);
    free(*nodes);
}

void free_tree(Node *root) {

    if (root == NULL)
        return;

    free_tree(root -> left);
    free_tree(root -> right);
    free(root);
}

// ===========================================
// File and Data Processing
// ===========================================

u_char *file_process(void) {

    int     size;
    u_char *input;

    FILE   *ptr = fopen("normal", "r");

    if (ptr == NULL)
        exit(1);

    fseek(ptr, 0L, SEEK_END);
    size = ftell(ptr);

    while (size == -1)
        size = ftell(ptr);

    fseek(ptr, 0L, SEEK_SET);
    input = malloc(size + 1);
    fread(input, 1, size, ptr);
    input[size - 1] = '\0';

    fclose(ptr);
    return input;
}

void compressed_file_creation(u_char *bytes, int byte_length) {

    FILE *ptr = fopen("compressed", "wb");

    if (ptr == NULL)
        exit(1);

    fwrite(bytes, 1, byte_length, ptr);
    fclose(ptr);
}

int get_unique_size(u_char *input) {

    int    result;
    u_char arr[ARR_SIZE + 1] = {0};

    arr[ARR_SIZE] = '\0';
    result        = 0;

    while (*input != '\0') {

        if (arr[*input] == 0) {
            result++;
            arr[*input]++;
        }

        input++;
    }

    return result;
}

int get_min_vals(int *arr, int val, int size) {

    int i, low;
    i   = 0;
    low = (val) ? 0 : 1;

    while (i < size) {

        if (i == val) {
            i++;
            continue;
        }

        if (arr[low] >= arr[i]) {
            low = i;
        }

        i++;
    }

    return low;
}

int get_value(u_char *arr, u_char target, int size) {

    int i = 0;

    while (i < size) {

        if (target == arr[i])
            return i;

        i++;
    }
    return -1;
}

void get_unique_vals(u_char *input, params *pars) {

    int index, length;
    length = 0;

    while (*input != '\0') {
        index = get_value(pars -> chrs, *input, length);

        if (index == -1) {
            pars -> chrs[length] = *input;
            pars -> vals[length]++;
            length++;
        }

        else {
            pars -> vals[index]++;
        }

        input++;
    }
}

int get_size(u_char *arr) {

    int i = 0;
    while (*arr != '\0') {
        arr++;
        i++;
    }
    return i;
}

int get_path_size(u_char **path, int size) {

    int i, result;

    while (i < size) {
        result += get_size(path[i]);
        i++;
    }

    return result;
}

void get_full_bytes(u_char *bytes, u_char *path, int length) {

    int i, j, k;
    i = j = k = 0;

    while (i < length) {

        if (k == 8) {
            j++;
            k = 0;
        }

        bytes[j] = bytes[j] << 1;
        bytes[j] = bytes[j] | (path[i] - '0');
        i++;
        k++;
    }
}

// ============================================
// Node and Tree Functions
// ============================================

Node *new_node(void) {

    Node *node = malloc(sizeof(Node));

    if (node == NULL)
        exit(1);

    memset(node, 0, sizeof(Node));
    node -> left   = NULL;
    node -> right  = NULL;
    node -> parent = NULL;
    return node;
}

Node *node_assign(Node *node, int val, char chr) {

    if (node == NULL) {
        node       = new_node();
        node -> chr  = chr;
        node -> freq = val;
    }

    return node;
}

Node *create_subtree(Node *left, Node *right, params *pars) {

    int  *min;
    Node *root;

    min   = pars -> min;
    root  = new_node();
    left  = node_assign(left, pars -> vals[min[0]], pars -> chrs[min[0]]);
    right = node_assign(right, pars -> vals[min[1]], pars -> chrs[min[1]]);

    root -> left    = left;
    root -> right   = right;
    left -> parent  = root;
    right -> parent = root;
    root -> freq    = left -> freq + right -> freq;
    return root;
}

Node *construct_tree(Node **nods, params *pars, int size) {

    Node *left, *right, *root;
    int   counter, low_a, low_b;

    pars -> min = malloc(2 * sizeof(int));
    counter   = 0;

    while (counter != size - 1) {

        pars -> min[0] = low_a = get_min_vals(pars -> vals, MAX_VAL, size);
        pars -> min[1] = low_b = get_min_vals(pars -> vals, low_a, size);

        left  = (nods[low_a] != 0) ? nods[low_a] : NULL;
        right = (nods[low_b] != 0) ? nods[low_b] : NULL;
        root  = create_subtree(left, right, pars);

        nods[low_a] = root;
        nods[low_b] = NULL;

        pars -> vals[low_a] = pars -> vals[low_a] + pars -> vals[low_b];
        pars -> vals[low_b] = MAX_VAL;

        counter++;
    }

    free(pars -> min);
    return root;
}

Node *find_node(Node *root, char target) {

    Node *curr;

    if (root == NULL)
        return NULL;

    if (root -> chr == target)
        return root;

    curr = find_node(root -> left, target);

    if (curr == NULL)
        curr = find_node(root -> right, target);

    return curr;
}

u_char *get_path(Node *curr, char target, u_char *path) {

    int     i;
    u_char *tem;
    Node   *temp, *dummy;

    temp = curr;
    i    = 0;
    tem  = path;
    temp = find_node(curr, target);

    while (temp != curr) {
        dummy  = temp -> parent;
        tem[i] = (dummy -> left == temp) ? '0' : '1';
        temp   = dummy;
        i++;
    }

    path[i] = '\0';
    return path;
}

void reverse_path(u_char *path, int size) {

    u_char temp;
    size--;

    for (int i = 0; i < size / 2; i++) {
        temp           = path[i];
        path[i]        = path[size - i];
        path[size - i] = temp;
    }
}

void get_dictionary(u_char *chrs, Node *root, u_char **path, int size) {

    Node *curr;
    curr = root;

    for (int i = 0; i < size; i++) {
        path[i] = get_path(curr, chrs[i], path[i]);
    }

    for (int i = 0; i < size; i++) {
        reverse_path(path[i], get_size(path[i]));
    }
}

void get_full_path(u_char *full_p, u_char **path, int size) {

    int i, j, k;
    i = j = k = 0;

    while (i < size) {

        if (path[k][j] == '\0') {
            j = 0;
            k++;
        }

        full_p[i] = path[k][j];
        i++;
        j++;
    }
}

// ============================================
// Main Function
// ============================================

int main(void) {

    params *pars;
    u_char *input, **path, *full_path, *bytes;
    Node   *root, **nodes;
    int     size_a, size_b, byte_length;

    input  = file_process();
    size_a = get_unique_size(input);

    memory_allocator(&pars, &path, &nodes, size_a);
    get_unique_vals(input, pars);
    root = construct_tree(nodes, pars, size_a);
    get_dictionary(pars -> chrs, root, path, size_a);

    size_b    = get_path_size(path, size_a);
    full_path = malloc(size_b);
    get_full_path(full_path, path, size_b);

    byte_length = (size_b % (int)8) ? (size_b / (int)8) + 1 : size_b / (int)8;
    bytes       = malloc(byte_length);
    memset(bytes, 0, byte_length);

    get_full_bytes(bytes, full_path, size_b);
    compressed_file_creation(bytes, byte_length);

    free_memory(&pars, &path, &nodes, size_a);
    free_tree(root);
    return 0;
}
