// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "JsonLiveLinkStyle.h"

class FJsonLiveLinkCommands : public TCommands<FJsonLiveLinkCommands>
{
public:

	FJsonLiveLinkCommands()
		: TCommands<FJsonLiveLinkCommands>(TEXT("JsonLiveLink"), NSLOCTEXT("Contexts", "JsonLiveLink", "JsonLiveLink Plugin"), NAME_None, FJsonLiveLinkStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};