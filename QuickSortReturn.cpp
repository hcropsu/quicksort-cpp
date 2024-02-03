#include <iostream>
#include <random>
#include <omp.h>
#include <fstream>

using namespace std;

// This program has the following sorting algorithms:
//
// insertionSort(): Basic insertionSort algorithm used in
// hybridSort() together with quickSortMOTforHybridSort()
// 
// quickSort(): Basic quickSort algorithm
// 
// quickSortMOT(): quickSort that uses the Median of Three 
// -method for choosing the pivot element
// 
// quickSortMOTforHybridSort(): same as above except there 
// is a cutoff value M for returning early. This is a separate 
// function from the above for practical purposes.
// This way the three functions (algorithms): quickSort, 
// quickSort with Median of Three and hybridSort with 
// a cutoff for changing from quickSort with Median of three 
// to insertionSort can be compared with a single
// run of the program, and they all sort the same array.
// 
// hybridSort(): calls quickSortMOTforHybridSort() first 
// and then insertionSort()


// createIntArray() creates an array containing 75000 elements between 0-1000000.
// It makes the distribution more even than using rand() and modulo -operator.



// To create  a more uniformly distributed array
// than by using rand() and modulo:
int* createIntArray() {
    int* array = new int[75000];
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<> dis(0, 1000000);
    for (int i = 0; i < 75000; i++) {
        array[i] = dis(gen);
    }
    return array;
}


