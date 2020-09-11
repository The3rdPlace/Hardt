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
        int _advance;
        int _skip;

        /**
         *
         * @param coefficients Fir coefficients
         */
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

        /** Filter a single sample value then add a further number of samples
         *  to the buffer without summing - polyphase filtering in a fir-decimator
         *
         *  @param value Value to filter
         *  @param advance If more than 1, then a futher 'advance' number of samples
         *               will be steppped into the delay line (used for polyphase filters)
         */
        inline T Filter(T* value, int advance) {

            T result = Filter(*value);

            for( int i = 1; i < advance; i++ ) {

                // Add new sample to the head of the delay line
                _taps[_head] = value[i];

                // Move tip of the delay line ringbuffer
                _head = _head == 0 ? _length : _head;
                _head--;
            }

            // Return the result
            return result;
        }

    public:

        /** Create a new FIR block (for calculating output values for a FIR filter)
         *
         *  @param coefficients The FIR coefficients
         *  @param length Length of the FIR filter (number of coefficients)
         */
        HFir(float *coefficients, int length) : _length(length), _head(0), _spacing(1), _advance(1), _skip(1) {

            HLog("HFir(float*, %d)", length);
            Init(coefficients);
        }

        /** Create a new FIR block (for calculating output values for a FIR filter)
         *
         *  @param coefficients The FIR coefficients
         *  @param length Length of the FIR filter (number of coefficients)
         *  @param spacing How many positions in the outbuffer to space output values
         *                 This is mostly usefull when implementing multiple polyphase FIR
         *                 filters (see use in the HInterpolator). A normal FIR filter uses
         *                 a spacing of 1 (write output to adjacent locations)
         *  @param advance How many steps to advance the delay line before next filter output
         *                 value is being calculated (see use in HFirDecimator). A normal FIR filter
         *                 uses advance=1 (do not further advance forward after each result) but a decimating
         *                 FIR filter will calculate 1 result, then step further a number of samples into the
         *                 delay line before calculating the next result.
         *  @param skip Skip this number of values in the input and output buffer between each value.
         *              This allows you to pass in sparse buffers and only use, say, each 2. sample. Do note
         *              that the skip is applied to the input AS WELL as the output
         */
        HFir(float *coefficients, int length, int spacing, int advance, int skip):
            _length(length), _head(0), _spacing(spacing), _advance(advance), _skip(skip) {

            HLog("HFir(float*, length=%d, spacing=%d, advance=%d, skip=%d)", length, spacing, advance, skip);
            Init(coefficients);
        }

        /**
         * Destroy this FIR block
         */
        ~HFir() {
            HLog("~HFir()");
            delete[] _taps;
            delete[] _coefficients;
        }

        /** Filter a block of values through the FIR block
         *
         * @param values The block to filter
         * @param result Place filtered block here
         * @param blocksize The number of samples in the input block
         * */
        inline void Filter(T *values, T* result, size_t blocksize) {

            /* TEMPORARY HACK
             *
             * Demultiplex incomming samples if skip is set higher
             * than 1 - usually iq samples
             *
             * Todo: This should be done in the filter loop
             */
            T tmp[blocksize / _skip];
            int x = 0;
            for( int i = 0; i < blocksize; i += _skip ) {
                tmp[x++] = values[i];
            }
            /* END OF HACK */

            int j = 0;
            for( int i = 0; i < blocksize / _skip; i += _advance ) {
                result[j] = Filter(&tmp[i], _advance);
                j += (_spacing - 1) + (_advance * _skip) ;//+ _skip;
            }
        }

        /** Change the coefficients for the FIR block
         *
         *  @param coefficients New coefficients
         *  @param length number of coefficients
         * */
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

        /** Get the coefficients for the FIR block
         */
        std::vector<float> GetCoefficients() {

            std::vector<float> coefficients;
            for (int i = 0; i < _length; i++) {
                coefficients.push_back(_coefficients[i]);
            }
            return coefficients;
        }
};

#endif
