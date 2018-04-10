// Copyright (c) 2006-2012 Audiokinetic Inc. / All Rights Reserved

/*=============================================================================
AkOcclusionObstructionService.h:
=============================================================================*/

#pragma once

#include "AkInclude.h"
#include "AkAudioDevice.h"
#include <functional>

class UAkComponent;
class AActor;
class AAkAcousticPortal;

struct FAkListenerOcclusionObstruction
{
	float CurrentValue;
	float TargetValue;
	float Rate;

	FAkListenerOcclusionObstruction(float in_TargetValue = 0.0f, float in_CurrentValue = 0.0f);

	void SetTarget(float in_TargetValue);
	bool ReachedTarget();
	bool Update(float DeltaTime);
};

struct FAkListenerOcclusionObstructionPair
{
	FAkListenerOcclusionObstruction Occ;
	FAkListenerOcclusionObstruction Obs;
	FVector Position;
	bool Update(float DeltaTime);
	bool ReachedTarget();
};

class AkOcclusionObstructionService
{
public:
	// Init for an AkComponent
	void Init(UAkComponent* in_akComponent, float in_refreshInterval);

	// Init for an acoustic portal
	void Init(AAkAcousticPortal* in_portalId, float in_refreshInterval);

	void Tick(const UAkComponentSet& in_Listeners, const FVector& SourcePosition, const AActor* Actor, AkRoomID RoomID, ECollisionChannel in_collisionChannel, float DeltaTime, float OcclusionRefreshInterval);

	/**
	 * Calculates updated occlusion and obstruction values synchronously and then sends them to the Wwise engine.
	 */
	void UpdateObstructionOcclusion(const UAkComponentSet& in_Listeners, const FVector& SourcePosition, const AActor* Actor, AkRoomID RoomID, ECollisionChannel in_collisionChannel, float OcclusionRefreshInterval);

	void ClearOcclusionValues();

	bool HasTasksInProgress();

private:
	FThreadSafeCounter NumAsyncTasksInProgress = 0;
	/**
	 * Fades active occlusions towards targets, sends updated values to the Wwise engine, then calculates refreshed occlusion and obstruction values asynchronously. 
	 */
	void RefreshObstructionOcclusion(const UAkComponentSet& in_Listeners, const FVector& SourcePosition, const AActor* Actor, AkRoomID RoomID, ECollisionChannel in_collisionChannel, const float DeltaTime, float OcclusionRefreshInterval);
	/**
	 * Loops through in_Listeners and sends the obstruction occlusion values on each to the Wwise engine.
	 */
	void SetObstructionOcclusion(const UAkComponentSet& in_Listeners, AkRoomID RoomID);
	/**
	 * Calculates updated occlusion and obstruction values asynchronously. 
	 */
	void CalculateObstructionOcclusionValuesAsync(const UAkComponentSet& in_Listeners, const FVector& SourcePosition, const AActor* Actor, AkRoomID RoomID, ECollisionChannel in_collisionChannel);
	/**
	* Calculates updated occlusion and obstruction values synchronously.
	*/
	void CalculateObstructionOcclusionValues(const UAkComponentSet& in_Listeners, const FVector& SourcePosition, const AActor* Actor, AkRoomID RoomID, ECollisionChannel in_collisionChannel);

	void _Init(UWorld* in_world, float in_refreshInterval);

	/** Last time occlusion was refreshed */
	float LastObstructionOcclusionRefresh = -1;

	bool ClearingOcclusionObstruction = false;

	typedef AkGameObjectIdKeyFuncs<FAkListenerOcclusionObstructionPair, false> ListenerOccObsPairGameObjectIDKeyFuncs;
	TMap<AkGameObjectID, FAkListenerOcclusionObstructionPair, FDefaultSetAllocator, ListenerOccObsPairGameObjectIDKeyFuncs> ListenerInfoMap;

	std::function<void(AkGameObjectID,float)> SetOcclusionObstructionFcn;
};