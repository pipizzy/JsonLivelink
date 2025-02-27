// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class JSONLiveLink : ModuleRules
{
    public JSONLiveLink(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        // Add the export definition
        PublicDefinitions.Add("JSONLIVELINK_API=");

        PublicIncludePaths.AddRange(
            new string[]
            {
                // Add public include paths required here
            }
        );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "LiveLinkInterface",
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
            });
    }
}