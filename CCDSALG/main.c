#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// From record.c:
typedef struct record {
    char name[500];
    int idNumber;
} Record;

// From filereader.c:
void readFile(Record *records, char path[500]);

// From timer.c:
long currentTimeMillis();

// From sortingalgorithms.c:
void insertionSort(Record *arr, int n);
void selectionSort(Record *arr, int n);
void mergeSort(Record *arr, int left, int right);
void bubbleSort(Record *arr, int n);

#define DATA_FILES 7
const char *data_files[DATA_FILES] = {
    "data/totallyreversed.txt",
    "data/almostsorted.txt",
    "data/random75000.txt",
    "data/random100000.txt",
    "data/random100.txt",
    "data/random25000.txt",
    "data/random50000.txt"
};

// Helper: Reads the record count from the file.
int getRecordCount(const char *path) {
    int count;
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Error: Could not open file %s\n", path);
        return 0;
    }
    if (fscanf(fp, "%d", &count) != 1) {
        printf("Error: Could not read record count from %s\n", path);
        fclose(fp);
        return 0;
    }
    fclose(fp);
    return count;
}

//Check if the array is sorted by idNumber in ascending order.
int isSorted(Record *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i].idNumber > arr[i + 1].idNumber) {
            return 0;
        }
    }
    return 1;
}

// Run a sort, time it, and print verification and the first 10 records.
void testSortingAlgorithm(void (*sortFunction)(Record *, int),
                          Record *records, int n,
                          const char *sortName, const char *filename) {
    long startTime, endTime;
    printf("\nSorting %s using %s...\n", filename, sortName);
    startTime = currentTimeMillis();
    sortFunction(records, n);
    endTime = currentTimeMillis();
    printf("%s completed in %ld ms.\n", sortName, endTime - startTime);
    
    if (isSorted(records, n))
        printf("Sorting verified: array is sorted correctly.\n");
    else
        printf("Sorting error: array is NOT sorted correctly.\n");

    printf("First 10 records after sorting:\n");
    for (int i = 0; i < 10 && i < n; i++) {
        printf("%d %s\n", records[i].idNumber, records[i].name);
    }
}

int main() {
    int algoChoice, fileChoice;
    
    // Prompt user for sorting algorithm:
    printf("Choose a sorting algorithm:\n");
    printf(" 1) Insertion Sort\n");
    printf(" 2) Selection Sort\n");
    printf(" 3) Merge Sort\n");
    printf(" 4) Bubble Sort\n");
    printf("Enter choice (1-4): ");
    if (scanf("%d", &algoChoice) != 1) {
        printf("Error reading algorithm choice.\n");
        return 1;
    }
    
    // Prompt user for data file:
    printf("\nChoose a data file:\n");
    for (int i = 0; i < DATA_FILES; i++) {
        printf(" %d) %s\n", i + 1, data_files[i]);
    }
    printf("Enter choice (1-%d): ", DATA_FILES);
    if (scanf("%d", &fileChoice) != 1) {
        printf("Error reading file choice.\n");
        return 1;
    }
    if (fileChoice < 1 || fileChoice > DATA_FILES) {
        printf("Invalid file choice.\n");
        return 1;
    }
    
    const char *filename = data_files[fileChoice - 1];
    int n = getRecordCount(filename);
    if (n <= 0) {
        printf("No records found or error reading file %s.\n", filename);
        return 1;
    }
    printf("File %s has %d records.\n", filename, n);
    
    // Allocate memory for records:
    Record *records = malloc(n * sizeof(Record));
    if (records == NULL) {
        printf("Memory allocation failed for records.\n");
        return 1;
    }
    
    // Read the records from the file:
    readFile(records, (char *)filename);
    
    // Create a copy for sorting:
    Record *recordsCopy = malloc(n * sizeof(Record));
    if (recordsCopy == NULL) {
        printf("Memory allocation failed for recordsCopy.\n");
        free(records);
        return 1;
    }
    memcpy(recordsCopy, records, n * sizeof(Record));
    
    // Run the chosen sorting algorithm:
    switch (algoChoice) {
        case 1:
            testSortingAlgorithm(insertionSort, recordsCopy, n, "Insertion Sort", filename);
            break;
        case 2:
            testSortingAlgorithm(selectionSort, recordsCopy, n, "Selection Sort", filename);
            break;
        case 3:
            {
                long startTime = currentTimeMillis();
                mergeSort(recordsCopy, 0, n - 1);
                long endTime = currentTimeMillis();
                printf("\nSorting %s using Merge Sort...\n", filename);
                printf("Merge Sort completed in %ld ms.\n", endTime - startTime);
                if (isSorted(recordsCopy, n))
                    printf("Sorting verified: array is sorted correctly.\n");
                else
                    printf("Sorting error: array is NOT sorted correctly.\n");
                printf("First 10 records after sorting:\n");
                for (int i = 0; i < 10 && i < n; i++) {
                    printf("%d %s\n", recordsCopy[i].idNumber, recordsCopy[i].name);
                }
            }
            break;
        case 4:
            testSortingAlgorithm(bubbleSort, recordsCopy, n, "Bubble Sort", filename);
            break;
        default:
            printf("Invalid algorithm choice.\n");
            free(records);
            free(recordsCopy);
            return 1;
    }
    
    // Free allocated memory:
    free(records);
    free(recordsCopy);
    
    // Wait for user input before exiting:
    printf("\nPress Enter to exit...");
    // Consume any leftover newline from previous input
    getchar();
    getchar();
    
    return 0;
}
