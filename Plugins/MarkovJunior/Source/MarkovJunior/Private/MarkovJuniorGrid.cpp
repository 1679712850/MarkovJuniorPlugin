// Fill out your copyright notice in the Description page of Project Settings.


#include "MarkovJuniorGrid.h"
#include "MarkovJuniorModel.h"
#include "Node/MarkovJuniorRule.h"

void UMarkovJuniorGrid::Initialize(TObjectPtr<UMarkovJuniorModel> Model)
{
	Resolution = Model->Resolution;
	State.SetNumZeroed(Size());
	Mask.SetNum(Size(),false);

	Values = Model->Values;
	for (int32 Index = 0; Index < Model->Values.Num(); ++Index)
	{
		ValueToIndexMap.Add(Model->Values[Index], Index);
		ValueToWaveMap.Add(Index, 1 << Index);
	}
	ValueToWaveMap.Add(-1,(1 << Values.Num()) - 1);
}

void UMarkovJuniorGrid::Clear()
{
	State.SetNumZeroed(Resolution.X * Resolution.Y * Resolution.Z);
}

int32 UMarkovJuniorGrid::CalculateWave(const TArray<int32>& ValueIndices)
{
	int32 Result = 0;
	for (auto ValueIndex : ValueIndices)
	{
		Result += ValueToWaveMap[ValueIndex];
	}
	return Result;
}

int32 UMarkovJuniorGrid::GetState(const FIntVector& Position) const
{
	if (Position.X < 0 || Position.X >= Resolution.X || Position.Y < 0 || Position.Y >= Resolution.Y || Position.Z < 0 || Position.Z >= Resolution.Z)
	{
		return -1;
	}
	return State[Position.X + Position.Y * Resolution.X + Position.Z * Resolution.X * Resolution.Y];
}

void UMarkovJuniorGrid::SetState(const FIntVector& Position, int32 Value)
{
	if (Position.X >= 0 && Position.X < Resolution.X && Position.Y >= 0 && Position.Y < Resolution.Y && Position.Z >= 0 && Position.Z < Resolution.Z)
	{
		State[Position.X + Position.Y * Resolution.X + Position.Z * Resolution.X * Resolution.Y] = Value;
	}
}

void UMarkovJuniorGrid::GetStates(TArray<int32>& OutStates) const
{
	OutStates = State;
}

bool UMarkovJuniorGrid::MatchRule(const FMarkovJuniorRule& Rule, const FIntVector& Position)
{
	FIntVector Delta = FIntVector(0);
	for (int32 Index = 0; Index < Rule.InputValueWaves.Num(); ++Index)
	{
		if (!(Rule.InputValueWaves[Index]
			& (1 << GetState(Position + Delta))) )
		{
			return false;
		}
		++Delta.X;
		if (Delta.X == Rule.Size.X)
		{
			Delta.X = 0;
			++Delta.Y;
			if (Delta.Y == Rule.Size.Y)
			{
				Delta.Y = 0;
				++Delta.Z;
			}
		}
	}
	return true;
}

FString UMarkovJuniorGrid::ToString()
{
	FString Result = "\n";
	for (int XIndex = 0; XIndex < Resolution.X; ++XIndex)
	{
		FString Row = "";
		for (int YIndex = 0; YIndex < Resolution.Y; ++YIndex)
		{
			Row += FString::FromInt(GetState(FIntVector(XIndex, YIndex, 0))) + " ";
		}
		Result += Row + "\n";
	}
	return Result;
}
