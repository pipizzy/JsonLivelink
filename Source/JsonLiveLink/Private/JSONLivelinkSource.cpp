#include "JSONLiveLinkSource.h"
#include "ILiveLinkClient.h"
#include "LiveLinkSourceSettings.h"
#include "Async/Async.h"
#include "Common/UdpSocketBuilder.h"
#include "HAL/RunnableThread.h"
#include "Json.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"

DEFINE_LOG_CATEGORY(LogJSONLiveLink);

#define LOCTEXT_NAMESPACE "JSONLiveLinkSource"

FJSONLiveLinkSource::FJSONLiveLinkSource(FIPv4Endpoint InEndpoint)
    : Socket(nullptr)
    , Stopping(false)
    , Thread(nullptr)
    , WaitTime(FTimespan::FromMilliseconds(100))
    , DeviceEndpoint(InEndpoint)
    , Client(nullptr)
    , SocketSubsystem(nullptr)
{
    SourceStatus = LOCTEXT("SourceStatus_DeviceNotFound", "Device Not Found");
    SourceType = LOCTEXT("JSONLiveLinkSourceType", "JSON LiveLink");
    SourceMachineName = LOCTEXT("JSONLiveLinkSourceMachineName", "localhost");

    UE_LOG(LogJSONLiveLink, Warning, TEXT("Setting up socket"));

    try
    {
        Socket = FUdpSocketBuilder(TEXT("JSONLiveLink_Socket"))
            .AsNonBlocking()
            .AsReusable()
            .BoundToAddress(DeviceEndpoint.Address)
            .BoundToPort(DeviceEndpoint.Port)
            .WithReceiveBufferSize(1024 * 1024);

        if (Socket)
        {
            RecvBuffer.SetNumUninitialized(1024 * 1024);
            SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

            Start();
            SourceStatus = LOCTEXT("SourceStatus_Receiving", "Receiving");
        }
        else
        {
            UE_LOG(LogJSONLiveLink, Error, TEXT("Failed to create socket"));
        }
    }
    catch (...)
    {
        UE_LOG(LogJSONLiveLink, Error, TEXT("Exception in socket creation"));
    }
}

FJSONLiveLinkSource::~FJSONLiveLinkSource()
{
    Stop();

    if (Thread)
    {
        Thread->WaitForCompletion();
        delete Thread;
        Thread = nullptr;
    }

    if (Socket)
    {
        Socket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
        Socket = nullptr;
    }
}

void FJSONLiveLinkSource::ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid)
{
    Client = InClient;
    SourceGuid = InSourceGuid;
}

void FJSONLiveLinkSource::InitializeSettings(ULiveLinkSourceSettings* Settings)
{
    // Settings initialization can be done here if needed
    // Empty implementation is fine if there are no special requirements
}

void FJSONLiveLinkSource::Update()
{
    // This method might be called every frame in UE5.3
    // No specific implementation needed unless there's content that needs updating each frame
}

bool FJSONLiveLinkSource::IsSourceStillValid() const
{
    // Source is valid if we have a valid thread and socket
    bool bIsSourceValid = !Stopping && Thread != nullptr && Socket != nullptr;
    return bIsSourceValid;
}

bool FJSONLiveLinkSource::RequestSourceShutdown()
{
    Stop();
    return true;
}

TSubclassOf<ULiveLinkSourceSettings> FJSONLiveLinkSource::GetSettingsClass() const
{
    return ULiveLinkSourceSettings::StaticClass();
}

void FJSONLiveLinkSource::Start()
{
    ThreadName = "JSON UDP Receiver ";
    ThreadName.AppendInt(FAsyncThreadIndex::GetNext());

    Thread = FRunnableThread::Create(this, *ThreadName, 128 * 1024, TPri_AboveNormal, FPlatformAffinity::GetPoolThreadMask());
}

void FJSONLiveLinkSource::Stop()
{
    Stopping = true;
}

uint32 FJSONLiveLinkSource::Run()
{
    TSharedRef<FInternetAddr> Sender = SocketSubsystem->CreateInternetAddr();

    while (!Stopping)
    {
        if (Socket->Wait(ESocketWaitConditions::WaitForRead, WaitTime))
        {
            uint32 Size;

            while (Socket->HasPendingData(Size))
            {
                int32 Read = 0;

                if (Socket->RecvFrom(RecvBuffer.GetData(), RecvBuffer.Num(), Read, *Sender))
                {
                    if (Read > 0)
                    {
                        UE_LOG(LogJSONLiveLink, Log, TEXT("Received %d bytes from %s"), Read, *Sender->ToString(true));

                        // We'll parse JSON and forward to LiveLink in a future step
                    }
                }
                else
                {
                    UE_LOG(LogJSONLiveLink, Error, TEXT("Failed to receive data from socket"));
                }
            }
        }
    }
    return 0;
}

#undef LOCTEXT_NAMESPACE