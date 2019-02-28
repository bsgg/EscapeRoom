// Fill out your copyright notice in the Description page of Project Settings.

#include "ToggleInteractive.h"
#include "UnrealNetwork.h"

AToggleInteractive::AToggleInteractive()
{
	// Inventory Component
	ObjectToToggle = CreateDefaultSubobject<USceneComponent>(TEXT("ObjectToToggle"));
	ObjectToToggle->SetupAttachment(RootComponent);
}

void AToggleInteractive::BeginPlay()
{
	Super::BeginPlay();

	if (ObjectToToggle == nullptr) return;
	
	ObjectToToggle->SetVisibility(IsOn, true);	

}

void AToggleInteractive::Toggle()
{
	if (Role == ROLE_Authority)
	{		
		IsOn = !IsOn;
		OnRep_IsOnChanged();
	}
	
}

void AToggleInteractive::OnRep_IsOnChanged()
{
	ObjectToToggle->SetVisibility(IsOn, true);
}


void AToggleInteractive::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AToggleInteractive, IsOn);
}


