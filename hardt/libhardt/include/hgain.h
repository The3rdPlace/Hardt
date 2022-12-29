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
        HGain(std::string id, HWriter<T>* writer, float gain, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HGain object that registers with an upstream writer */
        HGain(std::string id, HWriterConsumer<T>* consumer, float gain, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HGain object that reads from a reader */
        HGain(std::string id, HReader<T>* reader, float gain, size_t blocksize, HProbe<T>* probe = NULL);

        /** Default destructor */
        ~HGain();

        /** Run a block of samples through the gain filter */
        virtual void Filter(T* src, T* dest, size_t blocksize);

        /** Set gain */
        void SetGain(float gain);

        /** Get gain */
        float GetGain()
        {
            return _gain;
        }
};

#endif