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
#include "hgenerator.h"

#include "hfilewriter.h"
#include "hfilereader.h"
#include "hwavwriter.h"
#include "hwavreader.h"
#include "hnetworkwriter.h"
#include "hnetworkreader.h"
#include "hnetworkprocessor.h"
#include "hsoundcardreader.h"
#include "hsinegenerator.h"
#include "hcosinegenerator.h"
#include "hvfo.h"

/********************************************************************
HWriter
********************************************************************/

// Start()
extern template
bool HWriter<int8_t>::Start(void* data);

extern template
bool HWriter<uint8_t>::Start(void* data);

extern template
bool HWriter<int16_t>::Start(void* data);

extern template
bool HWriter<int32_t>::Start(void* data);

// Stop()
extern template
bool HWriter<int8_t>::Stop();

extern template
bool HWriter<uint8_t>::Stop();

extern template
bool HWriter<int16_t>::Stop();

extern template
bool HWriter<int32_t>::Stop();

/********************************************************************
HFileWriter
********************************************************************/

// HFileWriter()
extern template
HFileWriter<int8_t>::HFileWriter(const char* path);

extern template
HFileWriter<uint8_t>::HFileWriter(const char* path);

extern template
HFileWriter<int16_t>::HFileWriter(const char* path);

extern template
HFileWriter<int32_t>::HFileWriter(const char* path);

// Start()
extern template
bool HFileWriter<int8_t>::Start(void* data);

extern template
bool HFileWriter<uint8_t>::Start(void* data);

extern template
bool HFileWriter<int16_t>::Start(void* data);

extern template
bool HFileWriter<int32_t>::Start(void* data);

// Stop()
extern template
bool HFileWriter<int8_t>::Stop();

extern template
bool HFileWriter<uint8_t>::Stop();

extern template
bool HFileWriter<int16_t>::Stop();

extern template
bool HFileWriter<int32_t>::Stop();

// Write()
extern template
int HFileWriter<int8_t>::Write(int8_t* dest, size_t blocksize);

extern template
int HFileWriter<uint8_t>::Write(uint8_t* dest, size_t blocksize);

extern template
int HFileWriter<int16_t>::Write(int16_t* dest, size_t blocksize);

extern template
int HFileWriter<int32_t>::Write(int32_t* dest, size_t blocksize);

/********************************************************************
HFileReader
********************************************************************/

// HFileReader()
extern template
HFileReader<int8_t>::HFileReader(const char* path);

extern template
HFileReader<uint8_t>::HFileReader(const char* path);

extern template
HFileReader<int16_t>::HFileReader(const char* path);

extern template
HFileReader<int32_t>::HFileReader(const char* path);

// Start()
extern template
bool HFileReader<int8_t>::Start(void* data);

extern template
bool HFileReader<uint8_t>::Start(void* data);

extern template
bool HFileReader<int16_t>::Start(void* data);

extern template
bool HFileReader<int32_t>::Start(void* data);

// Stop()
extern template
bool HFileReader<int8_t>::Stop();

extern template
bool HFileReader<uint8_t>::Stop();

extern template
bool HFileReader<int16_t>::Stop();

extern template
bool HFileReader<int32_t>::Stop();

// Read()
extern template
int HFileReader<int8_t>::Read(int8_t* dest, size_t blocksize);

extern template
int HFileReader<uint8_t>::Read(uint8_t* dest, size_t blocksize);

extern template
int HFileReader<int16_t>::Read(int16_t* dest, size_t blocksize);

extern template
int HFileReader<int32_t>::Read(int32_t* dest, size_t blocksize);

/********************************************************************
HWavWriter
********************************************************************/

// HWavWriter()
extern template
HWavWriter<int8_t>::HWavWriter(const char* filename, H_SAMPLE_FORMAT format, int channels, H_SAMPLE_RATE rate);

extern template
HWavWriter<uint8_t>::HWavWriter(const char* filename, H_SAMPLE_FORMAT format, int channels, H_SAMPLE_RATE rate);

extern template
HWavWriter<int16_t>::HWavWriter(const char* filename, H_SAMPLE_FORMAT format, int channels, H_SAMPLE_RATE rate);

extern template
HWavWriter<int32_t>::HWavWriter(const char* filename, H_SAMPLE_FORMAT format, int channels, H_SAMPLE_RATE rate);

// Start()
extern template
bool HWavWriter<int8_t>::Start(void* unused);

extern template
bool HWavWriter<uint8_t>::Start(void* unused);

