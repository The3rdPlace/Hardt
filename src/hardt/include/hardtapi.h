#ifndef __HARDTAPI_H
#define __HARDTAPI_H

#include "hardt.h"

#include "hobject.h"
#include "hwav.h"
#include "hsoundcard.h"
#include "hexceptions.h"

#include "hwriter.h"
#include "hreader.h"
#include "hprocessor.h"

#include "hfilewriter.h"
#include "hwavwriter.h"
#include "hnetworkwriter.h"
#include "hnetworkreader.h"
#include "hnetworkprocessor.h"
#include "hsoundcardreader.h"

/********************************************************************
HWriter
********************************************************************/

// Start()
extern template
bool HWriter<int>::Start(void* data);

// Stop()
extern template
bool HWriter<int>::Stop();

/********************************************************************
HFileWriter
********************************************************************/

// HFileWriter()
extern template
HFileWriter<char>::HFileWriter(const char* path);

extern template
HFileWriter<unsigned char>::HFileWriter(const char* path);

extern template
HFileWriter<int>::HFileWriter(const char* path);

extern template
HFileWriter<int8_t>::HFileWriter(const char* path);

extern template
HFileWriter<int16_t>::HFileWriter(const char* path);

// Start()
extern template
bool HFileWriter<char>::Start(void* data);

extern template
bool HFileWriter<unsigned char>::Start(void* data);

extern template
bool HFileWriter<int>::Start(void* data);

extern template
bool HFileWriter<int8_t>::Start(void* data);

extern template
bool HFileWriter<int16_t>::Start(void* data);

// Stop()
extern template
bool HFileWriter<char>::Stop();

extern template
bool HFileWriter<unsigned char>::Stop();

extern template
bool HFileWriter<int>::Stop();

extern template
bool HFileWriter<int8_t>::Stop();

extern template
bool HFileWriter<int16_t>::Stop();

// Write()
extern template
int HFileWriter<char>::Write(char* dest, size_t blocksize);

extern template
int HFileWriter<unsigned char>::Write(unsigned char* dest, size_t blocksize);

extern template
int HFileWriter<int>::Write(int* dest, size_t blocksize);

extern template
int HFileWriter<int8_t>::Write(int8_t* dest, size_t blocksize);

extern template
int HFileWriter<int16_t>::Write(int16_t* dest, size_t blocksize);

/********************************************************************
HWavWriter
********************************************************************/

extern template
HWavWriter<int>::HWavWriter(const char* filename, H_SAMPLE_FORMAT format, int channels, H_SAMPLE_RATE rate);

extern template
int HWavWriter<int>::Write(int* dest, size_t blocksize);

extern template
bool HWavWriter<int>::Start(void* data);

extern template
bool HWavWriter<int>::Stop();

/********************************************************************
HNetworkWriter
********************************************************************/

// Write()
extern template
int HNetworkWriter<int>::Write(int* src, size_t blocksize);

// Start()
extern template
bool HNetworkWriter<int>::Start(void* socket);

/********************************************************************
HNetworkReader
********************************************************************/

// Read()
extern template
int HNetworkReader<int>::Read(int* dest, size_t blocksize);

// Start()
extern template
bool HNetworkReader<int>::Start(void* socket);

/********************************************************************
HSoundcardReader
********************************************************************/

extern template
HSoundcardReader<int>::HSoundcardReader(int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer = DEFAULT_FRAMESIZE);

extern template
HSoundcardReader<int>::~HSoundcardReader();

extern template
int HSoundcardReader<int>::Read(int* dest, size_t blocksize);

/********************************************************************
HProcessor
********************************************************************/

// HProcessor()
extern template
HProcessor<int>::HProcessor(HWriter<int>* writer, HReader<int>* reader);

// Read()
extern template
int HProcessor<int>::Read(int32_t* dest, int blocksize);

// Write()
extern template
int HProcessor<int>::Write(int* src, int blocksize);

// Start()
extern template
bool HProcessor<int>::Start(void* data);

// Stop()
extern template
bool HProcessor<int>::Stop();

/********************************************************************
HNetworkProcessor
********************************************************************/

// HNetworkProcessor()
extern template
HNetworkProcessor<int>::HNetworkProcessor(const char* address, int port, HWriter<int>* writer, int blocksize, bool* terminationToken);

extern template
HNetworkProcessor<int>::HNetworkProcessor(const char* address, int port, HReader<int>* reader, int blocksize, bool* terminationToken);

extern template
HNetworkProcessor<int>::HNetworkProcessor(int port, HWriter<int>* writer, int blocksize, bool* terminationToken);

extern template
HNetworkProcessor<int>::HNetworkProcessor(int port, HReader<int>* reader, int blocksize, bool* terminationToken);

// ~HNetworkProcessor
extern template
HNetworkProcessor<int>::~HNetworkProcessor();

// Run()
extern template
void HNetworkProcessor<int>::Run();

// Halt()
extern template
void HNetworkProcessor<int>::Halt();

#endif
