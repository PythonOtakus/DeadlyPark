// Copyright Epic Games, Inc. All Rights Reserved.


#include "ShooterCharacter.h"
#include "ShooterWeapon.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"
#include "ShooterGameMode.h"
#include "Inventory/Equipment.h"
#include "Inventory/InventoryComponent.h"
#include "Minimap/MinimapCapture.h"
#include "Net/UnrealNetwork.h"
#include "Utility/Common.h"

AShooterCharacter::AShooterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	PawnNoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("Pawn Noise Emitter"));
	
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
}

void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = MaxHP;

	OnDamaged.Broadcast(1.0f);
	
	AddWeaponClass(InitialWeaponClass);
	
	MinimapCapture = GetWorld()->SpawnActor<AMinimapCapture>(MinimapCaptureClass);
}

void AShooterCharacter::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);
}

void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AShooterCharacter::DoStartFiring);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AShooterCharacter::DoStopFiring);

		EnhancedInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Triggered, this, &AShooterCharacter::DoSwitchWeapon);
		
		EnhancedInputComponent->BindAction(PickupAction, ETriggerEvent::Triggered, this, &AShooterCharacter::DoPickup);
		
		EnhancedInputComponent->BindAction(ShowInventoryAction, ETriggerEvent::Triggered, this, &AShooterCharacter::DoShowInventory);
		
		EnhancedInputComponent->BindAction(HideInventoryAction, ETriggerEvent::Completed, this, &AShooterCharacter::DoHideInventory);
		
		EnhancedInputComponent->BindAction(MarkPointAction, ETriggerEvent::Triggered, this, &AShooterCharacter::DoMarkPoint);
	}
}

void AShooterCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	OnPickupUpdated.Broadcast(Inventory->DetectAheadEquipment());
	MinimapCapture->CaptureAround(this);
}

float AShooterCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (CurrentHP <= 0.0f) return 0.0f;
	CurrentHP -= Damage;
	if (CurrentHP <= 0.0f) Die();
	OnDamaged.Broadcast(FMath::Max(0.0f, CurrentHP / MaxHP));
	return Damage;
}

void AShooterCharacter::DoAim(float Yaw, float Pitch)
{
	if (!IsDead()) Super::DoAim(Yaw, Pitch);
}

void AShooterCharacter::DoMove(float Right, float Forward)
{
	if (!IsDead())
	{
		Super::DoMove(Right, Forward);
	}
}

void AShooterCharacter::DoJumpStart()
{
	if (!IsDead())
	{
		Super::DoJumpStart();
	}
}

void AShooterCharacter::DoJumpEnd()
{
	if (!IsDead())
	{
		Super::DoJumpEnd();
	}
}

void AShooterCharacter::DoStartFiring()
{
	if (CurrentWeapon && !IsDead())
	{
		CurrentWeapon->StartFiring();
	}
}

void AShooterCharacter::DoStopFiring()
{
	if (CurrentWeapon && !IsDead())
	{
		CurrentWeapon->StopFiring();
	}
}

void AShooterCharacter::DoSwitchWeapon()
{
	if (OwnedWeapons.Num() > 1 && !IsDead())
	{
		CurrentWeapon->DeactivateWeapon();

		int32 WeaponIndex = OwnedWeapons.Find(CurrentWeapon);

		if (WeaponIndex == OwnedWeapons.Num() - 1)
		{
			WeaponIndex = 0;
		}
		else {
			++WeaponIndex;
		}

		CurrentWeapon = OwnedWeapons[WeaponIndex];

		CurrentWeapon->ActivateWeapon();
	}
}

void AShooterCharacter::DoPickup()
{
	if (AEquipment* Equipment = Inventory->DetectAheadEquipment())
	{
		int32 Quantity = Inventory->AddEquipment(Equipment->GetClass(), Equipment->Count);
		
		UCommon::Debug(FString::Printf(TEXT("拾取%d个%s"),
			Quantity, *Equipment->GetEquipmentData()->Name.ToString()));
		
		Equipment->Count -= Quantity;
		if (Equipment->Count <= 0)
		{
			Equipment->Destroy();
		}
	}
}

void AShooterCharacter::DoShowInventory()
{
	OnInventoryShown.Broadcast(Inventory);
}

