#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int *read_file();

int *merge(int *array, int left, int mid, int right);

int *merge_sort(int *array, int left, int right);

void swap(int *a, int *b);

int partition(int array[], int low, int high);

void quickSort(int arr[], int low, int high);

int main() {
    int i = 0;
    int N = 100000;

    //MERGE SORT

    clock_t tic = clock();

    int *array = read_file();
    int *array_sorted = merge_sort(array, 0, N - 1);

    clock_t toc = clock();
    double time_spent = (double) (toc - tic) / CLOCKS_PER_SEC;

    printf("\n===[Merge Sort]===\n");
    printf("Merge Time: %f\n", time_spent);

    //QUICK SORT

    clock_t tic2 = clock();

    int *array_2 = read_file();
    quickSort(array_2, 0, N - 1);

    clock_t toc2 = clock();
    double time_spent_2 = (double) (toc2 - tic2) / CLOCKS_PER_SEC;

    printf("\n===[Quick Sort]===\n");
    printf("Quick Time: %lf\n", time_spent_2);
}

int *merge(int *array, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;    // μέγεθος temp_left πίνακα
    int n2 = right - mid;       // μέγεθος temp_right πίνακα

    int temp_left[n1], temp_right[n2];  // δημιουργία temp arrays

    for (i = 0; i < n1; i++)
        temp_left[i] = array[left + i]; // προσθήκη δεδομένων σε temp_left
    for (j = 0; j < n2; j++)
        temp_right[j] = array[mid + 1 + j];  // προσθήκη δεδομένων σε temp_right

    i = 0; // αρχικός δείκτης temp_left
    j = 0; // αρχικός δείκτης temp_right
    k = left; // αρχικός δείκτης merged πίνακα

    while (i < n1 && j < n2) {
        if (temp_left[i] <= temp_right[j]) {
            array[k] = temp_left[i];
            i++;
        } else {
            array[k] = temp_right[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        array[k] = temp_left[i];
        i++;
        k++;
    }

    while (j < n2) {
        array[k] = temp_right[j];
        j++;
        k++;
    }

    return array;
}

int *merge_sort(int *array, int left, int right) {
    int *sorted_array;
    if (left < right) {
        int mid = (left + right) / 2;

        merge_sort(array, left, mid);
        merge_sort(array, mid + 1, right);

        sorted_array = merge(array, left, mid, right);
    }

    return sorted_array;
}

void swap(int *a, int *b) {
    int c = *a;
    *a = *b;
    *b = c;
}

int partition(int array[], int low, int high) {
    int pivot = array[high];    // pivot
    int i = (low - 1);  // Index of smaller element
    int j;

    for (j = low; j <= high - 1; j++) {

        if (array[j] <= pivot) {
            i++;
            swap(&array[i], &array[j]);
        }
    }
    swap(&array[i + 1], &array[high]);
    return (i + 1);
}


void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
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
