#ifndef __HTYPECONVERTER_H
#define __HTYPECONVERTER_H

#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hconverter.h"

/** Convert from one type to another. The input can be scaled if needed so that, say, an
    int8_t value is converter to an int16_t value and scaled to represent the same percentwise
    size. The int8_t value 40 would scale to the int16_t value 10.239 and so on.

    When converting to and from float values, the value is scaled as for an int16_t type.
    When converting to and from double values, the value is scaled as for an int32_t type.

    Remember though that scaling would potentially introduce even greater aliasing problems
    than a simple conversion. So use with respect. */
template <class Tin, class Tout>
class HTypeConverter: public HConverter<Tin, Tout> {

    private:

        bool _scale;

        int Convert(Tin* src, Tout* dest, size_t blocksize);

    public:

        /** Create a new type-converter with a reader */
        HTypeConverter(HReader<Tin>* reader, size_t blocksize, bool scale = false):
            HConverter<Tin, Tout>(reader, blocksize, blocksize),
            _scale(scale) {}

        /** Create a new type-converter with a writer */
        HTypeConverter(HWriter<Tout>* writer, size_t blocksize, bool scale = false):
            HConverter<Tin, Tout>(writer, blocksize, blocksize),
            _scale(scale) {}

        /** Create a new type-converter with a writerconsumer */
        HTypeConverter(HWriterConsumer<Tin>* consumer, size_t blocksize, bool scale = false):
            HConverter<Tin, Tout>(consumer, blocksize, blocksize),
            _scale(scale) {}

        /** Destruct this type-converter instance */
        ~HTypeConverter() {}
};

#endif
