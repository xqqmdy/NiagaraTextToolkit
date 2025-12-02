// Copyright Epic Games, Inc. All Rights Reserved.

// NiagaraTextToolkit.cpp

#include "NiagaraTextToolkit.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FNiagaraTextToolkitModule"

void FNiagaraTextToolkitModule::StartupModule()
{
    // No shader mapping here anymore – handled by NiagaraTextToolkitVertexFactory module.
}

void FNiagaraTextToolkitModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FNiagaraTextToolkitModule, NiagaraTextToolkit)