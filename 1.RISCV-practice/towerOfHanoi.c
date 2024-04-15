#include <stdio.h>
#define num_disks 4
unsigned num_moves = 0;
unsigned A[4] = {num_disks, 59, 43, 31};
unsigned B[4] = {0, 0, 0, 0};
unsigned C[4] = {0, 0, 0, 0};
// Moving disks from (*from rod) to (*to rod) using the (*aux rod).
void TowerOfHanoi(unsigned n, unsigned *from, unsigned *to, unsigned *aux) {
    if(n == 0) return;
    printf("calling 1st tower of hanoi recursive function with n = %d\n\n", n-1);
    TowerOfHanoi(n-1, from, aux, to);
    to[++to[0]] = from[from[0]--];
    num_moves++;
    printf("calling 2nd tower of hanoi recursive function with n = %d\n\n", n-1); 
    TowerOfHanoi(n-1, aux, to, from);
}

int main(void) {
    TowerOfHanoi(num_disks, A, C, B);
    return 0;
}

