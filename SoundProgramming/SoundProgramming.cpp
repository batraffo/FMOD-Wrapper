
#include "fmod.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#include "FMODWrapper.h"
#include "common.h"


#include <filesystem>
#include <string>
#include <iostream>
#include "MenuState.h"

int FMOD_Main()
{
	void* extradriverdata = 0;
	FMOD_RESULT result;
	FMOD::System* system = NULL;
	FMOD::Sound* sound, * sound_to_play;
	FMOD::Channel* channel = 0;
	FMOD::ChannelGroup* channelGroup = 0;
	int               numsubsounds;
	MenuState		state = MenuState::MainMenu;
	std::string		nameOfTheSound;
	bool isStream = 0;
	bool paused = 0;
	float volume;
	FMOD_VECTOR      listenerpos = { 0.0f, 0.0f, -1.0f * DISTANCEFACTOR };
	float t = 0;
	FMOD_VECTOR lastpos = { 0.0f, 0.0f, 0.0f };
	
	
	MyFMODLib::FMODWrapper wrapper;

	Common_Init(&extradriverdata);

	//wrapper.InitFMOD(extradriverdata);

	result = FMOD::System_Create(&system);      // Create the main system object.
	ERRCHECK(result);
	result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);    // Initialize FMOD.
	ERRCHECK(result);
	result = system->set3DSettings(1.0, DISTANCEFACTOR, 1.0f);
	ERRCHECK(result);
	std::string firstNineFilesInMediaDirectory[9];
	std::string dirPath = "../Media/";

	int i = 0;
	for(const auto& entry : std::filesystem::directory_iterator(dirPath)){
		firstNineFilesInMediaDirectory[i] = entry.path().generic_string().substr(entry.path().generic_string().find_last_of("/\\") + 1);
		std::cout << firstNineFilesInMediaDirectory[i] << "\n";

		//Currently we have max 9 track, we'll expand that in the future
		i++;
		if (i >= 9)
			break;
	}

	/*
	   Main loop.
   */
	do
	{
		Common_Update();

		switch (state) {

			case MenuState::MainMenu:
				paused = 0;
				for (int y = 0; y < i; y++) {
					if (Common_BtnPress((Common_Button)y)) {
						nameOfTheSound = firstNineFilesInMediaDirectory[y];
						state = MenuState::SelectLoad;
						break;
					}
				}

				Common_Draw("Press %s to quit", Common_BtnStr(BTN_QUIT));
				for (int y = 0; y < i; y++)
					Common_Draw("Press %s to open %s", Common_BtnStr((Common_Button)y), firstNineFilesInMediaDirectory[y].c_str());
				break;


			case MenuState::SelectLoad:

				//use static
				if (Common_BtnPress(BTN_ACTION1)) {
					isStream = 0;
					state = MenuState::SelectLoop;
					break;
				}
				//use stream
				if (Common_BtnPress(BTN_ACTION2)) {
					isStream = 1;
					state = MenuState::SelectLoop;
					break;
				}

				//pan left and right
				if (Common_BtnPress(BTN_ACTION3)) {
					
					ERRCHECK(result);
					result = system->createSound(Common_MediaPath(nameOfTheSound.c_str()), FMOD_LOOP_NORMAL | FMOD_3D, 0, &sound);
					ERRCHECK(result);
					result = sound->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR);
					ERRCHECK(result);
					state = MenuState::PlayingPan;

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

					break;
				}

				Common_Draw("Press %s to quit", Common_BtnStr(BTN_QUIT));
				Common_Draw("Plain Sound:\n\n");
				Common_Draw("Press %s to load %s statically", Common_BtnStr(BTN_ACTION1), nameOfTheSound.c_str());
				Common_Draw("Press %s to load %s in streaming", Common_BtnStr(BTN_ACTION2),nameOfTheSound.c_str());
				Common_Draw("\nSpecial Effects:\n\n");
				Common_Draw("Press %s to load %s panning right and left", Common_BtnStr(BTN_ACTION3), nameOfTheSound.c_str());

				break;

			case MenuState::SelectLoop:

				//one-shot
				if (Common_BtnPress(BTN_ACTION1)) {
					if (isStream) {
						result = system->createStream(Common_MediaPath(nameOfTheSound.c_str()), FMOD_LOOP_OFF, 0, &sound);
						ERRCHECK(result);
						
					}
					else {
						result = system->createSound(Common_MediaPath(nameOfTheSound.c_str()), FMOD_LOOP_OFF, 0, &sound);
						ERRCHECK(result);
					}


					state = MenuState::Playing;

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

					break;
				}
				//loop
				if (Common_BtnPress(BTN_ACTION2)) {
					if (isStream) {
						result = system->createStream(Common_MediaPath(nameOfTheSound.c_str()), FMOD_LOOP_NORMAL, 0, &sound);
						ERRCHECK(result);
					}
					else {
						result = system->createSound(Common_MediaPath(nameOfTheSound.c_str()), FMOD_LOOP_NORMAL, 0, &sound);
						ERRCHECK(result);
					}

					state = MenuState::Playing;

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

					break;
				}

				Common_Draw("Press %s to quit", Common_BtnStr(BTN_QUIT));
				Common_Draw("Press %s to play %s one-shot", Common_BtnStr(BTN_ACTION1), nameOfTheSound.c_str());
				Common_Draw("Press %s to play %s in loop", Common_BtnStr(BTN_ACTION2), nameOfTheSound.c_str());
				break;

			case MenuState::Playing:

				if (Common_BtnPress(BTN_ACTION1)) {
					result = channelGroup->getPaused(&paused);
					ERRCHECK(result);
					paused = !paused;
					result = channelGroup->setPaused(paused);
					ERRCHECK(result);
				}

				if (Common_BtnPress(BTN_ACTION2)) {
					result = channelGroup->stop();
					ERRCHECK(result);
					state = MenuState::MainMenu;
					break;
				}

				if (Common_BtnPress(BTN_ACTION3)) {
					state = MenuState::SelectNewSoundToLoad;
					break;
				}

				if (Common_BtnPress(BTN_UP)) {
					volume++;
					result = channelGroup->setVolume(volume);
					ERRCHECK(result);
				}

				if (Common_BtnPress(BTN_DOWN)) {
					volume--;
					if (volume <= 0)
						volume = 0;
					result = channelGroup->setVolume(volume);
					ERRCHECK(result);
				}

				Common_Draw("Press %s to quit", Common_BtnStr(BTN_QUIT));
				if(!paused)
					Common_Draw("Press %s to pause", Common_BtnStr(BTN_ACTION1));
				else
					Common_Draw("Press %s to resume", Common_BtnStr(BTN_ACTION1));
				Common_Draw("Press %s to stop go to the main menu", Common_BtnStr(BTN_ACTION2));
				Common_Draw("Press %s to to add a new audio channel", Common_BtnStr(BTN_ACTION3));
				Common_Draw("Press %s to turn up the volume", Common_BtnStr(BTN_UP));
				Common_Draw("Press %s to turn down the volume", Common_BtnStr(BTN_DOWN));
				break;

			case MenuState::SelectNewSoundToLoad:

				for (int y = 0; y < i; y++) {
					if (Common_BtnPress((Common_Button)y)) {
						nameOfTheSound = firstNineFilesInMediaDirectory[y];
						state = MenuState::Playing;
						result = system->createStream(Common_MediaPath(nameOfTheSound.c_str()), FMOD_LOOP_NORMAL, 0, &sound);
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
						break;
					}
				}

				Common_Draw("Press %s to quit", Common_BtnStr(BTN_QUIT));
				for (int y = 0; y < i; y++)
					Common_Draw("Press %s to open %s", Common_BtnStr((Common_Button)y), firstNineFilesInMediaDirectory[y].c_str());
				Common_Draw("\nThe new sound will be streamed in loop");

				break;

			case MenuState::PlayingPan:

				if (Common_BtnPress(BTN_ACTION1)) {
					result = channel->stop();
					state = MenuState::MainMenu;
					break;
				}

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

				Common_Draw("Press %s to quit", Common_BtnStr(BTN_QUIT));
				Common_Draw("Press %s to stop the sound and go to the main menu", Common_BtnStr(BTN_ACTION1));
				Common_Sleep(INTERFACE_UPDATETIME - 1);
				break;

			default:
				Common_Draw("You are not supposed to be here");
				Common_Draw("Press %s to quit", Common_BtnStr(BTN_QUIT));
				break;
		}
		

	} while (!Common_BtnPress(BTN_QUIT));

	result = system->close();
	ERRCHECK(result);
	result = system->release();
	ERRCHECK(result);

	Common_Close();


	return 0;
}
