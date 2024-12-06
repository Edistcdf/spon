#include <stdio.h>
#include <assert.h>
#include "../add.h"

void test_add() {
    assert(add(2, 3) == 5);
    assert(add(-1, 1) == 0);
    assert(add(0, 0) == 0);
    assert(add(-5, -5) == -10);
}

int main() {
    test_add();
    return 0;
}

