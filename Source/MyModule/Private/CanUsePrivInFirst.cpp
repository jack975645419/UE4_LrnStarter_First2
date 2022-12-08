// Fill out your copyright notice in the Description page of Project Settings.


#include "CanUsePrivInFirst.h"

// #include "PrivaClassInFirstModule.h" 无法添加

CanUsePrivInFirst::CanUsePrivInFirst()
{
	UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s%d\n    __________________"), __LINE__, *FString(""),
	1, // PrivaClassInFirstModule::BACK_YARM_CONST, //注定添加失败，即使 First2 模块在 MyModule的 PrivateDependencyModuleNames 私有依赖模块中
	2 // PrivatClassInFirst::SHEEP //注定添加失败，即使 First2 模块在 MyModule的 PrivateDependencyModuleNames 私有依赖模块中
	);

}

CanUsePrivInFirst::~CanUsePrivInFirst()
{
}
