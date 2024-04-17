#include <stdio.h>
#define num_disks 3
unsigned num_moves = 0;
unsigned A[4] = {num_disks, 59, 43, 31};
unsigned B[4] = {0, 0, 0, 0};
unsigned C[4] = {0, 0, 0, 0};
const char* A_name = "A"; 
const char* B_name = "B"; 
const char* C_name = "C"; 

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
void TowerOfHanoi(unsigned n, unsigned *from, unsigned *to, unsigned *aux, const char* from_name, const char* to_name, const char* aux_name) {
    if(n == 0) return;
    printf("calling first towerofhanoi with n = %d, the input of n for the next call will be %d\n\n", n, n-1);
    TowerOfHanoi(n-1, from, aux, to, from_name, aux_name, to_name);

    //to[++to[0]] = from[from[0]--]; // this code is so fucking confusing, rewrite as below to make it easier to understand 
    printf("at this specific function call, n is : %d\n", n); 
    printf("moving %d, from %s (index: %d) to %s (index: %d)\n\n", from[from[0]], from_name, from[0] , to_name, to[0]+1); 
    to[0] += 1;
    to[to[0]] = from[from[0]]; 
    from[0] -= 1; 
    printAll();
    num_moves++;

    printf("calling second towerofhanoi with n = %d, the input of n for the next call will be %d\n\n", n, n-1); 
    TowerOfHanoi(n-1, aux, to, from, aux_name, to_name, from_name);
    printf("end of function that was called for n=%d\n", n);
}


int main(void) {
    TowerOfHanoi(num_disks, A, C, B, A_name, C_name, B_name);
    printAll();
    return 0;
}


