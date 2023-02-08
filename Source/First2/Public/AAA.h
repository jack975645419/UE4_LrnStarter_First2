// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PrivaClassInFirstModule.h"
#include "PublicClasInFirstModule.h"
#include "AAA.generated.h"


/**
 * 
 */
UCLASS()
class FIRST2_API AAA:public AActor
{
	GENERATED_BODY()
public:
	void Hi()
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s%d%d\n    __________________"), __LINE__, *FString(""),
			PublicClasInFirstModule::PIG,//在PublicIncludePaths中
			PrivaClassInFirstModule::BACK_YARM_CONST
		);
		

	}

	static void Hi2(){}
};
