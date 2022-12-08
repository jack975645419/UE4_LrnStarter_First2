// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class First2 : ModuleRules
{
	public First2(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "XmlParser" ,"Json"});
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
