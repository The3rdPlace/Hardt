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
#include "hconverter.h"

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
#include "hstreamprocessor.h"
#include "hsoundcardwriter.h"

/********************************************************************
HWriter
********************************************************************/

// Start()
extern template
bool HWriter<int8_t>::Start();

extern template
bool HWriter<uint8_t>::Start();

extern template
bool HWriter<int16_t>::Start();

extern template
bool HWriter<int32_t>::Start();

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
bool HFileWriter<int8_t>::Start();

extern template
bool HFileWriter<uint8_t>::Start();

extern template
bool HFileWriter<int16_t>::Start();

extern template
bool HFileWriter<int32_t>::Start();

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
bool HFileReader<int8_t>::Start();

extern template
bool HFileReader<uint8_t>::Start();

extern template
bool HFileReader<int16_t>::Start();

extern template
bool HFileReader<int32_t>::Start();

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
bool HWavWriter<int8_t>::Start();

extern template
bool HWavWriter<uint8_t>::Start();

extern template
bool HWavWriter<int16_t>::Start();

extern template
bool HWavWriter<int32_t>::Start();

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
bool HWavReader<int8_t>::Start();

extern template
bool HWavReader<uint8_t>::Start();

extern template
bool HWavReader<int16_t>::Start();

extern template
bool HWavReader<int32_t>::Start();

/********************************************************************
HNetworkWriter
********************************************************************/

// HNetworkWriter
extern template
HNetworkWriter<int8_t>::HNetworkWriter();

extern template
HNetworkWriter<uint8_t>::HNetworkWriter();

extern template
HNetworkWriter<int16_t>::HNetworkWriter();

extern template
HNetworkWriter<int32_t>::HNetworkWriter();

extern template
HNetworkWriter<int8_t>::HNetworkWriter(int socket);

extern template
HNetworkWriter<uint8_t>::HNetworkWriter(int socket);

extern template
HNetworkWriter<int16_t>::HNetworkWriter(int socket);

extern template
HNetworkWriter<int32_t>::HNetworkWriter(int socket);

// SetSocket()
extern template
void HNetworkReader<int8_t>::SetSocket(int socket);

extern template
void HNetworkReader<uint8_t>::SetSocket(int socket);

extern template
void HNetworkReader<int16_t>::SetSocket(int socket);

extern template
void HNetworkReader<int32_t>::SetSocket(int socket);

// Write()
extern template
int HNetworkWriter<int8_t>::Write(int8_t* src, size_t blocksize);

extern template
int HNetworkWriter<uint8_t>::Write(uint8_t* src, size_t blocksize);

extern template
int HNetworkWriter<int16_t>::Write(int16_t* src, size_t blocksize);

extern template
int HNetworkWriter<int32_t>::Write(int32_t* src, size_t blocksize);

/********************************************************************
HNetworkReader
********************************************************************/

// HNetworkReader
extern template
HNetworkReader<int8_t>::HNetworkReader();

extern template
HNetworkReader<uint8_t>::HNetworkReader();

extern template
HNetworkReader<int16_t>::HNetworkReader();

extern template
HNetworkReader<int32_t>::HNetworkReader();

extern template
HNetworkReader<int8_t>::HNetworkReader(int socket);

extern template
HNetworkReader<uint8_t>::HNetworkReader(int socket);

extern template
HNetworkReader<int16_t>::HNetworkReader(int socket);

extern template
HNetworkReader<int32_t>::HNetworkReader(int socket);

// SetSocket()
extern template
void HNetworkReader<int8_t>::SetSocket(int socket);

extern template
void HNetworkReader<uint8_t>::SetSocket(int socket);

extern template
void HNetworkReader<int16_t>::SetSocket(int socket);

extern template
void HNetworkReader<int32_t>::SetSocket(int socket);

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
bool HNetworkReader<int8_t>::Start();

extern template
bool HNetworkReader<uint8_t>::Start();

extern template
bool HNetworkReader<int16_t>::Start();

extern template
bool HNetworkReader<int32_t>::Start();

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
HProcessor<int8_t>::HProcessor(HWriter<int8_t>* writer, HReader<int8_t>* reader, size_t blocksize, bool* terminationToken);

extern template
HProcessor<uint8_t>::HProcessor(HWriter<uint8_t>* writer, HReader<uint8_t>* reader, size_t blocksize, bool* terminationToken);

extern template
HProcessor<int16_t>::HProcessor(HWriter<int16_t>* writer, HReader<int16_t>* reader, size_t blocksize, bool* terminationToken);

extern template
HProcessor<int32_t>::HProcessor(HWriter<int32_t>* writer, HReader<int32_t>* reader, size_t blocksize, bool* terminationToken);

// ~HProcessor
extern template
HProcessor<int8_t>::~HProcessor();

extern template
HProcessor<uint8_t>::~HProcessor();

