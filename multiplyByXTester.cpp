#include <iostream>
#include "IntArray.h"
extern void multiplyByX(IntArray *p, int x);
int main()
{
    IntArray arr;
    arr.size = 5;
    arr.elements = new int[arr.size]{1, 2, 3, 4, 5};
    int x = 10;
    multiplyByX(&arr, x);
    for (int i = 0; i < arr.size; i++) {
        std::cout << arr.elements[i] << " ";
    }
    delete[] arr.elements;
    return 0;
}