#include <stdio.h>
#include <iostream>

#include "test.h"
#include <thread>

HNetworkServer<int> *server;
HNetworkClient<int> *client;

class HNetwork_Test: public Test
{
    public:

        void run()
        {
            test_reading_server_to_writing_client();
            test_reading_client_to_writing_server();
        }

        const char* name()
        {
            return "HNetwork(Reader|Writer)";
        }

    private:

        class TestReader : public HReader<int>
        {
            private:

                int* _data;

            public:

                TestReader(int* data, int blocksize):
                    _data(data)
                {}

                int Read(int* dest, size_t blocksize)
                {
                    memcpy(dest, _data, blocksize);
                    return blocksize;
                }

        };

        class TestWriter : public HWriter<int>
        {
            private:

                int* _received;

            public:

                TestWriter(int* received, int blocksize):
                    _received(received)
                {}

                int Write(int* src, size_t blocksize)
                {
                    memcpy(_received, src, blocksize);
                    return 0;
                }
        };

        static void runServer()
        {
            server->Run();
        }

        static void runClient()
        {
            client->Run();
        }

        void test_reading_server_to_writing_client()
        {
            bool terminated = false;
            int expected[] = {1, 3, 2, 4, 3, 5, 4, 6, 5, 7, 6, 8, 7, 9};
            int received[14];
            memset(received, 0, 14);

            TestReader rdr(expected, 14);
            server = new HNetworkServer<int>(1234, &rdr, 14, &terminated);
            std::thread serverThread(runServer);

            TestWriter wr(received, 14);
            client = new HNetworkClient<int>("127.0.0.1", 1234, &wr, 14, &terminated);
            std::thread clientThread(runClient);

            clientThread.join();
            delete client;
            delete server;
            serverThread.join();

            ASSERT_IS_EQUAL(memcmp(received, expected, 14), 0);
        }

        void test_reading_client_to_writing_server()
        {
            bool terminated = false;
            int expected[] = {1, 3, 2, 4, 3, 5, 4, 6, 5, 7, 6, 8, 7, 9};
            int received[14];
            memset(received, 0, 14);

            TestWriter wr(received, 14);
            server = new HNetworkServer<int>(1234, &wr, 14, &terminated);
            std::thread serverThread(runServer);

            TestReader rdr(expected, 14);
            client = new HNetworkClient<int>("127.0.0.1", 1234, &rdr, 14, &terminated);
            std::thread clientThread(runClient);

            clientThread.join();
            delete client;
            delete server;
            serverThread.join();

            ASSERT_IS_EQUAL(memcmp(received, expected, 14), 0);
        }
} hnetwork_test;
