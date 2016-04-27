#ifndef __FILTER_REVERB__
#define __FILTER_REVERB__

/**
*Filtre abstrait
*/
#include "filter.h"

class FilterReverb : public FilterAudio
{
public:
	static const int TAILLE_BUFFER =44100;
	float gain = 0.25f;
	void setReverb(float reverb)
	{
		_reverb = reverb;
	}

	FilterReverb() : FilterAudio()
	{
		for (int i = 0; i < TAILLE_BUFFER;i++) {
			bufferReverb[i] = 0.0f;
		}
	}

	void init()
	{
		
	}

	virtual float doFilter(float ech)
	{
		/*numEch += 1 / _Frequency;*/

		if (bufferPlein) {
			tmpEch = ech + bufferReverb[indiceLectEcr1];
		}


		// mise dans le buffer
		bufferReverb[indiceLectEcr1] = ech * gain;
		indiceLectEcr1++;

		bufferPlein = (indiceLectEcr1 >= (TAILLE_BUFFER*_reverb) || bufferPlein);

		if (indiceLectEcr1 >= (TAILLE_BUFFER * _reverb)) indiceLectEcr1 = 0;
				
		return tmpEch;
	}

	float reverb1(float ech, float gain) {
		bufferReverb[indiceLectEcr1] += ech * gain;
		return 0.0f;
	}


private :
	float _reverb;
	float tmpEch = 0;
	float bufferReverb[TAILLE_BUFFER];
	int numEch = 0;
	int indiceLectEcr1 = 0;
	bool bufferPlein = false;

};


#endif
