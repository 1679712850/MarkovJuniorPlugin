// Fill out your copyright notice in the Description page of Project Settings.


#include "MarkovJuniorFunctionLibrary.h"

#include "MarkovJuniorInterpreter.h"
#include "MarkovJuniorLog.h"


void UMarkovJuniorFunctionLibrary::ForEachElement(TArray<int32>& Array, FIntVector Resolution,
                                                  TFunction<void(int32,FIntVector)> Func)
{
	for (int ZIndex = 0; ZIndex < Resolution.Z; ++ZIndex)
	{
		for (int YIndex = 0; YIndex < Resolution.Y; ++YIndex)
		{
			for (int XIndex = 0; XIndex < Resolution.X; ++XIndex)
			{
				const int32 Value = Array[XIndex + YIndex * Resolution.X + ZIndex * Resolution.X * Resolution.Y];
				const FIntVector Position = FIntVector(XIndex, YIndex, ZIndex);
				Func(Value, Position);
			}
		}
	}
}

int32 UMarkovJuniorFunctionLibrary::PositionAsIndex(FIntVector Position, FIntVector Resolution)
{
	return Position.X + Position.Y * Resolution.X + Position.Z * Resolution.X * Resolution.Y;
}

void UMarkovJuniorFunctionLibrary::Run(UObject* WorldContext, UMarkovJuniorModel* Model)
{
	auto Interpreter = NewObject<UMarkovJuniorInterpreter>(WorldContext);
	Interpreter->Initialize(Model);

	Interpreter->Run(FMath::Rand());

	TArray<int32> States;
	Interpreter->GetStates(States);

	TArray<FColor> Color;
	Color.SetNum(States.Num() * Model->PixelSize * Model->PixelSize);
	auto Size = Model->Resolution;
	TArray<FString> Indices;
	for (int32 RowIndex = 0; RowIndex < Size.Y; ++RowIndex)
	{
		for (int32 ColumnIndex = 0; ColumnIndex < Size.X; ++ColumnIndex)
		{
			const int32 Index = ColumnIndex + RowIndex * Size.X;
			const int32 Value = States[Index];
			const FColor PixelColor = Model->Values[Value].Color.ToFColor(true);
			const int32 StartY = RowIndex * Model->PixelSize;
			const int32 StartX = ColumnIndex * Model->PixelSize;
			for (int32 Y = StartY; Y < StartY + Model->PixelSize; ++Y)
			{
				for (int32 X = StartX; X < StartX + Model->PixelSize; ++X)
				{
					const int32 PixelIndex = X + Y * Size.X * Model->PixelSize;
					Indices.Add(FString::Printf(TEXT("%d"), PixelIndex));
					Color[PixelIndex] = PixelColor;
				}
			}
		}
	}
	CreateTexture(Model->ResultPath, Model->ResultName, FIntVector2(Size.X * Model->PixelSize, Size.Y * Model->PixelSize), Color);
}

UTexture* UMarkovJuniorFunctionLibrary::CreateTexture(const FString& Path,const FString& Name,FIntVector2 Size,TArray<FColor> Data)
{
	const ETextureSourceFormat format = ETextureSourceFormat::TSF_BGRA8;  
  
	//包的完整路径：
	const FString PackageName = Path + TEXT("/") + Name;  
	//创建包：  
	UPackage* pacakge = CreatePackage(*PackageName);  
	pacakge->FullyLoad();  
  
	//创建纹理对象：  
	UTexture2D* Texture = NewObject<UTexture2D>(pacakge, FName(*Name), RF_Public | RF_Standalone);  
  
	//初始化数据：  
	Texture->Source.Init(Size.X, Size.Y, 1, 1, format);  
	//得到数据的指针  
	uint32* BufferAddress = (uint32*)Texture->Source.LockMip(0);  
	//数据应有的尺寸（应该和 Data 一致）  
	const int32 BufferSize = Texture->Source.CalcMipSize(0);  
	FMemory::Memcpy(BufferAddress, Data.GetData(), BufferSize);  
	//数据拷贝结束  
	Texture->Source.UnlockMip(0);  
  
	//给包标脏  
	Texture->MarkPackageDirty();  
	//结束修改  
	Texture->PostEditChange();

	return Texture;
}
