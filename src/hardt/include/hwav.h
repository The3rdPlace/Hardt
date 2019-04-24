#ifndef __HWAV_H
#define __HWAV_H

#include "hsoundcard.h"

class HWav
{
    private:

        // Header initializer. All fields that must be modified uses
        // little endian byte order
        //
        // See http://soundfile.sapp.org/doc/WaveFormat/ for a very good
        // primer on the wav format. The test data used in this test is
        // sourced from that page.
        const uint8_t emptyHeader[44] = {
            0x52, // 'R'
            0x49, // 'I'
            0x46, // 'F'
            0x46, // 'F'
            0x24, // Size of data
            0x08, // ..
            0x00, // ..
            0x00, // ..
            0x57, // 'W'
            0x41, // 'A'
            0x56, // 'V'
            0x45, // 'E'

            0x66, // 'f'
            0x6d, // 'm'
            0x74, // 't'
            0x20, // (blankspace)
            0x10, // Chunksize for 'fmt' chunk. Always 16 bytes for wav
            0x00, // ..
            0x00, // ..
            0x00, // ..
            0x01, // Audio format = 1 (PCM)
            0x00, // ..
            0x02, // Number of channels
            0x00, // ..
            0x22, // Samplerate
            0x56, // ..
            0x00, // ..
            0x00, // ..
            0x88, // Byterate
            0x58, // ..
            0x01, // ..
            0x00, // ..
            0x04, // Blockalign = total number of bytes for all channels for 1 sample
            0x00, // ..
            0x10, // Bits per sample
            0x00, // ..

            0x64, // 'd'
            0x61, // 'a'
            0x74, // 't'
            0x61, // 'a'
            0x00, // Total size of data
            0x08, // ..
            0x00, // ..
            0x00, // ..
        };

        const char* _filename;

    protected:

        struct WavHeader
        {
            uint8_t ChunkId[4];
            uint32_t Chunksize;
            uint8_t Format[4];

            uint8_t SubChunk_1_Id[4];
            uint32_t SubChunk_1_size;
            uint16_t AudioFormat;
            uint16_t NumChannels;
            uint32_t SampleRate;
            uint32_t ByteRate;
            uint16_t BlockAlign;
            uint16_t BitsPerSample;

            int8_t SubChunk_2_Id[4];
            int32_t SubChunk_2_size;
        } _header;

        HWav(HWav const& wav)
        {
            _header = wav._header;
            _filename = wav._filename;
        }

        HWav(const char* filename, H_SAMPLE_FORMAT format, int channels, H_SAMPLE_RATE rate, size_t size)
        {
            // Copy the header initializer data into the header member
            memcpy((void*) &_header, (void*) emptyHeader, sizeof(WavHeader));

            // Initialize header
            // Todo: Set all fields that differs according to format and rate

            // Keep filename
            _filename = filename;
        }

    public:

        H_SAMPLE_FORMAT GetFormat()
        {
            return H_SAMPLE_FORMAT_INT_32;
        }

        int GetChannels()
        {
            return 1;
        }

        H_SAMPLE_RATE GetRate()
        {
            return H_SAMPLE_RATE_48K;
        }

        size_t GetSize()
        {
            return 0;
        }

        const char* GetFilename()
        {
            return _filename;
        }

        HWav(const char* filename)
        {
            _filename = filename;
        }
};

#endif