// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "First2/Backyarm/PrivaClassInFirstModule.h"
#include "First2/Private/PrivatClassInFirst.h"

/**
 * 
 */
class MYMODULE_API CanUsePrivInFirst
{
public:
	CanUsePrivInFirst();
	~CanUsePrivInFirst();

	void foo1  ()
	{
		PrivaClassInFirstModule::BACK_YARM_CONST; // 可以访问，需要写全路径
		PrivatClassInFirst::SHEEP; // 可以访问，需要写全路径
	}

	static void foo2()
	{

		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString(""));

	}
};
