#pragma once

#include "ILiveLinkSource.h"
#include "HAL/Runnable.h"
#include "HAL/ThreadSafeBool.h"
#include "IMessageContext.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "LiveLinkSourceSettings.h"

// Enable logging
DECLARE_LOG_CATEGORY_EXTERN(LogJSONLiveLink, Log, All)

class FRunnableThread;
class FSocket;
class ILiveLinkClient;
class ISocketSubsystem;

class FJSONLiveLinkSource : public ILiveLinkSource, public FRunnable
{
public:
    FJSONLiveLinkSource(FIPv4Endpoint Endpoint);
    virtual ~FJSONLiveLinkSource();

    // Begin ILiveLinkSource Interface
    virtual void ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid) override;
    virtual void InitializeSettings(ULiveLinkSourceSettings* Settings) override;
    virtual void Update() override;
    virtual bool IsSourceStillValid() const override;
    virtual bool RequestSourceShutdown() override;
    virtual TSubclassOf<ULiveLinkSourceSettings> GetSettingsClass() const override;
    virtual FText GetSourceType() const override { return SourceType; };
    virtual FText GetSourceMachineName() const override { return SourceMachineName; }
    virtual FText GetSourceStatus() const override { return SourceStatus; }
    // End ILiveLinkSource Interface

    // Begin FRunnable Interface
    virtual bool Init() override { return true; }
    virtual uint32 Run() override;
    void Start();
    virtual void Stop() override;
    virtual void Exit() override {}
    // End FRunnable Interface

private:
    ILiveLinkClient* Client;
    FGuid SourceGuid;
    FText SourceType;
    FText SourceMachineName;
    FText SourceStatus;
    FIPv4Endpoint DeviceEndpoint;
    FSocket* Socket;
    ISocketSubsystem* SocketSubsystem;
    FThreadSafeBool Stopping;
    FRunnableThread* Thread;
    FString ThreadName;
    FTimespan WaitTime;
    TArray<uint8> RecvBuffer;
};