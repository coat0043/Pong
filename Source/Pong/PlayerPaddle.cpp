// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPaddle.h"

#include "PaperSpriteComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Classes/PaperSprite.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ArrowComponent.h"

#include "Ball.h"

// Sets default values
APlayerPaddle::APlayerPaddle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

#pragma region CapsuleComponent
    //CREATE the CapsuleComponent
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
    //SET the Collision Profile Name to BlockAll in the CapsuleComponent
    CapsuleComponent->SetCollisionProfileName("BlockAll");
    //SET Notify Rigid Body Collisions to true on the CapsuleComponent
    CapsuleComponent->SetNotifyRigidBodyCollision(true);
    //SET Simulate Physics to true on the CapsuleComponent
    CapsuleComponent->SetSimulatePhysics(true);

    //CALL SetLinearDamping() on the CapsuleComponent passing in 10. Play around with these values
    CapsuleComponent->SetLinearDamping(10);
    //CALL SetAngularDamping() on the CapsuleComponent passing in 10. Play around with these values
    CapsuleComponent->SetAngularDamping(10);
    //CALL LockPhysicsTo2D on the CapsuleComponent using the function in UMyBlueprintFunctionLibrary (Have a look at MyBlueprintFunctionLibrary.cpp)
    //UMyBlueprintFunctionLibrary::LockPhysicsTo2DAxis(CapsuleComponent);
    //CALL AddDynamic() on CapsuleComponent->OnComponentHit passing in: this, &APlayerPawn::HandleBoxHit
    CapsuleComponent->OnComponentHit.AddDynamic(this, &APlayerPaddle::HandleBoxHit);
    //SET the CapsuleComponent as the RootComponent
    SetRootComponent(CapsuleComponent);
#pragma endregion CapsuleCOmponent will be new RootComponent

#pragma region PlayerSprite
    //COMMENT: Setup the Player Visual 
    //CREATE the PlayerSprite Component
    PlayerSprite = CreateDefaultSubobject<UPaperSpriteComponent>("Player");
    //SET the Collision Profile Name to NoCollision on the PlayerSprite
    PlayerSprite->SetCollisionProfileName("NoCollision");
    //SET the Collision Enabled to NoCollision on the PlayerSprite
    PlayerSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    //SET Generate Overlap Events to false on the PlayerSprite. Look up the SetGenerateOverlapEvents() function in the documentation
    PlayerSprite->SetGenerateOverlapEvents(false);
    //ATTACH the PlayerSprite to the RootComponent
    PlayerSprite->SetupAttachment(RootComponent);
#pragma endregion PlayerSprite Setup and Set it as the Root Component, It also performs the Blocking when it hits something. Does not report the collisions

#pragma region SpringArmComponent And Camera
    /** COMMENT: camera boom  */
   //CREATE the SpringArmComponent
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
    //ATTACH the SpringArmComponent to the RootComponent
    SpringArmComponent->SetupAttachment(RootComponent);
    //SET the Inherit Pitch, Yaw and Roll values to false on the SpringArmComponent
    SpringArmComponent->bInheritPitch = false;
    SpringArmComponent->bInheritYaw = false;
    SpringArmComponent->bInheritRoll = false;
    //SET the Relative Rotation to -90.0f on the Yaw of the SpringArmComponent 
    SpringArmComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

    //CREATE the FollowCamera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>("Camera");
    //SET the Projection Mode to Orthographic on the FollowCamera
    FollowCamera->SetProjectionMode(ECameraProjectionMode::Orthographic);
    //SET the Ortho Width to 5000 on the FollowCamera
    FollowCamera->OrthoWidth = 5000;
    //ATTACH the FollowCamera to the SpringArmComponent
    FollowCamera->SetupAttachment(SpringArmComponent);
#pragma endregion SpringArmComponent SpringArm and Camera Component Setup

    /**COMMENT: projectile spawn point  */
    //CREATE the ProjectileSpawnPoint
    ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>("Projectile");
    //ATTACH the ProjectileSpawnPoint to the PlayerSprite
    ProjectileSpawnPoint->SetupAttachment(PlayerSprite);
    //CALL SetRelativeLocation() on ProjectileSpawnPoint passing in: FVector(40.f, 0.f, 30.f)
    ProjectileSpawnPoint->SetRelativeLocation(FVector(40.f, 0.f, 30.f));

    //CALL Add() on Tags (This is a variable in AActor), passing in: "Player". A tag is a label, so we can label this Actor with multiple tags  
    Tags.Add("Player");
}

void APlayerPaddle::SetCanShoot()
{
    //SET bCanShoot to true
    bCanShoot = true;
}

