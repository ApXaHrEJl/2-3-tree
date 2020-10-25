#include "tree23.h"

void tree23_clear(tree23** tree)
{
    if (!(*tree)) {
        return;
    }
    int i;
    for (i = 0; i < (*tree)->size; i++) {
        tree23_clear(&(*tree)->child[i]);
    }
    free(*tree);
    *tree = NULL;
}

tree23* tree23_lookup(tree23* tree, int k)
{
    if (!tree)
        return NULL;
    int found = 0, i;
    for (i = 0; i < tree->size; i++) {
        if (tree->key[i] == k) {
            found = 1;
            break;
        }
    }
    if (found) {
        return tree;
    } else if (k < tree->key[0]) {
        return tree23_lookup(tree->child[0], k);
    } else if (((tree->size == 2) && (k < tree->key[1])) || (tree->size == 1)) {
        return tree23_lookup(tree->child[1], k);
    } else if (tree->size == 2) {
        return tree23_lookup(tree->child[2], k);
    }
    return NULL;
}

int is_leaf(tree23* tree)
{
    return (tree->child[0] == NULL) && (tree->child[1] == NULL)
            && (tree->child[2] == NULL);
}

void swap(int* a, int* b)
{
    int buf = *a;
    *a = *b;
    *b = buf;
}

void sort(tree23* tree)
{
    if (tree->size == 1) {
        return;
    }
    if (tree->size == 2) {
        if (tree->key[0] > tree->key[1]) {
            swap(&tree->key[0], &tree->key[1]);
        }
    } else {
        if (tree->key[0] > tree->key[1]) {
            swap(&tree->key[0], &tree->key[1]);
        }
        if (tree->key[0] > tree->key[2]) {
            swap(&tree->key[0], &tree->key[2]);
        }
        if (tree->key[1] > tree->key[2]) {
            swap(&tree->key[1], &tree->key[2]);
        }
    }
}

void add_key(tree23* tree, int k)
{
    tree->key[tree->size++] = k;
    sort(tree);
}

tree23* split(tree23* tree)
{
    tree23* x = (tree23*)malloc(sizeof(tree23));
    tree23* y = (tree23*)malloc(sizeof(tree23));
    if (!x || !y) {
        exit(1);
    }
    x->size = 1;
    x->key[0] = tree->key[0];
    x->child[0] = tree->child[0];
    x->child[1] = tree->child[1];
    x->child[2] = NULL;
    x->child[3] = NULL;
    x->parent = tree->parent;
    y->size = 1;
    y->key[0] = tree->key[2];
    y->child[0] = tree->child[2];
    y->child[1] = tree->child[3];
    y->child[2] = NULL;
    y->child[3] = NULL;
    y->parent = tree->parent;
    if (x->child[0]) {
        x->child[0]->parent = x;
    }
    if (x->child[1]) {
        x->child[1]->parent = x;
    }
    if (y->child[0]) {
        y->child[0]->parent = y;
    }
    if (y->child[1]) {
        y->child[1]->parent = y;
    }
    if (tree->parent) {
        add_key(tree->parent, tree->key[1]);
        if (tree->parent->child[0] == tree) {
            tree->parent->child[0] = NULL;
        } else if (tree->parent->child[1] == tree) {
            tree->parent->child[1] = NULL;
        } else if (tree->parent->child[2] == tree) {
            tree->parent->child[2] = NULL;
        }
        if (tree->parent->child[0] == NULL) {
            tree->parent->child[3] = tree->parent->child[2];
            tree->parent->child[2] = tree->parent->child[1];
            tree->parent->child[1] = y;
            tree->parent->child[0] = x;
        } else if (tree->parent->child[1] == NULL) {
            tree->parent->child[3] = tree->parent->child[2];
            tree->parent->child[2] = y;
            tree->parent->child[1] = x;
        } else {
            tree->parent->child[3] = y;
            tree->parent->child[2] = x;
        }
        tree23* buf = tree->parent;
        free(tree);
        return buf;
    } else {
        x->parent = tree;
        y->parent = tree;
        tree->key[0] = tree->key[1];
        tree->size = 1;
        tree->child[0] = x;
        tree->child[1] = y;
        tree->child[2] = NULL;
        tree->child[3] = NULL;
        return tree;
    }
}

tree23* tree23_insert(tree23* tree, int k)
{
    if (!tree) {
        tree = (tree23*)malloc(sizeof(tree23));
        if (!tree) {
            exit(1);
        }
        tree->size = 1;
        tree->key[0] = k;
        tree->parent = NULL;
        int i;
        for (i = 0; i < 4; i++) {
            tree->child[i] = NULL;
        }
        return tree;
    }
    if (is_leaf(tree)) {
        add_key(tree, k);
    } else if (k < tree->key[0]) {
        tree23_insert(tree->child[0], k);
    } else if ((tree->size == 1) || ((tree->size == 2) && (k < tree->key[1]))) {
        tree23_insert(tree->child[1], k);
    } else {
        tree23_insert(tree->child[2], k);
    }
    if (tree->size <= 2) {
        return tree;
    }
    return split(tree);
}

