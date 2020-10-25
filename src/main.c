#include "tree23.h"
#define N 100

int main()
{
    tree23* root = NULL;
    for (int i = 1; i <= N; i++) {
        root = tree23_insert(root, i);
        printf("Добавлен узел %d\n", i);
    }
    tree23* t = NULL;
    for (int i = 1; i <= 100; i++) {
        t = tree23_lookup(root, i);
        if (!t) {
            printf("Узел %d не найден\n", i);
        } else {
            printf("Узел %d найден\n", i);
        }
    }
    for (int i = 1; i <= N; i++) {
        root = tree23_delete(root, i);
        t = tree23_lookup(root, i);
        if (!t) {
            printf("Узел %d был удалён\n", i);
        } else {
            printf("Узел %d не был удален\n", i);
        }
    }
    tree23_clear(&root);
    return 0;
}

