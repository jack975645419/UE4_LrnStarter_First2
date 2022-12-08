// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AAA.h" // 不清楚为什么不需要标注First/Public/AAA.h
#include "MyModule/Public/CanUsePrivInFirst.h"

/**
 * 
 */
class M3_API VisitorInM3
{
private:
	void TryToVisiPri()
	{
		AAA::Hi2();
		CanUsePrivInFirst::foo2();
 
		

		
	}
public:
	VisitorInM3();
	~VisitorInM3();

	void TryToVisit()
	{
		AAA::Hi2();
		CanUsePrivInFirst::foo2();
		
	}
};
