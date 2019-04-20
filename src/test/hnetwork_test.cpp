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
            //test_reading_client_to_writing_server();
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
                int _blocksize;

            public:

                TestReader(int* data, int blocksize):
                    _data(data),
                    _blocksize(blocksize)
                {}

                int Read(int* dest)
                {
                    memcpy(dest, _data, _blocksize);
                    return _blocksize;
                }

                int Blocksize()
                {
                    return _blocksize;
                }
        };

        class TestWriter : public HWriter<int>
        {
            private:

                int* _received;
                int _blocksize;

            public:

                TestWriter(int* received, int blocksize):
                    _received(received),
                     _blocksize(blocksize)
                {}

                int Write(int* src)
                {
                    memcpy(_received, src, _blocksize);
                    return 0;
                }

                int Blocksize()
                {
                    return _blocksize;
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
            server = new HNetworkServer<int>(1234, &rdr, &terminated);
            std::thread serverThread(runServer);

            TestWriter wr(received, 14);
            client = new HNetworkClient<int>("127.0.0.1", 1234, &wr, &terminated);
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
            server = new HNetworkServer<int>(1234, &wr, &terminated);
            std::thread serverThread(runServer);

            TestReader rdr(expected, 14);
            client = new HNetworkClient<int>("127.0.0.1", 1234, &rdr, &terminated);
            std::thread clientThread(runClient);

            clientThread.join();
            delete client;
            delete server;
            serverThread.join();

            ASSERT_IS_EQUAL(memcmp(received, expected, 14), 0);
        }
} hnetwork_test;
