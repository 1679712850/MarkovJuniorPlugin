﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "MatrixDetailsCustomization.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "MarkovJuniorEditorLog.h"
#include "MarkovJuniorModel.h"
#include "PropertyCustomizationHelpers.h"
#include "Container/Matrix2D.h"

FInOutMatrixDetailsCustomization::FInOutMatrixDetailsCustomization()
{
	ValueOptions.Add(MakeShared<int32>(-1));

	FCoreUObjectDelegates::OnObjectPropertyChanged.AddRaw(this,&FInOutMatrixDetailsCustomization::OnValuePropertyChanged);
	
}

void FInOutMatrixDetailsCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
                                                  FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	MatrixPropertyHandle = PropertyHandle;
	InOutMatrix = GetInOutMatrix();

	SizeXPropertyHandle = PropertyHandle->GetChildHandle(0)->GetChildHandle(0);
	SizeYPropertyHandle = PropertyHandle->GetChildHandle(0)->GetChildHandle(1);

	// add the delegate to the size property
	SizeXPropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateRaw(this,&FInOutMatrixDetailsCustomization::OnMatrixSizeChanged));
	SizeYPropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateRaw(this,&FInOutMatrixDetailsCustomization::OnMatrixSizeChanged));
	SizeXPropertyHandle->SetOnPropertyResetToDefault(FSimpleDelegate::CreateRaw(this,&FInOutMatrixDetailsCustomization::OnMatrixSizeChanged));
	SizeYPropertyHandle->SetOnPropertyResetToDefault(FSimpleDelegate::CreateRaw(this,&FInOutMatrixDetailsCustomization::OnMatrixSizeChanged));

	InMatrixPropertyHandle = PropertyHandle->GetChildHandle(1);
	OutMatrixPropertyHandle = PropertyHandle->GetChildHandle(2);
	
	HeaderRow.NameContent()
	[
		PropertyHandle->CreatePropertyNameWidget()
	];
	HeaderRow.ValueContent()
	.HAlign(EHorizontalAlignment::HAlign_Fill)
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.FillWidth(1)
		[
			SizeXPropertyHandle->CreatePropertyValueWidget()
		]
		.FillWidth(1)
		+SHorizontalBox::Slot()
		[
			SizeYPropertyHandle->CreatePropertyValueWidget()
		]
	];
}

void FInOutMatrixDetailsCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	
	auto& InRow = ChildBuilder.AddCustomRow(FText::FromString(TEXT("InMatrix")));
	
	InMatrixGridPanel = SNew(SGridPanel);

	GenerateMatrixGridWidget(InMatrixGridPanel, &InOutMatrix->InMatrix);
	InRow.WholeRowContent()
	[
		SNew(SBox)
		.HAlign(HAlign_Fill)
		.Padding(FMargin(0,25))
		.Content()
		[
			InMatrixGridPanel.ToSharedRef()
		]
	];
	auto& OutRow = ChildBuilder.AddCustomRow(FText::FromString(TEXT("OutMatrix")));

	OutMatrixGridPanel = SNew(SGridPanel);
	GenerateMatrixGridWidget(OutMatrixGridPanel, &InOutMatrix->OutMatrix);

	OutRow.WholeRowContent()
		[
			SNew(SBox)
			.HAlign(HAlign_Fill)
			.Padding(FMargin(0,25))
			.Content()
			[
				OutMatrixGridPanel.ToSharedRef()
			]
		];
}

