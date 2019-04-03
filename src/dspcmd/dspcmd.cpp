//#include "dspcmd.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <signal.h>

#include <cstdlib>
#include <string>
#include <unistd.h>


// SERVER
//#include <unistd.h>
//#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
//#include <string.h>
#define PORT 8080

// CLIENT
//#include <sys/socket.h>
//#include <stdlib.h>
//#include <netinet/in.h>
//#include <string.h>
#include <arpa/inet.h>

#include <fstream>

#include "../hardt/include/hardtapi.h"
#include "dspcmd.h"

#include "portaudio.h"

// condition_variable::wait (with predicate)
#include <iostream>           // std::cout
#include <thread>             // std::thread, std::this_thread::yield
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable

struct USERDATA
{
	int a;
	int b;
};

std::mutex mtx;
std::condition_variable cv;
std::condition_variable cv2;

#define FRAME_SIZE 1024
#define NUMBER_OF_BUFFERS 4

float buffer[NUMBER_OF_BUFFERS * FRAME_SIZE];
unsigned int wrloc = 0;
unsigned int rdloc = 0;
int frames_in = 0;
int frames_out = 0;
long latencyAvail = -1;

static int callback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData );
void writeToNw(int new_socket);



/*void consume (int n) {
  for (int i=0; i<n; ++i) {
    std::unique_lock<std::mutex> lck(mtx);
    cv.wait(lck,shipment_available);
    // consume:
    std::cout << cargo << '\n';
    cargo=0;
  }
}*/

bool shipment_available() {return wrloc != rdloc;}


static int s_interrupted = 0;
static void s_signal_handler (int signal_value)
{
    s_interrupted = 1;
    std::unique_lock<std::mutex> lck(mtx);
    std::cout << "outer handler locked" << std::endl;
    cv2.notify_one();
    std::cout << "outer handler cv2.notify_one()" << std::endl;
}

bool is_interrupted() {
    return s_interrupted != 0;
}

static void s_catch_signals (void)
{
    struct sigaction action;
    action.sa_handler = s_signal_handler;
    action.sa_flags = 0;
    sigemptyset (&action.sa_mask);
    sigaction (SIGINT, &action, NULL);
    sigaction (SIGTERM, &action, NULL);
}

int main(int argc, char**argv)
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char hello[] = "Hello from server";

    s_catch_signals();

    std::cout << argv[1] << std::endl;
    if( argc == 2 && strncmp(argv[1], "c", 1) == 0 ) {
        //struct sockaddr_in address;
        int sock = 0, valread;
        struct sockaddr_in serv_addr;
        //char *hello = "Hello from client";
        char buffer[1024] = {0};
        std::cout << "client" << std::endl;


        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            printf("\n Socket creation error \n");
            return -1;
        }

        memset(&serv_addr, '0', sizeof(serv_addr));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
        {
            printf("\nInvalid address/ Address not supported \n");
            return -1;
        }

        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            printf("\nConnection Failed \n");
            return -1;
        }


        std::ofstream myfile;
        myfile.open ("data.raw", std::ios::out | std::ios::binary | std::ios::trunc);
        //myfile << "Writing this to a file.\n";
        //myfile.close();

        do
        {
            valread = read( sock , buffer, FRAME_SIZE);
            //printf("%s\n",buffer );
            std::cout << "valread: " << valread << std::endl;
            myfile.write(buffer, valread);
        }
        while( valread > -1 && s_interrupted == 0);
        close(sock);
        myfile.close();

        return 0;
    }



    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }



    //send(new_socket , hello , strlen(hello) , 0 );
    //printf("Hello message sent\n");
    //close(new_socket);

	std::cout << "dspcmd: using Hardt " + getversion() << "\n" ;
    //return 0;




    /*Generator* g = new Generator();

    int freqs[1];
    freqs[0] = 25;
    std::complex<double>* samples = g->GetSamples(1000, 1, 1, freqs, 1);
    //print_array< std::complex<double> >(samples, 1000);

    delete g;*/





    // CLOSE STDERR
    fprintf(stderr, "This will go to the terminal.\n");
    //Save position of current standard output
    fpos_t pos;
    fgetpos(stderr, &pos);
    int fd = dup(fileno(stderr));
    freopen("/tmp/somefile.txt", "w", stderr);

    fprintf(stderr, "This will go to the file /tmp/somefile.txt.\n");
    //Flush stdout so any buffered messages are delivered


	PaError err = Pa_Initialize();
	if( err != paNoError )
	{
		printf("Initialization error\n");
		return 1;
	}


    fflush(stderr);
    //Close file and restore standard output to stdout - which should be the terminal
    dup2(fd, fileno(stderr));
    close(fd);
    clearerr(stderr);
    fsetpos(stderr, &pos);
    fprintf(stderr, "This will go to the terminal again.\n");
    fflush(stderr);

    // BACK AGAIN



    std::cout << "INIT complete" << std::endl;

	int numDevices;
	numDevices = Pa_GetDeviceCount();
	if( numDevices < 0 )
	{
		printf("No devices\n");
		return 2;
	}
	printf("Number of devices: %d\n", numDevices);

    PaDeviceIndex x = Pa_GetDefaultInputDevice ();
    std::cout << "Default device is: " << x << std::endl;


    if( argc == 1 )
    {
        const   PaDeviceInfo *deviceInfo;
        for( int i=0; i<numDevices; i++ )
        {

            double desiredSampleRate;
            PaStreamParameters testParameters;
            testParameters.device = i;
            testParameters.channelCount = 1;
            testParameters.sampleFormat = paFloat32;
            testParameters.suggestedLatency = Pa_GetDeviceInfo(i)->defaultLowInputLatency ;
            testParameters.hostApiSpecificStreamInfo = NULL; //See you specific host's API docs for info on using this field

            int rates[] = { 8000, 11025, 22050, 32000, 44100, 48000 };
            for( int j = 0; j < 6; j++)
            {
                std::cout << "-  test rate " << rates[j] << std::endl;
                PaError err;
                err = Pa_IsFormatSupported( &testParameters, NULL, rates[j] );
                if( err == paFormatIsSupported )
                {
                   std::cout << "   * is ok" << std::endl;
                }
                else
                {
                    std::cout << "   * is bad" << std::endl;
                }
            }
        }
        return 0;
    }

    int device = atoi(argv[1]);
    std::cout << "Using device " << device << std::endl;
    PaStreamParameters inputParameters;
    inputParameters.device = device;
    inputParameters.channelCount = 1;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(device)->defaultLowInputLatency ;
    inputParameters.hostApiSpecificStreamInfo = NULL; //See you specific host's API docs for info on using this field



	#define SAMPLE_RATE 48000
	static USERDATA ud;
	
	PaStream *stream;
	PaStreamFlags flags = paNoFlag;
	err = Pa_OpenStream(&stream, &inputParameters, NULL,
		SAMPLE_RATE,
		FRAME_SIZE,   /* Frames pr. buffer */
		flags,
		callback,
		&ud);

	if( err != paNoError )
	{
		printf("error setting up sampling");
		return 4;
	}

	std::cout << "Sampling :: press ctrl+c to stop" << std::endl;

    err = Pa_StartStream( stream );
    if( err != paNoError )
    {
        printf("error starting sampling");
        Pa_Terminate();
        return 5;
    }


    std::thread consumer_thread (writeToNw, new_socket);

    // produce 10 items when needed:
    /*
    for (int i=0; i<10; ++i) {
        while (shipment_available()) std::this_thread::yield();
        std::unique_lock<std::mutex> lck(mtx);
        cargo = i+1;
        cv.notify_one();
    }
    */

    int outerExecs = 0;
    while(1)
    {
        /*try
        {
            //std::cout << "wr\n";
            writeToNw();
        }
        catch( std::exception )
        {
            std::cout << "exception\n";
        }*/


        //std::this_thread::yield();

        //while (!shipment_available()) std::this_thread::yield();

        std::cout << "before outer lock" << std::endl;
        std::unique_lock<std::mutex> lck(mtx);
        std::cout << "before outer wait" << std::endl;
        cv2.wait(lck,is_interrupted);
        std::cout << "after outer wait" << std::endl;

        if (s_interrupted) {
                    std::cout << "Stopping" << std::endl;
                    //consumer_thread.terminate();
                    break;
                    }

        outerExecs++;
        std::cout << "outer yielding" << std::endl;
        std::this_thread::yield();
        //std::cout << "spin" << std::endl;
    }

    consumer_thread.join();

	/*char ch;
	do
	{
		ch = getchar();
	}
	while( ch != 'a' );*/


    err = Pa_StopStream( stream );
    if( err != paNoError )
    {
        printf("error stopping sampling");
        Pa_Terminate();
        return 6;
    }

	err = Pa_Terminate();
	if ( err != paNoError )
	{
		printf("Termination error\n");
		return 3;
	}

	std::cout << "Frames read: " << frames_in << std::endl;
	std::cout << "Frames transmitted: " << frames_out << std::endl;
	std::cout << "Latency: " << latencyAvail/1000 << std::endl;
    std::cout << "Outer latency: " << outerExecs << std::endl;
	return 0;
}

