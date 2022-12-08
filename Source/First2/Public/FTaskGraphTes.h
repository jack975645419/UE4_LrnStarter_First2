// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * [UE4 C++入门到进阶]9.多线程图文(2/3) - 哔哩哔哩 https://www.bilibili.com/read/cv9486649
 * Graph是指什么？
 * 一种简单的多线程写法
 *
 * 1. Graph类不需要继承任何类
 * 2. 构造函数的参数是任意的
 * 3. 必须具有 DoTask / GetStatId / GetDesiredThread / GetSubsequentsMode
 * 4. 固定的写法见代码后面
 * 5. 调用的写法也是固定的，见First2GameMode.h 里的写法 TGraphTask<FTaskGraphTes>::CreateTask(NULL, ENamedThreads::AnyNormalThreadNormalTask).ConstructAndDispatchWhenReady(10)
 */
class FIRST2_API FTaskGraphTes
{
public:
	FTaskGraphTes(int32 TotalCount) : TotalCount(TotalCount)
	{
	}

	~FTaskGraphTes();
	int32 TotalCount;
	int32 CurrentCount = 0;

	/*
	 当传入3时，成功打印
		LogTemp: Warning: ---[LogByWenjie]E:\pro\First2\First2\Source\First2\Public\FTaskGraphTes.h at line 21
		0
			__________________
		LogTemp: Warning: ---[LogByWenjie]E:\pro\First2\First2\Source\First2\Public\FTaskGraphTes.h at line 21
		1
			__________________
		LogTemp: Warning: ---[LogByWenjie]E:\pro\First2\First2\Source\First2\Public\FTaskGraphTes.h at line 21
		2
			__________________
	*/
	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& GraphEvent)
	{
		while (CurrentCount < TotalCount)
		{
			UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s%d\n    __________________"), __LINE__, *FString(""), CurrentCount);

			CurrentCount++;
			FPlatformProcess::Sleep(1.f);
		}
	}

#pragma region 固定的写法，暂不深究

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTaskGraphTes, STATGROUP_TaskGraphTasks);
	}

	// 表示次graph在哪一个线程里执行
	// GameThread会阻塞主线程
	static ENamedThreads::Type GetDesiredThread() { return ENamedThreads::AnyNormalThreadNormalTask; }

	//该函数照写即可,目前暂未发现其他用途
	static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::TrackSubsequents; }

#pragma endregion 固定的写法
};
