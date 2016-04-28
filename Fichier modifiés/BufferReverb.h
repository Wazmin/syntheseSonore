#pragma once

class BufferReverb
{
public:

	BufferReverb() {
	};

	// constructeur par recopie
	BufferReverb( const BufferReverb &);

	// constructeur et parametres
	BufferReverb(int nbEchantillon, float delay, float gain) 
	{
		_nbEchantillons = nbEchantillon;
		_buffer = new float [nbEchantillon];
		_iterator = 0;
		_delay = delay;
		_gain = gain;
		InitBuffer();
	};


	// changement du delai des premiere reverberation
	void BufferReverb::SetDelay(float delay) 
	{
		_delay = delay;
	};

	// obtenir l'echantillon a ajouter
	float BufferReverb::GetEchantillon(float fToSave) {
		float tmpEchantillon = _buffer[_iterator];
		_buffer[_iterator] = fToSave * _gain;
		UpIterator();
		return tmpEchantillon;
	};



private :
	int _nbEchantillons;
	int _iterator;
	float _gain;
	float _delay;
	float * _buffer;

	//gestion de l'iterateur de lecture ecriture
	void BufferReverb::UpIterator() 
	{
		if (++_iterator >= _nbEchantillons*_delay)
			_iterator = 0;
	};

	// mise a zero des donnes du buffer
	void BufferReverb::InitBuffer() {
		for (int i = 0; i < _nbEchantillons;i++) {
			_buffer[i] = 0.0f;
		}
	};
};