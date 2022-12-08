// Fill out your copyright notice in the Description page of Project Settings.


#include "FMyThreadTest.h"


TMap<FString, FMyThreadTest*> FMyThreadTest::ThreaMap = TMap<FString, FMyThreadTest*>{};


bool FMyThreadTest::Init()
{
	// 没有特别需要执行的初始化逻辑
	return true;
}

uint32 FMyThreadTest::Run()
{
	FPlatformProcess::Sleep(0.03);
	int IntVar = 0;
	while (Runing)
	{
		if (PausingThread)
		{
			ThreadEvent->Wait();
		}
		//主要的线程逻辑，例如每隔1s打印一句话
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString(""));
		IntVar ++;
		FPlatformProcess::Sleep(1.f);
	}
	return 0;
}

void FMyThreadTest::Stop()
{
	Runing = false; //通过自定义的方式确保Run方法不再执行
}

//退出后自动执行的逻辑
void FMyThreadTest::Exit()
{
	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    Exiting __________________"), __LINE__, *FString(""));
}

void FMyThreadTest::PauseThread()
{
	PausingThread = true; // 让Run方法中的ThreadEvent调用Wait 接口
}

void FMyThreadTest::WakeUpThread()
{
	PausingThread = false;
	ThreadEvent->Trigger();
}

// 包含停止的功能，应该是安全stop掉的功能
void FMyThreadTest::EnsureCompletion()
{
	Stop();
	ThreadIns->WaitForCompletion();
}

FMyThreadTest* FMyThreadTest::JoyInit(const FString& ThreadName)
{
	FMyThreadTest* PNW = nullptr;
	if (FPlatformProcess::SupportsMultithreading())
	{
		PNW = new FMyThreadTest(ThreadName); //新建一个实例，也不用主动调用Run，就会开始Run了
		ThreaMap.Add(ThreadName, PNW);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString("not support multithreading "));
	}
	return PNW;
}

void FMyThreadTest::Suspend(const FString& ThreadName, bool Suspend, bool UseSuspend)
{
	if (FMyThreadTest* Runnable = *ThreaMap.Find(ThreadName))
	{
		//使用FRunnableThread线程原生接口
		if (UseSuspend)
		{
			//线程实例 FRunnableThread* ThreadIns 的suspend接口
			//		true是激活，false是挂起
			Runnable->ThreadIns->Suspend(Suspend);
			return;
		}
		//使用封装方式暂停（ThreadEvent）
		if (Suspend)
		{
			Runnable->PauseThread();
			return;
		}
		//使用封装方式唤醒（ThreadEvent）
		Runnable->WakeUpThread();
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString("Just Return"));
}

bool FMyThreadTest::IsThreadFinished(const FString& ThreadName)
{
	if (const FMyThreadTest* Runnable = *ThreaMap.Find(ThreadName))
	{
		return Runnable->Runing;
	}
	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s %s\n    __________________"), __LINE__, *FString("NOT EXIST"), *ThreadName);
	return false;
}

void FMyThreadTest::Shutdown(const FString& ThreadName)
{
	if (ThreaMap.Find(ThreadName))
	{
		auto Runnable = *ThreaMap.Find(ThreadName);
		Runnable->EnsureCompletion(); //此时已经走完了Exit方法了
		ThreaMap.Remove(ThreadName);
	}
}
