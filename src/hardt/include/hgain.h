#ifndef __HGAIN_H
#define __HGAIN_H

template <class T>
class HGain : public HWriter<T>, public HReader<T>
{
    private:

        HWriter<T>* _writer;
        HReader<T>* _reader;

        int _blocksize;
        T* _buffer;

        float _gain;

    public:

        HGain(HWriter<T>* writer, float gain, size_t blocksize);
        HGain(HReader<T>* reader, float gain, size_t blocksize);

        ~HGain();

        int Write(T* src, size_t blocksize);
        int Read(T* dest, size_t blocksize);

        void SetGain(float gain);
};

#endif