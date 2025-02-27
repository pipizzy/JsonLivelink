// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class JSONLiveLink : ModuleRules
{
    public JSONLiveLink(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "LiveLink",
                "LiveLinkInterface",
                "Messaging",
            });

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "InputCore",
                "Json",
                "JsonUtilities",
                "Networking",
                "Slate",
                "SlateCore",
                "Sockets",
                "LiveLink",
                "LiveLinkCore",
            });
    }
}