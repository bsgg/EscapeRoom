// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "SessionRow.h"

#include "UObject/ConstructorHelpers.h"

#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> SessionRowBPClass(TEXT("/Game/MainMenu/UI/SessionRow_WBP"));
	if (SessionRowBPClass.Class == nullptr) return;

	SessionRowClass = SessionRowBPClass.Class;
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	if (NewGameMenuButton == nullptr) return false;
	NewGameMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OnNewGamePressed);

	if (JoinGameMenuButton == nullptr) return false;
	JoinGameMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinGamePressed);

	if (QuitMainMenuButton == nullptr) return false;
	QuitMainMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OnQuitPressed);

	if (CancelJoinSessionButton == nullptr) return false;
	CancelJoinSessionButton->OnClicked.AddDynamic(this, &UMainMenu::OnCancelJoinSession);

	if (JoinSessionButton == nullptr) return false;
	JoinSessionButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinSession);

		
	return true;
}

void UMainMenu::OnNewGamePressed()
{
	if (MenuInterface == nullptr) return;
	MenuInterface->Host("BSGG SERVER");
}

void UMainMenu::InitializeSessionsList(TArray<FServerData> Data)
{
	UE_LOG(LogTemp, Warning, TEXT("[UMainMenu::InitializeSessionsList] %i"), Data.Num());
	if (ScrollSessionList == nullptr) return;

	UWorld* World = this->GetWorld();
	if (World == nullptr) return;

	ScrollSessionList->ClearChildren();
	uint32 IndexRow = 0;
	for (const FServerData& ServerData : Data)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMainMenu::InitializeSessionsList] %s"), *ServerData.Name);

		USessionRow* Row = CreateWidget<USessionRow>(World, SessionRowClass);
		if (Row == nullptr) return;

		Row->ServerName->SetText(FText::FromString(ServerData.Name));
		Row->HostUser->SetText(FText::FromString(ServerData.HostUsername));

		FString FractionText = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
		Row->ConnectionFraction->SetText(FText::FromString(FractionText));

		Row->Setup(this, IndexRow);
		++IndexRow;
		ScrollSessionList->AddChild(Row);
	}
}

void UMainMenu::SelectIndexSessionList(uint32 Index)
{
	UE_LOG(LogTemp, Warning, TEXT("[UMainMenu::SelectIndex] SelectIndex: %i"), Index);

	SelectedScrollIndex = Index;
	UpdateSessionList();
}

void UMainMenu::UpdateSessionList()
{
	if (ScrollSessionList == nullptr) return;

	// Start from 1, not counting Header
	int indexRow = 0;
	for (int32 i = 1; i < ScrollSessionList->GetChildrenCount(); ++i)
	{
		auto Row = Cast<USessionRow>(ScrollSessionList->GetChildAt(i));
		if (Row != nullptr)
		{
			Row->Selected = (SelectedScrollIndex.IsSet() && (SelectedScrollIndex.GetValue() == indexRow));

			indexRow++;
		}
	}
}


void UMainMenu::OnJoinGamePressed()
{
	if ((MenuSwitcher == nullptr) || (SessionListMenuWidget == nullptr)) return;

	MenuSwitcher->SetActiveWidget(SessionListMenuWidget);

	if (MenuInterface == nullptr) return;

	MenuInterface->OpenSessionListMenu();
}

void UMainMenu::OnQuitPressed()
{
	UWorld*	World = GetWorld();
	if (World == nullptr) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr) return;

	PlayerController->ConsoleCommand("quit");
}


void UMainMenu::OnCancelJoinSession()
{
	if ((MenuSwitcher == nullptr) || (CreateGameMenuWidget == nullptr)) return;

	MenuSwitcher->SetActiveWidget(CreateGameMenuWidget);
}

void UMainMenu::OnJoinSession()
{
	if ((ScrollSessionList == nullptr) && (MenuInterface == nullptr)) return;

	if (SelectedScrollIndex.IsSet())
	{
		int32 ScrollCount = ScrollSessionList->GetChildrenCount();
		int32 SelectedIndex = (int32)SelectedScrollIndex.GetValue();
		if ((ScrollCount > 0) && (SelectedIndex >= 0) &&  (SelectedIndex < ScrollCount))
		{
			MenuInterface->JoinSession(SelectedScrollIndex.GetValue());
		}
		else
		{
			ShowMessageBlocker(FText::FromString("No sessions available"), 0.3f);
		}		
	}
	else
	{
		ShowMessageBlocker(FText::FromString("Unable to Join Session"), 0.3f);
	}	
}


void UMainMenu::ShowMessageBlocker(FText Message, float HideAfterDelay)
{
	if ((MessageBlockerWidget == nullptr) || (MessageBlockerText == nullptr)) return;

	MessageBlockerText->SetText(Message);

	MessageBlockerWidget->SetVisibility(ESlateVisibility::Visible);

	if (HideAfterDelay > 0.0f)
	{
		UWorld* World = GetWorld();
		if (World != nullptr)
		{
			World->GetTimerManager().SetTimer(MessageBlockerTimerHandle, this, &UMainMenu::HideMessageBlocker, HideAfterDelay);
		}
	}
}

void UMainMenu::HideMessageBlocker()
{
	if (MessageBlockerWidget == nullptr) return;

	MessageBlockerWidget->SetVisibility(ESlateVisibility::Hidden);

}