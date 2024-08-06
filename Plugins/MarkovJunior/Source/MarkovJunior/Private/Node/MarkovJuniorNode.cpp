// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/MarkovJuniorNode.h"


void UMarkovJuniorNode::PostModelEdited(const TArray<FMarkovJuniorValue>& Values)
{
	
}

bool UMarkovJuniorNode::Initialize_Implementation(UMarkovJuniorInterpreter* InInterpreter,UMarkovJuniorGrid* InGrid)
{
	Interpreter = InInterpreter;
	Grid = InGrid;
	
	return true;
}
