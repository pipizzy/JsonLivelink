#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#if PLATFORM_WINDOWS
#ifdef JSONLIVELINK_API
#undef JSONLIVELINK_API
#endif
#if JSONLIVELINK_EXPORTS
#define JSONLIVELINK_API __declspec(dllexport)
#else
#define JSONLIVELINK_API __declspec(dllimport)
#endif
#else
#define JSONLIVELINK_API
#endif

class JSONLIVELINK_API FJSONLiveLinkModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};