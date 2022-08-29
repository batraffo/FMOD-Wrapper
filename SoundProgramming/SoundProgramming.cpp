
#include "fmod.h"
#include "fmod.hpp"
#include "fmod_errors.h"

#include <filesystem>
#include <string>
#include <iostream>
#include "common.h"
#include "MenuState.h"


int FMOD_Main()
{
	FMOD_RESULT result;
	FMOD::System* system = NULL;
	FMOD::Sound* sound, * sound_to_play;
	FMOD::Channel* channel = 0;
	int               numsubsounds;
	MenuState		state = MenuState::MainMenu;
	
	Common_Init(0);

	result = FMOD::System_Create(&system);      // Create the main system object.
	ERRCHECK(result);
	result = system->init(32, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
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

		if (state == MenuState::MainMenu) {



			Common_Draw("Press %s to quit", Common_BtnStr(BTN_QUIT));
			for(int y = 0; y<i; y++)
				Common_Draw("Press %s to open %s", Common_BtnStr((Common_Button) y), firstNineFilesInMediaDirectory[y].c_str());
		}
		else if (false) {
			result = system->createStream(Common_MediaPath("swish.wav"), FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);
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

	} while (!Common_BtnPress(BTN_QUIT));

	return 0;
}
