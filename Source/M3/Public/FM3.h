#pragma once

#include "Modules/ModuleManager.h"


class FM3 : public IModuleInterface
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};
