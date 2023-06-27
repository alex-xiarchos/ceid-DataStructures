#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int *read_file();

int linear_search(int *array, int n, int x);

int binary_search(int *array, int first, int last, int x);

int interpolation_search(int *array, int n, int item);

int main() {
    int target;
    int N = 100000;

    int *array1 = read_file();

    //LINEAR SEARCH

    printf("Enter the number to search\n");
    scanf("%d", &target);
    clock_t tic = clock();

    int result = linear_search(array1, N, target);

    clock_t toc = clock();
    if (result == -1) {
        printf("%d is not present in array.\n", target);
    } else {
        printf("%d is present at %d", target, result);
    }
    double time_spent = (double) (toc - tic);
    printf("\nLinear Time: %lf\n", time_spent);

    //BINARY SEARCH

    printf("Enter the number to search\n");
    scanf("%d", &target);
    clock_t tic1 = clock();
    int result1 = binary_search(array1, 0, N, target);
    clock_t toc1 = clock();
    if (result1 == -1) {
        printf("%d is not present in array.\n", target);
    } else {
        printf("%d is present at %d", target, result1);
    }
    double time_spent1 = (double) (toc1 - tic1);
    printf("\nBinary Time: %lf\n", time_spent1);

    //INTERPOLATION SEARCH

    printf("Enter the number to search\n");
    scanf("%d", &target);
    clock_t tic2 = clock();
    int pos = interpolation_search(array1, N, target);
    clock_t toc2 = clock();
    if (pos == -1) {
        printf("%d is not present in array.\n", target);
    } else {
        printf("%d is present at %d", target, pos);
    }
    double time_spent2 = (double) (toc2 - tic2);
    printf("\nInterpolation Time: %lf\n", time_spent2);
}

int linear_search(int *array, int n, int x) {
    int i;

    for (i = 0; i < n; i++) {
        if (array[i] == x)
            return i + 1;
    }
    return -1;
}


int binary_search(int *array, int first, int last, int x) {
    if (last >= 1) {
        int mid = first + (last - first) / 2;
        if (array[mid] == x)
            return mid + 1;
        if (array[mid] > x)
            return binary_search(array, first, mid - 1, x);
        return binary_search(array, mid + 1, last, x);
    }
    return -1;
}


int interpolation_search(int *array, int n, int item) {
    int bottom = 0;
    int top = n - 1;
    while (bottom <= top && item >= array[bottom] && item <= array[top]) {
        if (bottom == top) {
            if (array[bottom] == item)
                return bottom + 1;
            return -1;
        }
        int mid = bottom + (((double) (top - bottom) / (array[top] - array[bottom])) * (item - array[bottom]));
        if (array[mid] == item)
            return mid + 1;
        if (array[mid] < item)
            bottom = mid + 1;
        else
            top = mid - 1;
    }
    return -1;
}


int *read_file() {
    FILE *fpointer;
    fpointer = fopen("integers sorted.txt", "r");
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
