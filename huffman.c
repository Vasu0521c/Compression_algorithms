//================================================
// Header Files
// ===============================================

#include "../../custom_header_files/downloaded_ones/vec.h"
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

    u_char **path;
    u_char  *chrs, *tree_path, *chr_path;
    Node   **nodes;
    Node    *root;
    int     *vals, *min;

} params;

//===========================================
// Forward declaratoins
// ==========================================

// Memory allocation and Free functions

void *memory_allocator(int dt_size, int size);
void  free_memory(params **pars, u_char ***path, Node ***nodes, int size);

// Tree and Dictionary operations

Node   *new_node(void);
Node   *node_assign(Node *node, int val, char chr);
Node   *create_subtree(Node *left, Node *right, params *pars);
Node   *construct_tree(params *pars, int size);
Node   *find_node(Node *root, char target);
void    get_path(Node *curr, char target, u_char *path);
void    get_dictionary(params *pars, int size);

// Data Processing

u_char *file_process(void);
int     get_min_vals(int *arr, int val, int size);
void    get_unique_vals(u_char *input, params *pars, int size);
int     get_unique_size(u_char *input);
int     get_value(u_char *arr, u_char target, int size);
int     get(Node *root);

// =========================================
// Memory Functions
// =========================================

void *memory_allocator(int dt_size, int size) {

    void *temp = malloc(dt_size * size);

    if (temp == NULL) {
        printf("Malloc Failed \n");
        exit(1);
    }

    memset(temp, 0, dt_size * size);
    return temp;
}

void free_memory(params **pars, u_char ***path, Node ***nodes, int size) {

    for (int i = 0; i < size; i++) {
        free((*path)[i]);
    }

    free((*pars)->vals);
    free((*pars)->chrs);
    free(*pars);
    free(*path);
}

