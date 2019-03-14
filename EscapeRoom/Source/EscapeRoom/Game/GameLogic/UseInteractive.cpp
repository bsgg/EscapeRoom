// Fill out your copyright notice in the Description page of Project Settings.

#include "UseInteractive.h"

#include "UnrealNetwork.h"

AUseInteractive::AUseInteractive()
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	PickupMesh->SetupAttachment(RootComponent);

	SetReplicates(true);
}

void AUseInteractive::BeginPlay()
{
	Super::BeginPlay();

}



void AUseInteractive::Use()
{
	if (Role == ROLE_Authority)
	{
		OnUseEvent();
		
		UseAction.IsActive = false;
		OnRep_UseActionChanged();

		if (UseAction.HasAnimation())
		{
			// Set timer
			GetWorld()->GetTimerManager().SetTimer(AnimationTimerHandle, this, &AUseInteractive::OnEndAnimationTimer, UseAction.AnimationLength, false);
		}
		else
		{
			// Enable Pickup action
			if (PickupAction.HasObject() && (!PickupAction.IsActive))
			{
				PickupAction.IsActive = true;
				OnRep_PickupActionChanged();
			}
		}
	}
}


void AUseInteractive::Pickup()
{
	if (Role == ROLE_Authority)
	{
		PickupAction.IsActive = false;
		PickupAction.ObjectID = "NONE";

		AInputIconMesh->SetVisibility(false);
		PickupMesh->SetVisibility(false);

		OnRep_PickupActionChanged();
	}
}

void AUseInteractive::OnEndAnimationTimer()
{
	if (Role == ROLE_Authority)
	{
		if (PickupAction.HasObject() && (!PickupAction.IsActive))
		{
			PickupAction.IsActive = true;
			OnRep_PickupActionChanged();
		}
	}
}

void AUseInteractive::OnRep_UseActionChanged()
{
	if (!UseAction.IsActive)
	{
		AInputIconMesh->SetVisibility(false);
	}
}

void AUseInteractive::OnRep_PickupActionChanged()
{
	if (PickupAction.IsActive)
	{
		AInputIconMesh->SetVisibility(true);
	}
	else
	{
		AInputIconMesh->SetVisibility(false);
	}

	if (PickupAction.HasObject())
	{
		PickupMesh->SetVisibility(true);
	}
	else
	{
		PickupMesh->SetVisibility(false);
	}
}


void AUseInteractive::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUseInteractive, UseAction);

	DOREPLIFETIME(AUseInteractive, PickupAction);
}

