/**
 * @file sum-files-threaded.c
 * @brief This program reads multiple files and calculates the sum of negative and positive numbers in each file using threads.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* processFile(void* arg);

/* Structure to hold the file information */
typedef struct {
    char* filename;
    long negative_numbers;
    long positive_numbers;
} FileData;

/* Function to process a file in a separate thread */
void* processFile(void* arg) {
    FileData* fileData = (FileData*) arg;
    FILE* fp = fopen(fileData->filename, "r");

    if (fp != NULL) {
        int n;
        fileData->negative_numbers = 0;
        fileData->positive_numbers = 0;

        fscanf(fp, "%d", &n);
        while (!feof(fp)) {
            if (n < 0)
                fileData->negative_numbers += n;
            else if (n > 0)
                fileData->positive_numbers += n;

            fscanf(fp, "%d", &n);
        }

        fclose(fp);
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    long negative_sum = 0, positive_sum = 0;
    int i;

    if (argc > 1) {
        pthread_t* tids = malloc((argc - 1) * sizeof(pthread_t));
        FileData* fileData = malloc((argc - 1) * sizeof(FileData));

        for (i = 1; i < argc; i++) {
            fileData[i - 1].filename = argv[i];

            /* Create a thread to process each file */
            pthread_create(&tids[i - 1], NULL, processFile, &fileData[i - 1]);
        }

        /* Wait for all threads to finish */
        for (i = 0; i < argc - 1; i++) {
            pthread_join(tids[i], NULL);

            /* Accumulate the results from each thread */
            negative_sum += fileData[i].negative_numbers;
            positive_sum += fileData[i].positive_numbers;
        }

        /* Free the dynamically allocated memory */
        free(tids);
        free(fileData);
    }

    printf("%ld %ld\n", negative_sum, positive_sum);
    return 0;
}
