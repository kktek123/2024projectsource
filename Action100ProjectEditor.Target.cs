
using UnrealBuildTool;
using System.Collections.Generic;

public class Action100ProjectEditorTarget : TargetRules
{
	public Action100ProjectEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("Action100Project");
	}
}
