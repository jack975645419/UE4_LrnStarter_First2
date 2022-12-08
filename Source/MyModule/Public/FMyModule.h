#pragma once
#include "Modules/ModuleManager.h"
#include "First2/Public/X.h"
class FMyModule : public IModuleInterface
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void HELLO_VISIS_ClassNotInIncludings ()
	{
		 UE_LOG(LogTemp, Warning, TEXT("---[LogByWenjie]" __FILE__ " at line %d\n%s%d%d\n    __________________"), __LINE__, *FString(""), AX::C, 1 /*  privatclass PrivatClassInFirst::SHEEP*/);
	}
};
