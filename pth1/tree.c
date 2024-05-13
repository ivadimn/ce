#include "tree.h"
#include "utils.h"
#include "sort.h"
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <sys/times.h>

static size_t count_nodes = 0;

static node_t*  _create_node(unsigned char* key, size_t value) {
    node_t* node = (node_t*) alloc(sizeof(node_t));
    node->data.key = (unsigned char*) alloc(sizeof(unsigned char*) * (strlen((char*)key) + 1));
    strcpy((char*)node->data.key, (char*)key);
    node->data.value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}
void tree_delete(node_t* tree) {
    if ( tree == NULL)
        return;
    if (tree->left)
        tree_delete(tree->left);
    if (tree->right)
        tree_delete(tree->right);
    free(tree);
}

void push(node_t** tree, unsigned char* key, size_t value) {
    
    node_t *tmp = *tree;
    int rc;
    
    if (tmp == NULL) {
        *tree = _create_node(key, value);
        return;
    }
    rc = CMP(tmp->data.key, key);
    if (rc == 0) {
        tmp->data.value += value;
    } else if (rc < 0) {
        if (tmp->left == NULL)
           tmp->left = _create_node(key, value);
        else
            push(&(tmp->left), key, value);
    } else {
        if (tmp->right == NULL)
           tmp->right = _create_node(key, value);
        else
            push(&(tmp->right), key, value);
    }
}

void print_tree(node_t* tree) {
    
    if (tree == NULL)
        return;
    print_tree(tree->left);
    print_tree(tree->right);
    printf("Key = %s, Value = %ld\n", tree->data.key, tree->data.value);

}
static void _calc_count(node_t* tree) {
    if (tree == NULL)
        return;
    if (tree->left)
        _calc_count(tree->left);
    if (tree->right)
        _calc_count(tree->right);
    count_nodes++;    
}

size_t tree_count_nodes(node_t* tree) {
    _calc_count(tree);
    size_t count = count_nodes;
    count_nodes = 0;
    return count;
}

void get_leafs(node_t *tree, unsigned char** keys, size_t* values) {
    static size_t index = 0;
    if (tree == NULL)
        return;
    if (tree->left)
        get_leafs(tree->left, keys, values);
    if (tree->right)
        get_leafs(tree->right, keys, values);
    keys[index] = tree->data.key;
    values[index] = tree->data.value;
    index++;
    

}

int main(int argc, char** argv) {
    time_t begin, end;
    node_t* root = NULL;
    size_t len = 1024 * 1024;
    unsigned char** keys = NULL;
    size_t* values = NULL; 
    size_t val;
    unsigned char key[16];

    for (size_t i = 0; i < len; i++) {
        val = rand();
        snprintf(key, 16, "%ld", val);
        push(&root, key, 1);
        memset(key, '\0', 16);
    }
    size_t count_nodes = tree_count_nodes(root);
    //print_tree(root);
    printf("len %lu\n", len);
    printf("count nodes %lu\n", count_nodes);

    
    keys = (unsigned char**) malloc(sizeof(unsigned char*) * count_nodes);
    values = (size_t*) malloc(sizeof(size_t) * count_nodes);
    get_leafs(root, keys, values);

    for (size_t i = 0; i < 10; i++) 
        printf("Key = %s, Value = %ld\n", keys[i], values[i]);

    printf("Sorting of Shell\n");
    begin = time(NULL);
    sort_shell(keys, values, 0, count_nodes - 1);
    end = time(NULL);
    printf("Time interval sorting of Shell %lf\n", difftime(end, begin));
    for (size_t i = count_nodes - 1; i > count_nodes - 10; i--) {
        printf("Key = %s, Value = %ld\n", keys[i], values[i]);
    }

    
    free(values);
    
    free(keys);
    tree_delete(root);
}