void AShooterCharacter::DoHideInventory()
{
	OnInventoryHidden.Broadcast(Inventory);
}

void AShooterCharacter::DoMarkPoint()
{
	UCommon::IgnoreActor(this);
	for (const FHitResult& Hit : UCommon::ScreenCenterTrace(MarkHits, MarkDistance, ECC_WorldStatic))
	{
		if (true)
		{
			OnPointMarked.Broadcast(Hit.Location);
			return;
		}
	}
}

void AShooterCharacter::AttachWeaponMeshes(AShooterWeapon* Weapon)
{
	const FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);

	Weapon->AttachToActor(this, AttachmentRule);

	Weapon->GetFirstPersonMesh()->AttachToComponent(GetFirstPersonMesh(), AttachmentRule, FirstPersonWeaponSocket);
	Weapon->GetThirdPersonMesh()->AttachToComponent(GetMesh(), AttachmentRule, FirstPersonWeaponSocket);
	
}

void AShooterCharacter::AddWeaponRecoil(float Recoil)
{
	AddControllerPitchInput(Recoil);
}

void AShooterCharacter::UpdateWeaponHUD(int32 CurrentAmmo, int32 MagazineSize)
{
	OnBulletCountUpdated.Broadcast(MagazineSize, CurrentAmmo);
}

FVector AShooterCharacter::GetWeaponTargetLocation()
{
	FHitResult OutHit;

	const FVector Start = GetFirstPersonCameraComponent()->GetComponentLocation();
	const FVector End = Start + (GetFirstPersonCameraComponent()->GetForwardVector() * MaxAimDistance);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, QueryParams);

	return OutHit.bBlockingHit ? OutHit.ImpactPoint : OutHit.TraceEnd;
}

void AShooterCharacter::AddWeaponClass(const TSubclassOf<AShooterWeapon>& WeaponClass)
{
	AShooterWeapon* OwnedWeapon = FindWeaponOfType(WeaponClass);

	if (!OwnedWeapon)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;

		AShooterWeapon* AddedWeapon = GetWorld()->SpawnActor<AShooterWeapon>(WeaponClass, GetActorTransform(), SpawnParams);

		if (AddedWeapon)
		{
			OwnedWeapons.Add(AddedWeapon);

			if (CurrentWeapon)
			{
				CurrentWeapon->DeactivateWeapon();
			}

			CurrentWeapon = AddedWeapon;
			CurrentWeapon->ActivateWeapon();
		}
	}
}

void AShooterCharacter::OnWeaponActivated(AShooterWeapon* Weapon)
{
	OnBulletCountUpdated.Broadcast(Weapon->GetMagazineSize(), Weapon->GetBulletCount());

	GetFirstPersonMesh()->SetAnimInstanceClass(Weapon->GetFirstPersonAnimInstanceClass());
	GetMesh()->SetAnimInstanceClass(Weapon->GetThirdPersonAnimInstanceClass());
}

AShooterWeapon* AShooterCharacter::FindWeaponOfType(TSubclassOf<AShooterWeapon> WeaponClass) const
{
	for (AShooterWeapon* Weapon : OwnedWeapons)
	{
		if (Weapon->IsA(WeaponClass))
		{
			return Weapon;
		}
	}

	return nullptr;
}

void AShooterCharacter::Die()
{
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->DeactivateWeapon();
	}

	if (AShooterGameMode* GM = Cast<AShooterGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->IncrementTeamScore(TeamByte);
	}

	Tags.Add(DeathTag);
	
	GetCharacterMovement()->StopMovementImmediately();

	DisableInput(nullptr);

	OnBulletCountUpdated.Broadcast(0, 0);

	BP_OnDeath();

	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &AShooterCharacter::OnRespawn, RespawnTime, false);
}

void AShooterCharacter::OnRespawn()
{
	Destroy();
}

void AShooterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// DOREPLIFETIME(AShooterCharacter, CurrentHP);
	// DOREPLIFETIME(AShooterCharacter, MaxHP);
	// DOREPLIFETIME(AShooterCharacter, TeamByte);
}

bool AShooterCharacter::IsDead() const
{
	return CurrentHP <= 0.0f;
}
