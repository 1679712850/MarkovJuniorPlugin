// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/MarkovJuniorBranchNode.h"
#include "MarkovJuniorInterpreter.h"

bool UMarkovJuniorBranchNode::Initialize_Implementation(UMarkovJuniorInterpreter* InInterpreter,UMarkovJuniorGrid* InGrid)
{
	Super::Initialize_Implementation(InInterpreter, InGrid);
	// todo:symmetry process

	for (auto& ChildNode : ChildNodes)
	{
		// initialize the child node
		if (!ChildNode->Initialize(InInterpreter, InGrid))
		{
			return false;
		}
		// set the parent node of the child node
		if (auto BranchNode = Cast<UMarkovJuniorBranchNode>(ChildNode))
		{
			BranchNode->ParentNode = this;
		}
	}
	return true;
}

void UMarkovJuniorBranchNode::Reset_Implementation()
{
	for (auto& ChildNode : ChildNodes)
	{
		ChildNode->Reset();
	}
	CurrentChildIndex = 0;
}

bool UMarkovJuniorBranchNode::Go_Implementation()
{
	for (; CurrentChildIndex < ChildNodes.Num(); ++CurrentChildIndex)
	{
		const auto ChildNode = ChildNodes[CurrentChildIndex];
		if (auto BranchNode = Cast<UMarkovJuniorBranchNode>(ChildNode))
		{
			// update the current node of the interpreter to the ChildNode
			Interpreter->SetCurrentNode(BranchNode);
		}
		if (ChildNode->Go())
		{
			return true;
		}
	}
	// recover the current node of the interpreter to the parent
	Interpreter->SetCurrentNode(ParentNode);

	Reset();
	return false;
}

void UMarkovJuniorBranchNode::PostModelEdited(const TArray<FMarkovJuniorValue>& Values)
{
	for (auto& Child : ChildNodes)
	{
		if (Child)
		{
			Child->PostModelEdited(Values);
		}
	}
}


bool UMarkovJuniorBranchNode_Markov::Go_Implementation()
{
	CurrentChildIndex = 0;
	return Super::Go_Implementation();
}
