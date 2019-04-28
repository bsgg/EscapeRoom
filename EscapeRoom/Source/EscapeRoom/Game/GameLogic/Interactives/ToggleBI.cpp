// Fill out your copyright notice in the Description page of Project Settings.

#include "ToggleBI.h"
#include "Characters/MainCharacter.h"
#include "Lobby/LobbyPlayerController.h"
#include "Game/GameLogic/Interactives/UI/UIBasicInteractive.h"
#include "UnrealNetwork.h"

AToggleBI::AToggleBI()
{
	ObjectToToggle = CreateDefaultSubobject<USceneComponent>(TEXT("ObjectToToggle"));
	ObjectToToggle->SetupAttachment(RootComponent);
}

void AToggleBI::BeginPlay()
{
	Super::BeginPlay();

	if (ObjectToToggle == nullptr) return;

	ObjectToToggle->SetVisibility(IsOn, true);

}

void AToggleBI::StartInteract(APawn* PawnInstigator)
{
	Super::StartInteract(PawnInstigator);

	if (CurrentController == nullptr) return;

	if (IsOn)
	{
		CurrentController->ShowMessage(ToggleOnDetail.ToString(), 2.0f);

		if (CharacterOverlapping != nullptr)
		{
			CharacterOverlapping->StartGesture(EGestureType::VE_INTERACT, 2.0f);
		}

		UE_LOG(LogTemp, Warning, TEXT("[AToggleBI::StartInteract] Toggle is on"));

		if (HasWidget())
		{
			ShowWidget();			
		}
	}
	else
	{
		CurrentController->ShowMessage(ToggleOffDetail.ToString(), 2.0f);

		if (CharacterOverlapping != nullptr)
		{
			CharacterOverlapping->StartGesture(EGestureType::VE_DISMISS, 2.0f);
		}
	}
}




void AToggleBI::InteractOnConnectedInteractive()
{
	if (Role < ROLE_Authority)
	{
		ServerDoToggleAction();
	}
	else
	{
		DoToggleAction();
	}
}

void AToggleBI::ServerDoToggleAction_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("[AToggleBI::ServerDoToggleAction]"));

	DoToggleAction();
}

bool AToggleBI::ServerDoToggleAction_Validate()
{
	return true;
}

void AToggleBI::DoToggleAction()
{
	IsOn = !IsOn;
	OnRep_IsOnChanged();
}

void AToggleBI::OnRep_IsOnChanged()
{
	ObjectToToggle->SetVisibility(IsOn, true);
}

void AToggleBI::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AToggleBI, IsOn);
}
