#ifndef TREE23_H
#define TREE23_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct tree23 {
	int size;
	int key[3];
	struct tree23* parent;
	struct tree23* child[4];
} tree23;

void tree23_clear(tree23** root);

int is_leaf(tree23* tree);

void swap(int* a, int* b);

void sort(tree23* tree);

void add_key(tree23* tree, int k);

tree23* split(tree23* tree);

tree23* tree23_min(tree23* p);

void delete_key(tree23* p, int k);

tree23* redistribute(tree23* leaf);

tree23* merge(tree23* leaf);

tree23* fix(tree23* leaf);

tree23* tree23_delete(tree23* p, int k);

tree23* tree23_insert(tree23* tree, int k);

tree23* tree23_lookup(tree23* tree, int k);

#endif
