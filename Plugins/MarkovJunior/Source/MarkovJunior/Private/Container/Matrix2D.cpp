// Fill out your copyright notice in the Description page of Project Settings.


#include "Container/Matrix2D.h"

#include "MarkovJuniorGrid.h"
#include "MarkovJuniorLog.h"


FMatrix2::FMatrix2()
{
	Size = FIntVector2(1);

	Data.Empty();
	Data.Add(-1);
}

void FMatrix2::OnAddRows(int32 Num)
{
	while (Num--)
	{
		for (int32 Steps = 0; Steps < GetColumnNum(); ++Steps)
		{
			Data.Add(-1);
		}	
	}
}

void FMatrix2::OnAddColumns(int32 Num)
{
	while (Num--)
	{
		// Y is new column num
		// the index of insert is X*Steps - 1
		for (int32 Steps = 1; Steps <= GetRowNum(); ++Steps)
		{
			int32 InsertIndex = GetColumnNum() * Steps - 1;
			InsertIndex = InsertIndex >= Data.Num() ? Data.Num() -1 : InsertIndex;
			Data.Insert(-1,InsertIndex);
		}
	}

}

void FMatrix2::OnRemoveLastRows(int32 Num)
{
	while (Num--)
	{
		for (int32 Steps = 0; Steps < GetColumnNum(); ++Steps)
		{
			Data.Pop();
		}
	}

}

void FMatrix2::OnRemoveLastColumns(int32 Num)
{
	// remove from the back
	// new size is (3,1)
	// last data is 
	// 0 1
	// 2 3
	// 4 5
	int32 PreColumnNum = GetColumnNum() + 1;
	while (Num--)
	{
		for (int32 Steps = GetRowNum(); Steps >= 1; --Steps)
		{
			Data.RemoveAt(Steps * PreColumnNum - 1);
		}
	}
}

void FMatrix2::SetSize(FIntVector2 NewSize)
{
	FIntVector2 PreSize = Size;
	if (PreSize == NewSize)
	{
		return;
	}
	FIntVector2 DeltaSize = NewSize - Size;
	Size = NewSize;
	// update matrix data
	if (DeltaSize.X > 0)
	{
		OnAddRows(DeltaSize.X);
	}
	else
	{
		OnRemoveLastRows(-DeltaSize.X);
	}
	if (DeltaSize.Y > 0)
	{
		OnAddColumns(DeltaSize.Y);
	}
	else
	{
		OnRemoveLastColumns(-DeltaSize.Y);
	}
	UE_LOG(LogMarkovJunior,Warning,TEXT("Matrix Size Changed %s"),*ToString());
}

FString FMatrix2::ToString()
{
	FString Result = "\n";

	for (int32 RowIndex = 0; RowIndex < GetRowNum(); ++RowIndex)
	{
		FString RowString;
		for (int32 ColumnIndex = 0; ColumnIndex < GetColumnNum(); ++ColumnIndex)
		{
			RowString += FString::Printf(TEXT("%d "),Get(RowIndex,ColumnIndex));
		}
		Result += RowString+"\n";
	}
	return Result;
}

const int32 FMatrix2::Get(int32 Row, int32 Column) const
{
	if (Row < 0 || Column < 0 || Row >= GetRowNum() || Column>= GetColumnNum())
	{
		return -1;
	}
	
	return Data[Row * GetColumnNum() + Column];
}

void FMatrix2::Set(int32 Row,int32 Column,int32 Value)
{
	if (Row < 0 || Column < 0 || Row >= GetRowNum() || Column>= GetColumnNum())
	{
		return;
	}
	Data[Row * GetColumnNum() + Column] = Value;
}


int32 FMatrix2::GetIndex(int32 Row, int32 Column)
{
	if (Row < 0 || Column < 0 || Row >= GetRowNum() || Column>= GetColumnNum())
	{
		return -1;
	}
	return Row * GetColumnNum() + Column;
}

void FMatrix2::ClampValue(int32 Max)
{
	for (int32 RowIndex = 0; RowIndex < GetRowNum(); ++RowIndex)
	{
		for (int32 ColumnIndex = 0; ColumnIndex < GetColumnNum(); ++ColumnIndex)
		{
			auto Value = Get(RowIndex,ColumnIndex);
			if (Value > Max)
			{
				Set(RowIndex,ColumnIndex,-1);
			}
		}
	}
}


void FInOutMatrix2::OnResize()
{
	if (Size.X <= 0 || Size.Y <= 0)
	{
		return;
	}
	InMatrix.SetSize(Size);
	OutMatrix.SetSize(Size);
}

void FInOutMatrix2::ClampValue(int32 Max)
{
	InMatrix.ClampValue(Max);
	OutMatrix.ClampValue(Max);
}
#if WITH_EDITOR
void FInOutMatrix2::CheckMatrixPropertyValue()
{
	ClampValue(ValueNames.Num() - 1);
}

void FInOutMatrix2::UpdateValueOptions()
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

void FInOutMatrix2::PostModelEdited(const TArray<FMarkovJuniorValue>& Values)
{
	ValueNames.Empty();
	for (auto& Value : Values)
	{
		ValueNames.Emplace(Value.Name);
	}
	CheckMatrixPropertyValue();

	UpdateValueOptions();
}
#endif