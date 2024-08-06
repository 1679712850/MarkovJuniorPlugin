// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MarkovJuniorModel.h"
#include "UObject/Object.h"
#include "MarkovJuniorFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MARKOVJUNIOR_API UMarkovJuniorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void ForEachElement(TArray<int32>& Array,FIntVector Resolution,
		TFunction<void(int32/* Value */,FIntVector /* Position */)> Func);

	static int32 PositionAsIndex(FIntVector Position,FIntVector Resolution);

	UFUNCTION(BlueprintCallable,Category="MarkovJunior",meta=(WorldContext="WorldContextObject"))
	static void Run(UObject* WorldContextObject,UMarkovJuniorModel* Model);

	static UTexture* CreateTexture(const FString& Path,const FString& Name,FIntVector2 Size,TArray<FColor> Data);
};
