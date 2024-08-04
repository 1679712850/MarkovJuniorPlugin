﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Matrix2D.generated.h"

/**
 * 
 */
USTRUCT()
struct MARKOVJUNIOR_API FMatrix2
{
	GENERATED_BODY()
	
	FMatrix2();
	UPROPERTY()
	FIntVector2 Size;
	UPROPERTY(EditAnywhere,meta=(ClampMin = -1))
	TArray<int32> Data;

	/**
	 * Add or remove elements based on the new size
	 * @param NewSize new size
	 */
	void SetSize(FIntVector2 NewSize);

	FORCEINLINE int32 GetRowNum() const
	{
		return Size.X;
	}
	FORCEINLINE int32 GetColumnNum() const
	{
		return Size.Y;
	}

	FString ToString();

	const int32 Get(int32 Row,int32 Column) const;
	void Set(int32 Row,int32 Column,int32 Value);
	int32 GetIndex(int32 Row,int32 Column);
	void ClampValue(int32 Max);
private:
	void OnAddRows(int32 Num = 1);
	void OnAddColumns(int32 Num = 1);
	void OnRemoveLastRows(int32 Num = 1);
	void OnRemoveLastColumns(int32 Num = 1);
};

USTRUCT()
struct MARKOVJUNIOR_API FInOutMatrix2
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,Category="MarkovJunior")
	FIntVector2 Size = FIntVector2(0);

	UPROPERTY(EditAnywhere,Category="MarkovJunior")
	FMatrix2 InMatrix;

	UPROPERTY(EditAnywhere,Category="MarkovJunior")
	FMatrix2 OutMatrix;

	void OnResize();

	void ClampValue(int32 Max);
};


