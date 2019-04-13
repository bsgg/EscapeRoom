// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveInterface.h"
#include "BasicInteractive.generated.h"

USTRUCT(BlueprintType)
struct FInteractiveProperties
{
	GENERATED_BODY()

public:

	FInteractiveProperties()
	{		
		ID = "Interactive_";
		EnableDefaultInspectMessage = false;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> InspectMessages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int IndexInspectMessage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableDefaultInspectMessage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DefaultInspectMessage;

};


UCLASS()
class ESCAPEROOM_API ABasicInteractive : public AActor, public IInteractiveInterface
{
	GENERATED_BODY()
	
public:	
	
	ABasicInteractive();

	FORCEINLINE FName GetInteractiveID() const { return Properties.ID; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* HighlightMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Collision;

	UPROPERTY(EditDefaultsOnly, Category = "Interactive")
	USoundBase* InteractSound;

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Basic Interactive Properties")
	FInteractiveProperties Properties;

	class AMainCharacter* CharacterOverlapping = nullptr;

	class ALobbyPlayerController* CurrentController;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void PlayInteractSound();

public:

	/////// IInteractiveInterface IMPLEMENTATION /////////
	virtual void Inspect(APawn* PawnInstigator)  override;

	virtual void StartInteract(APawn* PawnInstigator) override;

	virtual FString GetInteractID() override;
	/////// IInteractiveInterface IMPLEMENTATION /////////

protected:
	
};
