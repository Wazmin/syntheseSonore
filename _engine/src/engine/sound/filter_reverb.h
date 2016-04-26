#ifndef __FILTER_REVERB__
#define __FILTER_REVERB__

/**
*Filtre abstrait
*/
#include "filter.h"

class FilterReverb : public FilterAudio
{
public:

	void setReverb(float reverb)
	{
		
	}

	FilterReverb() : FilterAudio()
	{
		
	}

	void init()
	{
		
	}

	virtual float doFilter(float ech)
	{
		return ech;
	}	

private :

};


#endif
