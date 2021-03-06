#ifndef __HKAISERBESSEL_H
#define __HKAISERBESSEL_H

/**
    Kaiser-Bessel filter FIR coefficients can be calculated using standard equations.

     The calculation of the coefficients are heavily inspired by
    the excelent page here: https://arc.id.au/FilterDesign.html

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

        int _points;
        float* _coefficients;
        int _fa;
        int _fb;
        int _rate;
        int _att;

        // Called to do additional calculations
        // (only a band-stop filter requires manipulation of the coefficients)
        virtual void Modify(float* coefficients, int points) {};

        float ZeroOrderBessel(float x);

    protected:

        /** Construct a new Kaiser-Bessel */
        HKaiserBessel(int fa, int fb, int rate, int points, int att);

        /** Destroy an instance of af KaiserBessel object */
        ~HKaiserBessel() {
            delete _coefficients;
        }

    public:

        /** Get the coefficients as they where calculated by the last call to Calculate() */
        float* GetCoefficients() {
            return _coefficients;
        }

        /** Get FIR coefficients */
        float* Calculate();
};

#endif