// Fill out your copyright notice in the Description page of Project Settings.


#include "UseWithUIBI.h"
#include "Lobby/LobbyPlayerController.h"
#include "Characters/MainCharacter.h"
#include "Game/GameLogic/Interactives/UI/UIBasicInteractive.h"


AUseWithUIBI::AUseWithUIBI()
{
	
}

void AUseWithUIBI::BeginPlay()
{
	Super::BeginPlay();

}

void AUseWithUIBI::StartInteract(APawn* PawnInstigator)
{
	Super::StartInteract(PawnInstigator);

	if (CurrentController == nullptr) return;

	if (IsCompleted)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::StartInteract] Completed"));

		CurrentController->ShowMessage(CompletedActionMessage, 2.0f);

		if (CharacterOverlapping != nullptr)
		{
			CharacterOverlapping->StartGesture(EGestureType::VE_DISMISS, 2.0f);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::StartInteract] Incompleted"));

		ShowWidget();

		if (CharacterOverlapping != nullptr)
		{
			CharacterOverlapping->StartGesture(EGestureType::VE_INTERACT, 2.0f);
		}
	}
}

void AUseWithUIBI::OnComplete()
{
	UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::OnComplete] Called"));

	Super::OnComplete();

	if (Role < ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::OnComplete] ServerDoAction"));
		ServerDoAction();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::OnComplete] Do Action"));
		DoAction();
	}
}



void AUseWithUIBI::ServerDoAction_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::ServerDoAction_Implementation]"));

	DoAction();
}

bool AUseWithUIBI::ServerDoAction_Validate()
{
	return true;
}

void AUseWithUIBI::DoAction()
{
	UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::DoAction]"));

	if (bDoActionOnThisInteractive)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::DoAction] Acting over this interactive "));

		OnCompleteActionEvent();
	}

	return;

	ABasicInteractive* interactive = FindConnectedInteractive();

	if (interactive != nullptr)
	{
		IsCompleted = true;

		UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::DoAction] Connected Interactive found "));

		interactive->InteractOnConnectedInteractive();
		// TODO: Call interactive type door
		// ADoorBI * DoorInteractive = = Cast<ADoorBI>(interactive);
		// if (DoorInteractive)
		//{
			//UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::DoAction] Calling open door "));

			//DoorInteractive->Open();
		//}
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[AUseWithUIBI::DoAction] Connected Interactive not found "));
	}

}

void AUseWithUIBI::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUseWithUIBI, IsCompleted);

	//DOREPLIFETIME(AUseWithUIBI, ConnectedInteractiveID);
	
}

