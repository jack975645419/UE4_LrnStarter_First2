
using UnrealBuildTool;

public class M3 : ModuleRules
{
	public M3(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore"}); // 写成first2也是可以的，大小不区分
		PrivateDependencyModuleNames.AddRange(new string[]{   "MyModule" });
	}
}