#ifndef __HWAV_H
#define __HWAV_H

#include "hsoundcard.h"
#include <iostream>
#include "stdlib.h"
#include <stdio.h>
#include <iostream>
#include <string.h>

#include <cstdlib>
#include <string>
#include <unistd.h>

/**
    This class contains the basic description of a wav file, a generic header
    and functions to map between an array of bytes and specific property names.

    Please be aware that the current wav implementation only supports 1 and 2 channels
    with 16 bit data.

    You should never have to use this class directly!
*/
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
            0x00, // Size of data
            0x00, // ..
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
            0x00, // Number of channels
            0x00, // ..
            0x00, // Samplerate
            0x00, // ..
            0x00, // ..
            0x00, // ..
            0x00, // Byterate
            0x00, // ..
            0x00, // ..
            0x00, // ..
            0x00, // Blockalign = total number of bytes for all channels for 1 sample
            0x00, // ..
            0x00, // Bits per sample
            0x00, // ..

            0x64, // 'd'
            0x61, // 'a'
            0x74, // 't'
            0x61, // 'a'
            0x00, // Total size of data
            0x00, // ..
            0x00, // ..
            0x00, // ..
        };

        const char* _filename;

    protected:

        /** Mapping of datafields in the wav header. Please see the wav specification
            for detailed description of the fields..  It can be found here:
            http://soundfile.sapp.org/doc/WaveFormat */
        //! @cond Doxygen_Suppress
        struct WavHeader
        {

            uint8_t ChunkId[4];
            uint32_t ChunkSize;
            uint8_t Format[4];

            uint8_t SubChunk_1_Id[4];
            uint32_t SubChunk_1_Size;
            uint16_t AudioFormat;
            uint16_t NumChannels;
            uint32_t SampleRate;
            uint32_t ByteRate;
            uint16_t BlockAlign;
            uint16_t BitsPerSample;

            uint8_t SubChunk_2_Id[4];
            uint32_t SubChunk_2_Size;
        };
        //! @endcond

        /** Contains the header data of the wav object */
        WavHeader _header;

        /** Construct a new wav object from another wav object */
        HWav(HWav const& wav):
            _header(wav._header),
            _filename(wav._filename)
        {}

        /** Construct a new wav object with the given format, channels and samplerate */
        HWav(const char* filename, H_SAMPLE_FORMAT format, int channels, H_SAMPLE_RATE rate);

    public:

        /** Return the sample format used by this object */
        H_SAMPLE_FORMAT GetFormat();

        /** Return the number of channels used by this object */
        int GetChannels();

        /** Return the samplerate used by this object */
        H_SAMPLE_RATE GetRate();

        /** Get the size of the sample data contained by this object */
        size_t GetSize();

        /** Get the name of the file from where the sample where read, or should
            be written */
        const char* GetFilename();

        /** Construct a new wav object from a local file */
        HWav(const char* filename);
};

#endif