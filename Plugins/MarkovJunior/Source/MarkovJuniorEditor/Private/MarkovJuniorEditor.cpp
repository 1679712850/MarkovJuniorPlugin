// Copyright Epic Games, Inc. All Rights Reserved.

#include "MarkovJuniorEditor.h"

#include "MarkovJuniorCommands.h"
#include "MarkovJuniorEditorLog.h"
#include "MarkovJuniorModel.h"
#include "MatrixDetailsCustomization.h"
#include "ToolMenus.h"
#include "Container/Matrix2D.h"

static const FName MarkovJuniorEditorTabName("MarkovJuniorEditor");
#define LOCTEXT_NAMESPACE "FMarkovJuniorEditorModule"

void FMarkovJuniorEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FMarkovJuniorStyle::Initialize();
	FMarkovJuniorStyle::ReloadTextures();

	FMarkovJuniorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMarkovJuniorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FMarkovJuniorEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	// register customization
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout(FInOutMatrix2::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FInOutMatrixDetailsCustomization::MakeInstance));

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMarkovJuniorEditorModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MarkovJuniorEditorTabName, FOnSpawnTab::CreateRaw(this, &FMarkovJuniorEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FMarkovJuniorTabTitle", "MarkovJunior"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);


	UAssetEditorSubsystem* const AssetEditorSubsystem = GEditor ? GEditor->GetEditorSubsystem<UAssetEditorSubsystem>() : nullptr;
	if (AssetEditorSubsystem)
	{
		OnAssetEditorOpenedHandle = AssetEditorSubsystem->OnAssetEditorOpened().AddRaw(this, &FMarkovJuniorEditorModule::OnModelEditorOpened);
	}
}

void FMarkovJuniorEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMarkovJuniorStyle::Shutdown();

	FMarkovJuniorCommands::Unregister();

	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomPropertyTypeLayout(FInOutMatrix2::StaticStruct()->GetFName());
	}
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MarkovJuniorEditorTabName);

	UAssetEditorSubsystem* const AssetEditorSubsystem = GEditor ? GEditor->GetEditorSubsystem<UAssetEditorSubsystem>() : nullptr;
	if (AssetEditorSubsystem)
	{
		AssetEditorSubsystem->OnAssetEditorOpened().Remove(OnAssetEditorOpenedHandle);
	}
}

void FMarkovJuniorEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(MarkovJuniorEditorTabName);
}

void FMarkovJuniorEditorModule::RegisterMenus()
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
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolbar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMarkovJuniorCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

TSharedRef<SDockTab> FMarkovJuniorEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
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

void FMarkovJuniorEditorModule::OnModelEditorOpened(UObject* Object)
{
	if (auto Model = Cast<UMarkovJuniorModel>(Object))
	{
		Model->UpdateRuleOptions();
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMarkovJuniorEditorModule, MarkovJuniorEditor);
DEFINE_LOG_CATEGORY(LogMarkovJuniorEditor);
