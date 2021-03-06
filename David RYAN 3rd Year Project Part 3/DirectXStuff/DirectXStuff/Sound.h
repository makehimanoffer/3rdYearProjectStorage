
 

#ifndef _SOUNDCLASS_H_
#define _SOUNDCLASS_H_

// LINKING //

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
 
 

// INCLUDES //

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>
 
 



class Sound
{
private:
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};
 public:
	Sound();
	~Sound();
	bool Init(HWND);
	void KillSafely();
 
private:
	bool InitializeDirectSound(HWND);
	void ShutdownDirectSound();
 
	bool LoadWaveFile(char*, IDirectSoundBuffer8**);
	void ShutdownWaveFile(IDirectSoundBuffer8**);
 
	bool PlayWaveFile();
 
private:
	IDirectSound8* _DirectSound;
	IDirectSoundBuffer* _primaryBuffer;

	//a secondary buffer for each sound. only one sound in this

	IDirectSoundBuffer8* _secondaryBuffer1;
};
 
#endif
