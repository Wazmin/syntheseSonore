#pragma once

class BufferReverb
{
public:

	BufferReverb(int nbEchantillon, float delay) 
	{
		_nbEchantillons = nbEchantillon;
		_buffer = new float [nbEchantillon];
		_iterator = 0;
		_delay = delay;
	};
	~BufferReverb();

private :
	int _nbEchantillons;
	int _iterator;
	float _delay;
	float * _buffer;
};