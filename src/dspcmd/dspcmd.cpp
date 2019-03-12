#include "dspcmd.h"
#include <stdio.h>

#include "../hardt/include/hardtapi.h"
#include "dspcmd.h"

int main(int argc, char**argv)
{
	std::cout << "dspcmd: using Hardt " + getversion() << "\n" ;

    Generator* g = new Generator();

    int freqs[1];
    freqs[0] = 25;
    std::complex<double>* samples = g->GetSamples(1000, 1, 1, freqs, 1);
    //print_array< std::complex<double> >(samples, 1000);

    delete g;

}

