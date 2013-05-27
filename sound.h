#ifndef SOUND_H
#define SOUND_H

#include <iostream>
#include "windows.h"
#include <cmath>

void anounceResult(MMRESULT);

class sound
{
public:
	sound(const double& duration);//blank sound
	sound(const double& frequency, const double& duration, const int& amplitude);//waveform sound
	sound(const sound&);
	sound& operator=(const sound&);
	virtual~sound();
	void prepare();
	void play();
	//void cat();
protected:
	long int* m_data;
	WAVEFORMATEX m_format;
private:
	LPHWAVEOUT open();
	WAVEFORMATEX setupFormat();
	LPSTR compress();
	LPHWAVEOUT m_device;
	unsigned int m_numSamples;
};
#endif // SOUND_H
