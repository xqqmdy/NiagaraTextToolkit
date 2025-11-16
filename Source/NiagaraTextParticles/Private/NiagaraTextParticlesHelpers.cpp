// Property of Lucian Tranc

#include "NiagaraTextParticlesHelpers.h"

#if WITH_EDITOR
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"
#include "AssetToolsModule.h"
#include "Misc/PackageName.h"
#include "Modules/ModuleManager.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "ScopedTransaction.h"
#include "Misc/ScopedSlowTask.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#endif

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

bool UNiagaraTextParticlesHelpers::SaveFontTexturesToAssets(UFont* FontAsset, const FString& FontAssetPath)
{
#if WITH_EDITOR
	if (!FontAsset)
	{
		UE_LOG(LogNiagaraTextParticles, Error, TEXT("UNiagaraTextParticlesHelpers::SaveFontTexturesToAssets: FontAsset is null"));
		return false;
	}

	if (FontAsset->Textures.Num() == 0)
	{
		UE_LOG(LogNiagaraTextParticles, Warning, TEXT("UNiagaraTextParticlesHelpers::SaveFontTexturesToAssets: Font '%s' has no textures"), *FontAsset->GetName());
		return false;
	}

	// Extract package path and base name using FPackageName utilities
	FString PackageName = FPackageName::ObjectPathToPackageName(FontAssetPath);
	FString PackagePathForValidation = FPackageName::GetLongPackagePath(PackageName);
	FString BaseName = TEXT("T_NTP_") + FPackageName::GetShortName(PackageName);

	// Validate package path (without trailing slash)
	FText InvalidPathReason;
	if (!FPackageName::IsValidLongPackageName(PackagePathForValidation, false, &InvalidPathReason))
	{
		UE_LOG(LogNiagaraTextParticles, Error, TEXT("UNiagaraTextParticlesHelpers::SaveFontTexturesToAssets: Invalid package path '%s': %s"), *PackagePathForValidation, *InvalidPathReason.ToString());
		return false;
	}
	
	// Create normalized package path with trailing slash for path construction
	FString NormalizedPackagePath = PackagePathForValidation + TEXT("/");

	// Add "T_" prefix to the base name
	bool bAllSuccessful = true;
	TArray<UObject*> CreatedAssets;

	// Editor transaction for Undo/Redo
	const FScopedTransaction Transaction(NSLOCTEXT("NiagaraTextParticles", "SaveFontTexturesToAssets", "Save Font Textures To Assets"));

	// Progress dialog
	FScopedSlowTask SlowTask(FontAsset->Textures.Num(), NSLOCTEXT("NiagaraTextParticles", "SavingFontTextures", "Saving font textures to assets..."));
	SlowTask.MakeDialog(true /*bShowCancelButton*/);

	for (int32 i = 0; i < FontAsset->Textures.Num(); i++)
	{
		if (SlowTask.ShouldCancel())
		{
			bAllSuccessful = false;
			break;
		}
		SlowTask.EnterProgressFrame(1);

		UTexture2D* SourceTexture = FontAsset->Textures[i];
		if (!SourceTexture)
		{
			bAllSuccessful = false;
			continue;
		}

		// Generate unique asset name
		FString AssetName = BaseName;
		
		if (FontAsset->Textures.Num() > 1)
		{
			AssetName += FString::Printf(TEXT("_%d"), i);
		}

		// Build base package name and ask AssetTools for a unique name
		const FString BasePackageName = NormalizedPackagePath + AssetName;
		FString UniquePackageName, UniqueAssetName;
		{
			FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
			AssetToolsModule.Get().CreateUniqueAssetName(BasePackageName, TEXT(""), UniquePackageName, UniqueAssetName);
		}

		// Create the package
		UPackage* Package = CreatePackage(*UniquePackageName);
		if (!Package)
		{
			UE_LOG(LogNiagaraTextParticles, Error, TEXT("UNiagaraTextParticlesHelpers::SaveFontTexturesToAssets: Failed to create package"));
			bAllSuccessful = false;
			continue;
		}
		Package->FullyLoad();

		// Duplicate the texture into the new package with unique name
		UTexture2D* NewTexture = DuplicateObject<UTexture2D>(SourceTexture, Package, *UniqueAssetName);
		if (!NewTexture)
		{
			UE_LOG(LogNiagaraTextParticles, Error, TEXT("UNiagaraTextParticlesHelpers::SaveFontTexturesToAssets: Failed to duplicate texture %d"), i);
			bAllSuccessful = false;
			continue;
		}

		// Set flags and mark dirty
		NewTexture->SetFlags(RF_Public | RF_Standalone);
		NewTexture->ClearFlags(RF_Transient);
		NewTexture->MarkPackageDirty();

		// Register with AssetRegistry so it appears immediately
		{
			FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
			AssetRegistryModule.AssetCreated(NewTexture);
		}

		// Save the package to disk
		{
			FSavePackageArgs SaveArgs;
			SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
			SaveArgs.SaveFlags = SAVE_NoError;

			const FString PackageFilename = FPackageName::LongPackageNameToFilename(UniquePackageName, FPackageName::GetAssetPackageExtension());
			if (!UPackage::SavePackage(Package, NewTexture, *PackageFilename, SaveArgs))
			{
				UE_LOG(LogNiagaraTextParticles, Warning, TEXT("UNiagaraTextParticlesHelpers::SaveFontTexturesToAssets: Save failed for '%s'"), *UniquePackageName);
				bAllSuccessful = false;
			}
		}

		CreatedAssets.Add(NewTexture);
	}

	// Sync Content Browser to the newly created assets
	if (CreatedAssets.Num() > 0)
	{
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
		ContentBrowserModule.Get().SyncBrowserToAssets(CreatedAssets);
	}

	if (bAllSuccessful)
	{
		UE_LOG(LogNiagaraTextParticles, Log, TEXT("UNiagaraTextParticlesHelpers::SaveFontTexturesToAssets: Successfully saved font textures to assets"));
	}
	else
	{
		UE_LOG(LogNiagaraTextParticles, Warning, TEXT("UNiagaraTextParticlesHelpers::SaveFontTexturesToAssets: Failed to save some font textures to assets"));
	}

	ShowSlateNotification(FText::FromString(FString::Printf(TEXT("Font textures saved to assets at:\n%s"), *FontAssetPath)), 5.0f);

	return bAllSuccessful;
#else
	UE_LOG(LogNiagaraTextParticles, Error, TEXT("UNiagaraTextParticlesHelpers::SaveFontTexturesToAssets: SaveFontTexturesToAssets is only available in editor builds"));
	return false;
#endif
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


void UNiagaraTextParticlesHelpers::ShowSlateNotification(const FText& Message, float Duration)
{
#if WITH_EDITOR
	FNotificationInfo Info(Message);

	Info.ExpireDuration = Duration;
	Info.Image = FCoreStyle::Get().GetBrush("icons.SuccessWithColor");

	FSlateNotificationManager::Get().AddNotification(Info);
#endif
}