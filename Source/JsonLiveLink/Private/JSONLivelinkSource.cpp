#include "JSONLiveLinkSource.h"
#include "ILiveLinkClient.h"
#include "LiveLinkSourceSettings.h"
#include "Async/Async.h"
#include "Common/UdpSocketBuilder.h"
#include "HAL/RunnableThread.h"
#include "Json.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"

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
    }
    catch (...)
    {
        UE_LOG(LogTemp, Error, TEXT("Exception in socket creation"));
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
    // No custom settings for now
}

void FJSONLiveLinkSource::Update()
{
    // Not needed for now
}

bool FJSONLiveLinkSource::IsSourceStillValid() const
{
    // Source is valid if we have a valid thread and socket
    return !Stopping && Thread != nullptr && Socket != nullptr;
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
                        UE_LOG(LogTemp, Log, TEXT("Received %d bytes from %s"), Read, *Sender->ToString(true));

                        // We'll parse JSON and forward to LiveLink in a future step
                    }
                }
            }
        }
    }
    return 0;
}

#undef LOCTEXT_NAMESPACE