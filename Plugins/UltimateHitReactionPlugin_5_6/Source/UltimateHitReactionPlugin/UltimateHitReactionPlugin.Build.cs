// Copyright StarExile Studio All Rights Reserved.

using UnrealBuildTool;

public class UltimateHitReactionPlugin : ModuleRules
{
	public UltimateHitReactionPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "Blutility"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"Chooser",
                "Blutility",
                "StructUtils"
				// ... add private dependencies that you statically link with here ...	
			}
			);
	}
}
