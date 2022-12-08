// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class FIRST2_API RawClas
{
public:
	RawClas();
	~RawClas();

	void ExeFun ()
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ __FUNCTION__  " at line %d\n%s\n    __________________"), __LINE__, *FString(""));

	}

	
};
