#include "../../custom_header_files/Donwloaded_ones/vec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define arr_size 255

typedef struct Node Node;

//==============================================//

struct Node {

    char *value;
    int   l_val, r_val, length;
    Node *left, *right, *parent;
};

typedef struct {

    int  *vals, *min;
    char *chrs, **path;

} params;

//===========================================//

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

Node *create_children(Node *left, Node *right) {

    Node *root  = new_node();
    root->l_val = 0;
    root->r_val = 1;

    if (left == NULL)
        root->left = new_node();
    else
        root->left = left;

    if (right == NULL)
        root->right = new_node();
    else
        root->right = right;

    root->right->parent = root;
    root->left->parent  = root;
    return root;
}

Node *create_memory(Node *node, char val) {

    node->length   = 1;
    node->value    = malloc(2);
    node->value[0] = val;
    node->value[1] = '\0';
    return node;
}

Node *assign_values(Node *root, char *l_val, char *r_val) {

    Node *temp_l, *temp_r, *temp_m;
    temp_m = root;
    temp_l = root->left;
    temp_r = root->right;

    if (temp_l->length == 0) {
        temp_l = create_memory(temp_l, *l_val);
    }

    if (temp_r->length == 0) {
        temp_r = create_memory(temp_r, *r_val);
    }

    root->length = temp_r->length + temp_r->length;
    root->value  = malloc(root->length + 1);
    memcpy(root->value, temp_l->value, temp_l->length);
    memcpy(root->value + (temp_l->length), temp_r->value, temp_r->length);
    root->value[root->length] = '\0';
    return root;
}

int is_present(Node *node, char target) {

    char *chr = node->value;

    while (*chr != '\0') {

        if (*chr == target)
            return 1;

        chr++;
    }

    return 0;
}

void get_min_vals(int *result, int *arr, int size) {

    int i, j, skip, low;
    low = i = j = 0;
    skip        = -1;

    while (i < size) {

        if (i == skip) {
            i++;
            if (i < size) {
                continue;
            }
        }

        if (i >= size - 1) {
            result[j] = low;
            skip      = low;
            j++;
            if (j > 1)
                break;
            low = 0;
            i   = 0;
        }

        if (arr[low] >= arr[i]) {
            low = i;
        }

        i++;
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

    if (ptr == NULL)
        exit(1);

    fseek(ptr, 0L, SEEK_END);
    size = ftell(ptr);
    fseek(ptr, 0L, SEEK_SET);
    input = malloc(size + 1);
    fread(input, 1, size, ptr);
    input[size] = '\0';

    return input;
}

void set_memory(char ***dict, Node ***nods, params **pars, int size) {

    *dict = malloc(size * sizeof(char *));
    *nods = malloc(size * sizeof(Node *));
    *pars = malloc(sizeof(params));

    (*pars)->min  = malloc(sizeof(int) * 2);
    (*pars)->vals = malloc(sizeof(int) * size);
    (*pars)->chrs = malloc(sizeof(char) * size + 1);
    (*pars)->path = malloc(sizeof(char *) * size);

    memset(*nods, 0, size * sizeof(Node *));
    memset(*dict, 0, size * sizeof(char *));
    memset((*pars)->min, 0, 2 * sizeof(int));
    memset((*pars)->vals, 0, size * sizeof(int));
    memset((*pars)->chrs, 0, size * sizeof(char));
    (*pars)->chrs[size] = '\0';
}

int get_value(char *arr, char target) {

    int i = 0;

    while (arr[i] != '\0') {

        if (arr[i] == target)
            return i;

        i++;
    }
    return -1;
}

void get_unique_vals(char *input, params *pars) {

    int index, length;
    length = 0;

    while (*input != '\0') {
        index = get_value(pars->chrs, *input);

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

Node *construct_tree(Node *root, Node **nods, params *pars, int size) {

    Node *left, *right, *ph;
    int   vl_counter, low_a, low_b;
    char *l_val, *r_val;
    vl_counter = 0;

    while (vl_counter != size - 1) {

        get_min_vals(pars->min, pars->vals, size);
        low_a = pars->min[0];
        low_b = pars->min[1];

        left  = (nods[low_a] != 0) ? nods[low_a] : NULL;
        right = (nods[low_b] != 0) ? nods[low_b] : NULL;
        l_val = (left != NULL) ? left->value : &(pars->chrs[low_a]);
        r_val = (right != NULL) ? right->value : &(pars->chrs[low_b]);

        root = create_children(left, right);
        root = assign_values(root, l_val, r_val);

        nods[low_a] = root;
        nods[low_b] = root;

        pars->vals[low_a] = pars->vals[low_a] + pars->vals[low_b];
        pars->vals[low_b] = pars->vals[low_a];

        vl_counter++;
    }

    return root;
}

char *get_path(Node *curr, char target, int size) {

    int   i, dummy;
    char *path = malloc(size + 1);
    path[size] = '\0';
    i          = 0;

    while (curr != NULL) {

        if (curr->value[0] == target) {
            break;
        }

        dummy = 0;

        if (curr->left != NULL) {
            if (is_present(curr->left, target)) {
                path[i] = '0' + curr->l_val;
                curr    = curr->left;
                i++;
                continue;
            }
        }

        if (curr->right != NULL) {
            if (is_present(curr->right, target)) {
                path[i] = '0' + curr->r_val;
                curr    = curr->right;
                i++;
                continue;
            }
        }
        break;
    }

    path[i] = '\0';
    return path;
}

void get_dictonary(params *pars, Node *root, int size) {

    Node *curr;
    int   i;
    curr = root;
    i    = 0;
    while (i < size) {
        pars->path[i] = get_path(curr, pars->chrs[i], size);
        printf("%s\n", pars->path[i]);
        i++;
    }
}

void free_memory(char **input, params **pars, char ***dict, Node ***nods,
                 int size) {

    int i = 1;

    while (i < size) {
        free(*dict[i]);
        free(*nods[i]);
        free((*pars)->path[i]);
        i++;
    }

    free(*dict);
    free(*nods);
    free((*pars)->vals);
    free((*pars)->min);
    free((*pars)->chrs);
    free((*pars)->path);
    free(pars);
    free(input);
}

void get_full_byte(char *path) {

    int           i = 0;
    unsigned char x = 0;
    while (i < 8 && path[i] != '\0') {
        x = (x << 1) | ('0' - path[i]);
        i++;
    }
}

void write_data(char *input, params *pars, int size) {

    int   i   = 0;
    FILE *ptr = fopen("dummy", "wb");

    if (ptr == NULL)
        exit(1);

    while (i < size) {
        fwrite(&pars->path[i], 1, 1, ptr);
        i++;
    }

    fclose(ptr);
}

int main(void) {

    params *pars;
    char   *input;
    char  **dict;
    Node   *root;
    Node  **nods;
    int     size;

    input = file_process();
    size  = get_unique_size(input);

    set_memory(&dict, &nods, &pars, size);
    get_unique_vals(input, pars);
    root = construct_tree(root, nods, pars, size);
    get_dictonary(pars, root, size);
    /* write_data(input, pars, size); */

    free_memory(&input, &pars, &dict, &nods, size);
    return 0;
}
