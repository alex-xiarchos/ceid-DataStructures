#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

int* read_file();
int BIS(int* array, int n, int x);
int BIS_improved(int* array, int n, int x);

int main()
{
    int target;
    int N=100000;

    int *array2 = read_file();

    //Binary Interpolation Search (BIS)

    printf ("Enter the number to search:\n");
    scanf ("%d", &target);
    clock_t tic = clock();

    int result = BIS(array2, N, target);

    clock_t toc = clock();
    if (result == -1)
        printf ("%d is not present in array.\n", target);
    else
        printf ("%d is present at %d", target, result);
    double time_spent = (double)(toc - tic);
    printf ("\nBIS Time: %lf\n", time_spent);

    //Improved BIS

    printf ("Enter the number to search:\n");
    scanf ("%d", &target);
    clock_t tic1 = clock();

    int result1 = BIS_improved(array2, N, target);

    clock_t toc1 = clock();
    if (result1 == -1)
        printf ("%d is not present in array.\n", target);
    else
        printf ("%d is present at %d", target, result1);
    double time_spent1 = (double)(toc1 - tic1);
    printf ("\nBIS Time: %lf\n", time_spent1);
}

int BIS(int* array, int n, int x)
{
    int bottom = 1;
    int top = n;
    int mid = n*((x - array[bottom]) / (double)(array[top] - array[bottom])) + 1;
    if (mid > n)
        mid = n;
    else if (mid < 1)
        mid = 1;
    while (x != array[mid]) {
        int i = 0;
        n = top - bottom + 1;
        if (n <= 3) {
            int j = 0;
            for (j = 0; j < n; j++) {
                if (array[bottom + j] == x)
                    return bottom + j;
            }
            return -1;
        }
        if (x >= array[mid]) {
            while (x > array[mid + i*(int)(sqrt(n)) - 1]) {
                i++;
            }
            top = mid + i*(int)(sqrt(n));
            bottom = mid + (i - 1)*(int)(sqrt(n));
        }
        else {
            while (x < array[(int)(mid - i*(int)(sqrt(n)) + 1)]) {
                i++;
            }
            top = mid - (i - 1)*(int)(sqrt(n));
            bottom = mid - i*(int)(sqrt(n));
        }
        mid = bottom + (top - bottom + 1)*((x - array[bottom]) / (double)(array[top] - array[bottom]));
    }
    if (x == array[mid])
        return mid + 1;
    else
        return -1;
}


int BIS_improved(int* array, int n, int x)
{
    int bottom = 1;
    int top = n;
    int mid = n*((x - array[bottom]) / (double)(array[top] - array[bottom])) + 1;
    if (mid > n)
        mid = n;
    else if (mid < 1)
        mid = 1;
    while (x != array[mid]) {
        int i = 0;
        n = top - bottom + 1;
        if (n <= 3) {
            int j = 0;
            for (j = 0; j < n; j++) {
                if (array[bottom + j] == x)
                    return bottom + j;
            }
            return -1;
        }
        if (x >= array[mid]) {
            while (x > array[mid + i*(int)(sqrt(n)) - 1]) {
                i = 2*i;
            }
            top = mid + i*(int)(sqrt(n));
            bottom = mid + (i - 1)*(int)(sqrt(n));
        }
        else {
            while (x < array[(int)(mid - i*(int)(sqrt(n)) + 1)]) {
                i = 2*i;
            }
            top = mid - (i - 1)*(int)(sqrt(n));
            bottom = mid - i*(int)(sqrt(n));
        }
        mid = bottom + (top - bottom + 1)*((x - array[bottom]) / (double)(array[top] - array[bottom]));
    }
    if (x == array[mid])
        return mid + 1;
    else
        return -1;
}



int* read_file() {
    FILE *fpointer;
    fpointer = fopen("integers sorted.txt", "r");
    FILE *fopen(const char *filename, const char *mode);

    int *array = (int*)malloc(100000*sizeof(int));
    char singleline[10];
    int number;
    int i=0,k=0;

    while(!feof(fpointer)) {
        fgets(singleline, 10, fpointer);
        number = atoi(singleline);
        array[i++] = number;
    }

    fclose(fpointer);

    return array;
}