static int callback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    /* Cast data passed through stream to our structure. */
    //paTestData *data = (paTestData*)userData;
    static unsigned int rotor = 0;

    std::cout << "data" << std::endl;
    memcpy((void*) &buffer[wrloc], inputBuffer, framesPerBuffer * sizeof(paFloat32));
    rotor = (rotor + 1) & 0x3;
    wrloc = FRAME_SIZE * rotor;
    frames_in++;


    if( wrloc == rdloc )
    {
        std::cout << "Dropped frame!" << std::endl;
    }

    std::unique_lock<std::mutex> lck(mtx);
    std::cout << "locked" << std::endl;
    cv.notify_one();
    std::cout << "cv.notify_one()" << std::endl;

    return 0;
}


void writeToNw(int new_socket)
{
    std::cout << "writeToNew(" << new_socket << ")" << std::endl;
    while(!s_interrupted){
        static unsigned int rotor = 0;
        static unsigned int latency = 0;

        std::cout << "before lock" << std::endl;
        std::unique_lock<std::mutex> lck(mtx);
        std::cout << "before wait" << std::endl;
        cv.wait(lck,shipment_available);
        std::cout << "after wait" << std::endl;

        /*if( s_interrupted ) {
            std::cout << "IS TERMINATED" << std::endl;
            //std::terminate();

        }*/

        if( wrloc != rdloc )
        {
            // Transfer frames to network
            //memcpy((void*) &buffer[wrloc], inputBuffer, framesPerBuffer * sizeof(paFloat32));
            send(new_socket, (void*) &buffer[rdloc], FRAME_SIZE * sizeof(paFloat32), 0 );



            std::cout << "read " << frames_out << std::endl;

            rotor = (rotor + 1) & 0x3;
            rdloc = FRAME_SIZE * rotor;

            frames_out++;

            if( latencyAvail == -1 )
                latencyAvail = latency;
            else if( latency > latencyAvail )
                latencyAvail++;
            else if( latency < latencyAvail )
                latencyAvail--;
            //std::cout << latency << std::endl;
            latency = 0;
        }
        else
        {
            std::cout << "waste" << std::endl;
            latency++;
            //std::cout << "skip" << std::endl;
        }
    }

    std::cout << "writeToNw GOODBYE" << std::endl;
}