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

	/** IPropertyTypeCustomization interface */
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
	                             IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder,
	                               IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	/** End of IPropertyTypeCustomization interface */
private:
	// 生成矩阵的编辑细节控件
	void GenerateMatrixGridWidget(TSharedPtr<SGridPanel>& GridPanel, FMatrix2* Matrix);

private:
	// 通过蓝图获取输入输出矩阵结构体
	FInOutMatrix2* GetInOutMatrix() const;

	// 当矩阵Size属性值变化的回调函数
	void OnMatrixSizeChanged();

private:
	// 输入和输出矩阵的属性句柄
	TSharedPtr<IPropertyHandle> MatrixPropertyHandle;

	FInOutMatrix2* InOutMatrix = nullptr;

	// 大小属性的句柄
	TSharedPtr<IPropertyHandle> SizeXPropertyHandle;
	TSharedPtr<IPropertyHandle> SizeYPropertyHandle;
	/**
	 * matrix property handle
	 */
	TSharedPtr<IPropertyHandle> InMatrixPropertyHandle;
	TSharedPtr<IPropertyHandle> OutMatrixPropertyHandle;

	// 矩阵网格的控件
	TSharedPtr<SGridPanel> InMatrixGridPanel;
	TSharedPtr<SGridPanel> OutMatrixGridPanel;


	FDelegateHandle OnObjectPropertyChangedHandle;
	FDelegateHandle OnAssetEditorOpenedHandle;
};


