/**
    \page dspcmd

	\section introduction_section_dspcmd About dspcmd
	todo


	\section overview_section_dspcmd Overview
	todo


	\section options_section_dspcmd Options

	\subpage fmgp Show spectrum plot in GnuPlot


	\page fmgp
	Syntax: dspcmd -fmgp -if file.pcm
*/

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