extern template
bool HWavWriter<int16_t>::Start(void* unused);

extern template
bool HWavWriter<int32_t>::Start(void* unused);

// Stop()
extern template
bool HWavWriter<int8_t>::Stop();

extern template
bool HWavWriter<uint8_t>::Stop();

extern template
bool HWavWriter<int16_t>::Stop();

extern template
bool HWavWriter<int32_t>::Stop();

// Write()
extern template
int HWavWriter<int8_t>::Write(int8_t* dest, size_t blocksize);

extern template
int HWavWriter<uint8_t>::Write(uint8_t* dest, size_t blocksize);

extern template
int HWavWriter<int16_t>::Write(int16_t* dest, size_t blocksize);

extern template
int HWavWriter<int32_t>::Write(int32_t* dest, size_t blocksize);

/********************************************************************
HWavReader
********************************************************************/

// HWavReader()
extern template
HWavReader<int8_t>::HWavReader(const char* filename);

extern template
HWavReader<uint8_t>::HWavReader(const char* filename);

extern template
HWavReader<int16_t>::HWavReader(const char* filename);

extern template
HWavReader<int32_t>::HWavReader(const char* filename);

// Start()
extern template
bool HWavReader<int8_t>::Start(void* unused);

extern template
bool HWavReader<uint8_t>::Start(void* unused);

extern template
bool HWavReader<int16_t>::Start(void* unused);

extern template
bool HWavReader<int32_t>::Start(void* unused);

/********************************************************************
HNetworkWriter
********************************************************************/

// Write()
extern template
int HNetworkWriter<int8_t>::Write(int8_t* src, size_t blocksize);

extern template
int HNetworkWriter<uint8_t>::Write(uint8_t* src, size_t blocksize);

extern template
int HNetworkWriter<int16_t>::Write(int16_t* src, size_t blocksize);

extern template
int HNetworkWriter<int32_t>::Write(int32_t* src, size_t blocksize);

// Start()
extern template
bool HNetworkWriter<int8_t>::Start(void* socket);

extern template
bool HNetworkWriter<uint8_t>::Start(void* socket);

extern template
bool HNetworkWriter<int16_t>::Start(void* socket);

extern template
bool HNetworkWriter<int32_t>::Start(void* socket);

/********************************************************************
HNetworkReader
********************************************************************/

// Read()
extern template
int HNetworkReader<int8_t>::Read(int8_t* dest, size_t blocksize);

extern template
int HNetworkReader<uint8_t>::Read(uint8_t* dest, size_t blocksize);

extern template
int HNetworkReader<int16_t>::Read(int16_t* dest, size_t blocksize);

extern template
int HNetworkReader<int32_t>::Read(int32_t* dest, size_t blocksize);

// Start()
extern template
bool HNetworkReader<int8_t>::Start(void* socket);

extern template
bool HNetworkReader<uint8_t>::Start(void* socket);

extern template
bool HNetworkReader<int16_t>::Start(void* socket);

extern template
bool HNetworkReader<int32_t>::Start(void* socket);

/********************************************************************
HSoundcardReader
********************************************************************/

// HSoundcardReader()
extern template
HSoundcardReader<int8_t>::HSoundcardReader(int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer = DEFAULT_FRAMESIZE);

extern template
HSoundcardReader<uint8_t>::HSoundcardReader(int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer = DEFAULT_FRAMESIZE);

extern template
HSoundcardReader<int16_t>::HSoundcardReader(int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer = DEFAULT_FRAMESIZE);

extern template
HSoundcardReader<int32_t>::HSoundcardReader(int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer = DEFAULT_FRAMESIZE);

// ~HSoundcardReader()
extern template
HSoundcardReader<int8_t>::~HSoundcardReader();

extern template
HSoundcardReader<uint8_t>::~HSoundcardReader();

extern template
HSoundcardReader<int16_t>::~HSoundcardReader();

extern template
HSoundcardReader<int32_t>::~HSoundcardReader();

// Read()
extern template
int HSoundcardReader<int8_t>::Read(int8_t* dest, size_t blocksize);

extern template
int HSoundcardReader<uint8_t>::Read(uint8_t* dest, size_t blocksize);

extern template
int HSoundcardReader<int16_t>::Read(int16_t* dest, size_t blocksize);

extern template
int HSoundcardReader<int32_t>::Read(int32_t* dest, size_t blocksize);

/********************************************************************
HProcessor
********************************************************************/

