#ifndef __HAGC_H
#define __HAGC_H

/**
    A filter providing automatic gain (or attenuation)
*/
template <class T>
class HAgc : public HGain<T>
{
    private:

        T _upper;
        T _limit;
        float _rate;

    public:

        /** Construct a new HAgc object that writes to a writer */
        HAgc(HWriter<T>* writer, T upper, T limit, float rate, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HAgc object that registers with an upstream writer */
        HAgc(HWriterConsumer<T>* consumer, T upper, T limit, float rate, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HAgc object that reads from a reader */
        HAgc(HReader<T>* reader, T upper, T limit, float rate, size_t blocksize, HProbe<T>* probe = NULL);

        /** Default destructor */
        ~HAgc();

        /** Run a block of samples through the gain filter */
        virtual void Filter(T* src, T* dest, size_t blocksize);

        /** Set bounds */
        void SetBounds(T upper, T lower)
        {
            _upper = upper;
            _limit = limit;
        }

        void SetRate(float rate)
        {
            _rate = rate;
        }
};

#endif