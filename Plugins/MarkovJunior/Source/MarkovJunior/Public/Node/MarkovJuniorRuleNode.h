// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MarkovJuniorNode.h"
#include "MarkovJuniorRule.h"
#include "UObject/Object.h"
#include "MarkovJuniorRuleNode.generated.h"


/**
 * 
 */
UCLASS(DisplayName="RuleNode")
class MARKOVJUNIOR_API UMarkovJuniorRuleNode : public UMarkovJuniorNode
{
	GENERATED_BODY()
public:
#if WITH_EDITOR
	virtual void PostModelEdited(const TArray<FMarkovJuniorValue>& Values);
#endif
protected:
	// UMarkovJuniorNode implementation
	virtual bool Initialize_Implementation(UMarkovJuniorInterpreter* InInterpreter,UMarkovJuniorGrid* InGrid,const FMarkovJuniorSymmetry& ParentSymmetry) override;
	virtual void Reset_Implementation() override;
	virtual bool Go_Implementation() override;
	// ~UMarkovJuniorNode implementation

	void Add(int32 RuleIndex, const FIntVector& Position);



protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FMarkovJuniorRule> Rules;
	
	/**
	 * all possible matches for the rules
	 * the pair if the position of the match and the index of the rule 
	 */
	TArray<TPair<FIntVector,int32>> AllMatches;

	/**
	 * the number of matches
	 */
	int32 MatchCount = 0;
	
	/**
	 * the turn that has been matched
	 */
	int32 LastMatchedTurn = -1;
	
	/**
	 * the masks of each rule with each state
	 */
	TArray<TArray<bool>> MatchMask;
	
	/**
	 * the potential value of each MarkovJuniorValue with each state
	 */
	TArray<TArray<int32>> Potentials;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FMarkovJuniorField> Fields;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FMarkovJuniorObservation> Observations;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	double Temperature = 0.0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bSearch = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Limit = -1;

	/**
	 * has the future been computed?
	 */
	bool bHasFutureComputed = false;
	/**
	 * the future of the current state
	 */
	TArray<int32> Future;
	/**
	 * todo:? what is this?
	 */
	TArray<TArray<uint8>> Trajectory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	double DepthCoefficient = 0.5;
	/**
	 * the mask for each rule
	 */
	TBitArray<> LastMask;
	
	/**
	 * current number of steps taken
	 */
	int32 Counter = 0;
	/**
	 * max number of steps to take before stopping
	 */
	int32 Steps = 0;
};
