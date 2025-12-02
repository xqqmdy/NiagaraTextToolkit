using UnrealBuildTool;

public class NiagaraTextToolkitVertexFactory : ModuleRules
{
	public NiagaraTextToolkitVertexFactory(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Engine",
				"RenderCore",
				"RHI",
				"Niagara",
				"NiagaraShader",
				"NiagaraVertexFactories",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Projects",
			}
		);
	}
}


