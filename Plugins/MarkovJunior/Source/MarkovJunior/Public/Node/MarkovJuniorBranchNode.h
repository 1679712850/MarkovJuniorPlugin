// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MarkovJuniorNode.h"
#include "UObject/Object.h"
#include "MarkovJuniorBranchNode.generated.h"

/**
 * a branch node that contains multiple child nodes and execute them in order
 */
UCLASS(Abstract)
class MARKOVJUNIOR_API UMarkovJuniorBranchNode : public UMarkovJuniorNode
{
	GENERATED_BODY()
public:
#if WITH_EDITOR
	virtual void PostModelEdited(const TArray<FMarkovJuniorValue>& Values);
#endif
protected:
	// UMarkovJuniorNode implementations
	virtual bool Initialize_Implementation(UMarkovJuniorInterpreter* InInterpreter,UMarkovJuniorGrid* InGrid,const FMarkovJuniorSymmetry& ParentSymmetry) override;
	virtual void Reset_Implementation() override;
	virtual bool Go_Implementation() override;
	// ~UMarkovJuniorNode implementations


protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMarkovJuniorBranchNode> ParentNode;
	
	UPROPERTY(EditAnywhere,Instanced)
	TArray<TObjectPtr<UMarkovJuniorNode>> ChildNodes;
	
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentChildIndex = 0;
};

/**
 *  Execute each child node sequentially once
 */
UCLASS(DisplayName="SequenceNode",BlueprintType,Blueprintable,ClassGroup=(MarkovJunior))
class MARKOVJUNIOR_API UMarkovJuniorBranchNode_Sequence : public UMarkovJuniorBranchNode
{
	GENERATED_BODY()
	
public:
	
};

/**
 *  Start with the first child every execution
 */
UCLASS(DisplayName="MarkovNode")
class MARKOVJUNIOR_API UMarkovJuniorBranchNode_Markov : public UMarkovJuniorBranchNode
{
	GENERATED_BODY()
protected:
	virtual bool Go_Implementation() override;
};
