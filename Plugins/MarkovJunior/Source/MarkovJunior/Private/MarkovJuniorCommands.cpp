// Copyright Epic Games, Inc. All Rights Reserved.

#include "MarkovJuniorCommands.h"

#define LOCTEXT_NAMESPACE "FMarkovJuniorModule"

void FMarkovJuniorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "MarkovJunior", "Bring up MarkovJunior window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
