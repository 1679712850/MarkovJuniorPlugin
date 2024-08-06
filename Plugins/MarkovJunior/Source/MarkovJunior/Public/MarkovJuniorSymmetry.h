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
struct FMarkovJuniorSymmetry
{
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
	TSet<T> GetSymmetries(const T& Element,TFunction<T(T)> YRotation,TFunction<T(T)> Reflection,
			TFunction<T(T)> ZRotation = nullptr)
	{
		if (bThreeDimension)
		{
			return GetCubeSymmetries(Element,ZRotation,YRotation,Reflection);
		}
		return GetSquareSymmetries(Element,YRotation,Reflection);
	}
	
private:
	template<typename T>
	TSet<T> GetSquareSymmetries(const T& Element,TFunction<T(T)> Rotation,TFunction<T(T)> Reflection)
	{
		TArray<T> SymmetryElements;
		SymmetryElements.Emplace(Element);
		SymmetryElements.Emplace(Reflection(Element));
		SymmetryElements.Emplace(Rotation(Element));
		SymmetryElements.Emplace(Reflection(SymmetryElements[2]));
		SymmetryElements.Emplace(Rotation(SymmetryElements[2]));
		SymmetryElements.Emplace(Reflection(SymmetryElements[4]));
		SymmetryElements.Emplace(Rotation(SymmetryElements[4]));
		SymmetryElements.Emplace(Reflection(SymmetryElements[6]));
		
		TSet<T> Result;
		for (int Index = 0; Index < Symmetries.Num(); ++Index)
		{
			if (Symmetries[Index])
			{
				Result.Emplace(SymmetryElements[Index]);
			}
		}
		return Result;
	}

	template<typename T>
	TSet<T> GetCubeSymmetries(const T& Element,TFunction<T(T)> ZRotation,
		TFunction<T(T)> YRotation,TFunction<T(T)> Reflection)
	{
		TArray<T> SymmetryElements;

		
		TSet<T> Result;

		return Result;
	}

private:
	bool bThreeDimension = false;
	
	TArray<bool> Symmetries;
	
};
 