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
