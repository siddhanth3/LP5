#include <iostream>   // Used for input and output
#include <vector>     // Used for dynamic array (vector)
#include <omp.h>      // OpenMP library for parallel programming

using namespace std;

// =====================================================
// Sequential Bubble Sort
// This sorting works using only ONE thread
// =====================================================
void bubbleSortSeq(vector<int>& arr) {

    // Get size of array
    int n = arr.size();

    // Outer loop controls number of passes
    for (int i = 0; i < n - 1; i++) {

        // Inner loop compares adjacent elements
        for (int j = 0; j < n - i - 1; j++) {

            // If left element is greater, swap them
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// =====================================================
// Parallel Bubble Sort
// Uses multiple threads for faster execution
// =====================================================
void bubbleSortParallel(vector<int>& arr) {

    int n = arr.size();

    // Repeat sorting phases
    for (int i = 0; i < n; i++) {

        // ---------------- EVEN PHASE ----------------
        // Compare elements at even indexes
        // Example: (0,1), (2,3), (4,5)

        #pragma omp parallel for
        for (int j = 0; j < n - 1; j += 2) {

            // Swap if elements are in wrong order
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }

        // ---------------- ODD PHASE ----------------
        // Compare elements at odd indexes
        // Example: (1,2), (3,4), (5,6)

        #pragma omp parallel for
        for (int j = 1; j < n - 1; j += 2) {

            // Swap if needed
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// =====================================================
// Merge Function
// Combines two sorted halves into one sorted array
// =====================================================
void merge(vector<int>& arr, int l, int m, int r) {

    // Store left half in temporary vector
    vector<int> left(arr.begin() + l, arr.begin() + m + 1);

    // Store right half in temporary vector
    vector<int> right(arr.begin() + m + 1, arr.begin() + r + 1);

    // i -> left array index
    // j -> right array index
    // k -> original array index
    int i = 0, j = 0, k = l;

    // Compare elements from both arrays
    // and store smaller element first
    while (i < left.size() && j < right.size()) {

        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        }

        else {
            arr[k++] = right[j++];
        }
    }

    // Copy remaining left elements if any
    while (i < left.size()) {
        arr[k++] = left[i++];
    }

    // Copy remaining right elements if any
    while (j < right.size()) {
        arr[k++] = right[j++];
    }
}

// =====================================================
// Sequential Merge Sort
// Divide and Conquer sorting using one thread
// =====================================================
void mergeSortSeq(vector<int>& arr, int l, int r) {

    // Continue only if more than one element exists
    if (l < r) {

        // Find middle index
        int m = (l + r) / 2;

        // Sort left half recursively
        mergeSortSeq(arr, l, m);

        // Sort right half recursively
        mergeSortSeq(arr, m + 1, r);

        // Merge both sorted halves
        merge(arr, l, m, r);
    }
}

// =====================================================
// Parallel Merge Sort
// Uses multiple threads for recursive sorting
// =====================================================
void mergeSortParallel(vector<int>& arr, int l, int r, int depth) {

    // Continue only if more than one element exists
    if (l < r) {

        // Find middle index
        int m = (l + r) / 2;

        // If depth becomes 0,
        // switch to sequential merge sort
        if (depth <= 0) {

            mergeSortSeq(arr, l, m);
            mergeSortSeq(arr, m + 1, r);
        }

        else {

            // Create parallel sections
            // Left and right halves run simultaneously

            #pragma omp parallel sections
            {

                // First thread handles left half
                #pragma omp section
                mergeSortParallel(arr, l, m, depth - 1);

                // Second thread handles right half
                #pragma omp section
                mergeSortParallel(arr, m + 1, r, depth - 1);
            }
        }

        // Merge sorted halves
        merge(arr, l, m, r);
    }
}

// =====================================================
// Function to print array elements
// =====================================================
void printArray(vector<int>& arr) {

    // Print each element of array
    for (int x : arr) {
        cout << x << " ";
    }

    cout << endl;
}

// =====================================================
// Main Function
// Program execution starts from here
// =====================================================
int main() {

    int n;

    // Take number of elements from user
    cout << "Enter number of elements: ";
    cin >> n;

    // arr -> original array
    // temp -> copy array for different sorting methods
    vector<int> arr(n), temp;

    cout << "Enter elements:\n";

    // Take array input
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    // Variables for execution time
    double start, end;

    // =================================================
    // Sequential Bubble Sort
    // =================================================

    // Copy original array
    temp = arr;

    // Start timer
    start = omp_get_wtime();

    // Call sequential bubble sort
    bubbleSortSeq(temp);

    // End timer
    end = omp_get_wtime();

    // Print sorted array
    cout << "\nSequential Bubble Sort:\n";
    printArray(temp);

    // Print execution time
    cout << "Execution Time: "
         << (end - start)
         << " seconds\n";

    // =================================================
    // Parallel Bubble Sort
    // =================================================

    temp = arr;

    start = omp_get_wtime();

    // Call parallel bubble sort
    bubbleSortParallel(temp);

    end = omp_get_wtime();

    cout << "\nParallel Bubble Sort:\n";
    printArray(temp);

    cout << "Execution Time: "
         << (end - start)
         << " seconds\n";

    // =================================================
    // Sequential Merge Sort
    // =================================================

    temp = arr;

    start = omp_get_wtime();

    // Call sequential merge sort
    mergeSortSeq(temp, 0, n - 1);

    end = omp_get_wtime();

    cout << "\nSequential Merge Sort:\n";
    printArray(temp);

    cout << "Execution Time: "
         << (end - start)
         << " seconds\n";

    // =================================================
    // Parallel Merge Sort
    // =================================================

    temp = arr;

    start = omp_get_wtime();

    // depth = 4 controls recursion parallelism
    mergeSortParallel(temp, 0, n - 1, 4);

    end = omp_get_wtime();

    cout << "\nParallel Merge Sort:\n";
    printArray(temp);

    cout << "Execution Time: "
         << (end - start)
         << " seconds\n";

    // Program completed successfully
    return 0;
}

// Enter number of elements: 8
// Enter elements:
// 64 34 25 12 22 11 90 5
// g++ -fopenmp -o bubblemerge  bubblemerge.cpp
// ./bubblemerge
