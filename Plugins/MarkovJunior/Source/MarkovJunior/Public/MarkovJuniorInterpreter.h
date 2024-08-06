// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Node/MarkovJuniorBranchNode.h"
#include "UObject/Object.h"
#include "MarkovJuniorInterpreter.generated.h"

class UMarkovJuniorBranchNode;
/**
 * 
 */
UCLASS(BlueprintType,Blueprintable,ClassGroup=(MarkovJunior))
class MARKOVJUNIOR_API UMarkovJuniorInterpreter : public UObject
{
	GENERATED_BODY()
public:
	void Initialize(TObjectPtr<UMarkovJuniorModel> Model);

	void Run(int32 Seed);

	void SetCurrentNode(TObjectPtr<UMarkovJuniorBranchNode> BranchNode);

	void GetStates(TArray<int32>& OutStates) const
	{
		Grid->GetStates(OutStates);
	}
public:
	
	TArray<FIntVector> Changes;
	
	TArray<int32> Firsts;

	int32 Counter;

	FRandomStream RandomStream;
private:
	TObjectPtr<UMarkovJuniorBranchNode> RootNode;
	TObjectPtr<UMarkovJuniorBranchNode> CurrentNode;

	TObjectPtr<UMarkovJuniorGrid> Grid;

	

	bool bOrigin;

	int32 Steps;

private:
};
