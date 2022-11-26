// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "First2GameMode.generated.h"


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
};
