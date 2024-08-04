// Fill out your copyright notice in the Description page of Project Settings.


#include "MatrixDetailsCustomization.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "MarkovJuniorEditorLog.h"
#include "PropertyCustomizationHelpers.h"
#include "Container/Matrix2D.h"

FInOutMatrixDetailsCustomization::FInOutMatrixDetailsCustomization()
{
	ValueOptions.Add(MakeShared<int32>(-1));

	FCoreUObjectDelegates::OnObjectPropertyChanged.AddRaw(this,&FInOutMatrixDetailsCustomization::OnValuePropertyChanged);

	ValueNames.Add(TEXT("ceshi1"));
	ValueNames.Add(TEXT("ceshi2"));
	ValueNames.Add(TEXT("ceshi3"));
	ValueNames.Add(TEXT("ceshi4"));
}

void FInOutMatrixDetailsCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
                                                  FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	MatrixPropertyHandle = PropertyHandle;
	InOutMatrix = GetInOutMatrix();

	SizeXPropertyHandle = PropertyHandle->GetChildHandle(0)->GetChildHandle(0);
	SizeYPropertyHandle = PropertyHandle->GetChildHandle(0)->GetChildHandle(1);

	SizeXPropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateRaw(this,&FInOutMatrixDetailsCustomization::OnMatrixSizeChanged));
	SizeYPropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateRaw(this,&FInOutMatrixDetailsCustomization::OnMatrixSizeChanged));


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
	GenerateInMatrixGridWidget();
	
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
	GenerateOutMatrixGridWidget();
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
// todo : combine to one function
void FInOutMatrixDetailsCustomization::GenerateInMatrixGridWidget()
{
	InMatrixGridPanel->ClearChildren();
	
	UpdateValueOptions();
	
	FMatrix2* InMatrix = &InOutMatrix->InMatrix;
	for (int32 RowIndex = 0; RowIndex < InMatrix->GetRowNum(); ++RowIndex)
	{
		for (int32 ColumnIndex = 0; ColumnIndex < InMatrix->GetColumnNum(); ++ColumnIndex)
		{
			InMatrixGridPanel->AddSlot(ColumnIndex,RowIndex)
			[
				// todo:change this to FName,更换为特定Value的Name，来显示
				SNew(SComboBox<TSharedRef<int32>>)
				.OptionsSource(&ValueOptions)
				.OnGenerateWidget_Lambda([this](TSharedRef<int32> ValueIndex)
				{
					return SNew(STextBlock)
						.Text(FText::FromString(FString::Printf(TEXT("%d"), *ValueIndex)))
						.Font(IPropertyTypeCustomizationUtils::GetRegularFont());
				})
				.OnSelectionChanged_Lambda([RowIndex,ColumnIndex,InMatrix, this](TSharedPtr<int32> InValue, ESelectInfo::Type InSelectType)
				{
					// This is for the initial selection.
					if (InSelectType == ESelectInfo::Direct)
					{
						return;
					}
					if (InValue.IsValid())
					{
						InMatrix->Set(RowIndex,ColumnIndex,*InValue);
					}
					UE_LOG(LogMarkovJuniorEditor,Warning,TEXT("MartrixData: %s"),*(InMatrix->ToString()))
				})
				[
					SNew(STextBlock)
					.Font(IPropertyTypeCustomizationUtils::GetRegularFont())
					.Text_Lambda([RowIndex,ColumnIndex,InMatrix,this]()
						{
							int32 Value = InMatrix->Get(RowIndex,ColumnIndex);
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

void FInOutMatrixDetailsCustomization::GenerateOutMatrixGridWidget()
{
	OutMatrixGridPanel->ClearChildren();
	
	UpdateValueOptions();

	FMatrix2* OutMatrix = &InOutMatrix->OutMatrix;
	for (int32 RowIndex = 0; RowIndex < OutMatrix->GetRowNum(); ++RowIndex)
	{
		for (int32 ColumnIndex = 0; ColumnIndex < OutMatrix->GetColumnNum(); ++ColumnIndex)
		{
			OutMatrixGridPanel->AddSlot(ColumnIndex,RowIndex)
			[
				// todo:change this to FName,更换为特定Value的Name，来显示
				SNew(SComboBox<TSharedRef<int32>>)
				.OptionsSource(&ValueOptions)
				.OnGenerateWidget_Lambda([this](TSharedRef<int32> ValueIndex)
				{
					return SNew(STextBlock)
						.Text(FText::FromString(FString::Printf(TEXT("%d"), *ValueIndex)))
						.Font(IPropertyTypeCustomizationUtils::GetRegularFont());
				})
				.OnSelectionChanged_Lambda([RowIndex,ColumnIndex,OutMatrix, this](TSharedPtr<int32> InValue, ESelectInfo::Type InSelectType)
				{
					// This is for the initial selection.
					if (InSelectType == ESelectInfo::Direct)
					{
						return;
					}
					if (InValue.IsValid())
					{
						OutMatrix->Data[OutMatrix->GetIndex(RowIndex,ColumnIndex)] = *InValue;
					}
					UE_LOG(LogMarkovJuniorEditor,Warning,TEXT("MartrixData: %s"),*OutMatrix->ToString())
				})
				[
					SNew(STextBlock)
					.Font(IPropertyTypeCustomizationUtils::GetRegularFont())
					.Text_Lambda([RowIndex,ColumnIndex,OutMatrix,this]()
						{
							int32 Value = OutMatrix->Get(RowIndex,ColumnIndex);
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
	// todo : when model changed
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
	UE_LOG(LogMarkovJuniorEditor,Warning,TEXT("NewSize:%d,%d"),InOutMatrix->Size.X,InOutMatrix->Size.Y)
	GenerateInMatrixGridWidget();
	GenerateOutMatrixGridWidget();
}

void FInOutMatrixDetailsCustomization::UpdateValueOptions()
{
	int32 PreOptionsNum = ValueOptions.Num();
	int32 CurrentOptionsNum = ValueNames.Num();
	if (PreOptionsNum <= CurrentOptionsNum)
	{
		for (int32 Index = PreOptionsNum - 1; Index <= CurrentOptionsNum; ++Index)
		{
			ValueOptions.Add(MakeShared<int32>(Index));
		}
	}
	else
	{
		ValueOptions.SetNum(CurrentOptionsNum);
	}
}
