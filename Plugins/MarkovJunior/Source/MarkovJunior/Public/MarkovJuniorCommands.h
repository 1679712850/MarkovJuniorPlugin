// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MarkovJuniorStyle.h"

class FMarkovJuniorCommands : public TCommands<FMarkovJuniorCommands>
{
public:

	FMarkovJuniorCommands()
		: TCommands<FMarkovJuniorCommands>(TEXT("MarkovJunior"), NSLOCTEXT("Contexts", "MarkovJunior", "MarkovJunior Plugin"), NAME_None, FMarkovJuniorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};