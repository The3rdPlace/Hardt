#include <stdio.h>
#include <iostream>
#include <string.h>

#include <cstdlib>
#include <string>
#include <unistd.h>


// SERVER
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>

// CLIENT
#include <arpa/inet.h>

#include <fstream>

#include "../hardt/include/hardtapi.h"
#include "dspcmd.h"

//#include "portaudio.h"




/*static int s_interrupted = 0;
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
}*/




int main(int argc, char**argv)
{




    if( argc == 2 && strncmp(argv[1], "d", 1) == 0 ) {

        fprintf(stderr, "stderr is being redirected to dspcmd.err\n");
        freopen("dspcmd.err", "w", stderr);

        PaError err = Pa_Initialize();
    	if( err != paNoError )
    	{
    		printf("Initialization error\n");
    		return 1;
    	}


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


        const   PaDeviceInfo *deviceInfo;
        for( int i=0; i<numDevices; i++ )
        {
            deviceInfo = Pa_GetDeviceInfo(i);
            double desiredSampleRate;
            PaStreamParameters testParameters;
            testParameters.device = i;
            testParameters.channelCount = 1;
            testParameters.sampleFormat = paInt16;
            testParameters.suggestedLatency = Pa_GetDeviceInfo(i)->defaultLowInputLatency ;
            testParameters.hostApiSpecificStreamInfo = NULL; //See you specific host's API docs for info on using this field

            std::cout << "==========================================" << std::endl;
            std::cout << "Device: " << i << " = " << deviceInfo->name << std::endl;
            std::cout << "Input: " << deviceInfo->maxInputChannels << " channels" << std::endl;
            std::cout << "Output: " << deviceInfo->maxOutputChannels << " channels" << std::endl;

            int rates[] = { 8000, 11025, 22050, 32000, 44100, 48000 };
            bool gotOne = false;
            for( int j = 0; j < 6; j++)
            {
                PaError err;
                err = Pa_IsFormatSupported( &testParameters, NULL, rates[j] );
                if( err == paFormatIsSupported )
                {
                   std::cout << (j > 0 ? ", " : "") << rates[j];
                   gotOne = true;
                }
            }
            if( !gotOne )
            {
                std::cout << "(no supported rates)";
            }
            std::cout << std::endl;
        }


        err = Pa_Terminate();
        if ( err != paNoError )
        {
            printf("Termination error\n");
            return 3;
        }


        return 0;

    }




    std::cout << argv[1] << std::endl;
    if( argc == 3 && strncmp(argv[1], "c", 1) == 0 ) {




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
        serv_addr.sin_port = htons(8080);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if(inet_pton(AF_INET, argv[2], &serv_addr.sin_addr)<=0)
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

        do
        {
            valread = read( sock , buffer, 1024);

            myfile.write(buffer, valread);
        }
        while( valread > -1 ); //&& s_interrupted == 0);
        close(sock);
        myfile.close();

        return 0;
    }




	std::cout << "dspcmd: using Hardt " + getversion() << "\n" ;




    /*Generator* g = new Generator();

    int freqs[1];
    freqs[0] = 25;
    std::complex<double>* samples = g->GetSamples(1000, 1, 1, freqs, 1);
    //print_array< std::complex<double> >(samples, 1000);

    delete g;*/





    std::cout << "INIT complete" << std::endl;
    int device = atoi(argv[1]);

    HSoundcardReader<int> rdr(device, 48000, 1, paInt32, 1024);
    HNetworkServer<int> srv = HNetworkServer<int>(8080, &rdr);
    try
    {
        srv.Run();
    }
    catch( ... )
    {
        std::cout << "OUTER EXCEPTION" << std::endl;
    }
    std::cout << "GOODBYE" << std::endl;

	return 0;
}

