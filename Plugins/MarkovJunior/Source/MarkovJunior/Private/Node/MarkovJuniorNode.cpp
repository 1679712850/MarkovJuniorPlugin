// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/MarkovJuniorNode.h"

#if WITH_EDITOR
void UMarkovJuniorNode::PostModelEdited(const TArray<FMarkovJuniorValue>& Values)
{
	
}
#endif

bool UMarkovJuniorNode::Initialize_Implementation(UMarkovJuniorInterpreter* InInterpreter,UMarkovJuniorGrid* InGrid,const FMarkovJuniorSymmetry& ParentSymmetry)
{
	Interpreter = InInterpreter;
	Grid = InGrid;
	
	return true;
}