tree23* tree23_min(tree23* p)
{
    if (!p)
        return p;
    if (!(p->child[0]))
        return p;
    else
        return tree23_min(p->child[0]);
}

void delete_key(tree23* p, int k)
{
    if (p->size >= 1 && p->key[0] == k) {
        p->key[0] = p->key[1];
        p->key[1] = p->key[2];
        p->size--;
    } else if (p->size == 2 && p->key[1] == k) {
        p->key[1] = p->key[2];
        p->size--;
    }
}

tree23* redistribute(tree23* node)
{
    tree23* parent = node->parent;
    tree23* child[3] = {parent->child[0], parent->child[1], parent->child[2]};
    if ((parent->size == 2) && (child[0]->size < 2) && (child[1]->size < 2)
        && (child[2]->size < 2)) {
        if (child[0] == node) {
            parent->child[0] = parent->child[1];
            parent->child[1] = parent->child[2];
            parent->child[2] = NULL;
            add_key(parent->child[0], parent->key[0]);
            parent->child[0]->child[2] = parent->child[0]->child[1];
            parent->child[0]->child[1] = parent->child[0]->child[0];
            if (node->child[0] != NULL)
                parent->child[0]->child[0] = node->child[0];
            else if (node->child[1] != NULL)
                parent->child[0]->child[0] = node->child[1];
            if (parent->child[0]->child[0] != NULL)
                parent->child[0]->child[0]->parent = parent->child[0];
            delete_key(parent, parent->key[0]);
            free(child[0]);
        } else if (child[1] == node) {
            add_key(child[0], parent->key[0]);
            delete_key(parent, parent->key[0]);
            if (node->child[0] != NULL)
                child[0]->child[2] = node->child[0];
            else if (node->child[1] != NULL)
                child[0]->child[2] = node->child[1];
            if (child[0]->child[2] != NULL)
                child[0]->child[2]->parent = child[0];
            parent->child[1] = parent->child[2];
            parent->child[2] = NULL;
            free(child[1]);
        } else if (child[2] == node) {
            add_key(child[1], parent->key[1]);
            parent->child[2] = NULL;
            delete_key(parent, parent->key[1]);
            if (node->child[0] != NULL)
                child[1]->child[2] = node->child[0];
            else if (node->child[1] != NULL)
                child[1]->child[2] = node->child[1];
            if (child[1]->child[2] != NULL)
                child[1]->child[2]->parent = child[1];
            free(child[2]);
        }
    } else if (
            (parent->size == 2)
            && ((child[0]->size == 2) || (child[1]->size == 2)
                || (child[2]->size == 2))) {
        if (child[2] == node) {
            if (node->child[0] != NULL) {
                node->child[1] = node->child[0];
                node->child[0] = NULL;
            }
            add_key(node, parent->key[1]);
            if (child[1]->size == 2) {
                parent->key[1] = child[1]->key[1];
                delete_key(child[1], child[1]->key[1]);
                node->child[0] = child[1]->child[2];
                child[1]->child[2] = NULL;
                if (node->child[0] != NULL)
                    node->child[0]->parent = node;
            } else if (child[0]->size == 2) {
                parent->key[1] = child[1]->key[0];
                node->child[0] = child[1]->child[1];
                child[1]->child[1] = child[1]->child[0];
                if (node->child[0] != NULL)
                    node->child[0]->parent = node;

                child[1]->key[0] = parent->key[0];
                parent->key[0] = child[0]->key[1];
                delete_key(child[0], child[0]->key[1]);
                child[1]->child[0] = child[0]->child[2];
                if (child[1]->child[0] != NULL)
                    child[1]->child[0]->parent = child[1];
                child[0]->child[2] = NULL;
            }
        } else if (child[1] == node) {
            if (child[2]->size == 2) {
                if (node->child[0] == NULL) {
                    node->child[0] = node->child[1];
                    node->child[1] = NULL;
                }
                add_key(child[1], parent->key[1]);
                parent->key[1] = child[2]->key[0];
                delete_key(child[2], child[2]->key[0]);
                child[1]->child[1] = child[2]->child[0];
                if (child[1]->child[1] != NULL)
                    child[1]->child[1]->parent = child[1];
                child[2]->child[0] = child[2]->child[1];
                child[2]->child[1] = child[2]->child[2];
                child[2]->child[2] = NULL;
            } else if (child[0]->size == 2) {
                if (node->child[1] == NULL) {
                    node->child[1] = node->child[0];
                    node->child[0] = NULL;
                }
                add_key(child[1], parent->key[0]);
                parent->key[0] = child[0]->key[1];
                delete_key(child[0], child[0]->key[1]);
                child[1]->child[0] = child[0]->child[2];
                if (child[1]->child[0] != NULL)
                    child[1]->child[0]->parent = child[1];
                child[0]->child[2] = NULL;
            }
        } else if (child[0] == node) {
            if (node->child[0] == NULL) {
                node->child[0] = node->child[1];
                node->child[1] = NULL;
            }
            add_key(child[0], parent->key[0]);
            if (child[1]->size == 2) {
                parent->key[0] = child[1]->key[0];
                delete_key(child[1], child[1]->key[0]);
                child[0]->child[1] = child[1]->child[0];
                if (child[0]->child[1] != NULL)
                    child[0]->child[1]->parent = child[0];
                child[1]->child[0] = child[1]->child[1];
                child[1]->child[1] = child[1]->child[2];
                child[1]->child[2] = NULL;
            } else if (child[2]->size == 2) {
                parent->key[0] = child[1]->key[0];
                child[1]->key[0] = parent->key[1];
                parent->key[1] = child[2]->key[0];
                delete_key(child[2], child[2]->key[0]);
                child[0]->child[1] = child[1]->child[0];
                if (child[0]->child[1] != NULL)
                    child[0]->child[1]->parent = child[0];
                child[1]->child[0] = child[1]->child[1];
                child[1]->child[1] = child[2]->child[0];
                if (child[1]->child[1] != NULL)
                    child[1]->child[1]->parent = child[1];
                child[2]->child[0] = child[2]->child[1];
                child[2]->child[1] = child[2]->child[2];
                child[2]->child[2] = NULL;
            }
        }
    } else if (parent->size == 1) {
        if (child[0] == node && child[1]->size == 2) {
            parent->key[0] = child[1]->key[0];
            delete_key(child[1], child[1]->key[0]);
            if (node->child[0] == NULL)
                node->child[0] = node->child[1];
            node->child[1] = child[1]->child[0];
            child[1]->child[0] = child[1]->child[1];
            child[1]->child[1] = child[1]->child[2];
            child[1]->child[2] = NULL;
            if (node->child[1] != NULL)
                node->child[1]->parent = node;
        } else if (child[1] == node && child[0]->size == 2) {
            parent->key[0] = child[0]->key[1];
            delete_key(child[0], child[0]->key[1]);
            if (node->child[1] == NULL)
                node->child[1] = node->child[0];
            node->child[0] = child[0]->child[2];
            child[0]->child[2] = NULL;
            if (node->child[0] != NULL)
                node->child[0]->parent = node;
        }
    }
    return parent;
}

