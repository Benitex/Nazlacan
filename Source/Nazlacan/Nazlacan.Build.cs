// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Nazlacan : ModuleRules
{
	public Nazlacan(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(["Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput"]);

		PrivateDependencyModuleNames.AddRange([
			"GameplayAbilities", "GameplayTags", "GameplayTasks",
			"PCG",
			"UMG", "Niagara",
			"NavigationSystem",
			"MoviePlayer",
		]);

		PrivateDependencyModuleNames.AddRange(["Slate", "SlateCore"]);

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
