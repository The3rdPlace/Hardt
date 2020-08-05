#ifndef __HKAISERBESSEL_H
#define __HKAISERBESSEL_H

/**
    Kaiser-Bessel filter FIR coefficients can be calculated using standard equations.
    One example is here: https://arc.id.au/FilterDesign.html 

    Create a new Kaiser-Bessel filter of a specific type, then get the coefficients
    by calling 'Calculate()' and pass them to a HFirFilter (or HFir if you need to do
    some internal processing by way of FIR filtering)

    You can use the specific Kaiser-Bessel filters as anonymous types during a 
    'new HFirFilter...' statement.

*/
template <class T>
class HKaiserBessel
{
    private:

        // Todo: add coefficients

        // Todo: add parameters 
        
        // Called to do additional calculations 
        // (only a band-stop filter requires manipulation of the coefficients)
        virtual float Calculate(float* coefficients, int points) = 0;

    protected:

        /** Construct a new HBiQuad */
        HKaiserBessel(int fa, int fb, float rate, int points, int att);

    public:

        /** Get FIR coefficients */
        float* Calculate();
};

#endif