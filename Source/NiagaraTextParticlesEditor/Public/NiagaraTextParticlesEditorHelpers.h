// Property of Lucian Tranc

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NiagaraTextParticlesEditorHelpers.generated.h"

class UFont;

UCLASS()
class NIAGARATEXTPARTICLESEDITOR_API UNiagaraTextParticlesEditorHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Niagara Text Particles Plugin")
	static bool SaveFontTexturesToAssets(UFont* FontAsset, const FString& FontAssetPath);

private:

	static void ShowSlateNotification(const FText& Message, float Duration = 3.0f);
};


