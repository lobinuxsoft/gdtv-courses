// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class ObstacleAssaultTarget : TargetRules
{
	public ObstacleAssaultTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
    bUseUnityBuild = false;
    bUsePCHFiles = false;
		DefaultBuildSettings = BuildSettingsVersion.Latest;

		ExtraModuleNames.AddRange( new string[] { "ObstacleAssault" } );
	}
}
