#include <stdio.h>
#include <iostream>

#include "test.h"
#include <thread>

HNetworkProcessor<int> *server;
HNetworkProcessor<int> *client;

class HNetwork_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(test_reading_server_to_writing_client);
            UNITTEST(test_reading_client_to_writing_server);
        }

        const char* name()
        {
            return "HNetworkProcessor(Reader|Writer)";
        }

    private:

        class TestReader : public HReader<int>
        {
            private:

                int* _data;
                bool _first;

            public:

                TestReader(int* data, int blocksize):
                    _data(data),
                    _first(true)
                {}

                int Read(int* dest, size_t blocksize)
                {
                    if (!_first)
                    {
                        return 0;
                    }
                    _first = false;
                    memcpy(dest, _data, blocksize * sizeof(int));
                    return blocksize;
                }
        };

        class TestWriter : public HWriter<int>
        {
            private:

                int* _received;
                bool _first;

            public:

                TestWriter(int* received, int blocksize):
                    _received(received),
                    _first(true)
                {}

                int Write(int* src, size_t blocksize)
                {
                    if (!_first)
                    {
                        return 0;
                    }
                    _first = false;
                    memcpy(_received, src, blocksize * sizeof(int));
                    return blocksize;
                }
        };

        static void runServer()
        {
            server->Run();
        }

        static void runClient()
        {
            client->Run();
            sleep(1);
        }

        void test_reading_server_to_writing_client()
        {
            bool terminated = false;
            int expected[] = {1, 3, 2, 4, 3, 5, 4, 6, 5, 7, 6, 8, 7, 9};
            int received[14];
            memset(received, 0, 14 * sizeof(int));

            TestReader rdr(expected, 14);
            server = new HNetworkProcessor<int>(1234, &rdr, 14, &terminated);
            std::thread serverThread(runServer);
            sleep(5);
            TestWriter wr(received, 14);
            client = new HNetworkProcessor<int>("127.0.0.1", 1234, &wr, 14, &terminated);
            std::thread clientThread(runClient);

            clientThread.join();
            server->Halt();
            serverThread.join();
            delete client;
            delete server;

            ASSERT_IS_EQUAL(memcmp(received, expected, 14 * sizeof(int)), 0);
        }

        void test_reading_client_to_writing_server()
        {
            bool terminated = false;
            int expected[] = {1, 3, 2, 4, 3, 5, 4, 6, 5, 7, 6, 8, 7, 9};
            int received[14];
            memset(received, 0, 14 * sizeof(int));

            TestWriter wr(received, 14);
            server = new HNetworkProcessor<int>(1235, &wr, 14, &terminated);
            std::thread serverThread(runServer);
            sleep(5);
            TestReader rdr(expected, 14);
            client = new HNetworkProcessor<int>("127.0.0.1", 1235, &rdr, 14, &terminated);
            std::thread clientThread(runClient);

            clientThread.join();
            server->Halt();
            serverThread.join();
            delete client;
            delete server;

            ASSERT_IS_EQUAL(memcmp(received, expected, 14 * sizeof(int)), 0);
        }
} hnetwork_test;
