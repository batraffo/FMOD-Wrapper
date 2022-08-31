#include "pch.h"

#include "fmod.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#include "common.h"
#include "common_platform.h"


#include "FMODWrapper.h"


void MyFMODLib::FMODWrapper::InitFMOD(void* extradriverdata)
{
	result = FMOD::System_Create(&system);      // Create the main system object.
	ERRCHECK(result);
	result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);    // Initialize FMOD.
	ERRCHECK(result);
	result = system->set3DSettings(1.0, DISTANCEFACTOR, 1.0f);
	ERRCHECK(result);
}

void MyFMODLib::FMODWrapper::LoadOneShot(bool isStream, const char* nameOfTheSound)
{
	if (isStream) {
		result = system->createStream(Common_MediaPath(nameOfTheSound), FMOD_LOOP_OFF, 0, &sound);
		ERRCHECK(result);

	}
	else {
		result = system->createSound(Common_MediaPath(nameOfTheSound), FMOD_LOOP_OFF, 0, &sound);
		ERRCHECK(result);
	}

	result = sound->getNumSubSounds(&numsubsounds);
	ERRCHECK(result);

	if (numsubsounds)
	{
		sound->getSubSound(0, &sound_to_play);
		ERRCHECK(result);
	}
	else
	{
		sound_to_play = sound;
	}
	result = system->playSound(sound_to_play, 0, false, &channel);
	ERRCHECK(result);

	result = channel->getVolume(&volume);
	ERRCHECK(result);

	result = channel->getChannelGroup(&channelGroup);
	ERRCHECK(result);

}

void MyFMODLib::FMODWrapper::LoadLoop(bool isStream, const char* nameOfTheSound)
{
	if (isStream) {
		result = system->createStream(Common_MediaPath(nameOfTheSound), FMOD_LOOP_NORMAL, 0, &sound);
		ERRCHECK(result);
	}
	else {
		result = system->createSound(Common_MediaPath(nameOfTheSound), FMOD_LOOP_NORMAL, 0, &sound);
		ERRCHECK(result);
	}

	result = sound->getNumSubSounds(&numsubsounds);
	ERRCHECK(result);

	if (numsubsounds)
	{
		sound->getSubSound(0, &sound_to_play);
		ERRCHECK(result);
	}
	else
	{
		sound_to_play = sound;
	}
	result = system->playSound(sound_to_play, 0, false, &channel);
	ERRCHECK(result);

	result = channel->getVolume(&volume);
	ERRCHECK(result);

	result = channel->getChannelGroup(&channelGroup);
	ERRCHECK(result);

}

void MyFMODLib::FMODWrapper::Load3DSound(const char* nameOfTheSound)
{
	result = system->createSound(Common_MediaPath(nameOfTheSound), FMOD_LOOP_NORMAL | FMOD_3D, 0, &sound);
	ERRCHECK(result);
	result = sound->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR);
	ERRCHECK(result);

	FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };

	result = sound->getNumSubSounds(&numsubsounds);
	ERRCHECK(result);

	if (numsubsounds)
	{
		sound->getSubSound(0, &sound_to_play);
		ERRCHECK(result);
	}
	else
	{
		sound_to_play = sound;
	}
	result = system->playSound(sound_to_play, 0, false, &channel);
	ERRCHECK(result);
	result = channel->set3DAttributes(&pos, &vel);
	ERRCHECK(result);
	t = 0;
	lastpos = { 0.0f, 0.0f, 0.0f };


}

void MyFMODLib::FMODWrapper::MakeSoundPanLeftToRight()
{
	// ==========================================================================================
				// UPDATE THE LISTENER
				// ==========================================================================================
	{
		FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
		FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };
		FMOD_VECTOR vel;


		listenerpos.x = (float)sin(t * 0.05f) * 24.0f * DISTANCEFACTOR; // left right pingpong


		// vel = how far we moved last FRAME (m/f), then time compensate it to SECONDS (m/s).
		vel.x = (listenerpos.x - lastpos.x) * (1000 / INTERFACE_UPDATETIME);
		vel.y = (listenerpos.y - lastpos.y) * (1000 / INTERFACE_UPDATETIME);
		vel.z = (listenerpos.z - lastpos.z) * (1000 / INTERFACE_UPDATETIME);

		// store pos for next time
		lastpos = listenerpos;

		result = system->set3DListenerAttributes(0, &listenerpos, &vel, &forward, &up);
		ERRCHECK(result);

		t += (30 * (1.0f / (float)INTERFACE_UPDATETIME));    // t is just a time value .. it increments in 30m/s steps in this example
	}

	result = system->update();
	ERRCHECK(result);
}

void MyFMODLib::FMODWrapper::RaiseVolume()
{
	volume++;
	result = channelGroup->setVolume(volume);
	ERRCHECK(result);
}

void MyFMODLib::FMODWrapper::LowerVolume()
{
	volume--;
	if (volume <= 0)
		volume = 0;
	result = channelGroup->setVolume(volume);
	ERRCHECK(result);
}

void MyFMODLib::FMODWrapper::AddNewSoundAfterLoad(const char* nameOfTheSound)
{
	result = system->createStream(Common_MediaPath(nameOfTheSound), FMOD_LOOP_NORMAL, 0, &sound);
	ERRCHECK(result);

	result = sound->getNumSubSounds(&numsubsounds);
	ERRCHECK(result);

	if (numsubsounds)
	{
		sound->getSubSound(0, &sound_to_play);
		ERRCHECK(result);
	}
	else
	{
		sound_to_play = sound;
	}
	result = system->playSound(sound_to_play, 0, false, &channel);
	ERRCHECK(result);
}

void MyFMODLib::FMODWrapper::PlayChannels()
{
	result = channelGroup->setPaused(true);
	ERRCHECK(result);
}

void MyFMODLib::FMODWrapper::PauseChannels()
{
	result = channelGroup->setPaused(false);
	ERRCHECK(result);
}

void MyFMODLib::FMODWrapper::GetGroupChannelPaused(bool* paused)
{
	result = channelGroup->getPaused(paused);
	ERRCHECK(result);
}

void MyFMODLib::FMODWrapper::StopChannels()
{
	result = channelGroup->stop();
	ERRCHECK(result);
}

void MyFMODLib::FMODWrapper::Stop()
{
	channel->stop();

}

void MyFMODLib::FMODWrapper::CloseFMOD()
{
	result = system->close();
	ERRCHECK(result);
	result = system->release();
	ERRCHECK(result);

	Common_Close();
}
