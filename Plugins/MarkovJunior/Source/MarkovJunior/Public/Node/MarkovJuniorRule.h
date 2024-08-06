// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MarkovJuniorGrid.h"
#include "MarkovJuniorTypes.h"
#include "Container/Matrix2D.h"
#include "UObject/Object.h"
#include "MarkovJuniorRule.generated.h"

USTRUCT(BlueprintType)
struct FMarkovJuniorRule
{
	GENERATED_BODY()

	void CalculateShifts(int32 ValueNum);
	void Construct(TObjectPtr<UMarkovJuniorGrid> Grid);

	/**
	 * the possibility of chosing this rule
	 */
	UPROPERTY(EditAnywhere)
	double Possibility = 1.0;

	FIntVector Size;
	/**
	 * the wave of the value in the input matrix
	 */
	TArray<int32> InputValueWaves;

	/**
	 * the index of the value in the input matrix
	 * -1 represents any value can be matched
	 */
	TArray<int32> InputValueTrailingZeroCounts;

	/**
	 * the index of the value in the output matrix
	 * -1 represents any value can be matched
	 */
	TArray<int32> OutputValueIndices;

	TArray<TArray<FIntVector>> InputShifts;
	TArray<TArray<FIntVector>> OutputShifts;


	/**
	 * Whether to match from the origin
	 */
	bool bOriginal = false;

	FMarkovJuniorRule ZRotated() const;
	FMarkovJuniorRule YRotated() const;
	FMarkovJuniorRule Reflected() const;

	bool operator==(const FMarkovJuniorRule& OtherRule) const;

public:
#if WITH_EDITOR
	virtual void PostModelEdited(const TArray<FMarkovJuniorValue>& Values);
#endif
private:
	UPROPERTY(EditAnywhere)
	FInOutMatrix2 InOutMatrix;
};

USTRUCT(BlueprintType)
struct FMarkovJuniorField
{
	GENERATED_BODY()
	
	FMarkovJuniorField() = default;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MarkovJunior|Field")
	bool bRecompute = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MarkovJunior|Field")
	bool bEssential = false;

	/**
	 * the element representing the ValueIndex
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MarkovJunior|Field")
	TArray<int32> ValuesBasedOn;
	/**
	 * todo : what the meaning of the name Zero?
	 * the element representing the ValueIndex
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MarkovJunior|Field")
	TArray<int32> ValuesFromOrTo;
	/**
	 * is used for To
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MarkovJunior|Field")
	bool bInversed = false;


	int32 SubstrateWave = 0;
	int32 ZeroWave = 0;

public:
	void Initialize(TObjectPtr<UMarkovJuniorGrid> Grid);
	
	// bool Compute(TArray<int32>& Potential,TObjectPtr<UMarkovJuniorGrid> Grid);
};

USTRUCT(BlueprintType)
struct FMarkovJuniorObservation
{
	GENERATED_BODY()
	// todo: add properties
};