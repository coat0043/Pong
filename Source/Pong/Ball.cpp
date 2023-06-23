// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PaperSpriteComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "PaperFlipbook.h"
//#include "Utils/MyBlueprintFunctionLibrary.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/** sphere collision  */
	//CREATE the SphereComponent
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SceneRoot");
	//CALL SetSphereRadius() on SphereComponent passing in: 4.0f
	SphereComponent->SetSphereRadius(4.0f);
	//CALL  SetCollisionProfileName() on SphereComponent, passing in: "BlockAll"
	SphereComponent->SetCollisionProfileName("BlockAll");
	//CALL AddDynamic() on SphereComponent->OnComponentHit passing in: this, &AProjectile::OnHit
	SphereComponent->OnComponentHit.AddDynamic(this, &ABall::OnHit);
	//CALL UMyBlueprintFunctionLibrary::LockPhysicsTo2DAxis() passing in: SphereComponent . Have a look at this function and the UMyBlueprintFunctionLibrary class
	//UMyBlueprintFunctionLibrary::LockPhysicsTo2DAxis(SphereComponent);

	//SET the SphereComponent as the RootComponent
	SetRootComponent(SphereComponent);

	/** paper sprite component  */
	ProjectileSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperFlipbookComponent"));
	ProjectileSprite->SetupAttachment(RootComponent);
	ProjectileSprite->SetRelativeLocation(FVector(-3.f, 0.f, 0.f));

	/** ProjectileMovementComponent  */
	//CREATE the ProjectileMovementComponent
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Movement");
	//SET the ProjectileMovementComponent->UpdatedComponent to the SphereComponent
	ProjectileMovementComponent->UpdatedComponent = SphereComponent;
	//SET ProjectileMovementComponent->InitialSpeed to 1000.f
	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	//SET ProjectileMovementComponent->MaxSpeed to 1000.f;
	ProjectileMovementComponent->MaxSpeed = 1000.0f;
	//SET ProjectileMovementComponent->bRotationFollowsVelocity to false
	ProjectileMovementComponent->bRotationFollowsVelocity = false;
	//SET ProjectileMovementComponent->bShouldBounce to true
	ProjectileMovementComponent->bShouldBounce = true;

	/** projectiles not intended to live forever  */
	//SET InitialLifeSpan to 5.f. This sets the life span of this actor to 5 seconds. It will be destroyed after 5 seconds
	InitialLifeSpan = 5.0f;
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABall::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}

