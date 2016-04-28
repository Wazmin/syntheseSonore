#ifndef __GRANULAR_SOUND__
#define __GRANULAR_SOUND__

/**
* Synthé granulaire.
* Lui charger un fichier de base ou il pioche les grains
* utiliser setGrainParam pour modifier les params d'extraction et d'ajout des grains
*/

#include "continuous_sound.h"

#define GRAIN_SYNTHE_BUFFER_LENGTH 1.0f //On prend 1s de buffer en memoire

/**
  * Utilise plusieurs buffers :
  * un buffer qui contient le fichier de base
  * un buffer dans lequel on fait la synthèse : on y ajoute les grains les un a la suite des autres
  *   en tenant compte de params de synthèse. C'est un buffer tournant.
  * un buffer temporaire qui permet de créer
  */
class SoundGrain : public ContinuousSound
{
private:

	//Datas du fichier de base
	float _DureeBase; ///< Duree du buffer de base
	void * _BaseBuffer; ///< Buffer qui contient les échantillons du fichier de base
	uint8 * _PtRdFinBaseBuffer; ///< Pointeur sur la fin du buffer de base
	ALenum _Format; ///< Format du fichier de base (enum OpenAL comme AL_FORMAT_MONO16)
	ALsizei _Size; ///< Taille du fichier de base en octets
	ALfloat _Frequency; ///< Frequence d'échantillonage du fichier de base (44100 par exemple)
	int _NbPistes; ///< Nombre de pistes du fichier de base (1 mono, 2 stéréo)
	int _NbBits; ///< Nombre de bits par échantillon (16 ou 8, surtout utile pour affichage)
	int _TailleEchantillon; ///< Taille d'un échantillon en octets (plus utile pour les calculs)

	//Buffer pour faire la synthèse (buffer tournant)
	//Nécessaire car overlap des grains 
	float _DureeBufferSyntheGrain; ///<Duree du buffer de synthèse
	void * _DatasSyntheGrain; ///< Pointeur sur le début du buffer de synthèse
	uint8 * _PtFinBufferSyntheGrain; ///< Pointeur sur la fin du buffer de synthe
	int _TailleBufferSyntheGrain; ///< Taille du buffer de synthèse
	uint8 * _PtRdSyntheGrain; ///< Pointeur de lecture du buffer de synthese
	uint8 * _PtWrSyntheGrain; ///< Pointeur d'ecriture dans le buffer de synthe
	
	//Paramètres de la generation de grains
	float _PosGrainf; ///< Position de prise du grain entre 0 et 1
	float _DureeGrain; ///< Durée du grain en secondes 
	int _TailleGrain; ///< Taille du grain en octets
	int _PosGraini; ///< Position du grain en octets
	int _RandomWidth; //Taille du random quand on chope un grain
	int _SizeOverlap; //Taille d'overlapp entre les grains
	float _CrossFade; //Si on utilise un crossfade linéaire ou equalpower (signal corelé ou pas)

