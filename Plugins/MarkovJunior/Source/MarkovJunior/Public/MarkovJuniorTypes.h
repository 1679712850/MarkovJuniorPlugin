// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MarkovJuniorTypes.generated.h"

UENUM(BlueprintType, Category = "MarkovJunior")
enum class EMarkovJuniorSymmetry : uint8
{
	None,
	X,
	Y,
	Z,
	XY,
	XYZ,
	XY_Plus,
	XYZ_Plus
};

