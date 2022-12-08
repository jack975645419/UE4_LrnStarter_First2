using UnrealBuildTool;

public class MyModule : ModuleRules
{
	public MyModule(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" }); // 写成first2也是可以的，大小不区分
		PrivateDependencyModuleNames.AddRange(new string[]{"First2"});
		PublicIncludePaths.AddRange(new string[] { "MyModule/Public"  /*,"First2/Public" */ });
		PrivateIncludePaths.AddRange(new string[] { "MyModule/Private" });
	}
}