#include "hardtapi.h"
#include <vector>

class Test;
extern std::vector<Test*> tests;

extern bool run_unit_tests;
extern bool run_integration_tests;

class Test
{
    private:

        static int failed;

    public:

        static char* sourceDir;

    public:

        Test() { tests.push_back(this); }

        static int execute()
        {
            int partialFailed = 0;
            for( std::vector<Test*>::iterator it = tests.begin(); it < tests.end(); it++ )
            {
                std::cout << "====================================================================" << std::endl;
                try
                {
                    std::cout << "Test: " << (*it)->name() << std::endl;
                    failed = 0;
                    (*it)->run();
                    partialFailed += failed;
                    std::cout << (failed == 0 ? "OK" : "FAILED") << std::endl;
                }
                catch(std::exception* e)
                {
                    std::cout << "EXCEPTION: " << e->what() << std::endl;
                    partialFailed++;
                }
            }
            std::cout << "====================================================================" << std::endl;
            return partialFailed;
        }

    public:

        template <typename T>
        bool assertIsEqual(std::string file, int line, T a, T b)
        {
            if( a != b )
            {
                std::cout << "- assertIsEqual(" << (T) a << ", " << (T) b << "): is not equal!" << std::endl;
                std::cout << "  in " << file << "@" << line << std::endl;
                Test::failed++;
                return false;
            }
            return true;
        }

        template <typename T>
        bool assertIsNotEqual(std::string file, int line, T a, T b)
        {
            if( a == b )
            {
                std::cout << "- assertIsNotEqual(" << (T) a << ", " << (T) b << "): is equal!" << std::endl;
                std::cout << "  in " << file << "@" << line << std::endl;
                Test::failed++;
                return false;
            }
            return true;
        }

        template <typename T>
        bool assertIsLessThanOrEqual(std::string file, int line, T a, T b)
        {
            if( a > b )
            {
                std::cout << "- assertIsLessThanOrEqual(" << a << ", " << b << "): is not less than or equal!" << std::endl;
                std::cout << "  in " << file << "@" << line << std::endl;
                Test::failed++;
                return false;
            }
            return true;
        }

        template <typename T>
        bool assertIsLessThan(std::string file, int line, T a, T b)
        {
            if( a >= b )
            {
                std::cout << "- assertIsLessThan(" << a << ", " << b << "): is not less than!" << std::endl;
                std::cout << "  in " << file << "@" << line << std::endl;
                Test::failed++;
                return false;
            }
            return true;
        }

        template <typename T>
        bool assertIsGreaterThanOrEqual(std::string file, int line, T a, T b)
        {
            if( a < b )
            {
                std::cout << "- assertIsGreaterThanOrEqual(" << a << ", " << b << "): is not greater than or equal!" << std::endl;
                std::cout << "  in " << file << "@" << line << std::endl;
                Test::failed++;
                return false;
            }
            return true;
        }

        template <typename T>
        bool assertIsGreaterThan(std::string file, int line, T a, T b)
        {
            if( a <= b )
            {
                std::cout << "- assertIsGreaterThan(" << a << ", " << b << "): is not greater than!" << std::endl;
                std::cout << "  in " << file << "@" << line << std::endl;
                Test::failed++;
                return false;
            }
            return true;
        }

        bool assertIsTrue(std::string file, int line, bool a)
        {
            if( a != true )
            {
                std::cout << "- assertIsTrue(" << a << "): is not true!" << std::endl;
                std::cout << "  in " << file << "@" << line << std::endl;
                Test::failed++;
                return false;
            }
            return true;
        }

        bool assertFail(std::string file, int line, std::string reason)
        {
            std::cout << "- assertFail: " << reason << std::endl;
            std::cout << "  in " << file << "@" << line << "\n";
            Test::failed++;
            return false;
        }

        bool assertIgnore(std::string file, int line, std::string reason)
        {
            std::cout << "Ignoring testcase: " << reason << std::endl;
            std::cout << "in " << file << "@" << line << "\n";
            return true;
        }

        template <typename T>
        bool assertIsBetween(std::string file, int line, T a, T b, T c)
        {
            if( a < b || a > c)
            {
                std::cout << "- assertIsBetween(" << a << ", " << b << ", " << c << "): " << a << " is not between " << b << " and " << c << "!" << std::endl;
                std::cout << "  in " << file << "@" << line << std::endl;
                Test::failed++;
                return false;
            }
            return true;
        }

