// Property of Lucian Tranc

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Engine/Font.h"
#include "Engine/Texture2D.h"
#include "Engine/DataAsset.h"
#include "NTPDataInterface.h"
#include "NiagaraTextParticlesHelpers.generated.h"

UCLASS()
class NIAGARATEXTPARTICLES_API UNiagaraTextParticlesHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Niagara Text Particles Plugin", meta = (DisplayName = "Set Niagara Variable (NTP Text)"))
	static void SetNiagaraNTPTextVariable(UNiagaraComponent* System, FString TextToDisplay);

	UFUNCTION(BlueprintCallable, Category = "Niagara Text Particles Plugin", meta = (DisplayName = "Set Niagara Variable (NTP Font)"))
	static void SetNiagaraNTPFontVariable(UNiagaraComponent* System, UFont* Font);

private:

	static UNTPDataInterface* FindNTPDataInterface(UNiagaraComponent* System);

};
