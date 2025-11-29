// Property of Lucian Tranc

#pragma once

#include "NiagaraDataInterface.h"
#include "VectorVM.h"
#include "Engine/Font.h"
#include "NTPDataInterface.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogNiagaraTextParticles, Log, All);

enum class ENiagaraSimTarget : uint8;
struct FNiagaraDataInterfaceGeneratedFunction;
struct FNiagaraFunctionSignature;
struct FVMExternalFunctionBindingInfo;

UENUM(BlueprintType)
enum class ENTPTextVerticalAlignment : uint8
{
	NTP_TVA_Top		UMETA(DisplayName = "Top"),
	NTP_TVA_Center	UMETA(DisplayName = "Center"),
	NTP_TVA_Bottom	UMETA(DisplayName = "Bottom"),
};

UENUM(BlueprintType)
enum class ENTPTextHorizontalAlignment : uint8
{
	NTP_THA_Left	UMETA(DisplayName = "Left"),
	NTP_THA_Center	UMETA(DisplayName = "Center"),
	NTP_THA_Right	UMETA(DisplayName = "Right"),
};

UCLASS(EditInlineNew, BlueprintType, Category = "Niagara Text Particles Plugin", meta = (DisplayName = "NTP Data Interface"))
class NIAGARATEXTPARTICLES_API UNTPDataInterface : public UNiagaraDataInterface
{
	GENERATED_UCLASS_BODY()

public:
	BEGIN_SHADER_PARAMETER_STRUCT(FShaderParameters, )
		// Normalized per-glyph texture UVs (USize, VSize, UStart, VStart), all in 0-1 texture space
		SHADER_PARAMETER_SRV(	StructuredBuffer<float4>,	CharacterTextureUvs)
		// Per-glyph sprite size in pixels, used for layout / particle sizing
		SHADER_PARAMETER_SRV(	StructuredBuffer<float2>,	CharacterSpriteSizes)
		SHADER_PARAMETER(		uint32,						NumRects)
		SHADER_PARAMETER_SRV(	StructuredBuffer<uint>,		TextUnicode)
		SHADER_PARAMETER_SRV(	StructuredBuffer<float2>,	CharacterPositions)
		SHADER_PARAMETER_SRV(	StructuredBuffer<uint>,		LineStartIndices)
		SHADER_PARAMETER_SRV(	StructuredBuffer<uint>,		LineCharacterCounts)
		SHADER_PARAMETER_SRV(	StructuredBuffer<uint>,		WordStartIndices)
		SHADER_PARAMETER_SRV(	StructuredBuffer<uint>,		WordCharacterCounts)
		SHADER_PARAMETER(		uint32,						NumChars)
		SHADER_PARAMETER(		uint32,						NumLines)
		SHADER_PARAMETER(		uint32,						NumWords)
		SHADER_PARAMETER(		uint32,						bFilterWhitespaceCharactersValue)
	END_SHADER_PARAMETER_STRUCT()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Font Asset"))
	UFont* FontAsset = nullptr;

	// The input text to compute character positions for; converted to Unicode and character positions per instance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Input Text", MultiLine = "true"))
	FString InputText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, meta = (DisplayName = "Horizontal Alignment"))
	ENTPTextHorizontalAlignment HorizontalAlignment = ENTPTextHorizontalAlignment::NTP_THA_Center;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, meta = (DisplayName = "Vertical Alignment"))
	ENTPTextVerticalAlignment VerticalAlignment = ENTPTextVerticalAlignment::NTP_TVA_Center;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, meta = (DisplayName = "Filter Whitespace Characters"))
	bool bFilterWhitespaceCharacters = true;

	//UObject Interface
	virtual void PostInitProperties() override;
	//UObject Interface End

	//UNiagaraDataInterface Interface
	virtual void GetFunctions(TArray<FNiagaraFunctionSignature>& OutFunctions) override;
	virtual void GetVMExternalFunction(const FVMExternalFunctionBindingInfo& BindingInfo, void* InstanceData, FVMExternalFunction& OutFunc) override;
	virtual bool CanExecuteOnTarget(ENiagaraSimTarget Target) const override { return true; }
	virtual void BuildShaderParameters(FNiagaraShaderParametersBuilder& ShaderParametersBuilder) const override;
	virtual void SetShaderParameters(const FNiagaraDataInterfaceSetShaderParametersContext& Context) const override;