// HProcessor()
extern template
HProcessor<int8_t>::HProcessor(HWriter<int8_t>* writer, HReader<int8_t>* reader);

extern template
HProcessor<uint8_t>::HProcessor(HWriter<uint8_t>* writer, HReader<uint8_t>* reader);

extern template
HProcessor<int16_t>::HProcessor(HWriter<int16_t>* writer, HReader<int16_t>* reader);

extern template
HProcessor<int32_t>::HProcessor(HWriter<int32_t>* writer, HReader<int32_t>* reader);

// Read()
extern template
int HProcessor<int8_t>::Read(int8_t* dest, int blocksize);

extern template
int HProcessor<uint8_t>::Read(uint8_t* dest, int blocksize);

extern template
int HProcessor<int16_t>::Read(int16_t* dest, int blocksize);

extern template
int HProcessor<int32_t>::Read(int32_t* dest, int blocksize);

// Write()
extern template
int HProcessor<int8_t>::Write(int8_t* src, int blocksize);

extern template
int HProcessor<uint8_t>::Write(uint8_t* src, int blocksize);

extern template
int HProcessor<int16_t>::Write(int16_t* src, int blocksize);

extern template
int HProcessor<int32_t>::Write(int32_t* src, int blocksize);

// Start()
extern template
bool HProcessor<int8_t>::Start(void* data);

extern template
bool HProcessor<uint8_t>::Start(void* data);

extern template
bool HProcessor<int16_t>::Start(void* data);

extern template
bool HProcessor<int32_t>::Start(void* data);

// Stop()
extern template
bool HProcessor<int8_t>::Stop();

extern template
bool HProcessor<uint8_t>::Stop();

extern template
bool HProcessor<int16_t>::Stop();

extern template
bool HProcessor<int32_t>::Stop();

/********************************************************************
HNetworkProcessor
********************************************************************/

// HNetworkProcessor()
extern template
HNetworkProcessor<int8_t>::HNetworkProcessor(const char* address, int port, HWriter<int8_t>* writer, int blocksize, bool* terminationToken);

extern template
HNetworkProcessor<uint8_t>::HNetworkProcessor(const char* address, int port, HWriter<uint8_t>* writer, int blocksize, bool* terminationToken);

extern template
HNetworkProcessor<int16_t>::HNetworkProcessor(const char* address, int port, HWriter<int16_t>* writer, int blocksize, bool* terminationToken);

extern template
HNetworkProcessor<int32_t>::HNetworkProcessor(const char* address, int port, HWriter<int32_t>* writer, int blocksize, bool* terminationToken);

extern template
HNetworkProcessor<int8_t>::HNetworkProcessor(const char* address, int port, HReader<int8_t>* reader, int blocksize, bool* terminationToken);

extern template
HNetworkProcessor<uint8_t>::HNetworkProcessor(const char* address, int port, HReader<uint8_t>* reader, int blocksize, bool* terminationToken);

extern template
HNetworkProcessor<int16_t>::HNetworkProcessor(const char* address, int port, HReader<int16_t>* reader, int blocksize, bool* terminationToken);

extern template
HNetworkProcessor<int32_t>::HNetworkProcessor(const char* address, int port, HReader<int32_t>* reader, int blocksize, bool* terminationToken);

extern template
HNetworkProcessor<int8_t>::HNetworkProcessor(int port, HWriter<int8_t>* writer, int blocksize, bool* terminationToken);

extern template
HNetworkProcessor<uint8_t>::HNetworkProcessor(int port, HWriter<uint8_t>* writer, int blocksize, bool* terminationToken);

extern template
HNetworkProcessor<int16_t>::HNetworkProcessor(int port, HWriter<int16_t>* writer, int blocksize, bool* terminationToken);

extern template
HNetworkProcessor<int32_t>::HNetworkProcessor(int port, HWriter<int32_t>* writer, int blocksize, bool* terminationToken);

extern template
HNetworkProcessor<int8_t>::HNetworkProcessor(int port, HReader<int8_t>* reader, int blocksize, bool* terminationToken);

extern template
HNetworkProcessor<uint8_t>::HNetworkProcessor(int port, HReader<uint8_t>* reader, int blocksize, bool* terminationToken);

extern template
HNetworkProcessor<int16_t>::HNetworkProcessor(int port, HReader<int16_t>* reader, int blocksize, bool* terminationToken);

extern template
HNetworkProcessor<int32_t>::HNetworkProcessor(int port, HReader<int32_t>* reader, int blocksize, bool* terminationToken);

