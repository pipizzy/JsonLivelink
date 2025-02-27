#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"

class SEditableTextBox;

class SJSONLiveLinkSourceFactory : public SCompoundWidget
{
public:
    DECLARE_DELEGATE_OneParam(FOnOkClicked, FIPv4Endpoint);

    SLATE_BEGIN_ARGS(SJSONLiveLinkSourceFactory) {}
        SLATE_EVENT(FOnOkClicked, OnOkClicked)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    void OnEndpointChanged(const FText& NewValue, ETextCommit::Type CommitType);
    FReply OnOkClicked();

    TWeakPtr<SEditableTextBox> EditableText;
    FOnOkClicked OkClicked;
};