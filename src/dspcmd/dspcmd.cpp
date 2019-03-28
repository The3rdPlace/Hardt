#include "dspcmd.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <signal.h>

#include <cstdlib>
#include <string>
#include <unistd.h>

#include "../hardt/include/hardtapi.h"
#include "dspcmd.h"

#include "portaudio.h"

struct USERDATA
{
	int a;
	int b;
};

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
void writeToNw();



static int s_interrupted = 0;
static void s_signal_handler (int signal_value)
{
    s_interrupted = 1;
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
	std::cout << "dspcmd: using Hardt " + getversion() << "\n" ;

    /*Generator* g = new Generator();

    int freqs[1];
    freqs[0] = 25;
    std::complex<double>* samples = g->GetSamples(1000, 1, 1, freqs, 1);
    //print_array< std::complex<double> >(samples, 1000);

    delete g;*/



    s_catch_signals();

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

    while(1)
    {
        try
        {
            //std::cout << "wr\n";
            writeToNw();
        }
        catch( std::exception )
        {
            std::cout << "exception\n";
        }

        if (s_interrupted) {
                    std::cout << "Stopping" << std::endl;
                    break;
                    }
    }

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


    memcpy((void*) &buffer[wrloc], inputBuffer, framesPerBuffer * sizeof(paFloat32));
    rotor = (rotor + 1) & 0x3;
    wrloc = FRAME_SIZE * rotor;
    frames_in++;


    if( wrloc == rdloc )
    {
        std::cout << "Dropped frame!" << std::endl;
    }

    return 0;
}


void writeToNw()
{
    static unsigned int rotor = 0;
    static unsigned int latency = 0;

    if( wrloc != rdloc )
    {
        // Transfer frames to network

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
        latency++;
        //std::cout << "skip" << std::endl;
    }
}