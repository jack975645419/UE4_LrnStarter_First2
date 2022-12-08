// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class FIRST2_API FADTask:public FNonAbandonableTask
{
public:
	// 在友元类的后面添加上模板定义
	 friend class FAutoDeleteAsyncTask<FADTask>;
	 FADTask(int32 TotalCount)
		: TotalCount(TotalCount)
	{
	}
	~FADTask(){}

	int32 TotalCount;
	int32 CurrentCount = 0;

	void DoWork()
	{
		while(CurrentCount<TotalCount)
		{

			UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s%d\n    __________________"), __LINE__, *FString(""),CurrentCount);
CurrentCount++;
			FPlatformProcess::Sleep(1.0f);
		}
	}

	// 固定写法，使用的是线程池
	FORCEINLINE TStatId GetStatId()const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FAutoDeleteTask, STATGROUP_ThreadPoolAsyncTasks)	;
	}
	
	
};
