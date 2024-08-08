// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/MarkovJuniorRuleNode.h"

#include "MarkovJuniorFunctionLibrary.h"
#include "MarkovJuniorInterpreter.h"

FMarkovJuniorRule ZRotate(const FMarkovJuniorRule& Rule)
{
	return Rule.ZRotated();
}

FMarkovJuniorRule YRotate(const FMarkovJuniorRule& Rule)
{
	return Rule.YRotated();
}
FMarkovJuniorRule Reflect(const FMarkovJuniorRule& Rule)
{
	return Rule.Reflected();
}

bool UMarkovJuniorRuleNode::Initialize_Implementation(UMarkovJuniorInterpreter* InInterpreter,UMarkovJuniorGrid* InGrid,const FMarkovJuniorSymmetry& ParentSymmetry)
{
	Super::Initialize_Implementation(InInterpreter, InGrid, ParentSymmetry);
	// initial the symmetry rule
	FMarkovJuniorSymmetry Symmetry(InGrid->GetResolution().Z != 1,SymmetryType);
	Symmetry |= ParentSymmetry;
	for (auto& Rule : Rules)
	{
		Rule.Construct(Grid);
	}
	TArray<FMarkovJuniorRule> TempRules = Rules;
	Rules.Empty();
	for (auto& Rule : TempRules)
	{
		TArray<FMarkovJuniorRule> SymmetryRules;
		Symmetry.GetSymmetries<FMarkovJuniorRule>(Rule,
			ZRotate,
			Reflect,
			YRotate,
			SymmetryRules);
		for (auto& SymmetryRule : SymmetryRules)
		{
			Rules.Add(SymmetryRule);
		}
	}
	// initial mask 
	MatchMask.SetNum(Rules.Num());
	for (auto& Mask : MatchMask)
	{
		Mask.Init(false,Grid->Size());
	}
	return true;
}

void UMarkovJuniorRuleNode::Reset_Implementation()
{
	LastMatchedTurn = -1;
	Counter = 0;
	bHasFutureComputed = false;

	LastMask.Init(false, Rules.Num());
}

bool UMarkovJuniorRuleNode::Go_Implementation()
{
	LastMask.Init(false, Rules.Num());

	if (Steps > 0 && Counter >= Steps)
	{
		return false;
	}

	auto GridSize = Grid->GetResolution();
	if (!Observations.IsEmpty() && !bHasFutureComputed)
	{
		// todo: ComputeFuture();
		// ComputeFuture();
	}

	if (LastMatchedTurn >= 0)
	{
		const auto& Changes = Interpreter->Changes;
		for (int32 Index = Interpreter->Firsts[LastMatchedTurn]; Index < Interpreter->Changes.Num(); ++Index)
		{
			auto& Change = Changes[Index];
			auto Value = Grid->GetState(Change);
			for (int32 RuleIndex = 0; RuleIndex < Rules.Num(); ++RuleIndex)
			{
				const FMarkovJuniorRule& Rule = Rules[RuleIndex];
				const auto& RuleMask = MatchMask[RuleIndex];
				const auto& Shifts = Rule.InputShifts[Value];
				for (auto& Shift : Shifts)
				// for (int32 ShiftIndex = 0;ShiftIndex < Shifts.Num();++ShiftIndex)
				{
					// todo: 减法优化
					const auto& Delta = Change - Shift;
					// check
					if (Delta.X < 0 || Delta.Y < 0 || Delta.Z < 0 ||
						Delta.X + Rule.Size.X > GridSize.X ||
						Delta.Y + Rule.Size.Y > GridSize.Y ||
						Delta.Z + Rule.Size.Z > GridSize.Z)
					{
						continue;
					}
					if (!RuleMask[UMarkovJuniorFunctionLibrary::PositionAsIndex(Delta,GridSize)] 
						&& Grid->MatchRule(Rule,Delta))
					{
						Add(RuleIndex, Delta);
					}
				}
			}
		}
	}
	else // the first time
	{
		MatchCount = 0;
		for (int32 RuleIndex = 0; RuleIndex < Rules.Num(); ++RuleIndex)
		{
			FMarkovJuniorRule& Rule = Rules[RuleIndex];
			for (int32 ZIndex = Rule.Size.Z - 1; ZIndex < GridSize.Z; ZIndex += Rule.Size.Z)
			{
				for (int32 YIndex = Rule.Size.Y - 1; YIndex < GridSize.Y; YIndex += Rule.Size.Y)
				{
					for (int32 XIndex = Rule.Size.X - 1; XIndex < GridSize.X; XIndex += Rule.Size.X)
					{
						auto Position = FIntVector(XIndex, YIndex, ZIndex);
						const auto& Shifts = Rule.InputShifts[Grid->GetState(Position)];
						for (auto& Shift : Shifts)
						{
							const auto& Delta = Position - Shift;
							// check
							if (Delta.X < 0 || Delta.Y < 0 || Delta.Z < 0 ||
								Delta.X + Rule.Size.X > GridSize.X ||
								Delta.Y + Rule.Size.Y > GridSize.Y ||
								Delta.Z + Rule.Size.Z > GridSize.Z)
							{
								continue;
							}
							if (Grid->MatchRule(Rule,Delta))
							{
								Add(RuleIndex, Delta);
							}
						}
					}
				}
			}
		}
	}
	if (!Fields.IsEmpty())
	{
		// todo : compute fields 
	}
	
	return true;
}

void UMarkovJuniorRuleNode::Add(int32 RuleIndex, const FIntVector& Position)
{
	MatchMask[RuleIndex][UMarkovJuniorFunctionLibrary::PositionAsIndex(Position,Grid->GetResolution())] = true;

	TPair<FIntVector,int32> Match(Position,RuleIndex);
	if (MatchCount < AllMatches.Num())
	{
		AllMatches[MatchCount] = Match;
	}
	else
	{
		AllMatches.Add(Match);
	}
	++MatchCount;
}
#if WITH_EDITOR
void UMarkovJuniorRuleNode::PostModelEdited(const TArray<FMarkovJuniorValue>& Values)
{
	for (auto& Rule : Rules)
	{
		Rule.PostModelEdited(Values);
	}
}
#endif