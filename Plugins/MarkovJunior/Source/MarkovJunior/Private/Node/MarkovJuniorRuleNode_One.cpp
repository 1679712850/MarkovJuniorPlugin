// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/MarkovJuniorRuleNode_One.h"

#include "MarkovJuniorFunctionLibrary.h"
#include "MarkovJuniorInterpreter.h"
#include "Algo/RandomShuffle.h"



bool UMarkovJuniorRuleNode_One::Go_Implementation()
{
	if (!Super::Go_Implementation())
	{
		return false;
	}
	LastMatchedTurn = Interpreter->Counter;

	if (MatchCount == 0)
	{
		return false;
	}


	TPair<FIntVector,int32> Match = RandomMatch();
	if (Match.Value < 0)
	{
		return false;
	}
	else
	{
		LastMask[Match.Value] = false;
		Apply(Rules[Match.Value],Match.Key);
		Counter++;
		return true;
	}
	{
		// todo:this code if for AllNode
		// TArray<int32> Shuffle;
		// Shuffle.SetNum(MatchCount);
		// for (int32 Index = 0; Index < MatchCount; ++Index)
		// {
		// 	Shuffle[Index] = Index;
		// }
		// Algo::RandomShuffle(Shuffle);
		// FMath::SRandInit()
		
	}
	return true;
}

void UMarkovJuniorRuleNode_One::Reset_Implementation()
{
	Super::Reset_Implementation();
	if (MatchCount != 0 )
	{
		MatchMask.SetNum(Rules.Num());
		MatchCount = 0;
	}
}

TPair<FIntVector, int32> UMarkovJuniorRuleNode_One::RandomMatch()
{
	// todo:potentials
	{
		while (MatchCount)
		{
			const int32 MatchIndex = Interpreter->RandomStream.RandRange(0,MatchCount - 1);

			const TPair<FIntVector,int32> Match = AllMatches[MatchIndex];
			const auto Index = UMarkovJuniorFunctionLibrary::PositionAsIndex(Match.Key,Grid->GetResolution());

			MatchMask[Match.Value][Index] = false;
			AllMatches[MatchIndex] = AllMatches[--MatchCount];

			if (Grid->MatchRule(Rules[Match.Value],Match.Key))
			{
				return Match;
			}
		}
		return {FIntVector(-1),-1};
	}
}

void UMarkovJuniorRuleNode_One::Apply(const FMarkovJuniorRule& Rule, const FIntVector& Position)
{
	FIntVector GridSize = Grid->GetResolution();
	auto& Changes = Interpreter->Changes;

	for (int ZIndex = 0; ZIndex < Rule.Size.Z; ++ZIndex)
	{
		for (int YIndex = 0; YIndex < Rule.Size.Y; ++YIndex)
		{
			for (int XIndex = 0; XIndex < Rule.Size.X; ++XIndex)
			{
				FIntVector NewPosition(XIndex,YIndex,ZIndex);
				
				auto NewValue = Rule.OutputValueIndices[UMarkovJuniorFunctionLibrary::PositionAsIndex(NewPosition,Rule.Size)];
				
				if (NewValue != -1)
				{
					auto OldPosition = Position + NewPosition;

					auto OldValue = Grid->GetState(OldPosition);
					if (OldValue != NewValue)
					{
						Grid->SetState(OldPosition,NewValue);
						Changes.Add(OldPosition);
					}
				}
			}
		}
	}
}