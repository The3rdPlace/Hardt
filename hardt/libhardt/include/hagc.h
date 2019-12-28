#ifndef __HAGC_H
#define __HAGC_H

/**
    A filter providing automatic gain (or attenuation)
*/
template <class T>
class HAgc : public HGain<T>
{
    private:

        T _limit;

    public:

        /** Construct a new HAgc object that writes to a writer */
        HAgc(HWriter<T>* writer, T limit, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HAgc object that registers with an upstream writer */
        HAgc(HWriterConsumer<T>* consumer, T limit, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HAgc object that reads from a reader */
        HAgc(HReader<T>* reader, T limit, size_t blocksize, HProbe<T>* probe = NULL);

        /** Default destructor */
        ~HAgc();

        /** Run a block of samples through the gain filter */
        virtual void Filter(T* src, T* dest, size_t blocksize);

        /** Set bounds */
        void SetLimit(T limit)
        {
            _limit = limit;
        }
};

#endif