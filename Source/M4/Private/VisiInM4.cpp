// Fill out your copyright notice in the Description page of Project Settings.


#include "M4/Public/VisiInM4.h"

#include "AAA.h"
#include "First2/Backyarm/PrivaClassInFirstModule.h"
#include "MyModule/Public/CanUsePrivInFirst.h"


// Sets default values
AVisiInM4::AVisiInM4()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
AAA::Hi2();
	CanUsePrivInFirst::foo2();
	PrivaClassInFirstModule::BACK_YARM_CONST;
	
}

// Called when the game starts or when spawned
void AVisiInM4::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVisiInM4::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

