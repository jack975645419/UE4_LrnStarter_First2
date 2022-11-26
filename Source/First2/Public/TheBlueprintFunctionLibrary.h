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
};
