// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DeadlyPark : ModuleRules
{
	public DeadlyPark(ReadOnlyTargetRules Target) : base(Target)
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
			"DeadlyPark",
			"DeadlyPark/Variant_Horror",
			"DeadlyPark/Variant_Horror/UI",
			"DeadlyPark/Variant_Shooter",
			"DeadlyPark/Variant_Shooter/AI",
			"DeadlyPark/Variant_Shooter/UI",
			"DeadlyPark/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
