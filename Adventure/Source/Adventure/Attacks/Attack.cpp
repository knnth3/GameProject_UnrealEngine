// By: Eric Marquez. All information and code provided is free to use and can be used comercially.Use of such examples indicates no fault to the author for any damages caused by them. The author must be credited.

#include "Attack.h"
#include "Adventure.h"


// Sets default values
AAttack::AAttack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AAttack::Initialize(FAttackReuqest Request)
{
	Damage = Request.Damage;
	Radius = Request.Radius;
	Destination = Request.Location;
	OnInitialized();
}

// Called when the game starts or when spawned
void AAttack::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

