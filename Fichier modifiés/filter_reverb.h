#ifndef __FILTER_REVERB__
#define __FILTER_REVERB__

/**
*Filtre abstrait
*/
#include "filter.h"
#include "engine/sound/BufferReverb.h"

class FilterReverb : public FilterAudio
{
public:
	static const int TAILLE_BUFFER =44100;
	float gain = 0.25f;

	BufferReverb bf1;
	BufferReverb bf2; // le meilleur des BF
	BufferReverb bf3;

	void setReverb(float reverb)
	{
		_reverb = reverb;
		bf1.SetDelay(_reverb );
		bf2.SetDelay(_reverb);
		bf3.SetDelay(_reverb);
	}

	FilterReverb() : FilterAudio()
	{
		
		bf1 = BufferReverb(TAILLE_BUFFER, 0.15f, 0.15f);
		bf2 = BufferReverb(TAILLE_BUFFER, 0.30f, 0.30f);
		bf3 = BufferReverb(TAILLE_BUFFER, 0.50f, 0.50f);
	}

	void init()
	{
		
	}

	virtual float doFilter(float ech)
	{
		/*numEch += 1 / _Frequency;*/

		//if (bufferPlein) {
		//	tmpEch = ech + bufferReverb[indiceLectEcr1];
		//}


		//// mise dans le buffer
		//bufferReverb[indiceLectEcr1] = ech * gain;
		//indiceLectEcr1++;

		//bufferPlein = (indiceLectEcr1 >= (TAILLE_BUFFER*_reverb) || bufferPlein);

		//if (indiceLectEcr1 >= (TAILLE_BUFFER * _reverb)) indiceLectEcr1 = 0;


				
		return bf1.GetEchantillon(ech + bf2.GetEchantillon(ech + bf3.GetEchantillon(ech)))  + 0.5f *ech;
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
