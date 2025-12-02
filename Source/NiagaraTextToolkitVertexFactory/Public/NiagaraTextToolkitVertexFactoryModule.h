#pragma once

#include "Modules/ModuleInterface.h"

class FNiagaraTextToolkitVertexFactoryModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};


