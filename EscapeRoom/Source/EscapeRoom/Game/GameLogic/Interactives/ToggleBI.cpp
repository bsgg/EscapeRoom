// Fill out your copyright notice in the Description page of Project Settings.

#include "ToggleBI.h"
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

void AToggleBI::Toggle()
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
