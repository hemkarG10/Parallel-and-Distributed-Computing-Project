#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#define RESET   "\033[0m"
#define BOLDRED "\033[1m\033[31m"      /* Bold Red */
#define PATH -1
#define NONE 0
#define UP 1
#define LEFT 2
#define DIAGONAL 3
void similarityScore(long long int i, long long int j, int* H, int* P, long long int* maxPos);
int matchMissmatchScore(long long int i, long long int j);
void backtrack(int* P, long long int maxPos);
void printMatrix(int* matrix);
void printPredecessorMatrix(int* matrix);
void generate(void);
long long int m = 11; //Columns - Size of string a
long long int n = 7;  //Lines - Size of string b
int matchScore = 5;
int missmatchScore = -3;
int gapScore = -4; 
char *a, *b;
int main(int argc, char* argv[]) {
    int thread_count = strtoll(argv[1], NULL, 10);
    m = strtoll(argv[2], NULL, 10);
    n = strtoll(argv[3], NULL, 10); 
    #ifdef DEBUG
    printf("\nMatrix[%lld][%lld]\n", n, m);
    #endif
    a = malloc(m * sizeof(char));
    b = malloc(n * sizeof(char));
    m++;
    n++;
    int *H;
    H = calloc(m * n, sizeof(int));
    int *P;
    P = calloc(m * n, sizeof(int));
    generate();
    long long int maxPos = 0;
    long long int i, j;
    double Time=0.0;
    double initialTime = omp_get_wtime(); 
         
      
    
    //serial implementation
    #pragma omp parallel for shared(H, P, maxPos) private(i,j) collapse(2)

for (i = 1; i < n; i++)  //Lines

        for (j = 1; j < m; j++)  //

           similarityScore(i, j, H, P, &maxPos);
    
    
    backtrack(P, maxPos);
    double finalTime = omp_get_wtime();
    Time+=(double)(finalTime - initialTime);
#ifdef DEBUG
    printf("\nSimilarity Matrix:\n");
    printMatrix(H);
    printf("\nPredecessor Matrix:\n");
    printPredecessorMatrix(P);
    printf("\nElapsed time: %f\n\n", Time);
    #endif
        
free(H);
    free(P);
    free(a);
    free(b);
    
    return 0;
    
}  

void similarityScore(long long int i, long long int j, int* H, int* P, long long int* maxPos) {
    int up, left, diag;
    long long int index = m * i + j;
    up = H[index - m] + gapScore;
    left = H[index - 1] + gapScore;
    diag = H[index - m - 1] + matchMissmatchScore(i, j);
    int max = NONE;
    int pred = NONE;
    if (diag > max) { //same letter ↖
        max = diag;
        pred = DIAGONAL;
    }
    if (up > max) { //remove letter ↑ 
        max = up;
        pred = UP;
    }
    if (left > max) { //insert letter ←
        max = left;
        pred = LEFT;
    }
    H[index] = max;
    P[index] = pred; 
    if (max > H[*maxPos]) {
        *maxPos = index;
    }

}  

int matchMissmatchScore(long long int i, long long int j) {
    if (a[j-1] == b[i-1])
        return matchScore;
    else
        return missmatchScore;

}  

void backtrack(int* P, long long int maxPos) {
    long long int predPos;
    do {
        if(P[maxPos] == DIAGONAL)
            predPos = maxPos - m - 1;
        else if(P[maxPos] == UP)
            predPos = maxPos - m;
        else if(P[maxPos] == LEFT)
            predPos = maxPos - 1;
        P[maxPos]*=PATH;
        maxPos = predPos;
    } while(P[maxPos] != NONE);
} 
void printMatrix(int* matrix) {
    long long int i, j;
    for (i = 0; i < n; i++) { //Lines
        for (j = 0; j < m; j++) {
            printf("%d\t", matrix[m * i + j]);
        }
        printf("\n");
    }

} 
void printPredecessorMatrix(int* matrix) {
    long long int i, j, index;
    for (i = 0; i < n; i++) { //Lines
        for (j = 0; j < m; j++) {
            index = m * i + j;
            if(matrix[index] < 0) {
                printf(BOLDRED);
                if (matrix[index] == -UP)
                    printf("↑ ");
                else if (matrix[index] == -LEFT)
                    printf("← ");
                else if (matrix[index] == -DIAGONAL)
                    printf("↖ ");
                else
                    printf("- ");
                printf(RESET);
            } else {
                if (matrix[index] == UP)
                    printf("↑ ");
                else if (matrix[index] == LEFT)
                    printf("← ");
                else if (matrix[index] == DIAGONAL)
                    printf("↖ ");
                else
                    printf("- ");
            }
        }
        printf("\n");
    }

} 
 void generate(){
    long long int i;
    for(i=0;i<m;i++){
        int aux=rand()%4;
        if(aux==0)
            a[i]='A';
        else if(aux==2)
            a[i]='C';
        else if(aux==3)
            a[i]='G';
        else
            a[i]='T';
    }
    for(i=0;i<n;i++){
        int aux=rand()%4;
        if(aux==0)
            b[i]='A';
        else if(aux==2)
            b[i]='C';
        else if(aux==3)
            b[i]='G';
        else
            b[i]='T';
    }
}
