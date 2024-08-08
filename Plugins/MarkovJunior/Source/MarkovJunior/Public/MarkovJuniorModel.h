// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MarkovJuniorGrid.h"
#include "MarkovJuniorSymmetry.h"
#include "Node/MarkovJuniorBranchNode.h"
#include "UObject/Object.h"
#include "MarkovJuniorModel.generated.h"

/**
 * 
 */
UCLASS()
class MARKOVJUNIOR_API UMarkovJuniorModel : public UDataAsset
{
	GENERATED_BODY()
public:
	UMarkovJuniorModel();

public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FString ResultPath = TEXT("/Game");
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FString ResultName = TEXT("Test");
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	bool bThreeDimensional = false;
	
	UPROPERTY(EditAnywhere,meta=(ClampMin=1))
	FIntVector Resolution = FIntVector3(1);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int32 Steps = 0;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(EditConditionHides="!bThreeDimensional"))
	int32 PixelSize = 4;
	/**
	 * whether start from origin or random
	 */
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	bool bOrigin = false;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	ESymmetryType SymmetryType = ESymmetryType::All;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TArray<FMarkovJuniorValue> Values;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Instanced)
	TObjectPtr<UMarkovJuniorBranchNode> RootNode;

#if WITH_EDITOR
public:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	void UpdateRuleOptions();
#endif
};