void FInOutMatrixDetailsCustomization::GenerateMatrixGridWidget(TSharedPtr<SGridPanel>& GridPanel, FMatrix2* Matrix)
{
	CheckMatrixPropertyValue();
	GridPanel->ClearChildren();
	
	UpdateValueOptions();

	for (int32 RowIndex = 0; RowIndex < Matrix->GetRowNum(); ++RowIndex)
	{
		for (int32 ColumnIndex = 0; ColumnIndex < Matrix->GetColumnNum(); ++ColumnIndex)
		{
			GridPanel->AddSlot(ColumnIndex,RowIndex)
			[
				SNew(SComboBox<TSharedRef<int32>>)
				.OptionsSource(&ValueOptions)
				.OnGenerateWidget_Lambda([this](TSharedRef<int32> ValueIndex)
				{
					FText DisplayText;
					if (*ValueIndex == -1)
					{
						DisplayText = FText::FromString(TEXT("-1"));
					}
					else
					{
						DisplayText = FText::FromName(ValueNames[*ValueIndex]);
					}
					return SNew(STextBlock)
						.Text(DisplayText)
						.Font(IPropertyTypeCustomizationUtils::GetRegularFont());
				})
				.OnSelectionChanged_Lambda([RowIndex,ColumnIndex,Matrix, this](TSharedPtr<int32> InValue, ESelectInfo::Type InSelectType)
				{
					// This is for the initial selection.
					if (InSelectType == ESelectInfo::Direct)
					{
						return;
					}
					if (InValue.IsValid())
					{
						Matrix->Data[Matrix->GetIndex(RowIndex,ColumnIndex)] = *InValue;
					}
					UE_LOG(LogMarkovJuniorEditor,Warning,TEXT("MartrixData: %s"),*Matrix->ToString())
				})
				[
					SNew(STextBlock)
					.Font(IPropertyTypeCustomizationUtils::GetRegularFont())
					.Text_Lambda([RowIndex,ColumnIndex,Matrix,this]()
						{
							int32 Value = Matrix->Get(RowIndex,ColumnIndex);
							FText DisplayText;
							if (Value == -1)
							{
								DisplayText = FText::FromString(FString::Printf(TEXT("%d"), Value));
							}
							else
							{
								DisplayText = FText::FromName(ValueNames[Value]);
							}
							return DisplayText;
						}
					)
				]
			];
		}
	}
}

void FInOutMatrixDetailsCustomization::OnValuePropertyChanged(UObject* Object, struct FPropertyChangedEvent& PropertyChangedEvent)
{
	if (auto Model = Cast<UMarkovJuniorModel>(Object))
	{
		ValueNames.Empty();
		for (auto& Value : Model->Values)
		{
			ValueNames.Emplace(Value.Name);
		}
		CheckMatrixPropertyValue();

		UpdateValueOptions();
	}
}


FInOutMatrix2* FInOutMatrixDetailsCustomization::GetInOutMatrix() const
{
	uint8* ObjectAddress = nullptr;
	if (MatrixPropertyHandle->GetNumOuterObjects() > 0)
	{
		TArray<UObject*> Objects;
		MatrixPropertyHandle->GetOuterObjects(Objects);
		ObjectAddress =  reinterpret_cast<uint8*>(Objects[0]);
	}
	if (MatrixPropertyHandle.IsValid())
	{
		return reinterpret_cast<FInOutMatrix2*>(MatrixPropertyHandle->GetValueBaseAddress(ObjectAddress));
	}
	return nullptr;
}

void FInOutMatrixDetailsCustomization::OnMatrixSizeChanged()
{
	InOutMatrix->OnResize();
	// GenerateInMatrixGridWidget();
	// GenerateOutMatrixGridWidget();

	GenerateMatrixGridWidget(OutMatrixGridPanel, &InOutMatrix->OutMatrix);
	GenerateMatrixGridWidget(InMatrixGridPanel, &InOutMatrix->InMatrix);
}

void FInOutMatrixDetailsCustomization::UpdateValueOptions()
{
	int32 PreOptionsNum = ValueOptions.Num() - 1;
	int32 CurrentOptionsNum = ValueNames.Num();
	if (PreOptionsNum <= CurrentOptionsNum)
	{
		for (int32 Index = PreOptionsNum; Index < CurrentOptionsNum; ++Index)
		{
			ValueOptions.Add(MakeShared<int32>(Index));
		}
	}
	else
	{
		ValueOptions.SetNum(CurrentOptionsNum);
	}
}

void FInOutMatrixDetailsCustomization::CheckMatrixPropertyValue()
{
	InOutMatrix->ClampValue(ValueNames.Num() - 1);
}