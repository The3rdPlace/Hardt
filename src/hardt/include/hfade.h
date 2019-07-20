#ifndef __HFADE_H
#define __HFADE_H

template <class T>
class HFade : public HGain<T>
{
    private:

        bool _fade;
        bool _direction; // true = up, false = down
        int _level;
        int _samples;
        int _counter;

    public:

        HFade(HReader<T>* reader, int initialLevel, int samples, bool fade, size_t blocksize);
        HFade(HWriter<T>* writer, int initialLevel, int samples, bool fade, size_t blocksize);

        void Init();

        ~HFade();

        virtual void Filter(T* src, T* dest, size_t blocksize);

        void Up()
        {
            _direction = true;
            _counter = _level * _samples;
            _fade = true;
        }

        void Down()
        {
            _direction = false;
            _counter = _level * _samples;
            _fade = true;
        }
};

#endif