#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int ARRAY_SIZE;

int* a;
void read(char *filename){
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error opening the file.\n");
    }
    fscanf(file, "%d",&ARRAY_SIZE);
    a=malloc(sizeof(int)*ARRAY_SIZE);
    for(int i=0;i<ARRAY_SIZE;i++)
        {
         fscanf(file,"%d",&a[i]);
        }
    fclose(file);
}
void merge(int l, int m, int h) {
    int n1 = m - l + 1;
    int n2 = h - m;

    int Left[n1], Right[n2];

    for (int i = 0; i < n1; i++)
        Left[i] = a[l + i];

    for (int j = 0; j < n2; j++)
        Right[j] = a[m + 1 + j];

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2) {
        if (Left[i] <= Right[j]) {
            a[k] = Left[i];
            i++;
        } else {
            a[k] = Right[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        a[k] = Left[i];
        i++;
        k++;
    }

    while (j < n2) {
        a[k] = Right[j];
        j++;
        k++;
    }
}
void* merge_thread(void* arg) {
    int* params = (int*)arg;
    int l = params[0];
    int h = params[1];
    pthread_t leftthread,rightthread;
    if (l < h) {
        int m = (l + h) / 2;

        int* index_left = (int*)malloc(2 * sizeof(int));
        index_left[0] = l;
        index_left[1] = m;
        pthread_create(&leftthread, NULL, merge_thread, (void*)index_left);
        int* index_right = (int*)malloc(2 * sizeof(int));
        index_right[0] = m + 1;
        index_right[1] = h;
        pthread_create(&rightthread, NULL, merge_thread, (void*)index_right);

        pthread_join(leftthread, NULL);
        pthread_join(rightthread, NULL);

        merge(l, m, h);
    }

    free(arg);
    return NULL;
}

int main(int argc,char *argv[]) {
    char *string = argv[1];
    read(string);
    pthread_t nthread;
    int* index = (int*)malloc(2 * sizeof(int));
    index[0] = 0;
    index[1] = ARRAY_SIZE - 1;
    pthread_create(&nthread, NULL, merge_thread, (void*)index);
    pthread_join(nthread, NULL);

    // Move the printing section to after the threads have finished
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", a[i]);
    }

    return 0;
}
