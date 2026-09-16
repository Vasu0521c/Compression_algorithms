#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define arr_size 255

typedef struct Node Node;

struct Node {

    char  value;
    int   l_val, r_val;
    Node *left, *right, *parent;
};

typedef struct {

    int  *vals, *min;
    char *chrs;

} params;

Node *new_node(void) {


    Node *some = malloc(sizeof(Node));
    if (some == NULL)
        exit(1);
    some -> l_val = 0;
    some -> r_val = 0;
    some -> left  = NULL;
    some -> right = NULL;
    some -> parent = NULL;
    return some;
}


Node *create_children(Node *left, Node *right) {

    Node *root  = new_node();
    root -> l_val = 0;
    root -> r_val = 1;

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

Node *assign_vals(Node *root, char lv, char rv) {

    root -> left -> value  = lv;
    root -> right -> value = rv;
    return root;
}

void get_min_vals(int *result, int *arr, int size) {

    int i, j, skip, low;
    i = j = 0;
    skip  = -1;

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

    int   index, length;
    length = 0;

    while (*input != '\0') {
        index = get_value(pars -> chrs, *input);

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

Node *construct_tree(Node *root, Node **nods, params *pars, int size) {

    Node *left, *right, *ph;
    int   vl_counter, low_a, low_b;
    char  l_val, r_val;
    vl_counter = 0;

    while (vl_counter != size - 1) {

        get_min_vals(pars -> min, pars -> vals, size);
        low_a = pars -> min[0];
        low_b = pars -> min[1];

        left  = (nods[low_a] != 0) ? nods[low_a] : NULL;
        right = (nods[low_b] != 0) ? nods[low_b] : NULL;
        l_val = (left != NULL) ? left -> value : pars -> chrs[low_a];
        r_val = (right != NULL) ? right -> value : pars -> chrs[low_b];

        root = create_children(left, right);
        root = assign_vals(root, l_val, r_val);

        nods[low_a] = root;
        nods[low_b] = root;

        pars -> vals[low_a] = pars -> vals[low_a] + pars -> vals[low_b];
        pars -> vals[low_b] = pars -> vals[low_a];

        vl_counter++;
    }

    return root;
}


void set_memory(Node ***nods, params **pars, int size) {

    *nods = malloc(size * sizeof(Node *));
    *pars = malloc(sizeof(params));

    (*pars) -> min  = malloc(sizeof(int) * 2);
    (*pars) -> vals = malloc(sizeof(int) * size);
    (*pars) -> chrs = malloc(sizeof(char) * size + 1);

    memset(*nods, 0, size * sizeof(Node *));
    memset((*pars) -> min, 0, 2 * sizeof(int));
    memset((*pars) -> vals, 0, size * sizeof(int));
    memset((*pars) -> chrs, 0, size * sizeof(char));
    (*pars) -> chrs[size] = '\0';
}


unsigned char get_path(Node *curr, char target) {

    unsigned char value = 0b0;

    if (curr -> value == target) {
        Node *temp = curr -> parent;

        if (temp -> left == curr)
            return temp -> l_val;

        return temp -> r_val;
    }

    while (curr != NULL) {
        value += get_path(curr -> left, target);

        if (value == 0)
            value += get_path(curr -> right, target);
    }

    if (value == 0)
        return -1;

    return value;
}


void get_dictonary(params *pars, Node *root, int size) {

    Node *curr;
    int   i;
    curr = root;
    i    = 0;
    while (i < size) {
        pars -> vals[i] = get_path(curr, pars -> chrs[i]);
        i++;
    }
}

void write_data(char *input, params *pars, int size) {

    int i = 0;
    FILE *ptr = fopen("dummy", "wb");

    if (ptr == NULL)
        exit(1);

    while (i < size) {
        fwrite(&pars -> vals[i], 1, 1, ptr);
        i++;
    }

    fclose(ptr);
}

int main(void) {

    params *pars;
    char   *input;
    Node   *root;
    Node  **nods;
    int     size;

    input = file_process();
    size  = get_unique_size(input);

    set_memory(&nods, &pars, size);
    get_unique_vals(input, pars);
    root = construct_tree(root, nods, pars, size);
    get_dictonary(pars, root, size);
    write_data(input, pars, size);

    free(input);
    free(nods);
    free(pars -> chrs);
    free(pars -> vals);
    free(pars -> min);
    return 0;
}
