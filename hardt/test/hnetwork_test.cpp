#include <stdio.h>
#include <iostream>

#include "test.h"
#include <thread>

HNetworkProcessor<int16_t> *server;
HNetworkProcessor<int16_t> *client;

class HNetwork_Test: public Test
{
    public:

        void run()
        {
            //INTEGRATIONTEST(test_reading_server_to_writing_client);
            //INTEGRATIONTEST(test_reading_client_to_writing_server);
            INTEGRATIONTEST(test_reading_server_to_writing_client_sending_commands);
        }

        const char* name()
        {
            return "HNetworkProcessor(Reader|Writer)";
        }

    private:

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
            int16_t expected[] = {1, 3, 2, 4, 3, 5, 4, 6, 5, 7, 6, 8, 7, 9};

            TestReader<int16_t> rdr(expected, 14, false);
            server = new HNetworkProcessor<int16_t>(1234, &rdr, 14, &terminated);
            std::thread serverThread(runServer);
            sleep(1);
            TestWriter<int16_t > wr(14);
            client = new HNetworkProcessor<int16_t>("127.0.0.1", 1234, &wr, 14, &terminated);
            std::thread clientThread(runClient);

            clientThread.join();
            server->Halt();
            serverThread.join();
            delete client;
            delete server;

            ASSERT_IS_EQUAL(memcmp(wr.Received, expected, 14 * sizeof(int16_t)), 0);
        }

        void test_reading_client_to_writing_server()
        {
            bool terminated = false;
            int16_t expected[] = {1, 3, 2, 4, 3, 5, 4, 6, 5, 7, 6, 8, 7, 9};

            TestWriter<int16_t> wr(14);
            server = new HNetworkProcessor<int16_t>(1235, &wr, 14, &terminated);
            std::thread serverThread(runServer);
            sleep(1);
            TestReader<int16_t> rdr(expected, 14, false);
            client = new HNetworkProcessor<int16_t>("127.0.0.1", 1235, &rdr, 14, &terminated);
            std::thread clientThread(runClient);

            clientThread.join();
            server->Halt();
            serverThread.join();
            delete client;
            delete server;

            ASSERT_IS_EQUAL(memcmp(wr.Received, expected, 14 * sizeof(int16_t)), 0);
        }

    void test_reading_server_to_writing_client_sending_commands()
    {
        bool terminated = false;
        int16_t expected[] = {1, 3, 2, 4, 3, 5, 4, 6, 5, 7, 6, 8, 7, 9};

        TestReader<int16_t> rdr(expected, 14, false);
        server = new HNetworkProcessor<int16_t>(1234, &rdr, 14, &terminated);
        std::thread serverThread(runServer);
        sleep(1);
        TestWriter<int16_t > wr(14);
        client = new HNetworkProcessor<int16_t>("127.0.0.1", 1234, &wr, 14, &terminated);
        std::thread clientThread(runClient);

        clientThread.join();
        server->Halt();
        serverThread.join();

        HCommandData cmdData;
        cmdData.State = false;
        client->Command(HCOMMAND_CLASS::NONE, HCOMMAND_OPCODE::NOP, 0, cmdData);

        delete client;
        delete server;

        ASSERT_IS_EQUAL(memcmp(wr.Received, expected, 14 * sizeof(int16_t)), 0);
        ASSERT_IS_EQUAL(wr.Commands, 1);
    }

} hnetwork_test;
