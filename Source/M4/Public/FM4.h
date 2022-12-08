#pragma once

#include "Modules/ModuleManager.h"


class FM4 : public IModuleInterface
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};
