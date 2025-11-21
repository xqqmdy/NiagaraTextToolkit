// Property of Lucian Tranc

#include "NiagaraTextParticlesHelpers.h"

#include "NiagaraComponent.h"
#include "NiagaraTypes.h"
#include "NiagaraUserRedirectionParameterStore.h"
#include "NTPDataInterface.h"

void UNiagaraTextParticlesHelpers::SetNiagaraNTPTextVariable(UNiagaraComponent* System, FString TextToDisplay)
{
	UNTPDataInterface* FoundDI = FindNTPDataInterface(System);

	if (FoundDI)
	{
		FoundDI->InputText = TextToDisplay;

		// Only reinitialize if the component is currently active
		if (System && System->IsActive() && System->GetSystemInstanceController())
		{
			System->ReinitializeSystem();
		}
	}
}

void UNiagaraTextParticlesHelpers::SetNiagaraNTPFontVariable(UNiagaraComponent* System, UFont* Font)
{
	UNTPDataInterface* FoundDI = FindNTPDataInterface(System);

	if (FoundDI)
	{
		FoundDI->FontAsset = Font;

		// Only reinitialize if the component is currently active
		if (System && System->IsActive() && System->GetSystemInstanceController())
		{
			System->ReinitializeSystem();
		}
	}
}

UNTPDataInterface* UNiagaraTextParticlesHelpers::FindNTPDataInterface(UNiagaraComponent* System)
{
	if (!System)
	{
		UE_LOG(LogNiagaraTextParticles, Warning, TEXT("FontFXHelpers: Niagara component is null"));
		return nullptr;
	}

	UNTPDataInterface* FoundDI = nullptr;

	FNiagaraUserRedirectionParameterStore& Overrides = System->GetOverrideParameters();
	TArray<FNiagaraVariable> OutParameters;

	Overrides.GetUserParameters(OutParameters);

	const int32 NumParams = OutParameters.Num();
	for (int32 i = 0; i < NumParams; ++i)
	{
		const FNiagaraVariable& Var = OutParameters[i];
		if (Var.GetType() == FNiagaraTypeDefinition(UNTPDataInterface::StaticClass()))
		{
			if (UNiagaraDataInterface* DI = Overrides.GetDataInterface(Var))
			{
				FoundDI = Cast<UNTPDataInterface>(DI);
				if (FoundDI)
				{
					UE_LOG(LogNiagaraTextParticles, Log, TEXT("FontFXHelpers: Found NTP DI on component overrides: %s (Param: %s)"),
						*GetNameSafe(FoundDI), *Var.GetName().ToString());
					break;
				}
			}
		}
	}

	if (!FoundDI)
	{
		UE_LOG(LogNiagaraTextParticles, Warning, TEXT("FontFXHelpers: No UNTPDataInterface user variable found on component or system"));
		return nullptr;
	}

	return FoundDI;
}