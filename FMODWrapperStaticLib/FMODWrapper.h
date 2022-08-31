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

		void InitFMOD(void *);
		void LoadStatic(bool looping);
		void LoadStreaming(bool looping);
		void MakeSoundPanLeftToRight();
		void RaiseVolume();
		void LowerVolume();
		void AddNewSoundAfterLoad();
		void Stop();
		void Play();
		void Pause();
	};

}