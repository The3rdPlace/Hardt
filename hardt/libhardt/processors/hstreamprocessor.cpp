#ifndef __HSTREAMPROCESSOR_CPP
#define __HSTREAMPROCESSOR_CPP

#include "hstreamprocessor.h"

template <class T>
HStreamProcessor<T>::HStreamProcessor(std::string id, HWriter<T>* writer, HReader<T>* reader, int blocksize, bool* terminationToken):
    HProcessor<T>(id, writer, reader, blocksize, terminationToken)
{
    HLog("HStreamProcessor(...), blocksize is %d", blocksize);
}

template <class T>
HStreamProcessor<T>::HStreamProcessor(std::string id, HReader<T>* reader, int blocksize, bool* terminationToken):
    HProcessor<T>(id, reader, blocksize, terminationToken)
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
HStreamProcessor<int8_t>::HStreamProcessor(std::string id, HWriter<int8_t>* writer, HReader<int8_t>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<uint8_t>::HStreamProcessor(std::string id, HWriter<uint8_t>* writer, HReader<uint8_t>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<int16_t>::HStreamProcessor(std::string id, HWriter<int16_t>* writer, HReader<int16_t>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<int32_t>::HStreamProcessor(std::string id, HWriter<int32_t>* writer, HReader<int32_t>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<double>::HStreamProcessor(std::string id, HWriter<double>* writer, HReader<double>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<float>::HStreamProcessor(std::string id, HWriter<float>* writer, HReader<float>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<std::complex<double>>::HStreamProcessor(std::string id, HWriter<std::complex<double>>* writer, HReader<std::complex<double>>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<int8_t>::HStreamProcessor(std::string id, HReader<int8_t>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<uint8_t>::HStreamProcessor(std::string id, HReader<uint8_t>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<int16_t>::HStreamProcessor(std::string id, HReader<int16_t>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<int32_t>::HStreamProcessor(std::string id, HReader<int32_t>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<double>::HStreamProcessor(std::string id, HReader<double>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<float>::HStreamProcessor(std::string id, HReader<float>* reader, int blocksize, bool* terminationToken);

template
HStreamProcessor<std::complex<double>>::HStreamProcessor(std::string id, HReader<std::complex<double>>* reader, int blocksize, bool* terminationToken);

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