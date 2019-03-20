// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveBase.generated.h"

USTRUCT(BlueprintType)
struct FInteractiveDefinition
{
	GENERATED_BODY()

public:

	FInteractiveDefinition()
	{
		IsDefaultDetailInspectActive = false;
		ID = "Interactive_";
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;

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

	// Whether the interactive has the use action or not, by default is true
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasUseAction = true;

};

UCLASS()
class ESCAPEROOM_API AInteractiveBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveBase();

	FORCEINLINE FName GetInteractiveID() const { return Definition.ID; } 

	FORCEINLINE FInteractiveDefinition GetData() const { return Definition; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractiveObject", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractiveObject", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractiveObject", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* XInputIconMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractiveObject", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Collision;

	UPROPERTY(Replicated)
	FInteractiveDefinition OldDefinition;

	UPROPERTY(ReplicatedUsing = OnRep_DefinitionChanged, EditAnywhere, BlueprintReadWrite, Category = "Interactive Basic Settings")
	FInteractiveDefinition Definition;

	UPROPERTY(Replicated)
	class AMainCharacter* CharacterOverlapping;



protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnRep_DefinitionChanged(FInteractiveDefinition PreviousData);

	void DisableCollider();

	void EnableCollider();

public:	

	virtual FString GetInspectDetail() const;

	virtual void ForwardInspectDetail();

	virtual void ResetInspectDetail();
};
