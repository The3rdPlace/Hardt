#include <iostream>
#include <thread>

#include "test.h"

HNetworkProcessor<int16_t> *server;
HNetworkProcessor<int16_t> *client;

class HNetwork_Test: public Test
{
    public:

        void run()
        {
            INTEGRATIONTEST(test_reading_server_to_writing_client);
            INTEGRATIONTEST(test_reading_client_to_writing_server);
            INTEGRATIONTEST(test_reading_server_to_writing_client_sending_commands);
            INTEGRATIONTEST(test_reading_client_to_writing_server_sending_commands);
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
            server = new HNetworkProcessor<int16_t>("hnetworkprocessor_server", 1231, 1232, &rdr, 14, &terminated);
            std::thread serverThread(runServer);
            sleep(1);
            TestWriter<int16_t > wr(14);
            client = new HNetworkProcessor<int16_t>("hnetworkprocessor_client", "127.0.0.1", 1231, 1232,  &wr, 14, &terminated);
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
            server = new HNetworkProcessor<int16_t>("hnetworkprocessor_server", 1233, 1234, &wr, 14, &terminated);
            std::thread serverThread(runServer);
            sleep(1);
            TestReader<int16_t> rdr(expected, 14, false);
            client = new HNetworkProcessor<int16_t>("hnetworkprocessor_client", "127.0.0.1", 1233, 1234, &rdr, 14, &terminated);
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
            server = new HNetworkProcessor<int16_t>("hnetworkprocessor_server", 1235, 1236, &rdr, 14, &terminated);
            std::thread serverThread(runServer);
            sleep(1);
            TestWriter<int16_t > wr(14);
            client = new HNetworkProcessor<int16_t>("hnetworkprocessor_client", "127.0.0.1", 1235, 1236, &wr, 14, &terminated);
            std::thread clientThread(runClient);

            sleep(5);
            HCommandData cmdData;
            cmdData.State = false;
            client->Command(H_COMMAND_CLASS::NONE, H_COMMAND_OPCODE::NOP, 0, cmdData);
            ASSERT_IS_EQUAL(rdr.Commands, 1);
            ASSERT_IS_EQUAL(rdr.LastCommand.Data.State, false);
            sleep(5);
            cmdData.State = true;
            client->Command(H_COMMAND_CLASS::NONE, H_COMMAND_OPCODE::NOP, 0, cmdData);
            ASSERT_IS_EQUAL(rdr.Commands, 2);
            ASSERT_IS_EQUAL(rdr.LastCommand.Data.State, true);
            sleep(5);
            char content[] = "thisismycontent";
            cmdData.Content = (void*) content;
            client->Command(H_COMMAND_CLASS::NONE, H_COMMAND_OPCODE::NOP, 15, cmdData);
            ASSERT_IS_EQUAL(rdr.Commands, 3);
            ASSERT_IS_EQUAL(memcmp(rdr.LastContent, (void*) "thisismycontent", 15), 0);
            sleep(5);

            clientThread.join();
            server->Halt();
            serverThread.join();

            delete client;
            delete server;

            ASSERT_IS_EQUAL(memcmp(wr.Received, expected, 14 * sizeof(int16_t)), 0);
            ASSERT_IS_EQUAL(wr.Commands, 3);
        }

        void test_reading_client_to_writing_server_sending_commands()
        {
            bool terminated = false;
            int16_t expected[] = {1, 3, 2, 4, 3, 5, 4, 6, 5, 7, 6, 8, 7, 9};

            TestWriter<int16_t> wr(14);
            server = new HNetworkProcessor<int16_t>("hnetworkprocessor_server", 1237, 1238, &wr, 14, &terminated);
            std::thread serverThread(runServer);
            sleep(1);
            TestReader<int16_t> rdr(expected, 14, false);
            client = new HNetworkProcessor<int16_t>("hnetworkprocessor_client", "127.0.0.1", 1237, 1238, &rdr, 14, &terminated);
            std::thread clientThread(runClient);

            sleep(5);
            HCommandData cmdData;
            cmdData.State = false;
            client->Command(H_COMMAND_CLASS::NONE, H_COMMAND_OPCODE::NOP, 0, cmdData);
            ASSERT_IS_EQUAL(wr.Commands, 1);
            ASSERT_IS_EQUAL(wr.LastCommand.Data.State, false);
            sleep(5);
            cmdData.State = true;
            client->Command(H_COMMAND_CLASS::NONE, H_COMMAND_OPCODE::NOP, 0, cmdData);
            ASSERT_IS_EQUAL(wr.Commands, 2);
            ASSERT_IS_EQUAL(wr.LastCommand.Data.State, true);
            sleep(5);
            char content[] = "thisismycontent";
            cmdData.Content = (void*) content;
            client->Command(H_COMMAND_CLASS::NONE, H_COMMAND_OPCODE::NOP, 15, cmdData);
            ASSERT_IS_EQUAL(wr.Commands, 3);
            ASSERT_IS_EQUAL(memcmp(wr.LastContent, (void*) "thisismycontent", 15), 0);
            sleep(5);

            clientThread.join();
            server->Halt();
            serverThread.join();

            delete client;
            delete server;

            ASSERT_IS_EQUAL(memcmp(wr.Received, expected, 14 * sizeof(int16_t)), 0);
            ASSERT_IS_EQUAL(wr.Commands, 3);
            ASSERT_IS_EQUAL(rdr.Commands, 3);
        }

} hnetwork_test;