#ifndef __HFADE_H
#define __HFADE_H

/**
    Provides a component that will fade the signal level up or down
    over a specified number of sample blocks.
*/
template <class T>
class HFade : public HGain<T>
{
    private:

        bool _fade;
        bool _direction; // true = up, false = down
        int _level;
        int _samples;
        int _counter;

        void Init();

    public:

        /** Construct a new HFade object that reads from a reader */
        HFade(std::string id, HReader<T>* reader, int initialLevel, int samples, bool fade, size_t blocksize);

        /** Construct a new HFade object that writes to a writer */
        HFade(std::string id, HWriter<T>* writer, int initialLevel, int samples, bool fade, size_t blocksize);

        /** Construct a new HFade object that registers with an upstream writer */
        HFade(std::string id, HWriterConsumer<T>* consumer, int initialLevel, int samples, bool fade, size_t blocksize);

        /** Default destructor */
        ~HFade();

        /** Run a block of samples through the fading filter */
        virtual void Filter(T* src, T* dest, size_t blocksize);

        /** Fade up */
        void Up()
        {
            _direction = true;
            _counter = _level * _samples;
            _fade = true;
        }

        /** Fade down */
        void Down()
        {
            _direction = false;
            _counter = _level * _samples;
            _fade = true;
        }
};

#endif