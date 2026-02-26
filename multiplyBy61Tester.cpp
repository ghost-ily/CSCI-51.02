#include <iostream>
#include "IntArray.h"
extern void multiplyBy61(IntArray *p);
int main()
{
    IntArray arr;
    arr.size = 5;
    arr.elements = new int[arr.size]{1, 2, 3, 4, 5};
    IntArray* p = &arr;

    multiplyBy61(p);

    for (int i = 0; i < arr.size; i++) {
        std::cout << arr.elements[i] << " ";
    }

    delete[] arr.elements;
    return 0;
}