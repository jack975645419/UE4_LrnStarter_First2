// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <d3d12sdklayers.h>
#include <string>

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "FunLibr.generated.h"

/**
 * 
 */
UCLASS()
class FIRST2_API UFunLibr : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	// 成功验证老式接口 StaticLoadObject
	UFUNCTION(BlueprintCallable, meta=(DisplayName="StaticLoadObject_ByPath", keywords="Load"), Category="LoadFile")
	static UTexture2D* LoadTextureFromPath(const FString& Path)
	{
		if (Path.IsEmpty())
		{
			return NULL;
		}
		return Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *(Path)));
	}

	// 成功验证LoadObject
	UFUNCTION(BlueprintCallable, meta=(DisplayName="LoadTextureFromPath_2", keywords="Load"), Category="LoadFile")
	static UTexture2D* LoadTextureFromPath_2()
	{
		// 为了简便验证，这里就直接硬编码
		auto Tex = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/FirstPerson/Textures/FirstPersonCrosshair.FirstPersonCrosshair'"));
		return Tex;
	}


	/* 成功验证FindObject。因为有言论说：如果资源已经加载完毕，使用 FindObject<>()。如果资源还没加载，使用 LoadObject<>()。
	 * 尝试重启UE4后首次使用，结果返回的是非空的值。
	 * */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="FindTextureFromPath", keywords="Load"), Category="LoadFile")
	static UTexture2D* FindTextureFromPath(const FString& Path)
	{
		if (Path.IsEmpty())
		{
			return NULL;
		}

		auto Tex = FindObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/FirstPerson/Textures/FirstPersonCrosshair.FirstPersonCrosshair'"));
		return Tex;
	}

	// FObjectFinder 没有验证成功。因为 只能在类的构造函数中使用，否则会crash
	UFUNCTION(BlueprintCallable, meta=(DisplayName="UseTextureFinder", keywords="Load"), Category="LoadFile")
	static UTexture2D* UseTextureFinder()
	{
		// 为了简便验证，这里就直接硬编码
		auto X = ConstructorHelpers::FObjectFinder<UTexture2D>(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));

		return X.Object;
	}

	// FClassFinder 没有验证成功。因为 只能在类的构造函数中使用，否则会crash
	UFUNCTION(BlueprintCallable, meta=(DisplayName="UseClassFinder", keywords="Load"), Category="LoadFile")
	static TSubclassOf<UAnimBlueprint> UseClassFinder()
	{
		auto X = ConstructorHelpers::FClassFinder<UAnimBlueprint>(TEXT("/Game/FirstPerson/Animations/FirstPerson_AnimBP"));

		if (!X.Succeeded())
		{
			UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString("X is empty"));
			return NULL;
		}

		return X.Class;
	}

	// LoadClass 
	UFUNCTION(BlueprintCallable, meta=(DisplayName="UseLoadClass", keywords="Load"), Category="LoadFile")
	static UClass* UseLoadClass()
	{
		auto Test = LoadClass<AActor>(NULL, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter.FirstPersonCharacter_C'"));
		return Test;
	}


	// 打印Package中的所有的资源的示例
	UFUNCTION(BlueprintCallable, meta=(DisplayName="PrintAllAssetsInPackage", keywords="Load"), Category="LoadFile")
	static FString PrintAllAssetsInPackage(FString Path)
	{
		FString result;
		auto obj = LoadPackage(NULL, *Path, LOAD_None);
		if (obj == nullptr)
		{
			return FString::Printf(TEXT("NotFound:\t%s\n"), *Path);
		}

		result += FString::Printf(TEXT("Loaded:\t%s\n"), *obj->GetFullName());
		result += PrintObject(obj, 1);

		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *result);

		return result;
	}

	static FString PrintObject(const UObject* obj, int tabs)
	{
		if (obj == nullptr)
		{
			return "";
		}

		std::string s(tabs, '\t'); //表示重复
		auto tab = FString(s.c_str());
		FString result = tab + PrintObjectSelf(obj) + "\n";

		if (const UEnum* enumClass = Cast<const UEnum>(obj))
		{
			result += tab + obj->GetFullName() + " [E]\n";
		}
		else if (const UScriptStruct* structClass = Cast<const UScriptStruct>(obj))
		{
			result += tab + obj->GetFullName() + " [S]\n";
		}
		else if (const UClass* classObj = Cast<const UClass>(obj))
		{
			result += tab + obj->GetFullName() + " [C]\n";
		}
		else if (const UFunction* funcObj = Cast<const UFunction>(obj))
		{
			result += tab + obj->GetFullName() + " [F]\n";
		}
		else
		{
			TArray<UObject*> children;

			// 用来获取objects
			GetObjectsWithOuter(obj, children, false);
			for (const auto kid : children)
			{
				result += PrintObject(kid, tabs + 1);
			}
		}

		return result;
	}

	static FString PrintObjectSelf(const UObject* object)
	{
		return object->GetFullName();
	}


	static void TMultiMap_Example()
	{
		// TMultiMap<int32, FString> x = new TMultiMap<int32, FString>();
	}

};
