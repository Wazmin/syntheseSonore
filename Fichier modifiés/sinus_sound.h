#ifndef __SINUS_SOUND__
#define __SINUS_SOUND__

/**
* Synthé sinusoidal.
*/
#include "continuous_sound.h"

class SinusSound : public ContinuousSound
{
public:
	
	SinusSound() : ContinuousSound()
	{

	}

	void setFreq(float freq, float stepFreqPerPeriod)
	{
		offset = _numEch * 2 * M_PI * _freq + offset;

		_numEch = 0;
		_freq = freq;
		_stepFreqPerPeriod = stepFreqPerPeriod;
	}
private:
	float offset=0;
	float _freq;
	double _numEch = 0;
	float _stepFreqPerPeriod;
	float truc;
protected :
	virtual void init()
	{

	}

	/**
	  * Remplissage du buffer de synthèse, jusqu'au pointeur de lecture
	  */
	virtual float getNextSample()
	{
		_numEch += 1/_Frequency;


		truc = _numEch * 2*M_PI * _freq + offset;

		return sin(truc)*0.8;
	}	

};


#endif
