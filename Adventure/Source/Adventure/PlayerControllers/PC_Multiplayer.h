// By: Eric Marquez. All information and code provided is free to use and can be used comercially.Use of such examples indicates no fault to the author for any damages caused by them. The author must be credited.

#pragma once

#include <vector>
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PC_Multiplayer.generated.h"


UCLASS()
class ADVENTURE_API APC_Multiplayer : public APlayerController
{
	GENERATED_BODY()
public:
	APC_Multiplayer();
	virtual void Tick(float DeltaTime) override;

	// Sets the active save file(Server only)
	void ServerOnly_SetActiveMapSave(const FString& Path);

	// Sets the map name (Used when downloading finishes on client)
	void SetMapName(const FString& Name);

	//Sets player ID (Server)
	void SetPlayerID(const int ID);

	// Gets player ID
	int GetPlayerID()const;

	// Exec function for debugging purposes
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowPathfindingDebugLines(bool Value);

private:

	UFUNCTION(Client, Reliable)
	void Client_Ping(const FVector& data);
	
	UPROPERTY()
	int UniqueID;

	FString m_MapName;
	TArray<uint8> m_RawSaveFile;

	float m_ElapsedTime;
};
