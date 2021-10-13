// COPYRIGHT 1995-2021 ESRI
// TRADE SECRETS: ESRI PROPRIETARY AND CONFIDENTIAL
// Unpublished material - all rights reserved under the
// Copyright Laws of the United States and applicable international
// laws, treaties, and conventions.
//
// For additional information, contact:
// Attn: Contracts and Legal Department
// Environmental Systems Research Institute, Inc.
// 380 New York Street
// Redlands, California 92373
// USA
//
// email: legal@esri.com
#include "ArcGISMapsSDKEditorModule.h"

// Unreal

#include "ToolMenus.h"

// ArcGISMapsSDKEditor

#include "ArcGISToolBar.h"
#include "ArcGISToolBarCommands.h"
#include "ArcGISToolBarStyle.h"
#include "Assets/ArcGISAssetDirectory.h"
#include "MapControllerDetailsPanel.h"
#include "MapControllerStyles.h"

static const FName ArcGISToolBarTabName("ArcGISToolBar");

#define LOCTEXT_NAMESPACE "FArcGISMapsSDKEditorModule"

void FArcGISMapsSDKEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	{
		PropertyModule.RegisterCustomClassLayout(AArcGISMapController::StaticClass()->GetFName(),
												 FOnGetDetailCustomizationInstance::CreateStatic(&MapControllerDetailsPanel::MakeInstance));
	}

	FArcGISAssetDirectory::LoadForCook();

	FArcGISToolBarStyle::Initialize();
	FArcGISToolBarCommands::Register();

	MapControllerStyles::Initialize();

	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(FArcGISToolBarCommands::Get().GetPluginAction(),
							  FExecuteAction::CreateRaw(this, &FArcGISMapsSDKEditorModule::PluginButtonClicked), FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FArcGISMapsSDKEditorModule::RegisterMenus));
}

void FArcGISMapsSDKEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		{
			PropertyModule.UnregisterCustomClassLayout(AArcGISMapController::StaticClass()->GetFName());
		}
	}

	MapControllerStyles::Shutdown();

	FArcGISAssetDirectory::ReleaseAll();
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	FArcGISToolBarStyle::Shutdown();
	FArcGISToolBarCommands::Unregister();
}

void FArcGISMapsSDKEditorModule::PluginButtonClicked()
{
	FArcGISToolBar::InitializeArcGISMapSetup();
}

void FArcGISMapsSDKEditorModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);
	auto* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
	auto& MenuSection = Menu->AddSection("ArcGIS Maps");
	MenuSection.AddMenuEntryWithCommandList(FArcGISToolBarCommands::Get().GetPluginAction(), PluginCommands);

	auto* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
	auto& ToolBarSection = ToolbarMenu->AddSection("ArcGIS Maps", TAttribute<FText>(), FToolMenuInsert("Settings", EToolMenuInsertType::After));
	auto& Entry = ToolBarSection.AddEntry(FToolMenuEntry::InitToolBarButton(FArcGISToolBarCommands::Get().GetPluginAction()));
	Entry.SetCommandList(PluginCommands);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FArcGISMapsSDKEditorModule, ArcGISMapsSDKEditor)
