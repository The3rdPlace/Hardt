#ifndef __HWAVWRITER_H
#define __HWAVWRITER_H

/**
    Write samples to a wav file
*/
template <class T>
class HWavWriter : public HWav, public HFileWriter<T>
{
    private:

        size_t _size;

    public:

        /** Construct a new wav writer */
        HWavWriter(std::string id, const char* filename, H_SAMPLE_FORMAT format, int channels, H_SAMPLE_RATE rate, bool sequence = false):
            HWav(filename, format, channels, rate),
            HFileWriter<T>(id, filename, sequence),
            _size(0)
        {}

        /** Construct a new wav writer */
        HWavWriter(std::string id, const char* filename, H_SAMPLE_FORMAT format, int channels, H_SAMPLE_RATE rate, HWriterConsumer<T>* consumer, bool sequence = false):
            HWav(filename, format, channels, rate),
            HFileWriter<T>(id, filename, consumer, sequence),
            _size(0)
        {}

        /** Initialize before first write */
        bool Start();

        /** Complete the wav header with the final size, and cleanup after last write */
        bool Stop();

        /** Write to this wav stream */
        virtual int WriteImpl(T* dest, size_t blocksize);

        /** Execute or carry through a command */
        bool Command(HCommand* command) {
            // No further propagation of commands
            return true;
        }
};

#endif