void free_tree(Node *root) {

    if (root == NULL)
        return;

    free_tree(root->left);
    free_tree(root->right);
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

void compressed_file_creation(u_char *bytes, int byte_length, char *mode) {

    FILE *ptr = fopen("compressed", mode);

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

void get_unique_vals(u_char *input, params *pars, int size) {

    int index, length;
    length = 0;

    pars->vals = memory_allocator(sizeof(int), size);
    pars->chrs = memory_allocator(sizeof(char), size);

    while (*input != '\0') {
        index = get_value(pars->chrs, *input, length);

        if (index == -1) {
            pars->chrs[length] = *input;
            pars->vals[length]++;
            length++;
        }

        else {
            pars->vals[index]++;
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

int get_path_length(u_char **path, int size) {

    int i, result;
    i = 0;

    while (i < size) {
        result += get_size(path[i]);
        i++;
    }

    return result;
}

void get_full_bytes(u_char *bytes, u_char *path, int length) {

    int    i, j, k;
    u_char answer;
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
    node->left   = NULL;
    node->right  = NULL;
    node->parent = NULL;
    return node;
}

Node *node_assign(Node *node, int val, char chr) {

    if (node == NULL) {
        node       = new_node();
        node->chr  = chr;
        node->freq = val;
    }

    return node;
}

Node *create_subtree(Node *left, Node *right, params *pars) {

    int  *min;
    Node *root;

    min   = pars->min;
    root  = new_node();
    left  = node_assign(left, pars->vals[min[0]], pars->chrs[min[0]]);
    right = node_assign(right, pars->vals[min[1]], pars->chrs[min[1]]);

    root->left    = left;
    root->right   = right;
    left->parent  = root;
    right->parent = root;
    root->freq    = left->freq + right->freq;
    return root;
}

Node *construct_tree(params *pars, int size) {

    Node **nods;
    Node  *left, *right, *root;
    int    counter, low_a, low_b;

    pars->nodes = memory_allocator(sizeof(Node *), size);
    pars->min   = memory_allocator(sizeof(int), 2);
    counter     = 0;
    nods        = pars->nodes;

    while (counter != size - 1) {

        pars->min[0] = low_a = get_min_vals(pars->vals, MAX_VAL, size);
        pars->min[1] = low_b = get_min_vals(pars->vals, low_a, size);

        left  = (nods[low_a] != 0) ? nods[low_a] : NULL;
        right = (nods[low_b] != 0) ? nods[low_b] : NULL;
        root  = create_subtree(left, right, pars);

        nods[low_a] = root;
        nods[low_b] = NULL;

        pars->vals[low_a] = pars->vals[low_a] + pars->vals[low_b];
        pars->vals[low_b] = MAX_VAL;

        counter++;
    }

    free(pars->nodes);
    free(pars->min);
    return root;
}

Node *find_node(Node *root, char target) {

    Node *curr;

    if (root == NULL)
        return NULL;

    if (root->chr == target)
        return root;

    curr = find_node(root->left, target);

    if (curr == NULL)
        curr = find_node(root->right, target);

    return curr;
}

void get_path(Node *curr, char target, u_char *path) {

    int   i;
    Node *temp, *dummy;

    temp = find_node(curr, target);
    i    = 0;

    while (temp != curr) {
        dummy   = temp->parent;
        path[i] = (dummy->left == temp) ? '0' : '1';
        temp    = dummy;
        i++;
    }

    path[i] = '\0';
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

void get_dictionary(params *pars, int size) {

    u_char *chrs = pars->chrs;
    pars->path   = memory_allocator(sizeof(u_char *), size);

    for (int i = 0; i < size; i++) {
        pars->path[i] = memory_allocator(sizeof(u_char), size);
        get_path(pars->root, chrs[i], pars->path[i]);
        reverse_path(pars->path[i], get_size(pars->path[i]));
    }
}

void get_array(Node *root, Node **vvec, int size) {

    int   i   = 0;
    Node *vec = *vvec;
    vector_add(&vec, *root);

    while (i < size) {
        Node temp = vec[i];

        if (temp.left != NULL) {
            vector_add(&vec, *temp.left);
        }

        if (temp.right != NULL) {
            vector_add(&vec, *temp.right);
        }

        i++;
    }
    *vvec = vec;
}

void get_full_path(params *pars, int size) {

    int i, j, k;
    u_char *par;
    i = j = k = 0;

    pars -> chr_path = memory_allocator(sizeof(u_char), size + 1);
    par = pars -> chr_path;

    while (i < size) {

        if (pars->path[k][j] == '\0') {
            free(pars -> path[k]);
            j = 0;
            k++;
        }

        par[i] = pars->path[k][j];
        i++;
        j++;
    }
    free(pars -> path[k]);
    free(pars -> path);
    par[i] = '\0';
}

void get_tree_path(params *pars, int size) {

    int     i, l, k;
    u_char *path;
    Node   *vec = vector_create();

    k = get(pars->root);
    i = l = 0;
    pars->tree_path = memory_allocator(sizeof(u_char), size + 1);
    path            = pars->tree_path;
    get_array(pars->root, &vec, k);

    while (i < k) {
        if (vec[i].chr) {
            path[l] = '1';
            l++;

            u_char temp;
            for (int j = 0; j < 8; j++, l++) {
                temp    = 0B00000001 & (vec[i].chr >> (7 - j));
                path[l] = (temp) ? '1' : '0';
                temp    = k;
            }
            l--;
        }

        else {
            path[l] = '0';
        }
        i++;
        l++;
    }
    path[l] = '#';
    path[l+1] = '\0';
}

int get(Node *root) {

    int result = 0;

    if (root != NULL)
        result++;
    else
        return result;

    result += get(root->left);
    result += get(root->right);

    return result;
}

int get_tree_length(Node *root, int size) {

    int result  = 0;
    result      = get(root);
    result     += size * 8;
    return result;
}

// ============================================
// Main Function
// ============================================

int main(void) {

    params *pars;
    u_char *input_data, *bytes;
    int     uni_val_size, path_length, tree_length, byte_length;

    input_data   = file_process();
    uni_val_size = get_unique_size(input_data);

    pars = memory_allocator(sizeof(params), 1);
    get_unique_vals(input_data, pars, uni_val_size);
    pars->root = construct_tree(pars, uni_val_size);
    get_dictionary(pars, uni_val_size);

    tree_length = get_tree_length(pars->root, uni_val_size);
    path_length = get_path_length(pars->path, uni_val_size);

    get_tree_path(pars, tree_length);
    get_full_path(pars, path_length);

    tree_length--;
    byte_length = (tree_length % 8 == 0) ? tree_length / (int)8 : (tree_length / (int)8) + 1;
    byte_length++;

    bytes = memory_allocator(sizeof(u_char), byte_length);

    get_full_bytes(bytes, pars -> tree_path, tree_length);
    compressed_file_creation(bytes, byte_length, "wb");

    free(bytes);

    byte_length = (path_length % 8 == 0) ? path_length / (int)8 : (path_length / (int)8) + 1;
    bytes = memory_allocator(sizeof(u_char), byte_length);
    get_full_bytes(bytes, pars -> chr_path, path_length);
    printf("%s %d\n", pars -> chr_path, byte_length);
    compressed_file_creation(bytes, byte_length, "a");

    /* free_memory(&pars, &path, &nodes, uni_val_size); */
    /* free_tree(root); */
    /* free(bytes); */
    /* free(full_path); */
    return 0;
}