tree23* merge(tree23* node)
{
    tree23* parent = node->parent;
    if (parent->child[0] == node) {
        add_key(parent->child[1], parent->key[0]);
        parent->child[1]->child[2] = parent->child[1]->child[1];
        parent->child[1]->child[1] = parent->child[1]->child[0];
        if (node->child[0] != NULL)
            parent->child[1]->child[0] = node->child[0];
        else if (node->child[1] != NULL)
            parent->child[1]->child[0] = node->child[1];
        if (parent->child[1]->child[0] != NULL)
            parent->child[1]->child[0]->parent = parent->child[1];
        delete_key(parent, parent->key[0]);
        free(parent->child[0]);
        parent->child[0] = NULL;
    } else if (parent->child[1] == node) {
        add_key(parent->child[0], parent->key[0]);
        if (node->child[0] != NULL)
            parent->child[0]->child[2] = node->child[0];
        else if (node->child[1] != NULL)
            parent->child[0]->child[2] = node->child[1];
        if (parent->child[0]->child[2] != NULL)
            parent->child[0]->child[2]->parent = parent->child[0];
        delete_key(parent, parent->key[0]);
        free(parent->child[1]);
        parent->child[1] = NULL;
    }
    if (parent->parent == NULL) {
        tree23* buf = NULL;
        if (parent->child[0] != NULL)
            buf = parent->child[0];
        else
            buf = parent->child[1];
        buf->parent = NULL;
        free(parent);
        return buf;
    }
    return parent;
}

tree23* fix(tree23* node)
{
    if (node->size == 0 && node->parent == NULL) {
        free(node);
        return NULL;
    }
    if (node->size != 0) {
        if (node->parent)
            return fix(node->parent);
        else
            return node;
    }
    tree23* parent = node->parent;
    if (parent->child[0]->size == 2 || parent->child[1]->size == 2
        || parent->size == 2)
        node = redistribute(node);
    else if (parent->size == 2 && parent->child[2]->size == 2)
        node = redistribute(node);
    else
        node = merge(node);
    return fix(node);
}

tree23* tree23_delete(tree23* p, int k)
{
    tree23* item = tree23_lookup(p, k);
    if (!item)
        return p;
    tree23* min = NULL;
    if (item->key[0] == k)
        min = tree23_min(item->child[1]);
    else
        min = tree23_min(item->child[2]);
    if (min) {
        int* z = (k == item->key[0] ? &(item->key[0]) : &(item->key[1]));
        swap(z, &min->key[0]);
        item = min;
    }
    delete_key(item, k);
    return fix(item);
}
