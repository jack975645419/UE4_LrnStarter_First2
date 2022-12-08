using UnrealBuildTool;

public class MyModule : ModuleRules
{
	public MyModule(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" , "first2"});
		PrivateDependencyModuleNames.AddRange(new string[]{"First2"});
		PublicIncludePaths.AddRange(new string[] { "MyModule/Public"  /*,"First2/Public" */ });
		PrivateIncludePaths.AddRange(new string[] { "MyModule/Private" });
	}
}