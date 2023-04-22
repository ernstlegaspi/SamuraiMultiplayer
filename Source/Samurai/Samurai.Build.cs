using UnrealBuildTool;

public class Samurai : ModuleRules {
	public Samurai(ReadOnlyTargetRules Target) : base(Target) {
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "Niagara", "NavigationSystem", "Slate", "SlateCore", "UMG", "OnlineSubsystem", "OnlineSubsystemSteam" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
