// Copyright Epic Games, Inc. All Rights Reserved.

#include "First2GameMode.h"

#include <ivehandler.h>

#include "First2HUD.h"
#include "First2Character.h"
#include "SimpleIntf.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AFirst2GameMode::AFirst2GameMode()
	: Super()
{
	////////////////////////////////////////////////////////////////////////// 展示了静态加载的代码

	// ClassFinder的用法1：没有blueprint前缀与.xx
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// ClassFinder的用法2：具有blueprint前缀，并且使用 X.X_C 的格式去加载
	static ConstructorHelpers::FClassFinder<APawn> C(TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter.FirstPersonCharacter_C'"));
	DefaultPawnClass = C.Class;


	// StaticLoadObject 动态加载 会崩溃：暂时没有理解为什么崩溃，网上说是OK的，可能是在构造函数里才会崩溃吧。
	// [UE4]C++实现动态加载UObject：StaticLoadObject()；以Texture和Material为例-pudn.com https://www.pudn.com/news/628f8299bf399b7f351e34fc.html

	// 找到Class的第三个方法 
	// static ConstructorHelpers::FObjectFinder<APawn> C2(TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter.FirstPersonCharacter'"));
	//		auto justSee=	C2.Object->GetClass();
	// DefaultPawnClass = C2.Object->GetClass();

	// static ConstructorHelpers::FObjectFinder<APawn> C3(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter.FirstPersonCharacter"));
	// auto justSee2 = C3.Object->GetClass();


	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString(""));


	UTexture2D* texture;
	static ConstructorHelpers::FObjectFinder<UTexture2D> theTextureFinder(TEXT("/Game/FirstPerson/Character/Textures/UE4_Mannequin_MAT_MASKA.UE4_Mannequin_MAT_MASKA"));
	if (theTextureFinder.Succeeded())
	{
		texture = theTextureFinder.Object;
	}

	//// 动态加载，使用 FObjectFinderOptional 去寻找object并加载
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UTexture> TextureFinder;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> MaterialFinder;

		FConstructorStatics():
			TextureFinder(TEXT("Texture2D'/Game/FirstPerson/Textures/FirstPersonCrosshair.FirstPersonCrosshair'")),
			MaterialFinder(TEXT("Material'/Game/FirstPerson/Meshes/BaseMaterial.BaseMaterial'"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	auto T = ConstructorStatics.TextureFinder.Get();
	auto M = ConstructorStatics.MaterialFinder.Get();


	// use our custom HUD class
	HUDClass = AFirst2HUD::StaticClass();
}

void AFirst2GameMode::Sample1() const
{
	if (SoftObjectPath1.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__,
		       *FString("1 valid"));
	}
	if (SoftObjectPath2.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__,
		       *FString("2 null"));
	}

	if (SoftObjectPath3.IsAsset())
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__,
		       *FString("3 Asset"));
	}

	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n%s\n    __________________"), __LINE__, *SoftObjectPath4.GetAssetName(), *SoftObjectPath4.GetAssetPathString());

	/* 打印内容示例：
	* LogTemp: Warning: ---[LogByWenjie]E:\pro\First2\First2\Source\First2\First2GameMode.cpp at line 73
	1 valid
		__________________
	LogTemp: Warning: ---[LogByWenjie]E:\pro\First2\First2\Source\First2\First2GameMode.cpp at line 84
	3 Asset
		__________________
	LogTemp: Warning: ---[LogByWenjie]E:\pro\First2\First2\Source\First2\First2GameMode.cpp at line 87
	T_Brick_Clay_Beveled_N
	/Game/StarterContent/Textures/T_Brick_Clay_Beveled_N.T_Brick_Clay_Beveled_N
	 */
}

void AFirst2GameMode::REquestAsyncLoad_Example()
{
	// 案例：GetStreamableManager LoadSynchronous 方式同步加载 softobjectpath
	auto ob1 = UAssetManager::Get().GetStreamableManager().LoadSynchronous(SoftObjectPath3);

	// 案例：GetStreamableManager RequestSyncLoad 同步加载 softobjectpath
	UAssetManager::Get().GetStreamableManager().RequestSyncLoad(SoftObjectPath3);


	const auto handle = FStreamableDelegate::CreateUObject(this, &ThisClass::OnLoadFinish);
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(ArrayOfSoftObjects, handle);
}


void AFirst2GameMode::OnLoadFinish()
{
	for (auto asset : ArrayOfSoftObjects)
	{
		auto SoftObjPtr = FSoftObjectPtr(asset);
		auto Obj = SoftObjPtr.Get();
		if (Obj)
		{
			UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *Obj->GetName());
		}
	}
}

FString AFirst2GameMode::FuncWithParamAndRtv(int32 Number)
{
	return FString::FromInt(Number);
}

FString AFirst2GameMode::FuncWithParamAndRtv_Pure(int32 Number)
{
	return this->FuncWithParamAndRtv(Number);
}

/*调用原生CPP代码*/
void AFirst2GameMode::CallCppInterface(AActor* SomeActor)
{
	UClass* ActorClass = SomeActor->GetClass();

	//如果ActorClass实现了StaticClass
	if (ActorClass->ImplementsInterface(USimpleIntf::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString("implementation!"));

		ISimpleIntf* InterfaceIns = CastChecked<ISimpleIntf>(SomeActor); //看看能不能cast到ISimpleIntf

		// InterfaceIns->Inte
		if (InterfaceIns != NULL)
		{
			InterfaceIns->Interface_cpp(TEXT("wenjiezou"));
		}
	}
}

void AFirst2GameMode::CallNativeEventIntf(AActor*SomeActor)
{
	UClass* ActorClass = SomeActor->GetClass();

	//如果ActorClass实现了StaticClass
	if (ActorClass->ImplementsInterface(USimpleIntf::StaticClass()))
	{
		ISimpleIntf::Execute_NativeInterfa(SomeActor, TEXT("小江村儿"));
	}
}


void AFirst2GameMode::CallImplementationIntf(AActor*SomeActor)
{
	UClass* ActorClass = SomeActor->GetClass();

	//如果ActorClass实现了StaticClass
	if (ActorClass->ImplementsInterface(USimpleIntf::StaticClass()))
	{
		ISimpleIntf::Execute_ImplementableInterf(SomeActor, TEXT("小江村儿"));
	}
}

void AFirst2GameMode::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFirst2GameMode::TimerFunction, 2.f, true);
	GetWorld()->GetTimerManager().PauseTimer(TimerHandle);
	GetWorld()->GetTimerManager().UnPauseTimer(TimerHandle);
	/*
	    打印结果示意：
		LogTemp: Warning: ---[LogByWenjie]E:\pro\First2\First2\Source\First2\First2GameMode.cpp at line 145
		tick per 1 s
			__________________
		LogTemp: Warning: ---[LogByWenjie]E:\pro\First2\First2\Source\First2\First2GameMode.cpp at line 145
		tick per 1 s
			__________________
		LogTemp: Warning: ---[LogByWenjie]E:\pro\First2\First2\Source\First2\First2GameMode.cpp at line 145
		tick per 1 s
			__________________
	*/
}

void AFirst2GameMode::TimerFunction()
{
	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *FString("tick per 1 s"));
}
