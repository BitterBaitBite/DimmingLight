// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DimmingLight : ModuleRules {
	public DimmingLight(ReadOnlyTargetRules Target) : base(Target) {
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
			{ "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "Niagara", "AIModule" });

		PrivateDependencyModuleNames.AddRange(new[] { "Slate", "SlateCore" });
	}
}
