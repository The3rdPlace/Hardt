#include <stdio.h>
#include <iostream>

#include "test.h"

class HNullReader_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_int8_reader);
            UNITTEST(test_uint8_reader);
            UNITTEST(test_int16_reader);
            UNITTEST(test_int32_reader);
        }

        const char* name()
        {
            return "HNullReader";
        }

    public:

        void test_int8_reader()
        {
            int8_t buffer[10];
            HNullReader<int8_t> reader;

            ASSERT_IS_EQUAL(reader.Read(buffer, 10), 10);
            for( int i = 0; i < 10; i++ )
            {
                ASSERT_IS_EQUAL(buffer[i], std::numeric_limits<int8_t>::lowest());
            }
        }

        void test_uint8_reader()
        {
            uint8_t buffer[10];
            HNullReader<uint8_t> reader;

            ASSERT_IS_EQUAL(reader.Read(buffer, 10), 10);
            for( int i = 0; i < 10; i++ )
            {
                ASSERT_IS_EQUAL(buffer[i], std::numeric_limits<uint8_t>::lowest());
            }
        }

        void test_int16_reader()
        {
            int16_t buffer[10];
            HNullReader<int16_t> reader;

            ASSERT_IS_EQUAL(reader.Read(buffer, 10), 10);
            for( int i = 0; i < 10; i++ )
            {
                ASSERT_IS_EQUAL(buffer[i], std::numeric_limits<int16_t>::lowest());
            }
        }

        void test_int32_reader()
        {
            int32_t buffer[10];
            HNullReader<int32_t> reader;

            ASSERT_IS_EQUAL(reader.Read(buffer, 10), 10);
            for( int i = 0; i < 10; i++ )
            {
                ASSERT_IS_EQUAL(buffer[i], std::numeric_limits<int32_t>::lowest());
            }
        }

} hnullreader_test;
