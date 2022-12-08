#pragma once
// #include "PrivaClassInFirstModule.h" // 无法包括别的模块的私有文件
#include "First2/Backyarm/PrivaClassInFirstModule.h" // 除非写它的路径全称，不推荐这么做，破坏了模块间的私有性概念的初衷

#include "PublicClasInFirstModule.h"
#include "X.h"
#include "Modules/ModuleManager.h"


class FMyModule : public IModuleInterface
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void HELLO_VISIS_ClassNotInIncludings ()
	{
		 UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s%d%d\n    __________________"), __LINE__, *FString(""), AX::C, PublicClasInFirstModule::PIG,

		 PrivaClassInFirstModule::BACK_YARM_CONST // 包含了其它模块的文件的路径全称后，则可以使用，但是不推荐这么做。
		 );
	}
};
