#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARR_SIZE 255
#define MAX_VAL  10000000

typedef struct Node   Node;
typedef unsigned char u_char;

//==============================================//

struct Node {

    u_char val;
    int    freq;
    Node  *left, *right, *parent;
};

//===========================================//

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


Node *create_children(Node *left, Node *right) {

    Node *root = new_node();

    if (left == NULL)
        root -> left = new_node();
    else
        root -> left = left;

    if (right == NULL)
        root -> right = new_node();
    else
        root -> right = right;

    root -> right -> parent = root;
    root -> left -> parent  = root;
    return root;
}


void get_min_vals(int *result, int *arr, int size) {

    int i, j, skip, low;

    low = i = j = 0;
    skip = -1;

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

    int result;
    char arr[ARR_SIZE + 1] = {0};

    arr[ARR_SIZE] = '\0';
    result = 0;

    while (*input != '\0') {

        if (arr[*input] == 0) {
            result++;
            arr[*input]++;
        }

        input++;
    }

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


int get_value(char *arr, char target) {

    int i = 0;

    while (arr[i] != '\0') {

        if (arr[i] == target)
            return i;

        i++;
    }
    return -1;
}


void get_unique_vals(char *input, int *vals, char *chrs) {

    int index, length;
    length = 0;

    while (*input != '\0') {
        index = get_value(chrs, *input);

        if (index == -1) {
            chrs[length] = *input;
            vals[length]++;
            length++;
        }

        else {
            vals[index]++;
        }

        input++;
    }
}


Node *construct_tree(Node **nods, int *vals, char *chrs, int size) {

    Node *left, *right, *root;
    int   vl_counter, low_a, low_b;
    int   min[2] = {0};

    vl_counter = 0;

    while (vl_counter != size - 1) {

        get_min_vals(min, vals, size);

        low_a = min[0];
        low_b = min[1];

        left  = (nods[low_a] != 0) ? nods[low_a] : NULL;
        right = (nods[low_b] != 0) ? nods[low_b] : NULL;
        root = create_children(left, right);

        root->left->val  = chrs[low_a];
        root->right->val = chrs[low_b];

        nods[low_a] = root;
        nods[low_b] = NULL;

        vals[low_a] = vals[low_a] + vals[low_b];
        vals[low_b] = MAX_VAL;

        vl_counter++;
    }

    return root;
}


char *get_path(Node *curr, char target, int size) {

    int   i, dummy;
    char *path;

    path = malloc(size + 1);
    i   = 0;

    while (curr != NULL) {

        if (curr -> val == target) {
            path[i] = '\0';
            return path;
        }

        dummy = 0;

        if (curr -> left != NULL) {
            path[i] = '0';
            curr    = curr -> left;
            i++;
            continue;
        }

        if (curr -> right != NULL) {
            path[i] = '1';
            curr    = curr -> right;
            i++;
            continue;
        }
        break;
    }

    path[i] = '\0';
    return path;
}


void get_dictonary(char *chrs, Node *root, int size) {

    Node *curr;
    int   i;
    curr = root;
    i    = 0;

    while (i < size) {
        path[i] = get_path(curr, chrs[i], size);
        printf("%s\n", path[i]);
        i++;
    }

}


void get_full_byte(char *path) {

    int           i = 0;
    unsigned char x = 0;
    while (i < 8 && path[i] != '\0') {
        x = (x << 1) | (path[i] - '0');
        i++;
    }
}


void write_data(char *input, params *pars, int size) {

    int   i   = 0;
    FILE *ptr = fopen("dummy", "wb");

    if (ptr == NULL)
        exit(1);

    while (i < size) {
        fwrite(&path[i], 1, 1, ptr);
        i++;
    }

    fclose(ptr);
}


void set_nodes_null(Node **nodes, int size) {

    int i = 0;

    while (i < size) {
        nodes[i] = NULL;
        i++;
    }
}


int main(void) {

    char *input, *chrs;
    Node *root, **nodes;
    int  *vals;
    int   size;

    input = file_process();
    size  = get_unique_size(input);

    nodes      = malloc(size * sizeof(Node *));
    vals       = malloc(size * sizeof(int));
    chrs       = malloc(size + 1);
    chrs[size] = '\0';

    set_nodes_null(nodes, size);
    get_unique_vals(input, vals, chrs);
    construct_tree(nodes, vals, chrs, size);

    return 0;
}
