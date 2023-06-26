// Fill out your copyright notice in the Description page of Project Settings.


#include "Board.h"
#include "PaperSpriteComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "PaperFlipbook.h"
#include "Components/BoxComponent.h"

// Sets default values
ABoard::ABoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SET the SceneComponent as the RootComponent
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneComponent);

	//Set the top and bottom sprites
	TopSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperFlipbookComponent"));
	TopSprite->SetupAttachment(RootComponent);
	TopSprite->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	/*BottomSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperFlipbookComponent2"));
	BottomSprite->SetupAttachment(RootComponent);
	BottomSprite->SetRelativeLocation(FVector(0.f, 0.f, 0.f));*/

	//Set the collision boxes
	TopComponent = CreateDefaultSubobject<UBoxComponent>("TopComponent");
	TopComponent->SetupAttachment(RootComponent);
	TopComponent->SetBoxExtent(FVector(10.f,10.f,10.f));

	BottomComponent = CreateDefaultSubobject<UBoxComponent>("BottomComponent");
	BottomComponent->SetupAttachment(RootComponent);
	BottomComponent->SetBoxExtent(FVector(10.f, 10.f, 10.f));

	LeftGoal = CreateDefaultSubobject<UBoxComponent>("LeftGoal");
	LeftGoal->SetupAttachment(RootComponent);
	LeftGoal->SetBoxExtent(FVector(10.f, 10.f, 10.f));

	RightGoal = CreateDefaultSubobject<UBoxComponent>("RightGoal");
	RightGoal->SetupAttachment(RootComponent);
	RightGoal->SetBoxExtent(FVector(10.f, 10.f, 10.f));

}

// Called when the game starts or when spawned
void ABoard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

