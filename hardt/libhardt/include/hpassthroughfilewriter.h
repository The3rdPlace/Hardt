#ifndef __HPASSTHROUGHFILEWRITER_H
#define __HPASSTHROUGHFILEWRITER_H

#include "hpassthrough.h"
#include "hfilewriter.h"

/**
 * Component that, while passing the data through, also writes
 * the data to a file, if enabled
 * @tparam T
 */
template <class T>
class HPassThroughFileWriter : public HPassThrough<T> {

    private:

            std::string _filename;
            HWriter<T>* _writer = NULL;
            HReader<T>* _reader = NULL;
            bool _enabled = false;

    public:

        HPassThroughFileWriter(HWriter<T>* writer, size_t blocksize, const char* filename, bool enabled = false, HProbe<T>* probe = NULL):
                HPassThrough<T>(writer, blocksize, probe),
                _writer(writer),
                _filename(std::string(filename)),
                _enabled(enabled) {
            HLog("HPassThroughFileWriter(writer*, %d, %s, %d)", blocksize, filename, enabled);

        }

        HPassThroughFileWriter(HWriterConsumer<T>* consumer, size_t blocksize, const char* filename, bool enabled = false, HProbe<T>* probe = NULL):
                HPassThrough<T>(consumer, blocksize, probe),
                _filename(std::string(filename)),
                _enabled(enabled) {
                HLog("HPassThroughFileWriter(consumer*, %d, %s, %d)", blocksize, filename, enabled);
            consumer->SetWriter(this->Writer());
        }

        HPassThroughFileWriter(HReader<T>* reader, size_t blocksize, const char* filename, bool enabled = false, HProbe<T>* probe = NULL):
                HPassThrough<T>(reader, blocksize, probe),
                _reader(reader),
                _filename(std::string(filename)),
                _enabled(enabled) {
            HLog("HPassThroughFileWriter(reader*, %d, %s, %d)", blocksize, filename, enabled);

        }

    private:

         void Intercept(T src, size_t blocksize) {
            printf("intercept %d\n", blocksize);
        }
};

#endif