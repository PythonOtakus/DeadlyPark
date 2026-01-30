
#include "Common.h"
#include "KismetTraceUtils.h"

FCollisionQueryParams UCommon::QueryParams = FCollisionQueryParams::DefaultQueryParam;

inline void UCommon::IgnoreActor(const AActor* Actor)
{
	QueryParams.AddIgnoredActor(Actor);
}

inline TArray<FOverlapResult>& UCommon::OverlapSphere(TArray<FOverlapResult>& Overlaps, const FVector& Center,
	const float Radius, const ECollisionChannel Channel)
{
	GWorld->OverlapMultiByChannel(Overlaps, Center, FQuat::Identity,
								  Channel, FCollisionShape::MakeSphere(Radius), QueryParams);
	QueryParams.ClearIgnoredSourceObjects();
	return Overlaps;
}

inline TArray<FHitResult>& UCommon::LineTrace(TArray<FHitResult>& Hits, const FVector& Start, const FVector& End, const ECollisionChannel Channel, const bool Debug)
{
	bool bHit = GWorld->LineTraceMultiByChannel(Hits, Start, End, Channel, QueryParams);
	if (Debug)
	{
		DrawDebugLineTraceMulti(GWorld, Start, End, EDrawDebugTrace::ForOneFrame, bHit, Hits,
			FColor::Green, FColor::Red, -1.0f);
	}
	QueryParams.ClearIgnoredSourceObjects();
	return Hits;
}

inline TArray<FHitResult>& UCommon::ScreenCenterTrace(TArray<FHitResult>& Hits, const float Distance, const ECollisionChannel Channel, const bool Debug)
{
	FVector Location;
	FRotator Rotation;
	GWorld->GetFirstPlayerController()->GetPlayerViewPoint(Location, Rotation);
	return LineTrace(Hits, Location, Location + Rotation.Vector() * Distance, Channel, Debug);
}

inline TArray<FHitResult>& UCommon::SweepSphere(TArray<FHitResult>& Hits, const FVector& Start, const FVector& End,
	const float Radius, const ECollisionChannel Channel)
{
	GWorld->SweepMultiByChannel(Hits, Start, End, FQuat::Identity,
								Channel, FCollisionShape::MakeSphere(Radius), QueryParams);
	QueryParams.ClearIgnoredSourceObjects();
	return Hits;
}
