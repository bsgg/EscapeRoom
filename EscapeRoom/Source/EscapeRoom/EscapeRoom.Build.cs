// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class EscapeRoom : ModuleRules
{
	public EscapeRoom(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "OnlineSubsystem", "OnlineSubsystemSteam" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
        //if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
       // {
            //DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
            //if (UEBuildConfiguration.bCompileSteamOSS == true)
            //{
            //    DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
            //}
        //}
    }
}
