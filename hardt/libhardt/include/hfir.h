#ifndef __HFIR_H
#define __HFIR_H

/** Implements the filtering for FIR-structured filters */
template <class T>
class HFir {

    private:

        float *_coefficients;
        int _length;
        T *_taps;
        int _head;
        int _spacing;

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

        /** Filter a single sample value */
        inline T Filter(T value) {

            // Add new sample to the head of the delay line
            _taps[_head] = value;

            // Sum all taps
            float result = 0;
            for( int k = 0; k < _length; k++ ) {
                result += _taps[_head++] * _coefficients[k];
                if( _head == _length ) {
                    _head = 0;
                }
            }

            // Move tip of the delay line ringbuffer
            _head = _head == 0 ? _length : _head;
            _head--;

            // Return the result
            return result;
        }

    public:

        /** Create a new FIR block (for calculating output values for a FIR filter)

            Parameters:
              coefficients = The FIR coefficients
              length = Length of the FIR filter (number of coefficients)
              [spacing] = How many positions in the outbuffer to space output values
                          This is mostly usefull when implementing multiple polyphase FIR
                          filters (see use in the HInterpolator)
        */
        HFir(float *coefficients, int length, int spacing = 1) : _length(length), _head(0), _spacing(spacing) {

            HLog("HFir(float*, %d, %d)", length, spacing);
            Init(coefficients);
        }

        /** Destroy this FIR block */
        ~HFir() {
            HLog("~HFir()");
            delete[] _taps;
            delete[] _coefficients;
        }

        /** Filter a block of values through the FIR block */
        inline void Filter(T *values, T* result, size_t blocksize) {
            int j = 0;
            for( int i = 0; i < blocksize; i++ ) {
                result[j] = Filter(values[i]);
                j += _spacing;
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

        /** get the coefficients for the FIR block */
        std::vector<float> GetCoefficients() {

            std::vector<float> coefficients;
            for (int i = 0; i < _length; i++) {
                coefficients.push_back(_coefficients[i]);
            }
            return coefficients;
        }

};

#endif
