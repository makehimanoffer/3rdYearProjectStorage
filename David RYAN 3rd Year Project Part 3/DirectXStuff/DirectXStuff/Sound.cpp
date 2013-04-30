
 

#include "Sound.h"
Sound::Sound()
{
	_DirectSound = 0;
	_primaryBuffer = 0;
	_secondaryBuffer1 = 0;
}
  
 
Sound::~Sound()
{
}
 
 
bool Sound::Init(HWND hwnd)
{
	bool result;

	//init DirectSound API as primary buffer.

	// Initialize direct sound and primary sound buffer.
	result = InitializeDirectSound(hwnd);
	if(!result)
	{
		return false;
	}
 
	// Load a wave audio file onto a secondary buffer.
	result = LoadWaveFile("../battlemusic.wav", &_secondaryBuffer1);
	if(!result)
	{
		return false;
	}
 
	// Play wave file now that it has been loaded.
	result = PlayWaveFile();
	if(!result)
	{
		return false;
	}
 
	return true;
}

void Sound::KillSafely()
{
	// Release secondary buffer.
	ShutdownWaveFile(&_secondaryBuffer1);

	// Shutdown Direct Sound API.
	ShutdownDirectSound();
 
	return;
}

//in init directsound. default sound device is used for convenience. further looking into 
//messing with that.
// see what happens
bool Sound::InitializeDirectSound(HWND hwnd)
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;
 
 
	// Initialize direct sound interface pointer for default sound device.
	result = DirectSoundCreate8(NULL, &_DirectSound, NULL);
	if(FAILED(result))
	{
		return false;
	}
 
	// Set cooperative level to priority so format of primary sound buffer can be modified.
	result = _DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if(FAILED(result))
	{
		return false;
	}

	//SETTING UP BUFFER DESCRIPTIONS AS DONE LIKE A MILLION TIMES AT THIS STAGE
	// Setup primary buffer description.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;
 
	// Get control of primary sound buffer on default sound device.
	result = _DirectSound->CreateSoundBuffer(&bufferDesc, &_primaryBuffer, NULL);
	if(FAILED(result))
	{
		return false;
	}

	//want to change format of desired format. audiowise.
	//setting format to being uncompressed cd quality.
	// Setup format of primary sound bufffer.
	// In this case it is a .WAV file recorded at 44,100 samples per second in 16-bit stereo (cd audio format).
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;
 
	// Set primary buffer to be wave format specified.
	result = _primaryBuffer->SetFormat(&waveFormat);
	if(FAILED(result))
	{
		return false;
	}
 
	return true;
}

void Sound::ShutdownDirectSound()
{
	// Release primary sound buffer pointer.
	if(_primaryBuffer)
	{
		_primaryBuffer->Release();
		_primaryBuffer = 0;
	}
 
	// Release direct sound interface pointer.
	if(_DirectSound)
	{
		_DirectSound->Release();
		_DirectSound = 0;
	}
 
	return;
}


//built for wav files only. see what it would be like for mp3 at some stage sher.
bool Sound::LoadWaveFile(char* filename, IDirectSoundBuffer8** secondaryBuffer)
{
	int error;
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char *bufferPtr;
	unsigned long bufferSize;

	// Open wave file in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if(error != 0)
	{
		return false;
	}
 
	// Read in wave file header.
	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if(count != 1)
	{
		return false;
	}
 /*
	// Check that chunk ID is RIFF format.
	if((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') || 
	   (waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
	{
		return false;
	}
 
	// Check that file format is WAVE format.
	if((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
	   (waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
	{
		return false;
	}
 
	// Check that sub chunk ID is fmt format.
	if((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
	   (waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
	{
		return false;
	}
 
	// Check that audio format is WAVE_FORMAT_PCM.
	if(waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}
 
	// Check that wave file was recorded in stereo format.
	if(waveFileHeader.numChannels != 2)
	{
		return false;
	}
 
	// Check that wave file was recorded at a sample rate of 44.1 KHz.
	if(waveFileHeader.sampleRate != 44100)
	{
		return false;
	}
 
	// Ensure that wave file was recorded in 16 bit format.
	if(waveFileHeader.bitsPerSample != 16)
	{
		return false;
	}
 
	// Check for data chunk header.
	if((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
	   (waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
		return false;
	}
	*/

	// Set wave format of secondary buffer that this wave file will be loaded onto.
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;
 
	// Set buffer description of secondary sound buffer that wave file will be loaded onto.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Create a temporary sound buffer with specific buffer settings.
	result = _DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if(FAILED(result))
	{
		return false;
	}
 
	// Test buffer format against direct sound 8 interface and create secondary buffer.
	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
	if(FAILED(result))
	{
		return false;
	}
 
	// Release temporary buffer.
	tempBuffer->Release();
	tempBuffer = 0;

	//secondary buffer is a circular buffer. 
	// can have two positions and two pointers. 
	//buffer is same size as audio file in this

	// Move to beginning of wave data which starts at end of data chunk header.
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);
 
	// Create a temporary buffer to hold wave file data.
	waveData = new unsigned char[waveFileHeader.dataSize];
	if(!waveData)
	{
		return false;
	}
 
	// Read in wave file data into newly created buffer.
	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if(count != waveFileHeader.dataSize)
	{
		return false;
	}
 
	// Close file once done reading.
	error = fclose(filePtr);
	if(error != 0)
	{
		return false;
	}
 
	// Lock secondary buffer to write wave data into it.
	result = (*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if(FAILED(result))
	{
		return false;
	}
 
	// Copy wave data into buffer.
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);
 
	// Unlock secondary buffer after data has been written to it.
	result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if(FAILED(result))
	{
		return false;
	}
	
	// Release wave data since it was copied into secondary buffer.
	delete [] waveData;
	waveData = 0;
 
	return true;
}

void Sound::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer)
{
	// Release secondary sound buffer.
	if(*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}

	return;
}


//now we're onto play wave.
//mix audio into primary buffer. 
//set position to start playing at beginning
//otherwise continue from where last stopped playing
//volume is also set to max.

bool Sound::PlayWaveFile()
{
	HRESULT result;
 
 
	// Set position at beginning of sound buffer.
	result = _secondaryBuffer1->SetCurrentPosition(0);
	if(FAILED(result))
	{
		return false;
	}
 
	// Set volume of buffer to 100%.
	result = _secondaryBuffer1->SetVolume(DSBVOLUME_MAX);
	if(FAILED(result))
	{
		return false;
	}
 
	// Play contents of secondary sound buffer.
	result = _secondaryBuffer1->Play(0, 0, 0);
	if(FAILED(result))
	{
		return false;
	}
 
	return true;
}