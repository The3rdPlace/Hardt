#include "hardt.h"
#include "hwav.h"
#include "hexceptions.h"

HWav::HWav(const char* filename, H_SAMPLE_FORMAT format, int channels, H_SAMPLE_RATE rate):
    _filename(filename)
{
    // Copy the header initializer data into the header member
    memcpy((void*) &_header, (void*) emptyHeader, sizeof(WavHeader));

    // Set sample format (bits per sample)
    switch(format)
    {
        case H_SAMPLE_FORMAT_UINT_8:
            _header.BitsPerSample = 8;
            break;
        case H_SAMPLE_FORMAT_INT_16:
            _header.BitsPerSample = 16;
            break;
        default:
            HError("Unsupported sample format %d", format);
            throw new HInitializationException("Unknown sample format");
    }

    // Set number of channels
    _header.NumChannels = (uint16_t) channels;

    // Set sample rate
    switch(rate)
    {
        case H_SAMPLE_RATE_8K:
            _header.SampleRate = 8000;
            break;
        case H_SAMPLE_RATE_11K:
            _header.SampleRate = 11025;
            break;
        case H_SAMPLE_RATE_22K:
            _header.SampleRate = 22050;
            break;
        case H_SAMPLE_RATE_32K:
            _header.SampleRate = 32000;
            break;
        case H_SAMPLE_RATE_44K1:
            _header.SampleRate = 44100;
            break;
        case H_SAMPLE_RATE_48K:
            _header.SampleRate = 48000;
            break;
        case H_SAMPLE_RATE_96K:
            _header.SampleRate = 96000;
            break;
        case H_SAMPLE_RATE_192K:
            _header.SampleRate = 192000;
            break;
        default:
            HError("Unsupported sample rate %d", rate);
            throw new HInitializationException("Unsupported sample rate");
    }

    // Set byterate, which is the total number of bytes per second
    // (samplerate * channels * bytes-per-sample)
    _header.ByteRate = (uint32_t) _header.SampleRate * _header.NumChannels * (_header.BitsPerSample / 8);

    // Set blockalign, which is the total number of bytes per sample, for all channels
    _header.BlockAlign = (uint16_t) (_header.BitsPerSample / 8) * _header.NumChannels;
    // Size of the data can only be set when all data has been written and the complete
    // size of the data is known
    _header.SubChunk_2_Size = (uint32_t) 0;

    // This also goes for the initial chunksize field. This is the size of the
    // entire file plus 36 (read the spec at http://soundfile.sapp.org/doc/WaveFormat/).
    // This value can only be written after we know the data size
    _header.ChunkSize = (uint32_t) 36;
}

HWav::HWav(const char* filename):
    _filename(filename)
{
    // Todo: Read header from the named file
}

H_SAMPLE_FORMAT HWav::GetFormat()
{
    switch(_header.BitsPerSample)
    {
        case 8:
            return H_SAMPLE_FORMAT_UINT_8;
        case 16:
            return H_SAMPLE_FORMAT_INT_16;
        default:
            HError("Invalid sample format (bits per sample is %d)", _header.BitsPerSample);
            return 0;
    }
}

int HWav::GetChannels()
{
    return _header.NumChannels;
}

H_SAMPLE_RATE HWav::GetRate()
{
    switch(_header.SampleRate)
    {
        case 8000:
            return H_SAMPLE_RATE_8K;
        case 11025:
            return H_SAMPLE_RATE_11K;
        case 22050:
            return H_SAMPLE_RATE_22K;
        case 32000:
            return H_SAMPLE_RATE_32K;
        case 44100:
            return H_SAMPLE_RATE_44K1;
        case 48000:
            return H_SAMPLE_RATE_48K;
        case 96000:
            return H_SAMPLE_RATE_96K;
        case 192000:
            return H_SAMPLE_RATE_192K;
        default:
            HError("Invalid sample rate %d", _header.SampleRate);
            return 0;
    }
}

size_t HWav::GetSize()
{
    return _header.SubChunk_2_Size;
}

const char* HWav::GetFilename()
{
    return _filename;
}
