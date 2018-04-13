// Copyright Anubhav Singh 2019.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Public/DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Grabber.generated.h"

// Player View Struct
struct PlayerViewPoint
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	FVector LineTraceEnd;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	

private:

	// Reach of the player
	UPROPERTY(EditAnywhere)
		float Reach=150.f;

	UPhysicsHandleComponent *PhysicsHandler = nullptr;
	UInputComponent *InputComponent = nullptr;

	// Ray-Cast and grab what's in reach
	void Grab();

	// Release the Grabber
	void Release();

	// Find the Physics handle
	void FindPhysicsHandle();

	// Setup Input Component handle
	void SetupInputComponent();

	// Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();	

	// Get PlayerView Information
	PlayerViewPoint GetPlayerView();

};
