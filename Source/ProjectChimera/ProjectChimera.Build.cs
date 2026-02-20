// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectChimera : ModuleRules
{
	public ProjectChimera(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"ProjectChimera",
			"ProjectChimera/Variant_Platforming",
			"ProjectChimera/Variant_Platforming/Animation",
			"ProjectChimera/Variant_Combat",
			"ProjectChimera/Variant_Combat/AI",
			"ProjectChimera/Variant_Combat/Animation",
			"ProjectChimera/Variant_Combat/Gameplay",
			"ProjectChimera/Variant_Combat/Interfaces",
			"ProjectChimera/Variant_Combat/UI",
			"ProjectChimera/Variant_SideScrolling",
			"ProjectChimera/Variant_SideScrolling/AI",
			"ProjectChimera/Variant_SideScrolling/Gameplay",
			"ProjectChimera/Variant_SideScrolling/Interfaces",
			"ProjectChimera/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
