#ifndef __NOISE_SOUND__
#define __NOISE_SOUND__

/**
* Bruit de variabilité ajustable
*/
#include "continuous_sound.h"

class NoiseSound : public ContinuousSound
{
public:
	
	NoiseSound() : ContinuousSound()
	{		
		for (int i = 0; i < MAX_DICES;i++) {
			time[i] = 0;
		}
	}

	void setFreq(float freq)
	{
		_FreqNoise = freq;

	}

private:
	// ajout variable pour capter la frequence;
	static const int MAX_DICES = 10;
	float _FreqNoise;
	int nbEchant = 0;
	float f;
	float val;
	float time[MAX_DICES];
	float dices[MAX_DICES];
	float freqDice[MAX_DICES];

protected :

	/**
	  * Remplissage du buffer de synthèse, jusqu'au pointeur de lecture
	  */
	virtual float getNextSample()
	{	
		float tVal = 0;
		for (int i = 0; i < MAX_DICES;i++) {
			time[i] += 1 / _Frequency ;
			if (time[i] > 1 / (_FreqNoise * i) )  {
				dices[i] = (randf()*2.0f - 1)*0.5f;
				time[i] = 0;
			}
			tVal += dices[i];
		}
		return tVal/MAX_DICES;
	}	
};

#endif
