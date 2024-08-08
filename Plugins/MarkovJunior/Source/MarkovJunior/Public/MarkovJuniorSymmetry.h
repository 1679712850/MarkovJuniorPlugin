// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MarkovJuniorSymmetry.generated.h"

UENUM()
enum class ESymmetryType : uint8
{
	None,
	// X reflected
	X,
	// Y reflected
	Y,
	// Z reflected
	Z,
	// X reflected and Y reflected and X ref then Y ref
	XY_Combine,
	// all rotation types
	AllRotated,
	// all symmetry
	All
};



/**
 * 
 */
USTRUCT(BlueprintType)
struct FMarkovJuniorSymmetry
{
	GENERATED_BODY()

	FMarkovJuniorSymmetry();
	
	FMarkovJuniorSymmetry(bool bThreeDimension,ESymmetryType SymmetryType = ESymmetryType::None);

	static TMap<ESymmetryType,TArray<bool>> SquareSubgroups;
	static TMap<ESymmetryType,TArray<bool>> CubeSubgroups;

	FMarkovJuniorSymmetry& operator|= (const FMarkovJuniorSymmetry& Other)
	{
		for (int Index = 0; Index < Symmetries.Num(); ++Index)
		{
			Symmetries[Index] = Symmetries[Index] || Other.Symmetries[Index];
		}
		return *this;
	}

public:

	template<typename T>
	void GetSymmetries(const T& Element,TFunction<T(const T&)> ZRotation,TFunction<T(const T&)> Reflection,
			TFunction<T(const T&)> YRotation,TArray<T>& Results)
	{
		if (bThreeDimension)
		{
			GetCubeSymmetries(Element,ZRotation,YRotation,Reflection,Results);
		}
		else
		{
			GetSquareSymmetries(Element,ZRotation,Reflection,Results);
		}
	}
	
private:
	template<typename T>
	void GetSquareSymmetries(const T& Element,TFunction<T(const T&)> Rotation,
			TFunction<T(const T&)> Reflection,TArray<T>& Results)
	{
		Results.Empty();
		Results.Emplace(Element);
		Results.Emplace(Reflection(Element));
		Results.Emplace(Rotation(Element));
		Results.Emplace(Reflection(Results[2]));
		Results.Emplace(Rotation(Results[2]));
		Results.Emplace(Reflection(Results[4]));
		Results.Emplace(Rotation(Results[4]));
		Results.Emplace(Reflection(Results[6]));

		int32 DeleteCount = 0;
		for (int Index = 0; Index < Symmetries.Num(); ++Index)
		{
			if (!Symmetries[Index])
			{
				Results.Swap(Index - DeleteCount, Symmetries.Num() - 1 - DeleteCount);
				DeleteCount++;
			}
		}
		Results.SetNum(Symmetries.Num() - DeleteCount);
	}

	template<typename T>
	void GetCubeSymmetries(const T& Element,TFunction<T(const T&)> ZRotation,
		TFunction<T(const T&)> YRotation,TFunction<T(const T&)> Reflection,TArray<T>& Results)
	{
		TArray<T> SymmetryElements;

		

	}

private:
	bool bThreeDimension = false;
	
	TArray<bool> Symmetries;
	
};
 