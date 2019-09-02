#include <stdio.h>
#include <iostream>

#include "test.h"

class HSwitch_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_read);
            UNITTEST(test_write);
        }

        const char* name()
        {
            return "HSwitch";
        }

    private:


        template <class T>
        class TestReader : public HReader<T>
        {
            public:

                int8_t *_data;

                TestReader(int8_t* data):
                    _data(data)
                {}

                int Read(T* dest, size_t blocksize)
                {
                    memcpy((void*) dest, _data, blocksize * sizeof(T));
                    return blocksize;
                }
        };

        template <class T>
        class TestWriter : public HWriter<T>
        {
            public:

                int8_t *_received;

                TestWriter(int8_t* received):
                    _received(received)
                {}

                int Write(T* src, size_t blocksize)
                {
                    memcpy((void*) _received, src, blocksize * sizeof(T));
                    return blocksize;
                }
        };

        void test_read()
        {
        }

        void test_write()
        {
        }

} hswitch_test;