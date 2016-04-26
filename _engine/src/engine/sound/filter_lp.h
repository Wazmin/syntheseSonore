#ifndef __FILTER_LP__
#define __FILTER_LP__

/**
*Filtre abstrait
*/
#include "filter.h"


class FilterLP : public FilterAudio
{
public:

	void setAlpha(float alpha)
	{
		_alpha = alpha;
	}

	FilterLP() : FilterAudio()
	{
		
	}

	virtual float doFilter(float ech)
	{
		return prevEch + _alpha*(ech - prevEch);
	}	

private :
	float _alpha;
	float prevEch = 0;
};


#endif