#if WITH_EDITORONLY_DATA
	virtual bool AppendCompileHash(FNiagaraCompileHashVisitor* InVisitor) const override;
	virtual bool GetFunctionHLSL(const FNiagaraDataInterfaceGPUParamInfo& ParamInfo, const FNiagaraDataInterfaceGeneratedFunction& FunctionInfo, int FunctionInstanceIndex, FString& OutHLSL) override;
	virtual void GetParameterDefinitionHLSL(const FNiagaraDataInterfaceGPUParamInfo& ParamInfo, FString& OutHLSL) override;
#endif
	virtual bool CopyToInternal(UNiagaraDataInterface* Destination) const override;
	virtual bool Equals(const UNiagaraDataInterface* Other) const override;
	virtual bool InitPerInstanceData(void* PerInstanceData, FNiagaraSystemInstance* SystemInstance) override;
	virtual void DestroyPerInstanceData(void* PerInstanceData, FNiagaraSystemInstance* SystemInstance) override;
	virtual int32 PerInstanceDataSize() const override;
	virtual void ProvidePerInstanceDataForRenderThread(void* DataForRenderThread, void* PerInstanceData, const FNiagaraSystemInstanceID& SystemInstance) override;
	//UNiagaraDataInterface Interface

	void GetCharacterUVVM(FVectorVMExternalFunctionContext& Context);
	void GetCharacterPositionVM(FVectorVMExternalFunctionContext& Context);
	void GetTextCharacterCountVM(FVectorVMExternalFunctionContext& Context);
	void GetTextLineCountVM(FVectorVMExternalFunctionContext& Context);
	void GetLineCharacterCountVM(FVectorVMExternalFunctionContext& Context);
	void GetTextWordCountVM(FVectorVMExternalFunctionContext& Context);
	void GetWordCharacterCountVM(FVectorVMExternalFunctionContext& Context);
	void GetWordTrailingWhitespaceCountVM(FVectorVMExternalFunctionContext& Context);
	void GetFilterWhitespaceCharactersVM(FVectorVMExternalFunctionContext& Context);
	void GetCharacterCountInWordRangeVM(FVectorVMExternalFunctionContext& Context);
	void GetCharacterCountInLineRangeVM(FVectorVMExternalFunctionContext& Context);
	void GetCharacterSpriteSizeVM(FVectorVMExternalFunctionContext& Context);

private:
	static const FName GetCharacterUVName;
	static const FName GetCharacterPositionName;
	static const FName GetTextCharacterCountName;
	static const FName GetTextLineCountName;
	static const FName GetLineCharacterCountName;
	static const FName GetTextWordCountName;
	static const FName GetWordCharacterCountName;
	static const FName GetWordTrailingWhitespaceCountName;
	static const FName GetFilterWhitespaceCharactersName;
	static const FName GetCharacterCountInWordRangeName;
	static const FName GetCharacterCountInLineRangeName;
	static const FName GetCharacterSpriteSizeName;

	// Computes per-character positions in local text space using per-glyph sprite sizes in pixels.
	static TArray<FVector2f> GetCharacterPositions(const TArray<FVector2f>& CharacterSpriteSizes, const TArray<int32>& VerticalOffsets, int32 Kerning, FString InputString, ENTPTextHorizontalAlignment XAlignment, ENTPTextVerticalAlignment YAlignment);

	// Extracts per-glyph sprite sizes (pixels), normalized texture UVs, vertical offsets, and global kerning from the font asset.
	static bool GetFontInfo(const UFont* FontAsset, TArray<FVector4>& OutCharacterTextureUvs, TArray<FVector2f>& OutCharacterSpriteSizes, TArray<int32>& OutVerticalOffsets, int32& OutKerning);

	static void ProcessText(
		const FString& InputText,
		const TArray<FVector2f>& CharacterPositionsUnfiltered,
		const bool bFilterWhitespace,
		TArray<int32>& OutUnicode,
		TArray<FVector2f>& OutCharacterPositions,
		TArray<int32>& OutLineStartIndices,
		TArray<int32>& OutLineCharacterCounts,
		TArray<int32>& OutWordStartIndices,
		TArray<int32>& OutWordCharacterCounts
	);

};

