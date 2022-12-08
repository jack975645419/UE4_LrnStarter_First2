// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class First2EditorTarget : TargetRules
{
	public First2EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		// ExtraModuleNames.Add("First2");
		ExtraModuleNames.AddRange(new string[] { "First2", });

	}
}
