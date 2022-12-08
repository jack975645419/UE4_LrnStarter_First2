using UnrealBuildTool;

public class MyModule : ModuleRules
{
	public MyModule(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
		PublicIncludePaths.AddRange(new string[] { "MyModule/Public" });
		PrivateIncludePaths.AddRange(new string[] { "MyModule/Private" });
	}
}