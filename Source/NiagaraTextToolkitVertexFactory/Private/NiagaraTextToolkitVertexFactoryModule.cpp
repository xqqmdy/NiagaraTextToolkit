#include "NiagaraTextToolkitVertexFactoryModule.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "Shader.h"
#include "Misc/Paths.h"

void FNiagaraTextToolkitVertexFactoryModule::StartupModule()
{
	// Map plugin shader directory to the virtual path used by shaders.
	// This must run in a module that loads before InitializeShaderTypes.
	const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("NiagaraTextToolkit"));
	if (Plugin.IsValid())
	{
		const FString ShaderDir = FPaths::Combine(Plugin->GetBaseDir(), TEXT("Shaders"));
		AddShaderSourceDirectoryMapping(TEXT("/Plugin/NiagaraTextToolkit"), ShaderDir);
	}
}

void FNiagaraTextToolkitVertexFactoryModule::ShutdownModule()
{
}

IMPLEMENT_MODULE(FNiagaraTextToolkitVertexFactoryModule, NiagaraTextToolkitVertexFactory);



