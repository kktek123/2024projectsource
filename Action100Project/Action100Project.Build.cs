// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Action100Project : ModuleRules
{
	public Action100Project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
