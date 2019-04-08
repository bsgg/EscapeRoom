#include "PauseMenu.h"
#include "Lobby/LobbyPlayerController.h"
#include "Components/Button.h"


bool UPauseMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (ResumeGameButton == nullptr) return false;
	ResumeGameButton->OnClicked.AddDynamic(this, &UPauseMenu::OnResumeGamePressed);

	if (ExitGameButton == nullptr) return false;
	ExitGameButton->OnClicked.AddDynamic(this, &UPauseMenu::OnExitGamePressed);
	return true;

}

void UPauseMenu::OnResumeGamePressed()
{
	UWorld*	World = GetWorld();
	if (World == nullptr) return;

	ALobbyPlayerController* PlayerController = Cast<ALobbyPlayerController>(World->GetFirstPlayerController());
	if (PlayerController == nullptr) return;

	PlayerController->UnPause();
}

void UPauseMenu::OnExitGamePressed()
{
	// TODO: EXIT SESSION
	if (MenuInterface == nullptr) return;
	MenuInterface->EndSession();
}
