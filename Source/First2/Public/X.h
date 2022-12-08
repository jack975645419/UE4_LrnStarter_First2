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
	AX(); // :CONST_VALU(100) 只能在实现里写

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//关于C++的const和static的初始化
	// https://www.runoob.com/w3cnote/cpp-static-const.html#:~:text=%E5%9C%A8%20C%2B%2B%20%E4%B8%AD%EF%BC%8Cstatic%20%E9%9D%99%E6%80%81%E6%88%90%E5%91%98%E5%8F%98%E9%87%8F%E4%B8%8D%E8%83%BD%E5%9C%A8%E7%B1%BB%E7%9A%84%E5%86%85%E9%83%A8%E5%88%9D%E5%A7%8B%E5%8C%96%E3%80%82,%E5%9C%A8%E7%B1%BB%E7%9A%84%E5%86%85%E9%83%A8%E5%8F%AA%E6%98%AF%E5%A3%B0%E6%98%8E%EF%BC%8C%E5%AE%9A%E4%B9%89%E5%BF%85%E9%A1%BB%E5%9C%A8%E7%B1%BB%E5%AE%9A%E4%B9%89%E4%BD%93%E7%9A%84%E5%A4%96%E9%83%A8%EF%BC%8C%E9%80%9A%E5%B8%B8%E5%9C%A8%E7%B1%BB%E7%9A%84%E5%AE%9E%E7%8E%B0%E6%96%87%E4%BB%B6%E4%B8%AD%E5%88%9D%E5%A7%8B%E5%8C%96%EF%BC%8C%E5%A6%82%EF%BC%9A%20double%20Account%3A%3ARate%20%3D%202.25%3B
	
	const int32 CONST_VALU = 1 ; //构造函数的实现中可以写初始化，但是在声明时必须具有一个默认的初始化
	const int32 CONST_VALU2  = 2; //在声明时可以且必须要有初始化
	static int32 STATIC_VALU ; // 在后面（cpp文件中）初始化
	static int32 STATIC_VALU2; //在声明时，不能够进行初始化
	
	const static int32 C; 
	static const int32 D = 12; //C和D其实是一样的声明，可以在声明时初始化，也可以在后面初始化，也可以不初始化。
	// 总结：包含const则声明时初始化，只包含static时在后面进行初始化
	
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
