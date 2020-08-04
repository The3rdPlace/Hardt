#ifndef __HFIR_H
#define __HFIR_H

/** Implements the filtering for FIR-structured filters */
template <class T>
class HFir {

private:

    float *_coefficients;
    int _length;
    T *_taps;
    int _factor;

    void Init(float* coefficients) {

        // Copy coefficients
        _coefficients = new float[_length];
        SetCoefficients(coefficients, _length);
        HLog("Copied filter coefficients");

        // Create delay line
        _taps = new T[_length];
        memset((void *) _taps, 0, _length * sizeof(T));
        HLog("Allocated and initialized delay buffer for %d taps", _length);
    }

public:

    /** Create a new FIR block (for calculating output values for a FIR filter) */
    HFir(float *coefficients, int length) : _length(length), _factor(0) {

        HLog("HFir(float*, %d)", length);
        Init(coefficients);
    }

    /** Create a new FIR block (for calculating output values for a FIR filter)
        as an interpolation FIR (thus rearranging the filter coefficients and configure
        as a polyphase FIR filter) */
    HFir(float *coefficients, int length, int factor) : _length(length), _factor(factor) {

        HLog("HFir(float*, %d, %d) - constructing polyphase FIR for interpolation", length, factor);
        Init(coefficients);
    }

    /** Destroy this FIR block */
    ~HFir() {
        HLog("~HFir()");
        delete[] _taps;
        delete[] _coefficients;
    }

    /** Filter a single sample value */
    inline T Filter(T value) {

        // Advance delay line 1 sample
        // Todo: We really should use a ringbuffer!
        memmove((void *) &_taps[1], (void *) _taps, (_length - 1) * sizeof(T));

        // Add new sample
        _taps[0] = value;

        // Sum all taps
        float result = 0;
        for (int j = 0; j < _length; j++) {
            result += _taps[j] * _coefficients[j];
        }

        // Store result for 1 sample
        return result;
    }

    /** Filter a block of values through the FIR block */
    inline void Filter(T *values, T* result, size_t blocksize) {

        // Polyphase FIR ?
        if( _factor > 0 ) {
            throw new HNotImplementedException("FIR does not yet support polyphase FIR filters");
            // Todo: implement polyphase interpolation filtering, then return
        }

        // Regular FIR
        for( int i = 0; i < blocksize; i++ ) {
            result[i] = Filter(values[i]);
        }
    }

    /** Change the coefficients for the FIR block */
    void SetCoefficients(float *coefficients, int length) {
        // Sanity check
        if (length != _length) {
            HError("It is not possible to assign a set of coefficients of different length than the length used at construction (%d)",
                   _length);
            throw new HFilterInitializationException("Length of coefficients differs from construction length");
        }

        // Copy coefficients
        memcpy(_coefficients, coefficients, length * sizeof(float));
    }
};

#endif
