#ifndef __HGAIN_H
#define __HGAIN_H

/**
    A filter providing gain (or attenuation)
*/
template <class T>
class HGain : public HFilter<T>
{
    private:

        float _gain;

    public:

        /** Construct a new HGain object that writes to a writer */
        HGain(HWriter<T>* writer, float gain, size_t blocksize);

        /** Construct a new HGain object that registers with an upstream writer */
        HGain(HWriterConsumer<T>* consumer, float gain, size_t blocksize);

        /** Construct a new HGain object that reads from a reader */
        HGain(HReader<T>* reader, float gain, size_t blocksize);

        /** Default destructor */
        ~HGain();

        /** Run a block of samples through the gain filter */
        virtual void Filter(T* src, T* dest, size_t blocksize);

        /** Set gain */
        void SetGain(float gain);
};

#endif