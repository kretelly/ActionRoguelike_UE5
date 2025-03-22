// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class ActionRoguelikeTarget : TargetRules
{
	public ActionRoguelikeTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
        //DefaultBuildSettings = BuildSettingsVersion.V2;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        ExtraModuleNames.AddRange( new string[] { "ActionRoguelike" } );
	}
}
