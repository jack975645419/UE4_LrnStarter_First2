// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "FMyThreadTest.h"
#include "SimpleIntf.h"
#include "GameFramework/Actor.h"
#include "X.generated.h"

UCLASS()
class FIRST2_API AX : public AActor, public ISimpleIntf
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AX();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* SphereMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* ConeMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* CubeMeshComp;

	//virtual表示这是虚函数，是接口的实现
	virtual void Interface_cpp(const FString str) override;

	virtual void NativeInterfa_Implementation(const FString& str) override;


	/*
	 * 用于观察 meta=(WorldContext="Obj") 的功能
	 */
	int32 Blood = 100;

	// 在蓝图中会产生一个self接口引入，因为其中需要用到成员变量如Blood
	// 有self引脚
	UFUNCTION(BlueprintCallable)
	void Example_CallableFunc(const FString& str)
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s blood %d\n    __________________"), __LINE__, *str, Blood);
	}

	// 避免展示一个self接口，因为如果用到WorldContext，则会通过Obj来使用
	// 并不是标记了 WorldContext="Obj" ，就不需要在方法里写 GetWorldFromContextObject，它只是给人看的
	// 有self引脚
	UFUNCTION(BlueprintCallable, meta=(WorldContext="Obj", CallableWithoutWorldContext))
	void Example_CallableFunc_CallableWithoutWorldContext(const FString& str, UObject* Obj)
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *str);
		UWorld* World = GEngine->GetWorldFromContextObject(Obj, EGetWorldErrorMode::LogAndReturnNull);
		if (World)
		{
			for (TActorIterator<AX> It(World, AX::GetClass()); It; ++It)
			{
				AActor* Actor = *It;
				UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *Actor->GetName());
				// 成员访问符要优先于取地址符&与取值符* 
			}
		}
	}
	
	// 避免展示一个self接口，因为存在 CallableWithoutWorldContext 标记
	// 有self引脚
	UFUNCTION(BlueprintCallable, meta=(CallableWithoutWorldContext))
	void Example_CallableFunc_WithoutWorldContext_WithoutMark(const FString& str)
	{
		UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s " __FUNCTION__ "\n    __________________"), __LINE__, *str); // __FUNCTION__ 用来表示函数名
	}



	
	

	
	
};
