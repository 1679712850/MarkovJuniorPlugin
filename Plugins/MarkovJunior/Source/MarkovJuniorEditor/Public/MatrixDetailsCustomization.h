// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Container/Matrix2D.h"


/**
 * 
 */
class FInOutMatrixDetailsCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShareable(new FInOutMatrixDetailsCustomization());
	}

	FInOutMatrixDetailsCustomization();

	/** IPropertyTypeCustomization interface */
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
	                             IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder,
	                               IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	/** End of IPropertyTypeCustomization interface */
private:
	void GenerateInMatrixGridWidget() ; 
	void GenerateOutMatrixGridWidget() ;

	/**
	 * the delegate of the values changed in model
	 * to update the ValueOptions
	 */
	void OnValuePropertyChanged(UObject* Object, struct FPropertyChangedEvent& PropertyChangedEvent);
	/**
	 * the value of matrix must be clamp between -1 and ValuesName.Num -1 
	 */
	void CheckMatrixPropertyValue();

private:
	FInOutMatrix2* GetInOutMatrix() const;

	void OnMatrixSizeChanged();

	void UpdateValueOptions();
private:
	TSharedPtr<IPropertyHandle> MatrixPropertyHandle;

	FInOutMatrix2* InOutMatrix = nullptr;

	TSharedPtr<IPropertyHandle> SizeXPropertyHandle;
	TSharedPtr<IPropertyHandle> SizeYPropertyHandle;
	/**
	 * matrix property handle
	 */
	TSharedPtr<IPropertyHandle> InMatrixPropertyHandle;
	TSharedPtr<IPropertyHandle> OutMatrixPropertyHandle;

	TSharedPtr<SGridPanel> InMatrixGridPanel;
	TSharedPtr<SGridPanel> OutMatrixGridPanel;

	FIntVector2 PreMatrixSize = FIntVector2(0);

	TArray<TSharedRef<int32>> ValueOptions;
	TArray<FName> ValueNames;
};


