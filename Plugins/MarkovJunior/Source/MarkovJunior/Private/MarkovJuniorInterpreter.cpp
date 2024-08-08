// Fill out your copyright notice in the Description page of Project Settings.


#include "MarkovJuniorInterpreter.h"

#include "MarkovJuniorLog.h"
#include "MarkovJuniorModel.h"

void UMarkovJuniorInterpreter::Initialize(TObjectPtr<UMarkovJuniorModel> Model)
{
	bOrigin = Model->bOrigin;

	Grid = NewObject<UMarkovJuniorGrid>(this);
	Grid->Initialize(Model);

	RootNode = Model->RootNode;

	Steps = Model->Steps;

	const FMarkovJuniorSymmetry Symmetry(Model->bThreeDimensional,Model->SymmetryType);
	if (RootNode)
	{
		RootNode->Initialize(this,Grid,Symmetry);
	}
}

void UMarkovJuniorInterpreter::Run(int32 Seed)
{
	RandomStream.Initialize(Seed);

	Grid->Clear();

	if (bOrigin)
	{
		Grid->SetState(Grid->GetResolution()/2,1);
	}
	Changes.Empty();
	Firsts.Empty();

	Firsts.Add(0);

	RootNode->Reset();
	CurrentNode = RootNode;

	Counter = 0;
	while (CurrentNode && (Steps <= 0 || Counter < Steps))
	{
		CurrentNode->Go();
		Counter++;
		Firsts.Add(Changes.Num());
	}
}

void UMarkovJuniorInterpreter::SetCurrentNode(TObjectPtr<UMarkovJuniorBranchNode> BranchNode)
{
	CurrentNode = BranchNode;
}
