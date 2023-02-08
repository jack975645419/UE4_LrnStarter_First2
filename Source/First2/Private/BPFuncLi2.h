// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPFuncLi2.generated.h"

/**
 * 
 */
UCLASS()
class FIRST2_API UBPFuncLi2:public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UBPFuncLi2();
	~UBPFuncLi2();

	UFUNCTION(BlueprintCallable, Category = "Utilities|Collision")
	static void Hello()
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString(""));

	}
};
