// Fill out your copyright notice in the Description page of Project Settings.


#include "MarkovJuniorModel.h"

#include "MarkovJuniorLog.h"

UMarkovJuniorModel::UMarkovJuniorModel()
{
    // 创建一个UMarkovJuniorBranchNode对象的默认实例
    RootNode = CreateDefaultSubobject<UMarkovJuniorBranchNode_Markov>(TEXT("Root"));
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
#endif

