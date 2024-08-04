// Copyright Epic Games, Inc. All Rights Reserved.

#include "MarkovJuniorEditor.h"
#include "MarkovJuniorEditorLog.h"
#include "MatrixDetailsCustomization.h"
#include "ToolMenus.h"
#include "Container/Matrix2D.h"

static const FName MarkovJuniorEditorTabName("MarkovJuniorEditor");
#define LOCTEXT_NAMESPACE "FMarkovJuniorEditorModule"

void FMarkovJuniorEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	PluginCommands = MakeShareable(new FUICommandList);

	// register customization
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout(FInOutMatrix2::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FInOutMatrixDetailsCustomization::MakeInstance));
}

void FMarkovJuniorEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomPropertyTypeLayout(FInOutMatrix2::StaticStruct()->GetFName());
	}
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MarkovJuniorEditorTabName);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMarkovJuniorEditorModule, MarkovJuniorEditor);
DEFINE_LOG_CATEGORY(LogMarkovJuniorEditor);
