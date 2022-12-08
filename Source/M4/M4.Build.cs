
using UnrealBuildTool;

public class M4 : ModuleRules
{
	public M4(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" }); // 写成first2也是可以的，大小不区分
		PrivateDependencyModuleNames.AddRange(new string[]{   "M3"});
	}
}