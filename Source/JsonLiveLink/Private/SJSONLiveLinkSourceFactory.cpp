#include "SJSONLiveLinkSourceFactory.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "SJSONLiveLinkSourceFactory"

void SJSONLiveLinkSourceFactory::Construct(const FArguments& InArgs)
{
    OkClicked = InArgs._OnOkClicked;

    FIPv4Endpoint DefaultEndpoint(FIPv4Address::Any, 54321);

    ChildSlot
        [
            SNew(SBox)
                .WidthOverride(250.0f)
                [
                    SNew(SVerticalBox)
                        + SVerticalBox::Slot()
                        .AutoHeight()
                        [
                            SNew(SHorizontalBox)
                                + SHorizontalBox::Slot()
                                .FillWidth(0.5f)
                                [
                                    SNew(STextBlock)
                                        .Text(LOCTEXT("JSONPortNumber", "Port Number"))
                                ]
                                + SHorizontalBox::Slot()
                                .FillWidth(0.5f)
                                [
                                    SAssignNew(EditableText, SEditableTextBox)
                                        .Text(FText::FromString(DefaultEndpoint.ToString()))
                                        .OnTextCommitted(this, &SJSONLiveLinkSourceFactory::OnEndpointChanged)
                                ]
                        ]
                        + SVerticalBox::Slot()
                        .HAlign(HAlign_Right)
                        .AutoHeight()
                        [
                            SNew(SButton)
                                .Text(LOCTEXT("Ok", "Ok"))
                                .OnClicked(this, &SJSONLiveLinkSourceFactory::OnOkClicked)
                        ]
                ]
        ];
}

void SJSONLiveLinkSourceFactory::OnEndpointChanged(const FText& NewValue, ETextCommit::Type CommitType)
{
    FIPv4Endpoint Endpoint;
    if (!FIPv4Endpoint::Parse(NewValue.ToString(), Endpoint))
    {
        FIPv4Endpoint DefaultEndpoint(FIPv4Address::Any, 54321);
        EditableText.Pin()->SetText(FText::FromString(DefaultEndpoint.ToString()));
    }
}

FReply SJSONLiveLinkSourceFactory::OnOkClicked()
{
    FIPv4Endpoint Endpoint;
    if (FIPv4Endpoint::Parse(EditableText.Pin()->GetText().ToString(), Endpoint))
    {
        OkClicked.ExecuteIfBound(Endpoint);
    }
    return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE