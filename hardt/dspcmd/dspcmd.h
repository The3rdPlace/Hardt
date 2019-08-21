#ifndef __DSPCMD_H
#define __DSPCMD_H

#include <stdio.h>

template <typename T>
void print_array(T* array, int sizeOfArray)
{
    for( int i = 0; i < sizeOfArray; i++ )
    {
        std::cout << i << " = " << array[i] << '\n';
    }

}

#endif

