#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "LiveLinkModule.h"

class JSONLIVELINK_API FJSONLiveLinkModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};