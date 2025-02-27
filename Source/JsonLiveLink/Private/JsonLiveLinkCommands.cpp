// Copyright Epic Games, Inc. All Rights Reserved.

#include "JsonLiveLinkCommands.h"

#define LOCTEXT_NAMESPACE "FJsonLiveLinkModule"

void FJsonLiveLinkCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "JsonLiveLink", "Bring up JsonLiveLink window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
