// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "TheBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class FIRST2_API UTheBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "CppFunctionLibra")
	static FString GetSystemTime()
	{
		auto Time = UKismetMathLibrary::Now();
		auto Year = FString::FromInt(Time.GetYear());
		auto Month = FString::FromInt(Time.GetMonth());
		auto Day = FString::FromInt(Time.GetDay());
		auto Hour = FString::FromInt(Time.GetHour());
		auto Minute = FString::FromInt(Time.GetMinute());
		return FString::Printf(TEXT("%s-%s-%s / %s:%s"), *Year, *Month, *Day, *Hour, *Minute);
	}

	// 使用 GetWorldFromContextObject 的例子：

	/* 这是引擎中 GetAllActorsOfClass 的代码
	 *void UGameplayStatics::GetAllActorsOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, TArray<AActor*>& OutActors)
	{
		QUICK_SCOPE_CYCLE_COUNTER(UGameplayStatics_GetAllActorsOfClass);
		OutActors.Reset();

		// We do nothing if no is class provided, rather than giving ALL actors!
		if (ActorClass)
		{
			if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			for(TActorIterator<AActor> It(World, ActorClass); It; ++It)
			{
				AActor* Actor = *It;
					OutActors.Add(Actor);
				}
			}
		}
	}*/

	UFUNCTION(BlueprintPure, meta=(WorldContext="Obj"), Category="CppFunctionLibra")
	static TArray<AActor*> Example_GetActorsOfClass(UObject* Obj, TSubclassOf<AActor> Class)
	{
		TArray<AActor*> OutActors{}; // 空列表初始化方式
		if (Class)
		{
			UWorld* World = GEngine->GetWorldFromContextObject(Obj, EGetWorldErrorMode::LogAndReturnNull);
			if (World)
			{
				for (TActorIterator<AActor> It(World, Class); It; ++It)
				{
					AActor* Actor = *It;
					OutActors.Add(Actor);
				}
			}
		}

		return OutActors;
	}


	/*
	 下面三个方法 对应 WorldContext 的使用示例 
● Meta修饰符
meta = (EditCondition = "变量名") 
按条件编辑变量

meta = (WorldContext="Obj", CallableWithoutWorldContext) 
Callable的方法在调用时原本是需要传入self的。而对于蓝图函数库里的静态函数而言，self信息可以默认传调用者作为上下文环境（也就是下面例子中的Obj参数）。在后面列出的蓝图截图中，没有WorldContext记号的，仍然需要主动传obj参数。而有WorldContext标记的，则不需要主动传self了。

	UE4中的Meta修饰符 - 知乎 https://zhuanlan.zhihu.com/p/510192710

	*/

	UFUNCTION(BlueprintCallable, meta=(WorldContext="Obj"), Category="CppFunctionLibra")
	static TArray<AActor*> CallableWithWC(UObject* Obj, TSubclassOf<AActor> Class)
	{
		TArray<AActor*> OutActors{}; // 空列表初始化方式
		if (Class)
		{
			UWorld* World = GEngine->GetWorldFromContextObject(Obj, EGetWorldErrorMode::LogAndReturnNull);
			if (World)
			{
				for (TActorIterator<AActor> It(World, Class); It; ++It)
				{
					AActor* Actor = *It;
					OutActors.Add(Actor);
				}
			}
		}

		return OutActors;
	}

	UFUNCTION(BlueprintCallable, meta=(WorldContext="Obj", CallableWithoutWorldContext), Category="CppFunctionLibra")
	static TArray<AActor*> CallableWithWC2(UObject* Obj, TSubclassOf<AActor> Class)
	{
		TArray<AActor*> OutActors{}; // 空列表初始化方式
		if (Class)
		{
			UWorld* World = GEngine->GetWorldFromContextObject(Obj, EGetWorldErrorMode::LogAndReturnNull);
			if (World)
			{
				for (TActorIterator<AActor> It(World, Class); It; ++It)
				{
					AActor* Actor = *It;
					OutActors.Add(Actor);
				}
			}
		}

		return OutActors;
	}

	UFUNCTION(BlueprintCallable, Category="CppFunctionLibra")
	static TArray<AActor*> CallableNormal(UObject* Obj, TSubclassOf<AActor> Class)
	{
		TArray<AActor*> OutActors{}; // 空列表初始化方式
		// do sth
		return OutActors;
	}

	/* 下面的示例是：容器
	 * [UE4 C++入门到进阶] 7.容器(TArray,TMap,TSet) - 哔哩哔哩 https://www.bilibili.com/read/cv9656511
	 * TArray示例代码
	 */
	UFUNCTION(BlueprintCallable, Category="Containers")
	static void ContainerExample()
	{
		TArray<int32> Arr;
		Arr.Init(2, 5);
		Arr.Add(3);
		Arr.AddUnique(3);
		Arr.Emplace(4);
		Arr = {1, 3, 4, 5};
		Arr.Insert(5, 2);
		TArray<int32> Arr2 = {7, 8, 9, 10, 11};
		Arr.Append(Arr2);
		Arr.Remove(2);
		Arr.RemoveSingle(3);
		// lambda表达式的使用
		Arr.RemoveAll([](int32 x) { return x % 2 == 0; });

		Arr.RemoveAll([](int32 x) -> bool { return x % 10 == 1; }); // lambda需要返回

		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s num is %d %s TOP:%d Last:%d, Last0:%d, Last1:%d. Contain?:%s, Find5:%d, Find50:%d \n    __________________"), __LINE__, *FString(""),
		       Arr.Num(),
		       *FString ( Arr.IsValidIndex(-100)?"valid":"invalid"),
		       //这三个的含义都是等价的，取最后一个，倒数第零个
		       Arr.Top(),
		       Arr.Last(),
		       Arr.Last(0),

		       // 取倒数第一个元素
		       Arr.Last(1),

		    * FString (  Arr.Contains(5)?"Contain":"Not Contains" ),
		       Arr.Find(5),
		       Arr.Find(50)
		);

		for (auto i = 0; i < Arr.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%d\n    __________________"), __LINE__, Arr[i]);
		}
		for (auto i : Arr)
		{
			UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%d\n    __________________"), __LINE__, i);
		}
		for (TArray<int32>::TIterator it = Arr.CreateIterator(); it; ++it)
		{
			UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%d\n    __________________"), __LINE__, *it);
		}
		for (TArray<int32>::TConstIterator cit = Arr.CreateConstIterator(); cit; ++cit)
		{
			UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%d\n    __________________"), __LINE__, *cit);
		}

		Arr.Sort([](const int32& a, const int32& b) { return a > b; });
		// PREDICATE_CLASS x 本质上是类，看到的调用Sort的地方是 IntroSort.h 里的 IntroSortInternal

		Arr.Empty();
		Arr.Reset();
	}
};
