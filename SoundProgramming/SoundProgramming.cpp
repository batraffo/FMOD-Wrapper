
#include "fmod.h"
#include "fmod.hpp"
#include "fmod_errors.h"

#include "common.h"
#include <iostream>


int FMOD_Main()
{
	FMOD_RESULT result;
	FMOD::System* system = NULL;

	
	Common_Init(0);

	result = FMOD::System_Create(&system);      // Create the main system object.
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	result = system->init(32, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	/*
	   Main loop.
   */
	do
	{
		Common_Update();

		Common_Draw("Press %s to quit", Common_BtnStr(BTN_QUIT));
	} while (!Common_BtnPress(BTN_QUIT));

	return 0;
}
