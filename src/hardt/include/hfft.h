#ifndef __HFFT_H
#define __HFFT_H

template <class T>
class HFft : public HConverter<T, long int>
{
    private:

        int _size;
        int _average;

        long int* _magnitude;

        int _count;
        HWriter<long int>* _magnitudeWriter;
        HWriter<int32_t>* _phaseWriter;
        HWindow<T>* _window;

        T* _buffer;

    public:

        HFft(int size, int average, HWriter<long int>* magnitudeWriter, HWindow<T>* window);
        HFft(int size, HWriter<long int>* magnitudeWriter, HWindow<T>* window);
        HFft(int size, HWriter<long int>* magnitudeWriter, HWriter<int32_t>* phaseWriter, HWindow<T>* window);
        HFft(int size, HWriter<int32_t>* phaseWriter, HWindow<T>* window);

        ~HFft()
        {
            delete _spectrum;
            delete _buffer;
        }

        int Convert(T* src, size_t size);

        bool Start()
        {
            return _writer != NULL ?_writer->Start() : true;
        }

        bool Stop()
        {
            return _writer != NULL ?_writer->Stop() : true;
        }

    private:

        HFft(int size, int average, HWriter<long int>* magnitudeWriter, HWriter<int32_t>* phaseWriter, HWindow<T>* window);
};

#endif
