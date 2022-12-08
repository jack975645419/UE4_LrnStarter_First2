// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * [UE4 C++入门到进阶]9.多线程图文(3/3) - 哔哩哔哩 https://www.bilibili.com/read/cv9488877
 *
 * 1. 不管是手动还是自动销毁，都继承自 FNonAbandonableTask，它是多线程比喻里的汽油
 * 2. 主要的逻辑是在 DoWork()
 * 3. 需要实现 GetStatId
 * 4. 调用的逻辑见First2GameMode里的 Excample_FNonAban1 等方法
 * 5. 需要声明友元关系
 * 6. new出来的方法是 FAutoDeleteAsyncTask<FADTask>(3) 允许传参
 * 7. 前台调和后台调的方式是 StartSynchronousTask / StartBackgroundTask，其中前台调用会卡死当前主线程
 * 8. 封装层是FAsyncTask/FAutoDeleteAsyncTask （比喻里的汽车） 
 */
class FIRST2_API FMDTask : public FNonAbandonableTask
{
public:
	friend class FAsyncTask<FMDTask>;

	~FMDTask()
	{
	}

	FMDTask(int32 TotalCount)
		: TotalCount(TotalCount)
	{
	}

	int32 TotalCount;
	int32 CurrentCount = 0;

	void DoWork()
	{
		while (CurrentCount < TotalCount)
		{
			UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s%d\n    __________________"), __LINE__, *FString(""), CurrentCount);
			CurrentCount++;
			FPlatformProcess::Sleep(1.0f);
		}
	}

	// 固定写法，使用的是线程池
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FAutoDeleteTask, STATGROUP_ThreadPoolAsyncTasks);
	}
};