    public:

        template <class T>
        class TestReader : public HReader<T>
        {
            private:

                T* _data;
                bool _first;
                bool _multipleReads;
                bool _continuedReads;
                int _continuedPosition;

            public:

                int Reads;
                int Samples;
                int Started;
                int Stopped;
                int Commands;
                HCommand LastCommand;
                void* LastContent;

                TestReader(std::string id, T* data, int blocksize, bool multipleReads = true, bool continuedReads = false):
                    HReader<T>(id),
                    Reads(0),
                    Samples(0),
                    _data(data),
                    _first(true),
                    _multipleReads(multipleReads),
                    _continuedReads(continuedReads),
                    _continuedPosition(0),
                    Started(0),
                    Stopped(0),
                    Commands(0),
                    LastContent(nullptr)
                {}

                ~TestReader()
                {
                    if( LastContent != nullptr )
                    {
                        free(LastContent);
                    }
                }

                int Read(T* dest, size_t blocksize)
                {
                    if (!_multipleReads && !_first)
                    {
                        return 0;
                    }

                    _first = false;
                    if( !_continuedReads ) {
                        memcpy(dest, _data, blocksize * sizeof(T));
                    } else {
                        memcpy(dest, &_data[_continuedPosition], blocksize * sizeof(T));
                        _continuedPosition += blocksize;
                    }
                    Reads++;
                    Samples += blocksize;
                    return blocksize;
                }

                bool Start()
                {
                    Started++;
                    return true;
                }

                bool Stop()
                {
                    Stopped++;
                    return true;
                }

                /** Execute or carry through a command */
                bool Command(HCommand* command) {

                    if( LastContent != nullptr )
                    {
                        free(LastContent);
                        LastContent = nullptr;
                    }

                    Commands++;
                    LastCommand.Length = command->Length;
                    LastCommand.Opcode = command->Opcode;
                    LastCommand.Class = command->Class;
                    LastCommand.Data = command->Data;

                    if( command->Length > 0 ) {
                        LastContent = malloc(command->Length);
                        memcpy(LastContent, (void*) command->Data.Content, command->Length);
                    }

                    return true;
                }
        };

        template <class T>
        class TestWriter : public HWriter<T>, public HWriterConsumer<T>
        {
            private:
                HWriter<T>* _writer;

            public:

                bool _first;
                bool _multipleWrites;
                int _writeDelay;
                bool _continuedWrites;
                int _continuedPosition;

                T* Received;
                int Writes;
                int Samples;
                int Started;
                int Stopped;
                int Commands;
                HCommand LastCommand;
                void* LastContent;

                TestWriter(std::string id, size_t blocksize, bool multipleWrites = true, int writeDelay = 0, bool continuedWrites = false):
                    HWriter<T>(id),
                    HWriterConsumer<T>(id),
                    Writes(0),
                    Samples(0),
                    _writer(nullptr),
                    Started(0),
                    Stopped(0),
                    _first(true),
                    _multipleWrites(multipleWrites),
                    _writeDelay(writeDelay),
                    Commands(0),
                    LastContent(nullptr),
                    _continuedPosition(0),
                    _continuedWrites(continuedWrites) {
                    Received = new T[blocksize];
                    memset((void*) Received, 0, blocksize * sizeof(T));
                }

                TestWriter(std::string id, HWriter<T>* writer, size_t blocksize, bool multipleWrites = true, int writeDelay = 0, bool continuedWrites = false):
                    HWriter<T>(id),
                    HWriterConsumer<T>(id),
                    Writes(0),
                    Samples(0),
                    _writer(writer),
                    Started(0),
                    Stopped(0),
                    _first(true),
                    _multipleWrites(multipleWrites),
                    _writeDelay(writeDelay),
                    Commands(0),
                    LastContent(nullptr),
                    _continuedPosition(0),
                    _continuedWrites(continuedWrites) {
                    Received = new T[blocksize];
                    memset((void*) Received, 0, blocksize * sizeof(T));
                }

                TestWriter(std::string id, HWriterConsumer<T>* consumer, size_t blocksize, bool multipleWrites = true, int writeDelay = 0, bool continuedWrites = false):
                    HWriter<T>(id),
                    HWriterConsumer<T>(id),
                    Writes(0),
                    Samples(0),
                    _writer(nullptr),
                    Started(0),
                    Stopped(0),
                    _first(true),
                    _multipleWrites(multipleWrites),
                    _writeDelay(writeDelay),
                    Commands(0),
                    LastContent(nullptr),
                    _continuedPosition(0),
                    _continuedWrites(continuedWrites)
                {
                    Received = new T[blocksize];
                    memset((void*) Received, 0, blocksize * sizeof(T));
                    consumer->SetWriter(this->Writer());
                }

