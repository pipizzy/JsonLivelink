#include "JSONLiveLink.h"
#include "Modules/ModuleManager.h"
#include "LiveLinkModule.h"

#define LOCTEXT_NAMESPACE "FJSONLiveLinkModule"

void FJSONLiveLinkModule::StartupModule()
{
    if (FModuleManager::Get().IsModuleLoaded("LiveLink"))
    {
        ILiveLinkModule& LiveLinkModule = FModuleManager::LoadModuleChecked<ILiveLinkModule>("LiveLink");
        UE_LOG(LogTemp, Log, TEXT("JSONLiveLink Module Started"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("LiveLink Module not loaded"));
    }
}

void FJSONLiveLinkModule::ShutdownModule()
{
    UE_LOG(LogTemp, Log, TEXT("JSONLiveLink Module Shutdown"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FJSONLiveLinkModule, JSONLiveLink)