// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleIntf.h"
#include "GameFramework/Actor.h"
#include "X.generated.h"

UCLASS()
class FIRST2_API AX : public AActor , public ISimpleIntf
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
	virtual  void Interface_cpp(const FString str) override;

	virtual void NativeInterfa_Implementation(const FString& str) override;
};