                ~TestWriter()
                {
                    delete[] Received;
                    if( LastContent != nullptr )
                    {
                        free(LastContent);
                    }
                }

                int Write(T* src, size_t blocksize)
                {
                    if (!_multipleWrites && !_first)
                    {
                        return 0;
                    }

                    _first = false;
                    memcpy((void*) &Received[_continuedPosition], (void*) src, blocksize * sizeof(T));
                    Writes++;
                    Samples += blocksize;
                    if( _writer != nullptr ) {
                        _writer->Write(src, blocksize);
                    }

                    if( _continuedWrites ) {
                        _continuedPosition += blocksize;
                    }
                    
                    usleep(_writeDelay);

                    return blocksize;
                }

                void SetWriter(HWriter<T>* writer)
                {
                    _writer = writer;
                }

                bool Start()
                {
                    Started++;
                    return true;
                }

                bool Stop()
                {
                    Stopped++;
                    return true;
                }

                /** Execute or carry through a command */
                bool Command(HCommand* command) {

                    if( LastContent != nullptr )
                    {
                        free(LastContent);
                        LastContent = nullptr;
                    }

                    if( _writer != nullptr )
                    {
                        if( !_writer->Command(command) )
                        {
                            return false;
                        }
                    }

                    Commands++;
                    LastCommand.Length = command->Length;
                    LastCommand.Opcode = command->Opcode;
                    LastCommand.Class = command->Class;
                    LastCommand.Data = command->Data;

                    if( command->Length > 0 ) {
                        LastContent = malloc(command->Length);
                        memcpy(LastContent, (void*) command->Data.Content, command->Length);
                    }

                    return true;
                }
        };

        static HCommand TestNopCommand;

    public:

        virtual void run() = 0;
        virtual const char* name() = 0;
};

#define TEST(a) std::cout << "* Running test: " << #a << std::endl; a();
#define UNITTEST(a) if( run_unit_tests ) { std::cout << "* Running unittest: " << #a << std::endl; a(); } else { std::cout << "- Skipping unittest: " << #a << std::endl; }
#define INTEGRATIONTEST(a) if( run_integration_tests ) { std::cout << "* Running integrationtest: " << #a << std::endl; a(); } else { std::cout << "- Skipping integrationtest: " << #a << std::endl; }

#define ASSERT_IS_EQUAL(a, b) assertIsEqual(__FILE__,__LINE__, a, b)
#define ASSERT_IS_NOT_EQUAL(a, b) assertIsNotEqual(__FILE__,__LINE__, a, b)
#define ASSERT_IS_LESS_THAN_OR_EQUAL(a, b) assertIsLessThanOrEqual(__FILE__,__LINE__, a, b)
#define ASSERT_IS_LESS_THAN(a, b) assertIsLessThan(__FILE__,__LINE__, a, b)
#define ASSERT_IS_GREATER_THAN_OR_EQUAL(a, b) assertIsGreaterThanOrEqual(__FILE__,__LINE__, a, b)
#define ASSERT_IS_GREATER_THAN(a, b) assertIsGreaterThan(__FILE__,__LINE__, a, b)
#define ASSERT_IS_TRUE(a) assertIsTrue(__FILE__,__LINE__, (bool) a)
#define ASSERT_FAIL(reason) assertFail(__FILE__,__LINE__, reason)
#define ASSERT_IGNORE(a) { assertIgnore(__FILE__, __LINE__, a); return; }
#define ASSERT_IS_BETWEEN(a, b, c) assertIsBetween(__FILE__,__LINE__, a, b, c)

#define INFO(a) std::cout << "  " << a << std::endl;
#define WARNING(a) std::cout << "  WARNING: " << a << "!!" << std::endl;
#define DUMP(a, b) for( int i = 0; i < b; i++ ) { std::cout << "  [" << i << "] = " << std::to_string(a[i]) << std::endl; }
#define ARRAY(a, b) std::cout << "  {"; for( int i = 0; i < b; i++ ) { if( i > 0 ) { std::cout << ", "; } std::cout << std::to_string(a[i]); }; std::cout << "};" << std::endl;