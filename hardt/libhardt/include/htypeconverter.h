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

public:

    #define INT8_TO_INT16    258          // 32.767 / 127
    #define UINT8_TO_INT16   129          // 16.383 / 127
    #define INT8_TO_INT32    16909320     // 214.748.3647 / 127
    #define UINT8_TO_INT32   8454660      // 107.374.1824 / 127
    #define INT8_TO_FLOAT    258          // 32.767 / 127
    #define INT8_TO_DOUBLE   16909320     // 214.748.3647 / 127
    #define INT16_TO_INT32   65537        // 214.748.3647 / 32767
    #define INT16_TO_DOUBLE  65537        // 214.748.3647 / 32767
    #define INT32_TO_INT16   0.000015259  // 1/(214.748.3647 / 32767)
    #define INT32_TO_INT8    0.000000059  // 1(214.748.3647 / 127)
    #define INT16_TO_INT8    0.003875969  // 1/(32.767 / 127)

    private:

        double _scale;

        int Convert(Tin* src, Tout* dest, size_t blocksize);

    public:

        /** Create a new type-converter with a reader */
        HTypeConverter(HReader<Tin>* reader, size_t blocksize, double scale = 1):
            HConverter<Tin, Tout>(reader, blocksize, blocksize),
            _scale(scale) {}

        /** Create a new type-converter with a writer */
        HTypeConverter(HWriter<Tout>* writer, size_t blocksize, double scale = 1):
            HConverter<Tin, Tout>(writer, blocksize, blocksize),
            _scale(scale) {}

        /** Create a new type-converter with a writerconsumer */
        HTypeConverter(HWriterConsumer<Tin>* consumer, size_t blocksize, double scale = 1):
            HConverter<Tin, Tout>(consumer, blocksize, blocksize),
            _scale(scale) {}

        /** Destruct this type-converter instance */
        ~HTypeConverter() {}
};

#endif