// ~HNetworkProcessor
extern template
HNetworkProcessor<int8_t>::~HNetworkProcessor();

extern template
HNetworkProcessor<uint8_t>::~HNetworkProcessor();

extern template
HNetworkProcessor<int16_t>::~HNetworkProcessor();

extern template
HNetworkProcessor<int32_t>::~HNetworkProcessor();

// Run()
extern template
void HNetworkProcessor<int8_t>::Run();

extern template
void HNetworkProcessor<uint8_t>::Run();

extern template
void HNetworkProcessor<int16_t>::Run();

extern template
void HNetworkProcessor<int32_t>::Run();

// Halt()
extern template
void HNetworkProcessor<int8_t>::Halt();

extern template
void HNetworkProcessor<uint8_t>::Halt();

extern template
void HNetworkProcessor<int16_t>::Halt();

extern template
void HNetworkProcessor<int32_t>::Halt();

/********************************************************************
HGenerator
********************************************************************/

// HGenerator
extern template
HGenerator<int8_t>::HGenerator(H_SAMPLE_RATE rate, int frequency, int8_t amplitude, float phase);

extern template
HGenerator<uint8_t>::HGenerator(H_SAMPLE_RATE rate, int frequency, uint8_t amplitude, float phase);

extern template
HGenerator<int16_t>::HGenerator(H_SAMPLE_RATE rate, int frequency, int16_t amplitude, float phase);

extern template
HGenerator<int32_t>::HGenerator(H_SAMPLE_RATE rate, int frequency, int32_t amplitude, float phase);

// ~HGenerator
extern template
HGenerator<int8_t>::~HGenerator();

extern template
HGenerator<uint8_t>::~HGenerator();

extern template
HGenerator<int16_t>::~HGenerator();

extern template
HGenerator<int32_t>::~HGenerator();

// Read
extern template
int HGenerator<int8_t>::Read(int8_t* dest, size_t blocksize);

extern template
int HGenerator<uint8_t>::Read(uint8_t* dest, size_t blocksize);

extern template
int HGenerator<int16_t>::Read(int16_t* dest, size_t blocksize);

extern template
int HGenerator<int32_t>::Read(int32_t* dest, size_t blocksize);

/********************************************************************
HSineGenerator
********************************************************************/

// HSineGenerator
extern template
HSineGenerator<int8_t>::HSineGenerator(H_SAMPLE_RATE rate, int frequency, int8_t amplitude);

extern template
HSineGenerator<uint8_t>::HSineGenerator(H_SAMPLE_RATE rate, int frequency, uint8_t amplitude);

extern template
HSineGenerator<int16_t>::HSineGenerator(H_SAMPLE_RATE rate, int frequency, int16_t amplitude);

extern template
HSineGenerator<int32_t>::HSineGenerator(H_SAMPLE_RATE rate, int frequency, int32_t amplitude);

/********************************************************************
HCosineGenerator
********************************************************************/

// HCosineGenerator
extern template
HCosineGenerator<int8_t>::HCosineGenerator(H_SAMPLE_RATE rate, int frequency, int8_t amplitude);

extern template
HCosineGenerator<uint8_t>::HCosineGenerator(H_SAMPLE_RATE rate, int frequency, uint8_t amplitude);

extern template
HCosineGenerator<int16_t>::HCosineGenerator(H_SAMPLE_RATE rate, int frequency, int16_t amplitude);

extern template
HCosineGenerator<int32_t>::HCosineGenerator(H_SAMPLE_RATE rate, int frequency, int32_t amplitude);

/********************************************************************
HVfo
********************************************************************/

// HVfo
extern template
HVfo<int8_t>::HVfo(H_SAMPLE_RATE rate, int frequency, int8_t amplitude, float phase);

extern template
HVfo<uint8_t>::HVfo(H_SAMPLE_RATE rate, int frequency, uint8_t amplitude, float phase);

extern template
HVfo<int16_t>::HVfo(H_SAMPLE_RATE rate, int frequency, int16_t amplitude, float phase);

extern template
HVfo<int32_t>::HVfo(H_SAMPLE_RATE rate, int frequency, int32_t amplitude, float phase);

// SetFrequency
extern template
void HVfo<int8_t>::SetFrequency(int frequency, float phase);

extern template
void HVfo<uint8_t>::SetFrequency(int frequency, float phase);

extern template
void HVfo<int16_t>::SetFrequency(int frequency, float phase);

extern template
void HVfo<int32_t>::SetFrequency(int frequency, float phase);

#endif