void insertionSort(int a[], int left, int right) {
    int i;
    int j;
    int key;
    for (i = left + 1; i <= right; i++) {
        key = a[i];
        j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}


void quickSort(int a[], int left, int right) {
    int start;
    int end;
    int pivot;
    int temp;
    if (right > left) {

        // If not using Median of three, pivot
        // is always the rightmost element, start
        // is left - 1 and end is the rightmost index
        pivot = a[right];
        start = left - 1;
        end = right;

        for (;;) {
            while (start < end && a[++start] < pivot) {
                if (start == right) { break; }
            }
            while (a[--end] > pivot) {
                if (end == left) { break; }
            }
            if (start >= end) { break; }
            temp = a[start];
            a[start] = a[end];
            a[end] = temp;
        }

        // For use without median of three
        temp = a[start];
        a[start] = a[right];
        a[right] = temp;

        quickSort(a, left, start - 1);
        quickSort(a, start + 1, right);
    }
}


void quickSortMOT(int a[], int left, int right) {
    int start;
    int end;
    int pivot;
    int temp;
    if (right - left <= 0) { return; }
    // Median of three below:
    // 1) Order the first, middle and last
    // elements correctly. Inline swapping seems
    // faster than calling separate swap()
    int middle = (left + right) / 2;
    if (a[left] > a[middle]) {
        temp = a[left];
        a[left] = a[middle];
        a[middle] = temp;
    }
    if (a[left] > a[right]) {
        temp = a[left];
        a[left] = a[right];
        a[right] = temp;
    }
    if (a[middle] > a[right]) {
        temp = a[middle];
        a[middle] = a[right];
        a[right] = temp;
    }

    // 2) Swap middle element with next-to-last
    temp = a[middle];
    a[middle] = a[right - 1];
    a[right - 1] = temp;

    // 3) Set start and end indexes correctly
    // (due to Median of three: first element is
    // already smaller than pivot and last is greater) and 
    // pivot to be the next-to-last element
    pivot = a[right - 1];
    start = left;
    end = right - 1;

    for (;;) {
        while (start < end && a[++start] < pivot) {
            if (start == right) { break; }
        }
        while (a[--end] > pivot) {
            if (end == left) { break; }
        }
        if (start >= end) { break; }
        temp = a[start];
        a[start] = a[end];
        a[end] = temp;
    }

    // For median of three
    temp = a[start];
    a[start] = a[right - 1];
    a[right - 1] = temp;

    quickSortMOT(a, left, start - 1);
    quickSortMOT(a, start + 1, right);
}


void quickSortMOTforHybridSort(int a[], int left, int right, int M) {
    int start;
    int end;
    int pivot;
    int temp;
    if (right - left <= M) { return; }
        // Median of three below:
        // 1) Order the first, middle and last
        // elements correctly. Inline swapping seems
        // faster than calling separate swap()
        int middle = (left + right) / 2;
        if (a[left] > a[middle]) {
            temp = a[left];
            a[left] = a[middle];
            a[middle] = temp;
        }
        if (a[left] > a[right]) {
            temp = a[left];
            a[left] = a[right];
            a[right] = temp;
        }
        if (a[middle] > a[right]) {
            temp = a[middle];
            a[middle] = a[right];
            a[right] = temp;
        }

        // 2) Swap middle element with next-to-last
        temp = a[middle];
        a[middle] = a[right - 1];
        a[right - 1] = temp;

        // 3) Set start and end indexes correctly
        // (due to Median of three: first element is
        // already smaller than pivot and last is greater) and 
        // pivot to be the next-to-last element
        pivot = a[right - 1];
        start = left;
        end = right - 1;

        for (;;) {
            while (start < end && a[++start] < pivot) {
                if (start == right) { break; }
            }
            while (a[--end] > pivot) {
                if (end == left) { break; }
            }
            if (start >= end) { break; }
            temp = a[start];
            a[start] = a[end];
            a[end] = temp;
        }

        // For median of three
        temp = a[start];
        a[start] = a[right - 1];
        a[right - 1] = temp;

        quickSortMOTforHybridSort(a, left, start - 1, M);
        quickSortMOTforHybridSort(a, start + 1, right, M);
}

void hybridSort(int a[], int left, int right, int M) {
    quickSortMOTforHybridSort(a, left, right, M);
    insertionSort(a, left, right);
}

int main()
{
    // doubles for measuring sorting time
    double begin;
    double stop;


    // create an array and two copies for
    // comparing time to sort between different algorithms
    int* a1 = createIntArray();
    int* a2 = new int[75000];
    int* a3 = new int[75000];

    for (int i = 0; i < 75000; i++) {
        a2[i] = a1[i];
        a3[i] = a1[i];
    }

    // output original unsorted array for debugging
    // and sorting in Excel for example
    ofstream originalArray("original_array.txt");
    if (originalArray.is_open()) {
        for (int i = 0; i < 75000; i++) {
            originalArray << a1[i] << endl;
        }
        originalArray.close();
    }
    else {
        cout << "Couldn't open file." << endl;
    }


    // measure time to sort using quickSort()
    // and output sorted array to file
    begin = omp_get_wtime();
    quickSort(a1, 0, 74999);
    stop = omp_get_wtime();
    cout << "Sorting with quickSort() took: " << stop - begin << " seconds" << endl;

    ofstream quickSortFile("quicksorted_array.txt");
    if (quickSortFile.is_open()) {
        for (int i = 0; i < 75000; i++) {
            quickSortFile << a1[i] << endl;
        }
        quickSortFile.close();
    }
    else {
        cout << "Couldn't open file." << endl;
    }


    // measure time to sort using quickSortMOT()
    // and output sorted array to file
    begin = omp_get_wtime();
    quickSortMOT(a2, 0, 74999);
    stop = omp_get_wtime();
    cout << "Sorting with quickSortMOT() took: " << stop - begin << " seconds" << endl;

    ofstream quickSortMOTFile("quicksortedMOT_array.txt");
    if (quickSortMOTFile.is_open()) {
        for (int i = 0; i < 75000; i++) {
            quickSortMOTFile << a2[i] << endl;
        }
        quickSortMOTFile.close();
    }
    else {
        cout << "Couldn't open file." << endl;
    }


    // measure time to sort using hybridSort()
    // and output sorted array to file
    begin = omp_get_wtime();
    hybridSort(a3, 0, 74999, 2);
    stop = omp_get_wtime();
    cout << "Sorting with hybridSort() where 'M' is 2" << " took: " << stop - begin << " seconds" << endl;

    ofstream hybridSortFile("hybridsorted_array.txt");
    if (hybridSortFile.is_open()) {
        for (int i = 0; i < 75000; i++) {
            hybridSortFile << a3[i] << endl;
        }
        hybridSortFile.close();
    }
    else {
        cout << "Couldn't open file." << endl;
    }
    
    // create a new array for testing different values of M
    // in order to try and determine fastest value
    int* arr = createIntArray();
    int M_max = 200;

    // make a copy of 'arr' for each value of M from 2 to M_max
    // and sort it and measure time to sort
    for (int M = 2; M <= M_max; M++) {
        int* arr_copy = new int[75000];
        copy(arr, arr + 75000, arr_copy);

        begin = omp_get_wtime();
        hybridSort(arr_copy, 0, 74999, M);
        stop = omp_get_wtime();
       
        cout << "Sorting with hybridSort() where 'M' is " << M << " took: " << stop - begin << " seconds" << endl;

        delete[] arr_copy;
    }

    delete[] arr;
    delete[] a1;
    delete[] a2;
    delete[] a3;
    return 0;
}