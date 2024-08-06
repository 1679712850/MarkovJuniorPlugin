// Fill out your copyright notice in the Description page of Project Settings.


#include "MarkovJuniorModel.h"

#include "MarkovJuniorLog.h"

UMarkovJuniorModel::UMarkovJuniorModel()
{
    // 创建一个UMarkovJuniorBranchNode对象的默认实例
    RootNode = CreateDefaultSubobject<UMarkovJuniorBranchNode_Markov>(TEXT("Root"));

#if WITH_EDITOR
    UAssetEditorSubsystem* const AssetEditorSubsystem = GEditor ? GEditor->GetEditorSubsystem<UAssetEditorSubsystem>() : nullptr;
    if (AssetEditorSubsystem)
    {
        OnAssetEditorOpenedHandle = AssetEditorSubsystem->OnAssetEditorOpened().AddUObject(this, &UMarkovJuniorModel::OnModelEditorOpened);
    }
#endif
}

UMarkovJuniorModel::~UMarkovJuniorModel()
{
#if WITH_EDITOR
    UAssetEditorSubsystem* const AssetEditorSubsystem = GEditor ? GEditor->GetEditorSubsystem<UAssetEditorSubsystem>() : nullptr;
    if (AssetEditorSubsystem)
    {
        AssetEditorSubsystem->OnAssetEditorOpened().Remove(OnAssetEditorOpenedHandle);
    }
#endif
}

#if WITH_EDITOR

void UMarkovJuniorModel::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    UpdateRuleOptions();
    UE_LOG(LogMarkovJunior, Warning, TEXT("UMarkovJuniorModel::PostEditChangeProperty"));
}

void UMarkovJuniorModel::UpdateRuleOptions()
{
    if (RootNode)
    {
        RootNode->PostModelEdited(Values);
    }
}

void UMarkovJuniorModel::OnModelEditorOpened(UObject* Object)
{
    if (Object == this)
    {
        UpdateRuleOptions();
    }
}
#endif

