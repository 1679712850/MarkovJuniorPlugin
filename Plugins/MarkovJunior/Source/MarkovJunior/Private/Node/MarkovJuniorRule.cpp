// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/MarkovJuniorRule.h"

#include "MarkovJuniorFunctionLibrary.h"

int32 GetTrailingZero(int32 Value)
{
	int32 Count = 0;
	while ((Value & 1) == 0)
	{
		++Count;
		Value >>= 1;
	}
	return Count;
}

void ArrayZRotated(const TArray<int32>& Array,FIntVector Resolution,TArray<int32>& OutArray)
{
	OutArray.SetNum(Array.Num());
	for (int32 ZIndex = 0;ZIndex < Resolution.Z; ++ZIndex)
	{
		for (int32 YIndex = 0;YIndex < Resolution.Y; ++YIndex)
		{
			for (int32 XIndex = 0;XIndex < Resolution.X; ++XIndex)
			{
				int32 Index = ZIndex * Resolution.X * Resolution.Y + YIndex * Resolution.X + XIndex;
				int32 ZRotatedIndex = Resolution.X -1 - YIndex + XIndex * Resolution.X + ZIndex * Resolution.X * Resolution.Y;
				OutArray[ZRotatedIndex] = Array[Index];
			}
		}
	}
	
}

void ArrayYRotated(const TArray<int32>& Array,FIntVector Resolution,TArray<int32>& OutArray)
{
	OutArray.SetNum(Array.Num());
	for (int32 ZIndex = 0;ZIndex < Resolution.Z; ++ZIndex)
	{
		for (int32 YIndex = 0;YIndex < Resolution.Y; ++YIndex)
		{
			for (int32 XIndex = 0;XIndex < Resolution.X; ++XIndex)
			{
				int32 Index = ZIndex * Resolution.X * Resolution.Y + YIndex * Resolution.X + XIndex;
				int32 YRotatedIndex = Resolution.X -1 - ZIndex + YIndex * Resolution.X + XIndex * Resolution.X * Resolution.Y;
				OutArray[YRotatedIndex] = Array[Index];
			}
		}
	}
}

void ArrayReflected(const TArray<int32>& Array,FIntVector Resolution,TArray<int32>& OutArray)
{
	OutArray.SetNum(Array.Num());
	for (int32 ZIndex = 0;ZIndex < Resolution.Z; ++ZIndex)
	{
		for (int32 YIndex = 0;YIndex < Resolution.Y; ++YIndex)
		{
			for (int32 XIndex = 0;XIndex < Resolution.X; ++XIndex)
			{
				int32 Index = ZIndex * Resolution.X * Resolution.Y + YIndex * Resolution.X + XIndex;
				int32 ReflectedIndex = Resolution.X -1 - XIndex + YIndex * Resolution.X + ZIndex * Resolution.X * Resolution.Y;
				OutArray[ReflectedIndex] = Array[Index];
			}
		}
	}
}

void FMarkovJuniorRule::CalculateShifts(int32 ValueNum)
{
	// construct the InputShifts
	InputShifts.Empty();
	InputShifts.SetNum(ValueNum);
	UMarkovJuniorFunctionLibrary::ForEachElement(InputValueWaves, Size,
	      [ValueNum,this](int32 ValueWave, FIntVector Position)
	      {
	          for (int Index = 0; Index < ValueNum; ++Index, ValueWave
	               >>= 1)
	          {
	              if ((ValueWave & 1) == 1)
	              {
	                  InputShifts[Index].Emplace(Position);
	              }
	          }
	      }
	);

	// construct the out shifts
	OutputShifts.Empty();
	OutputShifts.SetNum(ValueNum);
	UMarkovJuniorFunctionLibrary::ForEachElement(OutputValueIndices, Size,
	      [ValueNum,this](int32 ValueIndex, FIntVector Position)
	      {
	          if (ValueIndex == -1)
	          {
	              for (int Index = 0; Index < ValueNum; ++Index)
	              {
	                  OutputShifts[Index].Emplace(Position);
	              }
	          }
	          else
	          {
	              OutputShifts[ValueIndex].Emplace(Position);
	          }
	      }
	);

	int32 Wildcard = (1 << ValueNum) - 1;
	InputValueTrailingZeroCounts.SetNum(InputValueWaves.Num());
	for (int Index = 0; Index < InputValueWaves.Num(); ++Index)
	{
		int ValueWave = InputValueWaves[Index];
		InputValueTrailingZeroCounts[Index] = (ValueWave == Wildcard) ? -1 : GetTrailingZero(ValueWave);
	}
}

void FMarkovJuniorRule::Construct(TObjectPtr<UMarkovJuniorGrid> Grid)
{
	int32 ValueNum = Grid->ValueNum();
	// construct input value wave
	InputValueWaves.Empty();
	for (auto Value : InOutMatrix.InMatrix.Data)
	{
		InputValueWaves.Emplace(Grid->CalculateWave({Value}));
	}
	// construct output value indices
	OutputValueIndices = InOutMatrix.OutMatrix.Data;

	// todo : 将二维大小和三维大小组合
	Size = FIntVector(InOutMatrix.InMatrix.Size.X, InOutMatrix.InMatrix.Size.Y, 1);

	CalculateShifts(ValueNum);
}

FMarkovJuniorRule FMarkovJuniorRule::ZRotated() const
{
	FMarkovJuniorRule Result = *this;

	ArrayZRotated(InputValueWaves, Size, Result.InputValueWaves);
	ArrayZRotated(OutputValueIndices, Size, Result.OutputValueIndices);
	
	Result.CalculateShifts(InputShifts.Num());
	return Result;
}

FMarkovJuniorRule FMarkovJuniorRule::YRotated() const
{
	FMarkovJuniorRule Result = *this;
	
	ArrayYRotated(InputValueWaves, Size, Result.InputValueWaves);
	ArrayYRotated(OutputValueIndices, Size, Result.OutputValueIndices);
	
	Result.CalculateShifts(InputShifts.Num());
	return Result;
}

FMarkovJuniorRule FMarkovJuniorRule::Reflected() const
{
	FMarkovJuniorRule Result = *this;

	ArrayReflected(InputValueWaves, Size, Result.InputValueWaves);
	ArrayReflected(OutputValueIndices, Size, Result.OutputValueIndices);

	Result.CalculateShifts(InputShifts.Num());
	
	return Result;
}

bool FMarkovJuniorRule::operator==(const FMarkovJuniorRule& OtherRule) const
{
	if (Size != OtherRule.Size)
	{
		return false;
	}
	if (InputValueWaves != OtherRule.InputValueWaves)
	{
		return false;
	}
	if (OutputValueIndices != OtherRule.OutputValueIndices)
	{
		return false;
	}
	return true;
}

void FMarkovJuniorRule::PostModelEdited(const TArray<FMarkovJuniorValue>& Values)
{
	InOutMatrix.PostModelEdited(Values);
}

void FMarkovJuniorField::Initialize(TObjectPtr<UMarkovJuniorGrid> Grid)
{
	SubstrateWave = Grid->CalculateWave(ValuesBasedOn);

	ZeroWave = Grid->CalculateWave(ValuesFromOrTo);
}
