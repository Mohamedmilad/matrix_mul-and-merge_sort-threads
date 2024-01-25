#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
int* a;
int* b;
int* result;
int* result1;
int rowa=0;
int cola=0;
int rowb=0;
int colb=0;
void readFromFile(char *filename){
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error opening the file.\n");
    }

    fscanf(file, "%d %d", &rowa, &cola);

    a = malloc(rowa *cola* sizeof(int));
    for (int i = 0; i < rowa; i++) {
        for (int j = 0; j < cola; j++) {
            fscanf(file, "%d", &a[i * cola + j]);
        }
    }
    fscanf(file, "%d %d", &rowb, &colb);
    b =malloc(rowb *colb* sizeof(int));
    for (int i = 0; i < rowb; i++) {
        for (int j = 0; j < colb; j++) {
            fscanf(file, "%d", &b[i * colb + j]);
        }
    }
    result =malloc(rowa *colb* sizeof(int *));
    result1 =malloc(rowa *colb* sizeof(int *));

    fclose(file);

}

void* mul_BY_element(void* arg) {
    int i = 0;
    int* params = (int*)arg;
    int r = params[0];
    int c = params[1];
    int sum = 0;
    while (i < rowb) {
        sum = sum + a[r * cola + i] * b[i * colb + c];
        i++;
    }
    result1[r * colb + c] = sum;
    return NULL;
}
void* mul_BY_ROW(void* arg) {
    int i = 0;
    int r = *((int*)arg);
    int sum,j= 0;
    while(j<colb){
        sum=0;
        i=0;
    while (i < cola) {
        sum = sum + a[r * cola + i] * b[i * colb + j];
        i++;
    }
    result[r * colb + j] = sum;
    j++;
    }
    return NULL;
}

void print_matrix(int rows, int cols, int a[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
}


int main(int argc,char *argv[]) {
    char *string = argv[1];
    readFromFile(string);
    if(cola!=rowb){
        printf("Can't multiply those matrices");
        exit(1);
    }
    clock_t start_time_element,start_time_row, end_time_element,end_time_row;
    int i= 0;
    int j = 0;
    pthread_t thread[rowa][colb];
    
    start_time_element = clock();
    while (i < rowa) {
        j = 0;
        while (j < colb) {
            int* index = (int*)malloc(2 * sizeof(int));  // Allocate memory for the index array
            index[0] = i;
            index[1] = j;
            pthread_create(&thread[i][j], NULL, mul_BY_element, (void*)index);\
            j++;
        }
        i++;
    }

    i = 0;
    while (i < rowa) {
        j = 0;
        while (j < colb) {
            pthread_join(thread[i][j], NULL);
            j++;
        }
        i++;
    }
    end_time_element = clock();








    pthread_t newthread[rowa];
     i=0;
    start_time_row = clock();
    while (i < rowa) {
        int* index = (int*)malloc( sizeof(int)); 
        index[0]=i;
            pthread_create(&newthread[i], NULL, mul_BY_ROW, (void*)index);
        i++;
    }

    i = 0;
    while (i < rowa) {
            pthread_join(newthread[i], NULL);
        i++;
    }
    end_time_row = clock();

    double execution_time_element = (double)(end_time_element - start_time_element) / CLOCKS_PER_SEC;
    double execution_time_row = (double)(end_time_row - start_time_row) / CLOCKS_PER_SEC;
    printf("Result of matrix by row done in %f \n\n",execution_time_row);
    print_matrix(rowa, colb,(int (*)[colb])result);
    printf("\n \n Result of matrix by element done in %f \n \n",execution_time_element);
    print_matrix(rowa, colb,(int (*)[colb])result1);
    return 0;
}
