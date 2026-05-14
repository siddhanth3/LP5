// ================================================================
// INCLUDES: Libraries needed for this program
// ================================================================
#include <iostream>   // For input/output (cin, cout)
#include <omp.h>      // For parallel processing and reduction operations
#include <climits>    // For INT_MAX and INT_MIN constants

using namespace std;

// ===== MIN_REDUCTION FUNCTION: Finds smallest number in array =====
// Multiple threads work together to find the minimum value quickly
void min_reduction(int arr[], int n)
{
    // Start with the largest possible number (so any array element will be smaller)
    int min_value = INT_MAX;

    // Parallel for - multiple threads process array elements simultaneously
    // reduction(min:min_value) = each thread finds its minimum, then they combine results
    #pragma omp parallel for reduction(min:min_value)
    for(int i = 0; i < n; i++)
    {
        // Compare each element with current minimum
        if(arr[i] < min_value)
        {
            min_value = arr[i];  // Update if found a smaller number
        }
    }

    cout << "Minimum value: " << min_value << endl;
}

// ===== MAX_REDUCTION FUNCTION: Finds largest number in array =====
// Multiple threads work together to find the maximum value quickly
void max_reduction(int arr[], int n)
{
    // Start with the smallest possible number (so any array element will be larger)
    int max_value = INT_MIN;

    // Parallel for - multiple threads process array elements simultaneously
    // reduction(max:max_value) = each thread finds its maximum, then they combine results
    #pragma omp parallel for reduction(max:max_value)
    for(int i = 0; i < n; i++)
    {
        // Compare each element with current maximum
        if(arr[i] > max_value)
        {
            max_value = arr[i];  // Update if found a larger number
        }
    }

    cout << "Maximum value: " << max_value << endl;
}

// ===== SUM_REDUCTION FUNCTION: Calculates total of all numbers =====
// Multiple threads add their parts, then combine results
void sum_reduction(int arr[], int n)
{
    // Start with 0 (neutral value for addition)
    int sum = 0;

    // Parallel for - multiple threads process array elements simultaneously
    // reduction(+:sum) = each thread adds to sum, then they combine all sums together
    #pragma omp parallel for reduction(+:sum)
    for(int i = 0; i < n; i++)
    {
        sum += arr[i];  // Add each element to the sum
    }

    cout << "Sum: " << sum << endl;
}

// ===== AVERAGE_REDUCTION FUNCTION: Calculates middle value =====
// Multiple threads add numbers together, then we divide by count
void average_reduction(int arr[], int n)
{
    // Start with 0 to collect the sum
    int sum = 0;

    // Parallel for - multiple threads process array elements simultaneously
    // reduction(+:sum) = each thread adds to sum, then combines all sums
    #pragma omp parallel for reduction(+:sum)
    for(int i = 0; i < n; i++)
    {
        sum += arr[i];  // Add each element
    }

    // Divide total sum by number of elements to get average
    double avg = (double)sum / n;  // Convert to double for accurate division

    cout << "Average: " << avg << endl;
}

// ================================================================
// MAIN FUNCTION: Entry point - coordinates the program
// ================================================================
int main()
{
    int *arr, n;  // arr = pointer to array, n = size of array

    // STEP 1: Get array size from user
    cout << "Enter total number of elements: ";
    cin >> n;

    // STEP 2: Create array with the specified size
    arr = new int[n];

    // STEP 3: Get each element from user
    cout << "Enter elements:\n";

    for(int i = 0; i < n; i++)
    {
        cin >> arr[i];  // Read each number into array
    }

    // STEP 4: Find and display minimum value (using parallel reduction)
    min_reduction(arr, n);

    // STEP 5: Find and display maximum value (using parallel reduction)
    max_reduction(arr, n);

    // STEP 6: Calculate and display sum (using parallel reduction)
    sum_reduction(arr, n);

    // STEP 7: Calculate and display average (using parallel reduction)
    average_reduction(arr, n);

    return 0;  // Program ends successfully
}

// Enter total number of elements: 6
// Enter elements:
// 5 15 25 35 45 55
// g++ -fopenmp -o reduction  reduction.cpp
// ./reduction.exe
