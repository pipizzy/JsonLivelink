#pragma once

#include "LiveLinkSourceFactory.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "JSONLiveLinkSourceFactory.generated.h"

/**
 * Factory class for creating the JSON-based LiveLink source.
 */
UCLASS()
class JSONLIVELINK_API UJSONLiveLinkSourceFactory : public ULiveLinkSourceFactory
{
    GENERATED_BODY()

public:
    virtual FText GetSourceDisplayName() const override;
    virtual FText GetSourceTooltip() const override;

    virtual EMenuType GetMenuType() const override { return EMenuType::SubPanel; }
    virtual TSharedPtr<SWidget> BuildCreationPanel(FOnLiveLinkSourceCreated OnLiveLinkSourceCreated) const override;
    virtual TSharedPtr<ILiveLinkSource> CreateSource(const FString& ConnectionString) const override;

private:
    void OnOkClicked(FIPv4Endpoint Endpoint, FOnLiveLinkSourceCreated OnLiveLinkSourceCreated) const;
};