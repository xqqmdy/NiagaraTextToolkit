// Copyright Epic Games, Inc. All Rights Reserved.

#include "NiagaraTextToolkitEditor.h"
#include "NiagaraSettings.h"
#include "NiagaraEditorModule.h"
#include "NiagaraRendererProperties.h"
#include "NTTNiagaraTextRendererProperties.h"

#define LOCTEXT_NAMESPACE "FNiagaraTextToolkitEditorModule"

void FNiagaraTextToolkitEditorModule::StartupModule()
{
	// Editor-only startup logic for NiagaraTextToolkit

	// Register ESpawnTextParticleMode as a Niagara Additional Parameter Enum
	if (UNiagaraSettings* NiagaraSettings = GetMutableDefault<UNiagaraSettings>())
	{
		const FSoftObjectPath EnumPath(TEXT("/NiagaraTextToolkit/Enums/ESpawnTextParticleMode.ESpawnTextParticleMode"));

		if (!NiagaraSettings->AdditionalParameterEnums.Contains(EnumPath))
		{
			NiagaraSettings->AdditionalParameterEnums.Add(EnumPath);
			NiagaraSettings->SaveConfig();
		}
	}

	FNiagaraEditorModule& NiagaraEditorModule = FModuleManager::LoadModuleChecked<FNiagaraEditorModule>("NiagaraEditor");

	FNiagaraRendererCreationInfo RendererInfo;
	RendererInfo.DisplayName = NSLOCTEXT("Niagara", "NTTTextRenderer", "Text Renderer");
	RendererInfo.Description = NSLOCTEXT("Niagara", "NTTTextRendererDesc", "Text renderer specialized for Niagara Text Toolkit.");
	RendererInfo.RendererClassPath = FTopLevelAssetPath(TEXT("/Script/NiagaraTextToolkit.UNTTNiagaraTextRendererProperties"));
	RendererInfo.RendererFactory = FNiagaraRendererCreationInfo::FRendererFactory::CreateLambda(
		[](UObject* Outer) -> UNiagaraRendererProperties*
		{
			return NewObject<UNTTNiagaraTextRendererProperties>(Outer ? Outer : GetTransientPackage());
		}
	);

	NiagaraEditorModule.RegisterRendererCreationInfo(RendererInfo);
}

void FNiagaraTextToolkitEditorModule::ShutdownModule()
{
	// Editor-only shutdown logic for NiagaraTextToolkit can be added here
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FNiagaraTextToolkitEditorModule, NiagaraTextToolkitEditor)


