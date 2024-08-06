// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MarkovJuniorGrid.generated.h"

struct FMarkovJuniorRule;
class UMarkovJuniorModel;
USTRUCT(BlueprintType)
struct MARKOVJUNIOR_API FMarkovJuniorValue
{
	GENERATED_BODY()
	/**
	 * the name to display
	 */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="MarkovJunior|Value")
	FName Name;
	/**
	 * the color of the value for tile is nullptr
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="MarkovJunior|Value")
	FLinearColor Color = FLinearColor::White;
	/**
	 * tile of the value to generate
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="MarkovJunior|Value",meta=(AllowedClasses="/Script/Engine.Texture2D,/Script/Engine.StaticMesh,/Script/Engine.Actor"))
	TObjectPtr<UObject> Tile;

	bool operator==(const FMarkovJuniorValue& OtherValue) const
	{
		return Name == OtherValue.Name && Color == OtherValue.Color && Tile == OtherValue.Tile;
	}
};

FORCEINLINE uint32 GetTypeHash(const FMarkovJuniorValue& Value)
{
	return GetTypeHash(Value.Name);
}

/**
 * 
 */
UCLASS(BlueprintType,blueprintable,ClassGroup=(MarkovJunior))
class MARKOVJUNIOR_API UMarkovJuniorGrid : public UObject
{
	GENERATED_BODY()
public:
	void Initialize(TObjectPtr<UMarkovJuniorModel> Model);

	/**
	 * clear the state array to initial state
	 */
	UFUNCTION(BlueprintCallable, Category = "MarkovJunior|Grid")
	void Clear();
	/**
	 * calculate the wave value of the given value indices
	 * @param ValueIndices the value index to calculate the wave
	 * @return the wave value calculated
	 */
	UFUNCTION(BlueprintCallable, Category = "MarkovJunior|Grid")
	int32 CalculateWave(const TArray<int32>& ValueIndices);

	// todo:规则匹配判定

	FORCEINLINE FIntVector GetResolution() const { return Resolution; }

	int32 GetState(const FIntVector& Position) const;

	void SetState(const FIntVector& Position, int32 Value);

	void GetStates(TArray<int32>& OutStates) const;

	int32 Size() const
	{
		return Resolution.X * Resolution.Y * Resolution.Z;
	}

	
	bool MatchRule(const FMarkovJuniorRule& Rule, const FIntVector& Position);

	int32 ValueNum() const
	{
		return Values.Num();
	}

	FString ToString();
protected:
	// 大小为X*Y*Z,每个值表示值的索引
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="MarkovJunior|Grid")
	TArray<int32> State;
	// 大小为X*Y*Z
	TBitArray<> Mask;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="MarkovJunior|Grid",meta=(ClampMin=0))
	FIntVector Resolution;
	/**
	 * the value type representing the ValueIndex
	 */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="MarkovJunior|Grid")
	TMap<FMarkovJuniorValue,int32> ValueToIndexMap;
	/**
	 * ValueIndex to WaveValue
	 */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="MarkovJunior|Grid")
	TMap<int32,int32> ValueToWaveMap;

	TArray<FMarkovJuniorValue> Values;
};
