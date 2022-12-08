// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class FIRST2_API FMyThreadTest : public FRunnable
{
public:
	//线程的实例
	FRunnableThread* ThreadIns;
	//线程的名称
	FString ThreadName;

	//FRunnable的实例，复写了Init Run Stop Exit等
	FMyThreadTest* RunnableIns;

	//FEvent 指针，用来挂起和激活线程
	FEvent* ThreadEvent;

	//用于控制线程挂起和激活
	bool PausingThread = false;
	bool Runing = true;

	FMyThreadTest(const FString& Name)
		: ThreadName(Name)
	{
		ThreadEvent = FPlatformProcess::GetSynchEventFromPool(); //创建事件的方法非常剥离，ThreadEvent和ThreadIns的创建都是分离的，没有关联的样子。
		ThreadIns = FRunnableThread::Create(this, *Name, 0, TPri_BelowNormal);
	}

	~FMyThreadTest()
	{
		delete ThreadIns;
		ThreadIns = nullptr;
		delete ThreadEvent;
		ThreadEvent = nullptr;
	}

	virtual bool Init() override;

	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;
	void PauseThread();
	void WakeUpThread();
	void EnsureCompletion();

	//下面是静态方法，是另外一种线程化的方法
	static FMyThreadTest* JoyInit(const FString& ThreadName);
	static void Suspend(const FString& ThreadName, bool Suspend, bool UseSuspend);
	static bool IsThreadFinished(const FString& ThreadName);
	static void Shutdown(const FString& ThreadName);


	static TMap<FString, FMyThreadTest*> GetThreadMap() { return ThreaMap; }

private:
	static TMap<FString, FMyThreadTest*> ThreaMap;
	//静态变量不允许原地赋值	static TMap< FString, FMyThreadTest* > ThreadMap=TMap<FString, FMyThreadTest*> {};
};
