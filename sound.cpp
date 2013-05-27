#include "sound.h"

sound::sound(const double& duration):
	m_data(),m_format(setupFormat()),m_device(open()),m_numSamples(m_format.nSamplesPerSec*duration)
{
	m_data = new long int[m_numSamples];
}

sound::sound(const double& frequency,const double& duration, const int& amplitude):
	m_data(),m_format(setupFormat()),m_device(open()),m_numSamples(m_format.nSamplesPerSec*duration)
{
		double omega = frequency/3.14159;
		double timeInterval = 1.0/m_format.nSamplesPerSec;
		//timeInterval /= m_format.nSamplesPerSec;
		std::cout<<"omega is "<<omega<<std::endl;
		m_data = new long int[m_numSamples];
		for(unsigned int i = 0; i < m_numSamples; i++)
		{
		//for(unsigned int j = 0; j < format.nBlockAlign; j++)
			//{

			double time = timeInterval*i;

			m_data[i] = amplitude*(sin(omega*time));
			m_data[i] *= -0.5*(cos(time*2.0*3.141592/duration)-1);
			//}
		}
}

sound::sound(const sound& other):
	m_data(),m_format(),m_device(),m_numSamples(other.m_numSamples)
{
	m_format = (other.m_format);
	m_data = new long int[m_numSamples];
	*m_device = *(other.m_device);
}

sound& sound::operator=(const sound& other)
{
	delete[] m_data;
	m_numSamples = other.m_numSamples;
	m_format = (other.m_format);
	m_data = new long int[m_numSamples];
	*m_device = *(other.m_device);
	return *this;
}

sound::~sound()
{
	delete[] m_data;
}

WAVEFORMATEX sound::setupFormat()
{
	WAVEFORMATEX format = {0};
	format.wFormatTag = WAVE_FORMAT_PCM;
	format.nChannels = 1;
	format.nSamplesPerSec = 44100*4;
	format.wBitsPerSample = 16;
	format.nBlockAlign = format.nChannels * format.wBitsPerSample / 8;//data must be dealt with in multiples of this many bytes
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
	format.cbSize = 0;
	return format;
}

LPHWAVEOUT sound::open()
{
	LPHWAVEOUT device = new(HWAVEOUT__*);//actually a pointer to a handle of a device
	MMRESULT status = waveOutOpen(device, WAVE_MAPPER, &m_format, 0,0,CALLBACK_NULL );//open a device, store the result
	anounceResult(status);
	return device;
}

void sound::prepare()
{

}

LPSTR sound::compress()
{
	char* data_out = new char[m_format.nBlockAlign*m_numSamples];
//	for(unsigned int i = 0;i<m_numSamples;i+=m_format.nBlockAlign)//genaric varient
//	{
//		for(unsigned int j = 0;j<m_format.nBlockAlign;j++)
//		{
//			int value = m_data[i]>>(m_format.nBlockAlign-j*8);
//			value = value&511;
//			data_out[i+j] = value;
//		}
//		//data_out[i] = m_data[i];
//	}

	for(unsigned int i = 0;i<m_numSamples;i++)//16 bit varient
	{
		*(((short*)data_out)+i) = m_data[i];
		//data_out[i+1] = m_data[i/2]/128;
		//data_out[i] = m_data[i];
	}

	return (LPSTR)data_out;
}

void sound::play()
{
		LPWAVEHDR firstWaveHeader = new WAVEHDR;
	(*firstWaveHeader).lpData = compress();
	(*firstWaveHeader).dwBufferLength = 44100*60;
	(*firstWaveHeader).dwFlags = 0;

	MMRESULT status = waveOutPrepareHeader(*m_device,firstWaveHeader,sizeof(WAVEHDR));
	anounceResult(status);

	status = waveOutWrite(*m_device,firstWaveHeader,sizeof(WAVEHDR));
	anounceResult(status);
}

void anounceResult(MMRESULT status)
{
	if (status == MMSYSERR_NOERROR)
	{
		std::cout<<"Yay, no errors!"<<std::endl;
	}
	else if(status == MMSYSERR_ALLOCATED)
	{
		std::cout<<"That device is aleady allocated."<<std::endl;
	}
	else if(status == MMSYSERR_BADDEVICEID)
	{
		std::cout<<"That's not a device ID!"<<std::endl;
	}
	else if(status == MMSYSERR_INVALHANDLE)
	{
		std::cout<<"Idiot, you used an invalid handle!"<<std::endl;
	}
	else if(status == MMSYSERR_NODRIVER)
	{
		std::cout<<"There's no driver for your device?"<<std::endl;
	}
	else if(status == WAVERR_UNPREPARED)
	{
		std::cout<<"You should prepare your data block first."<<std::endl;
	}
	else if(status == MMSYSERR_NOMEM)
	{
		std::cout<<"Oops, out of memory!"<<std::endl;
	}
	else if(status == WAVERR_BADFORMAT)
	{
		std::cout<<"That's not a valid format!"<<std::endl;
	}
	else if(status == WAVERR_SYNC)
	{
		std::cout<<"Try WAVE_ALLOWSYNC."<<std::endl;
	}
	else
	{
		std::cout<<"Good work, you're not even supposed to be able to get here."<<std::endl;
	}
}