// Called when the game starts or when spawned
void APlayerPaddle::BeginPlay()
{
	Super::BeginPlay();

    //COMMENT: DON't Forget to set bCanShoot
    //SET bCanShoot to true
    bCanShoot = true;

    APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (controller)
    {
        FString command = "show collision";
        controller->ConsoleCommand(command, true);
        controller->bShowMouseCursor = true;
        controller->bEnableClickEvents = true;
        controller->bEnableMouseOverEvents = true;
    }
}

void APlayerPaddle::HandleBoxHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    //IF OtherActor NOT EQUAL to nullptr
    if (OtherActor != nullptr)
    {
        //IF OtherActor->ActorHasTag("Floor")
        if (OtherActor->ActorHasTag("Floor"))
        {
            //SET bOnFloor to true
            bOnFloor = true;
        }
        //ENDIF
    }
    //ENDIF
}

void APlayerPaddle::SpawnProjectile()
{
    //IF ProjectileTemplate NOT EQUAL to nullptr
    if (ProjectileTemplate != nullptr)
    {
        //CREATE a variable called World of  UWorld* and assign it to the return value of GetWorld()
        UWorld* World = GetWorld();
        //IF World != nullptr
        if (World != nullptr)
        {
            //IF bCanShoot IS true
            if (bCanShoot == true)
            {
                //SET bCanShoot to false
                bCanShoot = false;
                //DECLARE a variable called SpawnParams of type FActorSpawnParameters
                FActorSpawnParameters SpawnParams;
                //SET  SpawnParams.Owner to this
                SpawnParams.Owner = this;
                //SET  SpawnParams.Instigator to the return value of GetInstigator()
                SpawnParams.Instigator = GetInstigator();
                //SET SpawnParams.SpawnCollisionHandlingOverride to ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
                SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
                //DECLARE a variable called SpawnLocation of type FVector and assign it to the return value of  GetProjectileSpawnPoint()->GetComponentLocation()
                FVector SpawnLocation = GetProjectileSpawnPoint()->GetComponentLocation();
                //DECLARE a variable called SpawnRotation of type FRotator and assign it to the return value of GetProjectileSpawnPoint()->GetComponentRotation()
                FRotator SpawnRotation = GetProjectileSpawnPoint()->GetComponentRotation();

                //DECLARE a variable called SpawnedProjectile of type AProjectile* and assign it to the return value of  World->SpawnActor<AProjectile>() passing in: ProjectileTemplate, SpawnLocation, SpawnRotation, SpawnParams
                ABall* SpawnedProjectile = World->SpawnActor<ABall>(ProjectileTemplate, SpawnLocation, SpawnRotation, SpawnParams);
                //IF SpawnedProjectile NOT EQUAL to nullptr
                if (SpawnedProjectile != nullptr)
                {
                    //CALL AddOnScreenDebugMessage() on GEngine passing in: -1, 15.0f, FColor::Blue, "Spawn Projectile"
                    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "Spawn Projectile");
                    //CALL SetLifeSpan() on SpawnedProjectile passing in: ProjectileLifeSpan
                    SpawnedProjectile->SetLifeSpan(ProjectileLifeSpan);
                    //CALL GetWorldTimerManager().SetTimer() passing in: SetCanShootTimer, this, &APlayerPawn::SetCanShoot, FireTimerDelay, false
                    GetWorldTimerManager().SetTimer(SetCanShootTimer, this, &APlayerPaddle::SetCanShoot, FireTimerDelay, false);
                }
                //ENDIF
            }
            //ENDIF
        }
        //ENDIF
    }
    //ELSE

    {

    }
    //ENDIF
}

// Called every frame
void APlayerPaddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    //IF MovementRight NOT EQUAL to 0
    if (MovementRight != 0)
    {
        //IF Movement Right is GREATER than 0
        if (MovementRight > 0)
        {
            //SET the Relative Rotation on the PlayerSprite to a ZeroRotator
            PlayerSprite->SetRelativeRotation(FRotator::ZeroRotator);
        }
        //ENDIF

        //IF MovementRight is LESS than 0
        if (MovementRight < 0)
        {
            //SET the Relative Rotation on the PlayerSprite to 180
            PlayerSprite->SetRelativeRotation(FRotator(0, 180, 0));
        }
        //ENDIF

        //CALL AddForce() on the CapsuleComponent's BodyInstance moving it on the Forward Direction * MovementRight * 40000
        CapsuleComponent->AddForce(GetActorForwardVector() * MovementRight * 4000000);
    }
    //ENDIF


    //ENDIF
}

// Called to bind functionality to input
void APlayerPaddle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    //CALL BindAxis() on the PlayerInputComponent, Binding the MoveRight Input Axis to the MoveRight Function
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerPaddle::MoveUp);

    //CALL BindAction() on the PlayerInputComponent, Binding the Fire Input Action to the SpawnProjectile Function when pressed
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerPaddle::SpawnProjectile);
}

void APlayerPaddle::MoveUp(float value)
{
    //SET MovementRight to value
    MovementRight = value;
}

