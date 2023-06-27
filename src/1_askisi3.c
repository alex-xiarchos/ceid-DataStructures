#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int *read_file();

void adjust(int *a, int i, int n);

void heapify(int *a, int n);

int *heap_sort(int *a, int n);

int main() {
    int i = 0, N = 100000;
    clock_t tic = clock();

    int *array = read_file();

    int *array_sorted = heap_sort(array, N);

    clock_t toc = clock();
    double time_spent = (double) (toc - tic) / CLOCKS_PER_SEC;
    printf("\nHeap Time: %lf\n", time_spent);
}


void adjust(int *a, int i, int n) {
    int j, item;
    j = 2 * i;
    item = a[i];
    while (j <= n) {
        if (j < n && a[j] < a[j + 1])
            j++;
        if (item > a[j])
            break;
        a[j / 2] = a[j];
        j = 2 * j;
    }
    a[j / 2] = item;
}

void heapify(int *a, int n) {
    int i;
    for (i = n / 2; i >= 1; i--) {
        adjust(a, i, n);
    }
}

int *heap_sort(int *a, int n) {
    int i, temp;
    heapify(a, n);
    for (i = n; i >= 2; i--) {
        temp = a[1];
        a[1] = a[i];
        a[i] = temp;
        adjust(a, 1, i - 1);
    }
    printf("\n\nSorted list in ascending order:\n");
    return a;
}

int *read_file() {
    FILE *fpointer;
    fpointer = fopen("integers.txt", "r");
    FILE *fopen(const char *filename, const char *mode);

    int *array = (int *) malloc(100000 * sizeof(int));
    char singleline[10];
    int number;
    int i = 0, k = 0;

    while (!feof(fpointer)) {
        fgets(singleline, 10, fpointer);
        number = atoi(singleline);
        array[i++] = number;
    }

    fclose(fpointer);
    return array;
}
