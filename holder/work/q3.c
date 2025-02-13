#include <stdio.h>

int func(int val);
int x = 34;
extern int z;

static int doubleIt(int val) {
        return 2*val;
}

int main() {
        int y;
        y = func(x);
        int *a = &y;
        int b = doubleIt(z);
        printf("%d\n", b);
        return 0;
}
