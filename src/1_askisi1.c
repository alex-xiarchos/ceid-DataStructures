#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int *read_file();
int *bubble_sort(int *array, int n);
int *insertion_sort(int *array, int n);
int *selection_sort(int *array, int n);

int main() {
    int i = 0;
    int N = 100000; //μέγεθος πίνακα

    //BUBBLE SORT

    clock_t tic = clock();

    int *array = read_file();
    int *array_sorted = bubble_sort(array, N);

    clock_t toc = clock();
    double time_spent = (double) (toc - tic) / CLOCKS_PER_SEC;
    printf("Bubble Time: %lf\n", time_spent);

    //INSERTION SORT

    tic = clock();

    array = read_file();
    array_sorted = insertion_sort(array, N);

    toc = clock();
    time_spent = (double) (toc - tic) / CLOCKS_PER_SEC;
    printf("Insertion Time: %lf\n", time_spent);

    //SELECTION SORT

    tic = clock();

    array = read_file();
    array_sorted = selection_sort(array, N);

    toc = clock();
    time_spent = (double)(toc - tic)/CLOCKS_PER_SEC;
    printf("Selection Time: %lf\n", time_spent);


    return 0;
}

int *bubble_sort(int *array, int n) {

    int c, d, swap;
    printf("\n===[Bubble Sort]===\n");
    printf("===[Bubble Sort: ░░░░░░░░░░░░]===\n");

    for (c = 0; c < (n - 1); c++) {
        for (d = 0; d < n - c - 1; d++)
            if (array[d] > array[d + 1]) {
                swap = array[d];
                array[d] = array[d + 1];
                array[d + 1] = swap;
            }
        if (c == n / 4)
            printf("===[Bubble Sort: ███░░░░░░░░░]===\n");
        else if (c== n/2)
            printf("===[Bubble Sort: ██████░░░░░░]===\n");
        else if (c == 3*n/4)
            printf("===[Bubble Sort: █████████░░░]===\n");
    }

    printf("===[Bubble Sort: ████████████]===\n");

    return array;
}


int *insertion_sort(int *array, int n) {
    int i, j, temp;

    printf("\n===[Insertion Sort]===\n");

    for (i = 1; i <= n - 1; i++) {
        j = i;
        while (j > 0 && array[j] < array[j - 1]) {
            temp = array[j];
            array[j] = array[j - 1];
            array[j - 1] = temp;
            j--;
        }
    }

    return array;

}

int *selection_sort(int* array, int n) {
    int i, j, position, swap;

    printf("\n===[Selection Sort]===\n");

    for (i=0; i< n - 1; i++)
    {
        position = i;
        for (j=i+1; j<n; j++)
        {
            if (array[position] > array[j])
                position = j;
        }
        if (position != i)
        {
            swap = array[i];
            array[i] = array[position];
            array[position] = swap;
        }
    }

    return array;

}


int *read_file() {
    FILE *fpointer;
    fpointer = fopen("integers.txt", "r");
    FILE *fopen(const char *filename, const char *mode);

    int *array = (int *) malloc(100000 * sizeof(int));
    char singleline[10];
    int number;
    int i = 0;

    while (!feof(fpointer)) {
        fgets(singleline, 10, fpointer);
        number = atoi(singleline);
        array[i++] = number;
    }

    fclose(fpointer);

    return array;
}
