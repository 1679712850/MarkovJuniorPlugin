// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MarkovJuniorGrid.h"
#include "UObject/Object.h"
#include "MarkovJuniorNode.generated.h"

class UMarkovJuniorInterpreter;
/**
 * 
 */
UCLASS(Abstract,EditInlineNew,Blueprintable,ClassGroup=(MarkovJunior))
class MARKOVJUNIOR_API UMarkovJuniorNode : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * reset the node to initial state
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MarkovJunior|Node")
	void Reset();

	/**
	 * execute the node one time
	 * @return whether the node has executed successfully
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MarkovJunior|Node")
	bool Go();
	/**
	 * initialize the node
	 * @param InInterpreter the interpreter that this node works with
	 * @param InGrid the grid that this node works with
	 * @return whether the initialization is successful
	 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "MarkovJunior|Node")
	bool Initialize(UMarkovJuniorInterpreter* InInterpreter,UMarkovJuniorGrid* InGrid);

#if WITH_EDITOR
	virtual void PostModelEdited(const TArray<FMarkovJuniorValue>& Values);
#endif
	
	
protected:
	/**
	 * the C++ native implementation of the reset function
	 */
	virtual void Reset_Implementation() PURE_VIRTUAL(UMarkovJuniorNode::Reset_Implementation);
	/**
	 * the C++ native implementation of the go function
	 */
	virtual bool Go_Implementation() PURE_VIRTUAL(UMarkovJuniorNode::Go_Implementation,return false;);
	/**
	 * the C++ native implementation of the initialize function
	 * @param InInterpreter the interpreter that this node works with
	 * @param InGrid the grid that this node works with
	 * @return whether the initialization is successful
	 */
	virtual bool Initialize_Implementation(UMarkovJuniorInterpreter* InInterpreter,UMarkovJuniorGrid* InGrid);
protected:
	/**
	 * The grid that this node works with
	 */
	UPROPERTY(BlueprintReadOnly, Category = "MarkovJunior|Node")
	TObjectPtr<UMarkovJuniorGrid> Grid;
	/**
	 * The interpreter that this node works with
	 */
	UPROPERTY(BlueprintReadOnly, Category = "MarkovJunior|Node")
	TObjectPtr<UMarkovJuniorInterpreter> Interpreter;
};
