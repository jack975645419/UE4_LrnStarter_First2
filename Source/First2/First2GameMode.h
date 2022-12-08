// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FMDTask.h"
#include "FMyThreadTest.h"
#include "FTaskGraphTes.h"
#include "RawClas.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "First2GameMode.generated.h"

// DEFINE_LOG_CATEGORY(TheGMLog) 和 DECLARE_LOG_CATEGORY_EXTERN(TheGMLog, Log, All) 是配套使用的
DECLARE_LOG_CATEGORY_EXTERN(TheGMLog, Log, All)

UENUM(BlueprintType)
enum class ECppScoreLevel:uint8
{
	A UMETA(DisplayName="优秀"),
	B UMETA(DisplayName="良好"),

	C UMETA(DisplayName="一般"),
	D UMETA(DisplayName="差"),
};


UCLASS(minimalapi)
class AFirst2GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFirst2GameMode();
	void Sample1() const;
	void REquestAsyncLoad_Example();
	void BeginPlay();

	//下面若干关于指针的用法，来自于 【UE4 C++ 基础知识】<15> 智能指针 TSharedPtr、UniquePtr、TWeakPtr、TSharedRef - 知乎 https://zhuanlan.zhihu.com/p/472486869
	TSharedPtr<RawClas> HowToUseSharedPtr();
	void HowToUseSharedRef();
	void HowToUseTWeakPtr();
	void HowToUseUniquePtr();
	void CastExample();
	void HowToCastRef();
	//指针用法结束
	
	void HowToUseTMapAndTSet();
	void TimerFunction();
	void OnLoadFinish();


	/*
	 * 下面代码的来源是： 【UE4 C++ 基础知识】<10>资源的引用 - 知乎 https://zhuanlan.zhihu.com/p/473636881
	 * 
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SoftObject", meta=(AllowedClasses="SkeletalMesh, StaticMesh"))
	FSoftObjectPath SoftObjectPath1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SoftObject", meta=(AllowedClasses="Texture2D"))
	FSoftObjectPath SoftObjectPath2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SoftObject", meta=(AllowedClasses="Blueprint Class")) //
	FSoftObjectPath SoftObjectPath3;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SoftObject", meta=(AllowedClasses="Texture2D")) // 不推荐写自定义的类型
	FSoftObjectPath SoftObjectPath4;


	/*
	 * LoadClass 和UGameplayStatics::GetAllActorsOfClass 的结合使用，打印某类的所有实体
	 * 代码来源
	 * [UE4 C++入门到进阶] 8.同步or异步资源加载 - 哔哩哔哩 https://www.bilibili.com/read/cv9950778/
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="AssetLoadActor")
	TArray<FSoftObjectPath> ArrayOfSoftObjects;


	UFUNCTION(Exec)
	void Test_PrintAllActor()
	{
		auto TargetClass = LoadClass<AActor>(NULL, TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter.FirstPersonCharacter_C"));

		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), TargetClass->GetClass(), OutActors);
		for (auto EachOne : OutActors)
		{
			UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, * EachOne->GetName());
		}
	}

	// 使用枚举值的示例
	UFUNCTION(Exec)
	void Test_Enum()
	{
		ECppScoreLevel ScoreLEvel = ECppScoreLevel::A;
		if (ScoreLEvel == ECppScoreLevel::A)
		{
			UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString("print A welldone"));
		}

		switch (ScoreLEvel)
		{
		case ECppScoreLevel::A:
			UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString("A"));
			break;
		case ECppScoreLevel::B:
			UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString("B"));
			break;
		case ECppScoreLevel::C: break;
		case ECppScoreLevel::D: break;
		default: break;
		}
	}

	// SpawnActor的示例
	UFUNCTION(Exec)
	void SpawnActor_Example()
	{
		auto SomeClass = LoadClass<AActor>(NULL,TEXT("Blueprint'/Game/StarterContent/Blueprints/Blueprint_Effect_Fire.Blueprint_Effect_Fire_C'"));
		if (SomeClass)
		{
			auto SpawnTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::OneVector);
			GetWorld()->SpawnActor<AActor>(SomeClass, SpawnTransform);
		}
	}


	/*
	C++函数的使用示例
	*/

	// BlueprintCallable 示例
	UFUNCTION(BlueprintCallable, Category=UFTest)
	FString FuncWithParamAndRtv(int32 Number);

	UFUNCTION(BlueprintPure, Category=UFTest)
	FString FuncWithParamAndRtv_Pure(int32 Number);

	// 必须蓝图重写，C++补充些
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="BeginPlay2"))
	void ReceiveBeginPlay2();
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="BeginPlay2"))
	void ReceiveBeginPlay_Ret(const FString& var);
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="BeginPlay2"))
	void ReceiveBeginPlay_InputAndOutput(int32 x, const FString& var);


	UPROPERTY(EditAnywhere)
	AActor* SomeMrX;

	UFUNCTION(BlueprintCallable)
	void CallCppInterface(AActor* SomeActor);
	void CallNativeEventIntf(AActor* SomeActor);
	void CallImplementationIntf(AActor* SomeActor);
	void TimerWorkExample();
	void HowToNewObject();
	void HowToUseCustomLog();


	// 下面的代码是线程相关的，配合FMyThreadTest 
	// [UE4 C++入门到进阶]9.多线程图文(1/3) - 哔哩哔哩 https://www.bilibili.com/read/cv9453564
	UFUNCTION(Exec)
	virtual void RunThread(const FString& ThreadName)
	{
		FMyThreadTest::JoyInit(ThreadName);
	}

	UFUNCTION(Exec)
	virtual void ShutdownThread(const FString& ThreadName)
	{
		FMyThreadTest::Shutdown(ThreadName);
	}

	UFUNCTION(Exec)
	virtual void Suspend(const FString& ThreadName, bool Suspend, bool UseSuspend)
	{
		FMyThreadTest::Suspend(ThreadName, Suspend, UseSuspend);
	}


	// 下面是 graph task 的用法
	UFUNCTION(Exec)
	void CreateTaskGraph() const
	{
		TGraphTask<FTaskGraphTes>::CreateTask(NULL, ENamedThreads::AnyNormalThreadNormalTask).ConstructAndDispatchWhenReady(10);
	}


	// 下面是 FNonAbandonableTask 的用法示例
	UFUNCTION(Exec)
	void Excample_FNonAban1();
	UFUNCTION(Exec)
	void Excample_FNonAban_Manu();
	void TryToRecollectTask();

	FAsyncTask<FMDTask>* ManTask;

	UFUNCTION(Exec)
	void Excample_FNonAban1_BG();
	UFUNCTION(Exec)
	void Excample_FNonAban_Manu_BG();

	FTimerHandle TH;


	// 下面是 TSubClassOf的使用示例

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor>TargetClas;

	UFUNCTION(Exec)
	void XMLReadExampl();

	UFUNCTION(Exec)
	void JsonReadExample();

	TSharedPtr<FJsonObject> JsonOb;
	TSharedPtr<FJsonObject> JsonObBig;

	TArray<TSharedPtr<  FJsonValue >> JsonValueArray;



















	
};
