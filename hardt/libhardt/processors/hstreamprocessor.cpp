#ifndef __HSTREAMPROCESSOR_CPP
#define __HSTREAMPROCESSOR_CPP

#include "hstreamprocessor.h"

template <class T>
HStreamProcessor<T>::HStreamProcessor(HWriter<T>* writer, HReader<T>* reader, int blocksize, bool* terminationToken):
    HProcessor<T>(writer, reader, blocksize, terminationToken)
{
    HLog("HStreamProcessor(...), blocksize is %d", blocksize);
}

template <class T>
HStreamProcessor<T>::HStreamProcessor(HReader<T>* reader, int blocksize, bool* terminationToken):
    HProcessor<T>(reader, blocksize, terminationToken)
{
    HLog("HStreamProcessor::HWriterConsumer(...), blocksize is %d", blocksize);
}

template <class T>
void HStreamProcessor<T>::Run(long unsigned int blocks)
{
    HProcessor<T>::Run(blocks);

    // Dump in- and output metrics to the log
    HLog( this->GetMetrics()->ToString().c_str() );
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HStreamProcessor()
template
HStreamProcessor<int8_t>::HStreamProcessor(HWriter<int8_t>* writer, HReader<int8_t>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<uint8_t>::HStreamProcessor(HWriter<uint8_t>* writer, HReader<uint8_t>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<int16_t>::HStreamProcessor(HWriter<int16_t>* writer, HReader<int16_t>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<int32_t>::HStreamProcessor(HWriter<int32_t>* writer, HReader<int32_t>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<double>::HStreamProcessor(HWriter<double>* writer, HReader<double>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<float>::HStreamProcessor(HWriter<float>* writer, HReader<float>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<std::complex<double>>::HStreamProcessor(HWriter<std::complex<double>>* writer, HReader<std::complex<double>>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<int8_t>::HStreamProcessor(HReader<int8_t>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<uint8_t>::HStreamProcessor(HReader<uint8_t>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<int16_t>::HStreamProcessor(HReader<int16_t>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<int32_t>::HStreamProcessor(HReader<int32_t>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<double>::HStreamProcessor(HReader<double>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<float>::HStreamProcessor(HReader<float>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<std::complex<double>>::HStreamProcessor(HReader<std::complex<double>>* reader, int blocksize, bool* terminationToken);

// Run()
template
void HStreamProcessor<int8_t>::Run(long unsigned int blocks);

template
void HStreamProcessor<uint8_t>::Run(long unsigned int blocks);

template
void HStreamProcessor<int16_t>::Run(long unsigned int blocks);

template
void HStreamProcessor<int32_t>::Run(long unsigned int blocks);

template
void HStreamProcessor<double>::Run(long unsigned int blocks);

template
void HStreamProcessor<float>::Run(long unsigned int blocks);

template
void HStreamProcessor<std::complex<double>>::Run(long unsigned int blocks);

//! @endcond
#endif