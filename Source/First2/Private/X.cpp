// Fill out your copyright notice in the Description page of Project Settings.


#include "X.h"

// Sets default values
AX::AX()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere_asRoot"));
	ConeMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cone"));
	CubeMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));


	auto R = RootComponent = SphereMeshComp;
	ConeMeshComp->SetupAttachment(R);
	CubeMeshComp->SetupAttachment(ConeMeshComp);


	auto ConeAssetPath = TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone'");
	auto CubeAssetPath = TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'");
	auto SphereAssetPath = TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'");

	// UStaticMesh是object，资产都是object
	ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(SphereAssetPath);
	if (SphereMeshAsset.Succeeded())
	{
		SphereMeshComp->SetStaticMesh(SphereMeshAsset.Object); // set上去相当于在蓝图中给它设置
	}
	ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(CubeAssetPath);
	if (CubeMeshAsset.Succeeded())
	{
		CubeMeshComp->SetStaticMesh(CubeMeshAsset.Object);
	}
	ConstructorHelpers::FObjectFinder<UStaticMesh> ConeMeshAsset(ConeAssetPath);
	if (ConeMeshAsset.Succeeded())
	{
		ConeMeshComp->SetStaticMesh(ConeMeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void AX::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AX::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AX::Interface_cpp(const FString str)
{
	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s\n    __________________"), __LINE__, *str);

}

void AX::NativeInterfa_Implementation(const FString& str)
{
	// 不需要调 ISimpleIntf::NativeInterfa_Implementation(str);

	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s NativeInterfa_Implementation \n    __________________"), __LINE__, *str);

}

