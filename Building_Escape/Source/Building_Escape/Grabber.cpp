// Copyright Anubhav Singh 2019.

#include "Grabber.h"
#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandle();
	SetupInputComponent();
}

/// Look for attached Physics Handle
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandler = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandler == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Handle Component for %s not found"), *(GetOwner()->GetName()))
	}
}

/// look for InputComponent
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (!InputComponent) { UE_LOG(LogTemp, Error, TEXT("Input Component for %s not found"), *(GetOwner()->GetName())) return; }
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Input Component for %s not found"), *(GetOwner()->GetName()))
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"))

	/// LINE TRACE and try to reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if (!PhysicsHandler) { UE_LOG(LogTemp, Error, TEXT("Physics Handle Component for %s not found"), *(GetOwner()->GetName())) return; }
	/// If we hit something attach a physics handle
	if (ActorHit)
	{	
		PhysicsHandler->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None, // no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation()
		);
	}
}

void UGrabber::Release()
{
	if (!PhysicsHandler) { UE_LOG(LogTemp, Error, TEXT("Physics Handle Component for %s not found"), *(GetOwner()->GetName())) return; }
	PhysicsHandler->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	PlayerViewPoint PlayerView = GetPlayerView();

	if (!PhysicsHandler) { return; }
	// if the physics handle is attached 
	if (PhysicsHandler->GrabbedComponent)
	{
		// move the object we're holding
		PhysicsHandler->SetTargetLocation(PlayerView.LineTraceEnd);
	}
	
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// Get player viewpoint this tick
	PlayerViewPoint PlayerView = GetPlayerView();

	// Line-Trace (AKA Ray-Cast) out to reach distance
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		PlayerView.PlayerViewPointLocation,
		PlayerView.LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	// See what we hit
	return HitResult;
}

PlayerViewPoint UGrabber::GetPlayerView()
{
	PlayerViewPoint PlayerView;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerView.PlayerViewPointLocation,
		OUT PlayerView.PlayerViewPointRotation
	);

	PlayerView.LineTraceEnd = PlayerView.PlayerViewPointLocation + (Reach * PlayerView.PlayerViewPointRotation.Vector());
	return PlayerView;
}
