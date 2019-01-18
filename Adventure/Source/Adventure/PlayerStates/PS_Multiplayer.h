// By: Eric Marquez. All information and code provided is free to use and can be used comercially.Use of such examples indicates no fault to the author for any damages caused by them. The author must be credited.

#pragma once

#include <bitset>
#include "Basics.h"
#include "Saves/MapSaveFile.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PS_Multiplayer.generated.h"

/**
 * 
 */

USTRUCT()
struct FLocationStats
{
	GENERATED_BODY()
public:

	int GetLocationSizeInBytes() const
	{
		return NameSize + HeightMapSize + TextureMapSize + ObjectsSize + (sizeof(BasicTransform) * ObjectTransformsSize) + sizeof(FGridCoordinate);
	}

	UPROPERTY()
	int NameSize;

	UPROPERTY()
	int HeightMapSize;

	UPROPERTY()
	int TextureMapSize;

	UPROPERTY()
	int ObjectsSize;

	UPROPERTY()
	int ObjectTransformsSize;

	UPROPERTY()
	TArray<int> BFFinished;
};

UENUM(BlueprintType)
enum class TURN_BASED_STATE : uint8
{
	FREE_ROAM,
	STANDBY,
	ACTIVE
};

UCLASS()
class ADVENTURE_API APS_Multiplayer : public APlayerState
{
	GENERATED_BODY()
	
public:
	APS_Multiplayer();

	// Sets the current gameID to refrence a player individually
	void ServerOnly_SetGameID(const int ID);

	// Sets the turn based player state
	UFUNCTION(BlueprintCallable, Category = "Player State")
	void ServerOnly_SetPlayerState(const TURN_BASED_STATE state);

	// Gets the current gameID to refrence a playre individually
	UFUNCTION(BlueprintCallable, Category = "Player State")
	int GetGameID() const;

	// Sets the map that the player state will use to query locations that will be displayed
	bool LoadMap(const FString& MapName);

	// Generates a new map with a given map size
	void GenerateEmptyMap(const FString& MapName, const FGridCoordinate& MapSize);

protected:

	// Sets the turn based player order
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn-Based Settings")
	TArray<int> InitiativePlayerOrder;

	UFUNCTION(BlueprintCallable, Category = "Player State")
	void UpdateDataTransfer(float DeltaTime);

	// Override for server to change player order
	UFUNCTION(BlueprintCallable, Category = "Turn-Based Settings")
	void OverrideCurrentPlayersTurn(const int ID);

	// Gets the current turn based state
	UFUNCTION(BlueprintCallable, Category = "Turn-Based Settings")
	TURN_BASED_STATE GetCurrentState() const;

	// Callback to signal when turn based state has changed
	UFUNCTION(BlueprintImplementableEvent, Category = "Player State")
	void OnStateChanged();

private:

	// Unique identifier
	UPROPERTY(Replicated)
	int m_GameID;

	// Turn based state
	UPROPERTY(ReplicatedUsing = OnStateChanged)
	TURN_BASED_STATE m_CurrentState;

	// Current active player
	UPROPERTY(Replicated)
	int m_CurrentPlayerActive;

	// Server
	bool m_bMapDownloaded;
	std::bitset<sizeof(int) * 8 * 2> m_BFSent;
	bool m_bNeedsNextPacket;
	float m_TotalTime;
	FMapLocation m_CurrentLocation;
	TArray<uint8> m_RawSaveFileServer;

	// Client
	int m_DownloadedSize;
	std::bitset<sizeof(int) * 8 * 2> m_BFRecieved;
	FLocationStats m_LocationStats;
	TArray<uint8> m_RawSaveFileClient;
	bool gotAuthority;

	// Get raw data at m_NextPacket (TRANSFER_DATA_SIZE interval)
	void GetNextPacketData(TArray<uint8>& Data, bool& LastPacket);

	// Converts raw data to Location data
	void LoadLocationDataFromBinary();

	// Server function sent from client to request more data from download
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DownloadMap(const TArray<int>& BFRecieved);

	// Client functin sent from server to give data to owning client
	UFUNCTION(Client, Reliable)
	void Client_RecievePacket(const TArray<uint8>& Data, const TArray<int>& Bitfield);

	// Transfers location data to owning client so that it may generate a grid
	UFUNCTION()
	void Client_GenerateGrid(const FMapLocation& Data);

	// Tells client to create a new map
	UFUNCTION(Client, Reliable)
	void Client_GenerateEmptyGrid(const FGridCoordinate& MapSize);

	// Tell Client the information to unpack new location data
	UFUNCTION(Client, Reliable)
	void Client_GetLocationStats(const FLocationStats& Stats);
};
