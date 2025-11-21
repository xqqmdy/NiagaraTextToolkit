// Copyright Epic Games, Inc. All Rights Reserved.

#include "NiagaraTextParticlesEditor.h"
#include "NiagaraSettings.h"

#define LOCTEXT_NAMESPACE "FNiagaraTextParticlesEditorModule"

void FNiagaraTextParticlesEditorModule::StartupModule()
{
	// Editor-only startup logic for NiagaraTextParticles

	// Register ESpawnTextParticleMode as a Niagara Additional Parameter Enum
	if (UNiagaraSettings* NiagaraSettings = GetMutableDefault<UNiagaraSettings>())
	{
		const FSoftObjectPath EnumPath(TEXT("/NiagaraTextParticles/Enums/ESpawnTextParticleMode.ESpawnTextParticleMode"));

		if (!NiagaraSettings->AdditionalParameterEnums.Contains(EnumPath))
		{
			NiagaraSettings->AdditionalParameterEnums.Add(EnumPath);
			NiagaraSettings->SaveConfig();
		}
	}
}

void FNiagaraTextParticlesEditorModule::ShutdownModule()
{
	// Editor-only shutdown logic for NiagaraTextParticles can be added here
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FNiagaraTextParticlesEditorModule, NiagaraTextParticlesEditor)


