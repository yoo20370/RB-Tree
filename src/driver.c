#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

void rbtree_to_print(node_t *t, node_t *nil);

int main(int argc, char *argv[])
{
    rbtree *t = new_rbtree();
    for (int i = 1; i <= 15; i ++) {
        rbtree_insert(t, i);
    }
    rbtree_to_print(t->root, t->nil);
    return 0;
}