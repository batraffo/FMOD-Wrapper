
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
	MenuState		state = MenuState::MainMenu;
	std::string		nameOfTheSound;
	bool isStream = 0;
	bool playing = 1;
	
	
	MyFMODLib::FMODWrapper wrapper;

	Common_Init(&extradriverdata);

	wrapper.InitFMOD(extradriverdata);

	
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
				playing = 1;
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
					wrapper.Load3DSound(nameOfTheSound.c_str());

					state = MenuState::PlayingPan;
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

					wrapper.LoadOneShot(isStream, nameOfTheSound.c_str());

					state = MenuState::Playing;

					break;
				}
				//loop
				if (Common_BtnPress(BTN_ACTION2)) {
					wrapper.LoadLoop(isStream, nameOfTheSound.c_str());
					state = MenuState::Playing;

					break;
				}

				Common_Draw("Press %s to quit", Common_BtnStr(BTN_QUIT));
				Common_Draw("Press %s to play %s one-shot", Common_BtnStr(BTN_ACTION1), nameOfTheSound.c_str());
				Common_Draw("Press %s to play %s in loop", Common_BtnStr(BTN_ACTION2), nameOfTheSound.c_str());
				break;

			case MenuState::Playing:

				if (Common_BtnPress(BTN_ACTION1)) {
					if (!playing) {
						wrapper.PauseChannels();
					}
					else {
						wrapper.PlayChannels();
					}
					playing = !playing;
				}

				if (Common_BtnPress(BTN_ACTION2)) {
					wrapper.StopChannels();
					state = MenuState::MainMenu;
					break;
				}

				if (Common_BtnPress(BTN_ACTION3)) {
					state = MenuState::SelectNewSoundToLoad;
					break;
				}

				if (Common_BtnPress(BTN_UP)) {
					wrapper.RaiseVolume();
				}

				if (Common_BtnPress(BTN_DOWN)) {
					wrapper.LowerVolume();
				}

				Common_Draw("Press %s to quit", Common_BtnStr(BTN_QUIT));
				if(playing)
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
						wrapper.AddNewSoundAfterLoad(nameOfTheSound.c_str());
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
					wrapper.Stop();
					state = MenuState::MainMenu;
					break;
				}

				wrapper.MakeSoundPanLeftToRight();

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

	wrapper.CloseFMOD();


	return 0;
}
