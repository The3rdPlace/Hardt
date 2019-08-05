#include <stdio.h>
#include <iostream>

#include "test.h"

class HSignalLevel_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_min_max_avg);
            UNITTEST(test_signallevel_int8);
            UNITTEST(test_signallevel_uint8);
            UNITTEST(test_signallevel_int16);
            UNITTEST(test_signallevel_int32);
        }

        const char* name()
        {
            return "HSignalLevel";
        }

    private:

        int Db;
        //int AvgDb;
        //int AvgS;
        int S;
        //int Min;
        //int Max;
        //int Avg;
        int callback(HSignalLevelResult* result, size_t length)
        {
            Db = result->Db;
            //AvgDb = result->AvgDb;
            S = result->S;
            //AvgS = result->AvgS;

            //Min = result->Min;
            //Max = result->Max;
            //Avg = result->Avg;

            return length;
        }

        void test_min_max_avg()
        {
            int8_t input[10] = { 2, 4, 6, 4, 2, -1, -3, -5, -3, -1 };

            auto wr = HCustomWriter<HSignalLevelResult>::Create<HSignalLevel_Test>(this, &HSignalLevel_Test::callback);
            HSignalLevel<int8_t> siglevel(wr, 0);

            siglevel.Write(input, 10);
            //ASSERT_IS_EQUAL(Min, 1);
            //ASSERT_IS_EQUAL(Max, 6);
            //ASSERT_IS_EQUAL(Avg, ?);
            ASSERT_IS_EQUAL(Db, -27);
            //ASSERT_IS_EQUAL(AvgDb, -27);

            siglevel.Write(input, 10);
            siglevel.Write(input, 10);
            siglevel.Write(input, 10);
            siglevel.Write(input, 10);
            siglevel.Write(input, 10);
            siglevel.Write(input, 10);
            siglevel.Write(input, 10);
            siglevel.Write(input, 10);
            siglevel.Write(input, 10);
            ASSERT_IS_EQUAL(Db, -27);
        }

        void test_signallevel_int8()
        {
            // Signal
            HSineGenerator<int8_t> g(48000, 1000, 0);
            int8_t input[1024];
            g.Read(input, 1024);

            auto wr = HCustomWriter<HSignalLevelResult>::Create<HSignalLevel_Test>(this, &HSignalLevel_Test::callback);
            HSignalLevel<int8_t> siglevel(wr, 0);

            // The low dynamic range for 8 bit samples reduces the possible signal level values
            g.Calculate(1000, 0, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -54);
            ASSERT_IS_EQUAL(S, 0);

            g.Calculate(1000, 1, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -42);
            ASSERT_IS_EQUAL(S, 2);

            g.Calculate(1000, 2, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -36);
            ASSERT_IS_EQUAL(S, 3);

            g.Calculate(1000, 4, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -30);
            ASSERT_IS_EQUAL(S, 4);

            g.Calculate(1000, 8, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -24);
            ASSERT_IS_EQUAL(S, 5);

            g.Calculate(1000, 16, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -18);
            ASSERT_IS_EQUAL(S, 6);

            g.Calculate(1000, 32, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -12);
            ASSERT_IS_EQUAL(S, 7);

            g.Calculate(1000, 64, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -6);
            ASSERT_IS_EQUAL(S, 8);

            g.Calculate(1000, 127, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, 0);
            ASSERT_IS_EQUAL(S, 9);

            delete wr;
        }

        void test_signallevel_uint8()
        {
            // Signal
            HSineGenerator<uint8_t> g(48000, 1000, 0);
            uint8_t input[1024];
            g.Read(input, 1024);

            auto wr = HCustomWriter<HSignalLevelResult>::Create<HSignalLevel_Test>(this, &HSignalLevel_Test::callback);
            HSignalLevel<uint8_t> siglevel(wr, 0);

            // The low dynamic range for 8 bit samples reduces the possible signal level values
            g.Calculate(1000, 0, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -54);
            ASSERT_IS_EQUAL(S, 0);

            g.Calculate(1000, 1, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -42);
            ASSERT_IS_EQUAL(S, 2);

            g.Calculate(1000, 2, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -36);
            ASSERT_IS_EQUAL(S, 3);

            g.Calculate(1000, 4, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -30);
            ASSERT_IS_EQUAL(S, 4);

            g.Calculate(1000, 8, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -24);
            ASSERT_IS_EQUAL(S, 5);

            g.Calculate(1000, 16, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -18);
            ASSERT_IS_EQUAL(S, 6);

            g.Calculate(1000, 32, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -12);
            ASSERT_IS_EQUAL(S, 7);

            g.Calculate(1000, 64, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -6);
            ASSERT_IS_EQUAL(S, 8);

            g.Calculate(1000, 127, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, 0);
            ASSERT_IS_EQUAL(S, 9);

            delete wr;
        }

        void test_signallevel_int16()
        {
            // Signal
            HSineGenerator<int16_t> g(48000, 1000, 0);
            int16_t input[1024];
            g.Read(input, 1024);

            auto wr = HCustomWriter<HSignalLevelResult>::Create<HSignalLevel_Test>(this, &HSignalLevel_Test::callback);
            HSignalLevel<int16_t> siglevel(wr, 0);

            g.Calculate(1000, 0, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -54);
            ASSERT_IS_EQUAL(S, 0);

            g.Calculate(1000, 128, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -48);
            ASSERT_IS_EQUAL(S, 1);

            g.Calculate(1000, 256, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -42);
            ASSERT_IS_EQUAL(S, 2);

            g.Calculate(1000, 512, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -36);
            ASSERT_IS_EQUAL(S, 3);

            g.Calculate(1000, 1024, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -30);
            ASSERT_IS_EQUAL(S, 4);

            g.Calculate(1000, 2048, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -24);
            ASSERT_IS_EQUAL(S, 5);

            g.Calculate(1000, 4096, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -18);
            ASSERT_IS_EQUAL(S, 6);

            g.Calculate(1000, 8192, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -12);
            ASSERT_IS_EQUAL(S, 7);

            g.Calculate(1000, 16384, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -6);
            ASSERT_IS_EQUAL(S, 8);

            g.Calculate(1000, 32767, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, 0);
            ASSERT_IS_EQUAL(S, 9);

            delete wr;
        }

        void test_signallevel_int32()
        {
            // Signal
            HSineGenerator<int32_t> g(48000, 1000, 0);
            int32_t input[1024];
            g.Read(input, 1024);

            auto wr = HCustomWriter<HSignalLevelResult>::Create<HSignalLevel_Test>(this, &HSignalLevel_Test::callback);
            HSignalLevel<int32_t> siglevel(wr, 0);

            // The high dynamic range for 32 bit samples will allow many lower db values, but all will register with S = 0
            g.Calculate(1000, 0, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -54);
            ASSERT_IS_EQUAL(S, 0);

            g.Calculate(1000, 8388608, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -48);
            ASSERT_IS_EQUAL(S, 1);

            g.Calculate(1000, 16777216, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -42);
            ASSERT_IS_EQUAL(S, 2);

            g.Calculate(1000, 33554432, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -36);
            ASSERT_IS_EQUAL(S, 3);

            g.Calculate(1000, 67108864, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -30);
            ASSERT_IS_EQUAL(S, 4);

            g.Calculate(1000, 134217728, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -24);
            ASSERT_IS_EQUAL(S, 5);

            g.Calculate(1000, 268435456, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -18);
            ASSERT_IS_EQUAL(S, 6);

            g.Calculate(1000, 536870912, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -12);
            ASSERT_IS_EQUAL(S, 7);

            g.Calculate(1000, 1073741824, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, -6);
            ASSERT_IS_EQUAL(S, 8);

            g.Calculate(1000, 2147483647, 0);
            g.Read(input, 1024);
            siglevel.Write(input, 1024);
            ASSERT_IS_EQUAL(Db, 0);
            ASSERT_IS_EQUAL(S, 9);

            delete wr;
        }

} hSignallevel_test;
