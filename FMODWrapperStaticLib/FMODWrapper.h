#pragma once

#include "fmod.h"
#include "fmod.hpp"
#include "fmod_errors.h"

const int   INTERFACE_UPDATETIME = 50;      // 50ms update for interface
const float DISTANCEFACTOR = 1.0f;          // Units per meter.

namespace MyFMODLib{

	class FMODWrapper {

	public:


		void* extradriverdata = 0;
		FMOD_RESULT result;
		FMOD::System* system = nullptr;
		FMOD::Sound* sound, * sound_to_play;
		FMOD::Channel* channel = 0;
		FMOD::ChannelGroup* channelGroup = 0;
		int               numsubsounds;
		float t = 0;
		FMOD_VECTOR lastpos = { 0.0f, 0.0f, 0.0f };
		FMOD_VECTOR      listenerpos = { 0.0f, 0.0f, -1.0f * DISTANCEFACTOR };
		float volume;

		void InitFMOD(void *);
		void LoadOneShot(bool isStream, const char* nameOfTheSound);
		void LoadLoop(bool isStream, const char* nameOfTheSound);
		void Load3DSound(const char * nameOfTheSound);
		void MakeSoundPanLeftToRight();
		void RaiseVolume();
		void LowerVolume();
		void AddNewSoundAfterLoad(const char* nameOfTheSound);
		void PlayChannels();
		void PauseChannels();
		void GetGroupChannelPaused(bool* paused);
		void StopChannels();
		void Stop();
		void CloseFMOD();
	};

}