extern template
HProcessor<int16_t>::~HProcessor();

extern template
HProcessor<int32_t>::~HProcessor();

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
bool HProcessor<int8_t>::Start();

extern template
bool HProcessor<uint8_t>::Start();

extern template
bool HProcessor<int16_t>::Start();

extern template
bool HProcessor<int32_t>::Start();

// Stop()
extern template
bool HProcessor<int8_t>::Stop();

extern template
bool HProcessor<uint8_t>::Stop();

extern template
bool HProcessor<int16_t>::Stop();

extern template
bool HProcessor<int32_t>::Stop();

// Run()
extern template
void HProcessor<int8_t>::Run(long unsigned int blocks);

extern template
void HProcessor<uint8_t>::Run(long unsigned int blocks);

extern template
void HProcessor<int16_t>::Run(long unsigned int blocks);

extern template
void HProcessor<int32_t>::Run(long unsigned int blocks);

// Halt()
extern template
void HProcessor<int8_t>::Halt();

extern template
void HProcessor<uint8_t>::Halt();

extern template
void HProcessor<int16_t>::Halt();

extern template
void HProcessor<int32_t>::Halt();

// GetReader()
extern template
HReader<int8_t>* HProcessor<int8_t>::GetReader();

extern template
HReader<uint8_t>* HProcessor<uint8_t>::GetReader();

extern template
HReader<int16_t>* HProcessor<int16_t>::GetReader();

extern template
HReader<int32_t>* HProcessor<int32_t>::GetReader();

// GetWriter()
extern template
HWriter<int8_t>* HProcessor<int8_t>::GetWriter();

extern template
HWriter<uint8_t>* HProcessor<uint8_t>::GetWriter();

extern template
HWriter<int16_t>* HProcessor<int16_t>::GetWriter();

extern template
HWriter<int32_t>* HProcessor<int32_t>::GetWriter();

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

/********************************************************************
HStreamProcessor
********************************************************************/

// HStreamProcessor()
extern template
HStreamProcessor<int8_t>::HStreamProcessor(HWriter<int8_t>* writer, HReader<int8_t>* reader, int blocksize, bool* terminationToken);

extern template
HStreamProcessor<uint8_t>::HStreamProcessor(HWriter<uint8_t>* writer, HReader<uint8_t>* reader, int blocksize, bool* terminationToken);

extern template
HStreamProcessor<int16_t>::HStreamProcessor(HWriter<int16_t>* writer, HReader<int16_t>* reader, int blocksize, bool* terminationToken);

extern template
HStreamProcessor<int32_t>::HStreamProcessor(HWriter<int32_t>* writer, HReader<int32_t>* reader, int blocksize, bool* terminationToken);

// Run()
extern template
void HStreamProcessor<int8_t>::Run(long unsigned int blocks);

extern template
void HStreamProcessor<uint8_t>::Run(long unsigned int blocks);

extern template
void HStreamProcessor<int16_t>::Run(long unsigned int blocks);

extern template
void HStreamProcessor<int32_t>::Run(long unsigned int blocks);

/********************************************************************
HSoundcardWriter
********************************************************************/

// HSoundcardWriter()
extern template
HSoundcardWriter<int8_t>::HSoundcardWriter(int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer = DEFAULT_FRAMESIZE);

extern template
HSoundcardWriter<uint8_t>::HSoundcardWriter(int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer = DEFAULT_FRAMESIZE);

extern template
HSoundcardWriter<int16_t>::HSoundcardWriter(int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer = DEFAULT_FRAMESIZE);

extern template
HSoundcardWriter<int32_t>::HSoundcardWriter(int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer = DEFAULT_FRAMESIZE);

// ~HSoundcardWriter()
extern template
HSoundcardWriter<int8_t>::~HSoundcardWriter();

extern template
HSoundcardWriter<uint8_t>::~HSoundcardWriter();

extern template
HSoundcardWriter<int16_t>::~HSoundcardWriter();

extern template
HSoundcardWriter<int32_t>::~HSoundcardWriter();

// Read()
extern template
int HSoundcardWriter<int8_t>::Write(int8_t* dest, size_t blocksize);

extern template
int HSoundcardWriter<uint8_t>::Write(uint8_t* dest, size_t blocksize);

extern template
int HSoundcardWriter<int16_t>::Write(int16_t* dest, size_t blocksize);

extern template
int HSoundcardWriter<int32_t>::Write(int32_t* dest, size_t blocksize);

/********************************************************************
HConverter
********************************************************************/

// Write()
extern template
int HConverter<int8_t>::Write(int8_t* src, size_t blocksize);

extern template
int HConverter<uint8_t>::Write(uint8_t* src, size_t blocksize);

extern template
int HConverter<int16_t>::Write(int16_t* src, size_t blocksize);

extern template
int HConverter<int32_t>::Write(int32_t* src, size_t blocksize);

#endif
