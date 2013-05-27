#include <iostream>
#include "windows.h"
#include <cmath>

#include "sound.h"

static const int sucsessfulExitStatus = 0;

LPSTR sineBlock(WAVEFORMATEX format, double frequency, double duration, int amplitude);

int main()
{
    int exitStatus = sucsessfulExitStatus;



//	if(PlaySound(TEXT("C:\\Users\\TELunus\\Music\\manedown3.mp3"), NULL, SND_FILENAME|SND_ASYNC))
//	{
//		std::cout<<"it worked!"<<std::endl;
//	}
//	else
//	{
//		std::cout<<"Nope!"<<std::endl;
//	}

	sound mySound(33100/10.0,32,256);
	mySound.play();


	system("PAUSE");

    return exitStatus;
}

LPSTR sineBlock(WAVEFORMATEX format, double frequency, double duration, int amplitude)//only for 1 channel 8 bit for now.
{
	unsigned int numSamples = format.nSamplesPerSec*duration;
	double omega = frequency/3.14159;
	double timeInterval = 1.0;
	timeInterval /= format.nSamplesPerSec;
	std::cout<<"omega is "<<omega<<std::endl;
	LPSTR data = new char[format.nBlockAlign*numSamples];
	for(unsigned int i = 0; i < numSamples; i++)
	{
		//for(unsigned int j = 0; j < format.nBlockAlign; j++)
		//{

		double time = timeInterval*i;

		data[format.nBlockAlign*i] = amplitude*(sin(omega*time));
		//}
	}
	return data;
}
