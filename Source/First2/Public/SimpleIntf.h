// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SimpleIntf.generated.h"

// This class does not need to be modified.
/*这一个类仅仅让UE4知道并用作反射，不需要修改*/
UINTERFACE(MinimalAPI)
class USimpleIntf : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FIRST2_API ISimpleIntf
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
//原生的CPP接口，没有被UFUNCTION修饰
	virtual void Interface_cpp(const FString str) = 0;

	//在子类中实现，但是无须调回来，因为接口本身没有实现。
UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
void NativeInterfa(const FString & str);

	//在蓝图中实现就好
UFUNCTION(BlueprintImplementableEvent)
void ImplementableInterf(const FString & str);


	
};
