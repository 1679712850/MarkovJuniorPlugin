// Copyright Epic Games, Inc. All Rights Reserved.

#include "MarkovJunior.h"
#include "MarkovJuniorStyle.h"
#include "MarkovJuniorCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName MarkovJuniorTabName("MarkovJunior");

#define LOCTEXT_NAMESPACE "FMarkovJuniorModule"

void FMarkovJuniorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMarkovJuniorStyle::Initialize();
	FMarkovJuniorStyle::ReloadTextures();

	FMarkovJuniorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMarkovJuniorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FMarkovJuniorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMarkovJuniorModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MarkovJuniorTabName, FOnSpawnTab::CreateRaw(this, &FMarkovJuniorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FMarkovJuniorTabTitle", "MarkovJunior"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMarkovJuniorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMarkovJuniorStyle::Shutdown();

	FMarkovJuniorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MarkovJuniorTabName);
}

TSharedRef<SDockTab> FMarkovJuniorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FMarkovJuniorModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("MarkovJunior.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FMarkovJuniorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(MarkovJuniorTabName);
}

void FMarkovJuniorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMarkovJuniorCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMarkovJuniorCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMarkovJuniorModule, MarkovJunior)