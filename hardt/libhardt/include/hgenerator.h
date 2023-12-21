#ifndef __HGENERATOR_H
#define __HGENERATOR_H

#include "hardt.h"
#include "hsoundcard.h"
#include "hobject.h"
#include "hreader.h"
#include <math.h>
#include <limits>

/**
    Base class for the signal generators.

    If you need to write a special generator, inherit this class.
*/
template <class T>
class HGenerator : public HReader<T>
{
    private:

        T* _lot;
        int _lotSize;
        int _it;
        float _delta;
        H_SAMPLE_RATE _rate;

        float* _flot;

        float _zero;

        void Init(H_SAMPLE_RATE rate, int frequency, T amplitude, float phase);

    protected:

        /**
         * Construct a new HGenerator
         *
         * @param id Element identifier
         * @param rate Sample rate of the generator
         * @param frequency Generator frequency
         * @param amplitude Generator amplitude (max abs. value)
         * @param phase Generator phase (in radians)
         * */
        HGenerator(std::string id, H_SAMPLE_RATE rate, int frequency, T amplitude, float phase = 0);

        /** Default destructor */
        ~HGenerator();

    public:

        /**
         * Read samples from the generator
         *
         * @param dest Destination buffer
         * @param blocksize Number of samples to read
         * */
        int ReadImpl(T* dest, size_t blocksize);

        /** Calculate and fill generator sample lookup table */
        void Calculate(int frequency, T amplitude, float phase);

        /**
         * Execute and/or pass on a command
         *
         * @param command Command to send or execute
         * */
        bool Command(HCommand* command) {
            // No further propagation of commands
            return true;
        }

        /**
         * Return current value from the generator
         *
         * @return Current sequential generator value
         */
        inline T Current() {
            return _lot[_it];
        }

        /**
         * Return current value from the generator and step to the next value
         *
         * @return Next sequential generator value
         */
        inline T Next() {

            T value = _lot[_it];

            // Increase to next lot value
            _it += _delta;

            // Past the end of the lot ?
            if( _it >= _lotSize )
            {
                _it -= _lotSize;
            }

            return value;
        }
};

#endif
