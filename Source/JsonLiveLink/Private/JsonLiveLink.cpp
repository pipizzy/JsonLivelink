// Copyright Epic Games, Inc. All Rights Reserved.

#include "JsonLiveLink.h"
#include "JsonLiveLinkStyle.h"
#include "JsonLiveLinkCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName JsonLiveLinkTabName("JsonLiveLink");

#define LOCTEXT_NAMESPACE "FJsonLiveLinkModule"

void FJsonLiveLinkModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FJsonLiveLinkStyle::Initialize();
	FJsonLiveLinkStyle::ReloadTextures();

	FJsonLiveLinkCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FJsonLiveLinkCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FJsonLiveLinkModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FJsonLiveLinkModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(JsonLiveLinkTabName, FOnSpawnTab::CreateRaw(this, &FJsonLiveLinkModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FJsonLiveLinkTabTitle", "JsonLiveLink"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FJsonLiveLinkModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FJsonLiveLinkStyle::Shutdown();

	FJsonLiveLinkCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(JsonLiveLinkTabName);
}

TSharedRef<SDockTab> FJsonLiveLinkModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FJsonLiveLinkModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("JsonLiveLink.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FJsonLiveLinkModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(JsonLiveLinkTabName);
}

void FJsonLiveLinkModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FJsonLiveLinkCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FJsonLiveLinkCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FJsonLiveLinkModule, JsonLiveLink)