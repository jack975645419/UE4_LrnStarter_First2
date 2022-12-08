// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class First2 : ModuleRules
{
	public First2(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "XmlParser" ,"Json"});
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
		
		 PublicIncludePaths.AddRange(new string[]{"First2/Farm", "First2/Public"});  
		 //Farm目录成为公共的路径
		 
		 // 尝试把 "First2/Backyarm" 即加公共也加私有目录，其实是可以的，编译上不会有问题，但是不容易理解，不建议这么做
		
		 PrivateIncludePaths.AddRange(new string[]{"First2/Private","First2/Backyarm"});  
		 //Backyarm是私有目录
	}
}
