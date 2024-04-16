#include <stdio.h>
#define num_disks 3
unsigned num_moves = 0;
unsigned A[4] = {num_disks, 59, 43, 31};
unsigned B[4] = {0, 0, 0, 0};
unsigned C[4] = {0, 0, 0, 0};

void printArray(unsigned *input){
    for (int i = 0; i < 4; i++){
        printf("%d,", input[i]);
    }
    printf("\n"); 
}
void printAll(){
    printf("array A:\n"); 
    printArray(A); 
    
    printf("array B:\n"); 
    printArray(B); 

    printf("array C:\n"); 
    printArray(C);
    printf("\n"); 
}
// Moving disks from (*from rod) to (*to rod) using the (*aux rod).
void TowerOfHanoi(unsigned n, unsigned *from, unsigned *to, unsigned *aux) {
    if(n == 0) return;
    printf("calling first towerofhanoi with n = %d\n", n);
    TowerOfHanoi(n-1, from, aux, to);

    //to[++to[0]] = from[from[0]--]; // this code is so fucking confusing, rewrite as below to make it easier to understand 
    printf("moving %d\n\n", from[from[0]]); 
    to[0] += 1;
    to[to[0]] = from[from[0]]; 
    from[0] -= 1; 
    printAll();
    num_moves++;

    printf("calling second towerofhanoi with n = %d\n", n); 
    TowerOfHanoi(n-1, aux, to, from); }


int main(void) {
    TowerOfHanoi(num_disks, A, C, B);
    printAll();
    return 0;
}


