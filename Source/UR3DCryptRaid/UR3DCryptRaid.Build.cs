// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UR3DCryptRaid : ModuleRules
{
	public UR3DCryptRaid(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
