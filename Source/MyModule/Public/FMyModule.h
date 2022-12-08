#pragma once
#include "Modules/ModuleManager.h"

class FMyModule : public IModuleInterface
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
