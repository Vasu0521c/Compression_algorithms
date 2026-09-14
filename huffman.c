#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define arr_size 255

typedef struct Node Node;

struct Node {

    char  value;
    int   l_val, r_val;
    Node *left, *right;
    
};

typedef struct {

    int *vals, *min;
    char *chrs;

} params;

Node *new_node(void) {

    Node *newnode    = malloc(sizeof(Node));
    newnode -> value = '\0';
    newnode -> l_val = 0;
    newnode -> r_val = 0;
    newnode -> left  = NULL;
    newnode -> right = NULL;
    return newnode;
}

Node *create_children(Node *left, Node *right) {

    Node *root    = new_node();
    root -> l_val = 0;
    root -> r_val = 1;

    if (left == NULL)
        root -> left  = new_node();
    else
        root -> left  = left;

    if (right == NULL)
        root -> right = new_node();
    else
        root -> right = right;

    return root;
}

Node *assign_vals(Node *root, char lv, char rv) {

    Node *temp = root;
    temp->left->value = lv;
    temp->right->value = rv;
    return root;
}

void get_min_vals(int *result, int *arr, int size) {

    int i, j, skip, low;
    i = j = 0;
    skip = -1;

    while (i < size - 1) {

        if (i == skip) {
            i++;
            continue;
        }

        if (arr[i] < arr[i + 1]) {
            low  = i;
            skip = i;
        }

        if (i == size - 1) {
            result[j] = low;
            j++;
            if (j > 1)
                break;
            i = 0;
        }
        i++;
    }
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
    char *chars;
    int  *vals;

    length = 0;
    chars = pars -> chrs;
    vals  = pars -> vals;

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


void construct_tree(Node **nods, params *pars, int size) {

    Node *left, *right;
    int i, vl_counter;
    int low_a, low_b;
    vl_counter = i = 0;

    while (vl_counter != size - 1) {
        get_min_vals(pars -> min, pars -> vals, size);
        low_a = pars -> min[0];
        low_b = pars -> min[1];

        if (nods[low_a] == NULL) {
            left = NULL;
        }

        if (nods[low_b] == NULL) {
            right = NULL;
        }

        nods[low_a] = create_children(left, right);
        nods[low_a] = nods[low_b];
    }
}

int main(void) {

    params *pars;
    char *input;
    Node *root;
    Node **nods;
    int size;

    input = file_process();
    size  = get_unique_size(input);

    nods  = malloc(size * sizeof(Node *));
    pars  = malloc(sizeof(params));

    pars -> min   = malloc(sizeof(int) * 2);
    pars -> vals  = malloc(sizeof(int) * size);
    pars -> chrs  = malloc(size + 1);

    memset(pars -> vals, 0, size * sizeof(int));
    memset(pars -> chrs, 0, size * sizeof(int));
    memset(nods, 0, size * sizeof(Node *));
    pars -> chrs[size] = '\0';

    root = new_node();
    get_unique_vals(input, pars);
    construct_tree(nods, pars, size);

    free(input);
    free(nods);
    free(pars -> chrs);
    free(pars -> vals);
    free(pars -> min);
    return 0;
}