	//Pour la lecture des grains. 
	uint8 * _PtRdGrain; ///< Pointeur de lecture du grain courant
	uint8 * _PtRdFinGrain; ///< Pointeur de fin du grain courant
	int _TailleLastGrain; ///< Taille du grain courant

public :
	void loadBaseFile(char * filename)
	{
		alutLoadMemoryFromFile(filename,&_Format,&_Size, &_Frequency);
		char entete[128] = "Chargement du fichier : ";
		strcat(entete, filename);
		Log::log(Log::USER_INFO, entete);

		// verif piste et bits
		char pistes[128] = "Nb Pistes : ";
		char format[128] = "Format : ";
		switch (_Format) {
			case  0x1100: {
				_NbPistes = 1;
				_NbBits = 8;
				Log::log(Log::USER_ERROR,"Attention piste Mono 8 bits");
				break;
			}
			case 0x1101: {
				_NbPistes = 1;
				_NbBits = 16;
				break;
			}
			case 0x1102: {
				_NbPistes = 2;
				_NbBits = 8;
				Log::log(Log::USER_ERROR, "Attention piste Stereo 8 bits");
				break;
			}
			case 0x1103: {
				_NbPistes = 2;
				_NbBits = 16;
				Log::log(Log::USER_ERROR, "Attention piste Stereo 16 bits");
				break;
			}
			default:{
				_NbPistes = -1;
				_NbBits = -1;
			}
		}

		char inte[128];
		sprintf(inte, "%d", _NbPistes);
		strcat(pistes, inte);
		Log::log(Log::USER_INFO, pistes);
		
		//Format
		sprintf(inte, "%d", _NbBits);
		strcat(format,inte);
		char nbBits[6] = " bits";
		strcat(format, nbBits);
		Log::log(Log::USER_INFO, format);

		//Frequence
		char frequence[128] = "Frequence : ";
		int i = (int)_Frequency;
		sprintf(inte, "%d", i);
		strcat(frequence, inte);
		char hertz[3] = "Hz";
		strcat(frequence, hertz);
		Log::log(Log::USER_INFO, frequence);

		//taille echantillon
		_TailleEchantillon = _NbBits == 8 ? 1 : 2;
		char tailleEchant[128] = "Taille d'un echantillon : ";
		sprintf(inte, "%d", _TailleEchantillon);
		strcat(tailleEchant, inte);
		char octet[8] = " octets";
		strcat(tailleEchant, octet);
		Log::log(Log::USER_INFO, tailleEchant);

		//nbEchantillons
		char nbEchant[128] = "Nombre total echantillon : ";
		//int j = _Size / _TailleEchantillon;
		sprintf(inte, "%d", (_Size / _TailleEchantillon));
		strcat(nbEchant, inte);
		Log::log(Log::USER_INFO, nbEchant);

		//duree du fichier
		char duree[128] = "Duree du fichier : ";
		float fo = (_Size / _TailleEchantillon)/_Frequency;
		sprintf(inte, "%f", fo);
		strcat(duree, inte);
		char seconde[10] = " secondes";
		strcat(duree, seconde);
		Log::log(Log::USER_INFO, duree);

		//On set des params par defaut au cas ou
		setGrainParam(0.5f,0.2f,0.2f,0.05f,0.5f);
	}

	void unload()
	{
		SAFEDELETE_TAB(_DatasSyntheGrain);
		free(_BaseBuffer); _BaseBuffer = NULL; //AlutLoad* = malloc;
	}

	void setGrainParam(float posGrain, float dureeGrain, float randomPos, float partOverlap, float crossFade)
	{
		_PosGrainf = posGrain;
		_DureeGrain = dureeGrain;
		_TailleGrain = (int) ( _DureeGrain * _Frequency * _TailleEchantillon );
		_TailleGrain = (_TailleGrain/4)*4; //On s'aligne sur stereo 16 bits
		_PosGraini = (int) (_Size * _PosGrainf);
		_PosGraini = (_PosGraini / 4) * 4;  //On s'aligne sur 16 bits deux voies au cas ou 
		_RandomWidth = (int) ( randomPos * _Frequency *_TailleEchantillon );
		_SizeOverlap = (int ) ( partOverlap * _DureeGrain * _Frequency * _TailleEchantillon );
		_SizeOverlap = (_SizeOverlap/4)*4; //On aligne
		_CrossFade = crossFade;
	}


private :

	/**
	  * Phase de synthèse
	  * Cette méthode va lire le buffer qui contient le fichier de base pour en extraire de grains
	  * qu'elle va copier au fur et à mesure dans le buffer de synthèse
	  * Quand elle a fini la synthèse, elle retourne un échantillon pris dans le buffer de synthèse, 
	  * sous le pointeur de lecture, et avance ce pointeur de lecture.
	  */
	virtual float getNextSample()
	{
		return 0.0f;
	}

	
};

#endif
