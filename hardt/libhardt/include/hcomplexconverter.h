#ifndef __HCOMPLEXCONVERTER_H
#define __HCOMPLEXCONVERTER_H

#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hconverter.h"

/** Convert to and from complex values. Complex values is always of type std::complex<double>
    so int8_t/int16_t/int32_t/float/double is always converted straight into a correspondig
    double value form re/im. Use the HTypeConverter first if you want to change datatype and
    scale values */
template <class Tin, class Tout>
class HComplexConverter: public HConverter<Tin, Tout> {

private:

    int Convert(Tin* src, Tout* dest, size_t blocksize);

public:

    /** Create a new complex-converter with a reader */
    HComplexConverter(std::string id, HReader<Tin>* reader, size_t blocksize):
            HConverter<Tin, Tout>(id, reader, blocksize, blocksize) {}

    /** Create a new complex-converter with a writer */
    HComplexConverter(std::string id, HWriter<Tout>* writer, size_t blocksize):
            HConverter<Tin, Tout>(id, writer, blocksize, blocksize) {}

    /** Create a new complex-converter with a writerconsumer */
    HComplexConverter(std::string id, HWriterConsumer<Tin>* consumer, size_t blocksize):
            HConverter<Tin, Tout>(id, consumer, blocksize, blocksize) {}

    /** Destruct this type-converter instance */
    ~HComplexConverter() = default;
};

#endif
