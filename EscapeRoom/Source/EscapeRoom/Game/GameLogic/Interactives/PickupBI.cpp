// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupBI.h"
#include "Lobby/LobbyPlayerController.h"
#include "UnrealNetwork.h"

APickupBI::APickupBI()
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	PickupMesh->SetupAttachment(RootComponent);

	SetReplicates(true);
}

void APickupBI::BeginPlay()
{
	Super::BeginPlay();

	if (PickupAction.IsActive)
	{
		PickupMesh->SetVisibility(true, true);
	}
	else
	{
		PickupMesh->SetVisibility(false, true);
	}
}



void APickupBI::StartInteract(APawn* Instigator)
{
	//Super::StartInteract(Instigator);

	UE_LOG(LogTemp, Warning, TEXT("[APickupBI::StartInteract] "));

	auto PlayerController = Cast<ALobbyPlayerController>(Instigator->GetController());

	if (PlayerController)
	{
		FString Debug = "Interacting Pickup Object" + PickupAction.ObjectID.ToString();

		PlayerController->ShowDebugLog("Interacting Pickup Object");

		//UE_LOG(LogTemp, Warning, TEXT("[APickupBI::StartInteract] Player controller Found %s"), *Debug);

		if ((PickupAction.ObjectID != "None") && (PickupAction.IsActive))
		{
			UE_LOG(LogTemp, Warning, TEXT("[APickupBI::StartInteract] OBJECT"));

			//if (Role == ROLE_Authority)
			//{


				UE_LOG(LogTemp, Warning, TEXT("[APickupBI::StartInteract] OBJECT EXECUTING PICKUP ACTION"));

				

				//PickupMesh->SetVisibility(false, true);

				//OnRep_PickupActionChanged();
			//}

				if (Role < ROLE_Authority)
				{

					ServerDoAction();
					
				}
				else
				{
					DoAction();
				}

				// TODO: SHOW ANIMATION AND SEND PICKUP ID
			PlayerController->ShowMessage(PickupAction.DetailDefaultAction.ToString());

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[APickupBI::StartInteract] NO OBJECT"));

			PlayerController->ShowMessage(PickupAction.DetailWrongAction.ToString());
		}
	}
}

void APickupBI::OnRep_PickupActionChanged()
{
	UE_LOG(LogTemp, Warning, TEXT("[APickupBI::OnRep_PickupActionChanged] Changed"));

	if (PickupAction.IsActive)
	{
		PickupMesh->SetVisibility(true, true);

	}
	else
	{
		PickupMesh->SetVisibility(false, true);

	}
}




void APickupBI::ServerDoAction_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("[APickupBI::DoAction_Implementation]"));
	DoAction();
	
	/*if (PickupAction.IsActive)
	{
		PickupMesh->SetVisibility(true, true);
	}
	else
	{
		PickupMesh->SetVisibility(false, true);

		if (PickupAction.DestroyWhenActionCompleted)
		{
			Destroy();
		}
	}*/


	//if (PickupAction.DestroyWhenActionCompleted)
	//{
		//Destroy();
	//}
}

bool APickupBI::ServerDoAction_Validate()
{
	return true;
}

void APickupBI::DoAction()
{
	PickupAction.ObjectID = "None";

	PickupAction.IsActive = false;

	Properties.EnableDefaultInspectMessage = true;

	PickupMesh->SetVisibility(false, true);

	// TODO: TEST IF THIS IS NECESARY
	OnRep_PickupActionChanged();
}



void APickupBI::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APickupBI, PickupAction);
}
