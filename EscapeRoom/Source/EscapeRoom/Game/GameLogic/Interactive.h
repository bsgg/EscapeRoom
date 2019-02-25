// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactive.generated.h"

USTRUCT(BlueprintType)
struct FInteractiveDefinition
{
	GENERATED_BODY()

public:

	FInteractiveDefinition()
	{	
		IsActive = true;
		IsDefaultDetailInspectActive = false;
		ID = "Interactive_";
		
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsLocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> DetailInspect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int IndexDetailInspect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DefaultDetailInspect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDefaultDetailInspectActive;	

};


UCLASS()
class ESCAPEROOM_API AInteractive : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractiveObject", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractiveObject", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractiveObject", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* AInputIconMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractiveObject", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* XInputIconMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractiveObject", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Collision;

	
public:	
	AInteractive();

	virtual FString GetInspectDetail() const;

	virtual void ForwardInspectDetail();

	virtual void ResetInspectDetail();


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(Replicated)
	FInteractiveDefinition OldDefinition;

	UPROPERTY(ReplicatedUsing = OnRep_DefinitionChanged, EditAnywhere, BlueprintReadWrite, Category = "Interactable Data")
	FInteractiveDefinition Definition;

	UPROPERTY(Replicated)
	class AMainCharacter* CharacterOverlapping;

	UFUNCTION()
	void OnRep_DefinitionChanged(FInteractiveDefinition PreviousData);


};
