// Fill out your copyright notice in the Description page of Project Settings.


#include "MarkovJuniorSymmetry.h"

/**
 * The function's arguments represent the index of each bit of a byte,
 * and the function's bool result represents whether the current bit is 1
 */
TArray<bool> MakeUInt64(TFunction<bool(int32)> Function)
{
	TArray<bool> Result;
	Result.Init(false, 64);
	for (int Index = 0; Index < 64; ++Index)
	{
		Result[Index] = Function(Index);
	}
	return Result;
}

TMap<ESymmetryType,TArray<bool>> FMarkovJuniorSymmetry::SquareSubgroups = {
	{ESymmetryType::None, { true, false, false, false, false, false, false, false }},
	{ESymmetryType::X, { true, true, false, false, false, false, false, false }},
	{ESymmetryType::Y, { true, false, false, false, false, true, false, false }},
	{ESymmetryType::XY_Combine, { true, true, false, false, true, true, false, false }},
	{ESymmetryType::AllRotated, { true, false, true, false, true, false, true, false }},
	{ESymmetryType::All, { true, true, true, true, true, true, true, true }}
};
TMap<ESymmetryType,TArray<bool>> FMarkovJuniorSymmetry::CubeSubgroups = {
	{ESymmetryType::None, MakeUInt64([](int32 Index){return Index == 0;})},
	{ESymmetryType::X, MakeUInt64([](int32 Index){return Index == 0 || Index == 1;})},
	{ESymmetryType::Z, MakeUInt64([](int32 Index){return Index == 0 || Index == 17;})},
	{ESymmetryType::XY_Combine, MakeUInt64([](int32 Index){return Index < 8;})},
	{ESymmetryType::AllRotated, MakeUInt64([](int32 Index){return Index % 2 ==0;})},
	{ESymmetryType::All, MakeUInt64([](int32 Index){return true;})}
};

FMarkovJuniorSymmetry::FMarkovJuniorSymmetry(bool bThreeDimension,ESymmetryType SymmetryType)
{
	Symmetries = bThreeDimension ? CubeSubgroups[SymmetryType] : SquareSubgroups[SymmetryType];
}
