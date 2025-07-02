#ifndef __HBREAKER_H
#define __HBREAKER_H

/**
    A filter that can be made to discard all samples. When switched off, reads will return
    zero samples and writes will silently discard the samples.

    When the component is switched off, Stop() will be called on the
    up- or downstream writer or reader. And when switched on, Start()
    will be called.
*/
template <class T>
class HBreaker : public HFilter<T>
{
    private:

        bool _off;

    public:

        /** Construct a new HBreaker object that writes to a writer */
        HBreaker(std::string id, HWriter<T>* writer, bool off, size_t blocksize);

        /** Construct a new HBreaker object that registers with an upstream writer */
        HBreaker(std::string id, HWriterConsumer<T>* consumer, bool off, size_t blocksize);

        /** Construct a new HBreaker object that reads from a reader */
        HBreaker(std::string id, HReader<T>* reader, bool off, size_t blocksize);

        /** Default destructor */
        ~HBreaker();

        /** Run a block of samples through the mute filter */
        virtual void Filter(T* src, T* dest, size_t blocksize);

        /** Set state */
        void SetOff(bool off);

        /** Get state */
        bool GetOff() {
            return _off;
        }

        /** Initialize before first write */
        bool Start();

        /** Cleanup after last write */
        bool Stop();

        int WriteImpl(T* src, size_t blocksize) {
            return _off ? blocksize : HFilter<T>::WriteImpl(src, blocksize);
        }

        int ReadImpl(T* dest, size_t blocksize) {
            return _off ? 0 : HFilter<T>::ReadImpl(dest, blocksize);
        }
};

#endif
