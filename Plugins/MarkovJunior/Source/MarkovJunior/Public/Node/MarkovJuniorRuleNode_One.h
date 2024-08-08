// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MarkovJuniorRuleNode.h"
#include "UObject/Object.h"
#include "MarkovJuniorRuleNode_One.generated.h"

/**
 * 
 */
UCLASS(DisplayName="OneNode")
class MARKOVJUNIOR_API UMarkovJuniorRuleNode_One : public UMarkovJuniorRuleNode
{
	GENERATED_BODY()

protected:
	// UMarkovJuniorNode implementation
	virtual bool Go_Implementation() override;
	virtual void Reset_Implementation() override;
	// ~UMarkovJuniorNode implementation

private:
	void RandomMatch(TPair<FIntVector, int32>& ResultMatch);
	void Apply(const FMarkovJuniorRule& Rule, const FIntVector& Position);
	
};
