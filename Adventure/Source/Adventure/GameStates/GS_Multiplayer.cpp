// Copyright 2019 Eric Marquez
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http ://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "GS_Multiplayer.h"

#include "Grid/WorldGrid.h"
#include "Character/MapPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "GameModes/GM_Multiplayer.h"
#include "PlayerStates/PS_Multiplayer.h"
#include "DataTables/InventoryDatabase.h"
#include "Adventure.h"

AGS_Multiplayer::AGS_Multiplayer()
{
	m_bFreeRoamActive = false;
	m_CurrentActivePlayer = 0;
}

void AGS_Multiplayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGS_Multiplayer, m_ActivePlayerName);
	DOREPLIFETIME(AGS_Multiplayer, m_PlayerNameArray);
}

void AGS_Multiplayer::HandleBeginPlay()
{
	Super::HandleBeginPlay();
	//SetActivePlayer(-1);
	GenerateGrid();
}

void AGS_Multiplayer::AddNewPlayer(int PlayerID, FString PlayerName)
{
	// Validity check
	if (m_PlayerNameArray.Num() == PlayerID)
	{
		m_PlayerNameArray.Push(PlayerName);
	}
}

void AGS_Multiplayer::GenerateGrid()
{
}

void AGS_Multiplayer::SetActivePlayer(const int ID)
{
	// Switch to someones turn
	if (ID != -1 && ID != m_CurrentActivePlayer)
	{
		UE_LOG(LogNotice, Warning, TEXT("Passing turn to: %i"), ID);
		m_bFreeRoamActive = false;
		m_CurrentActivePlayer = ID;

		for (auto& player : PlayerArray)
		{
			APS_Multiplayer* playerState = Cast<APS_Multiplayer>(player);
			if (playerState)
			{
				if (playerState->GetGameID() == ID)
				{
					m_ActivePlayerName = playerState->GetPlayerName();
					playerState->ServerOnly_SetPlayerState(TURN_BASED_STATE::ACTIVE);
				}
				else
				{
					playerState->ServerOnly_SetPlayerState(TURN_BASED_STATE::STANDBY);
				}
			}
		}
	}
	else if (!m_bFreeRoamActive && ID == -1)
	{
		UE_LOG(LogNotice, Warning, TEXT("Starting Free-Roam"));
		m_ActivePlayerName = TEXT("Free-Roam");
		m_bFreeRoamActive = true;
		m_CurrentActivePlayer = -1;

		for (auto& player : PlayerArray)
		{
			APS_Multiplayer* playerState = Cast<APS_Multiplayer>(player);
			if (playerState)
			{
				playerState->ServerOnly_SetPlayerState(TURN_BASED_STATE::FREE_ROAM);
			}
		}
	}
}

FString AGS_Multiplayer::GetActivePlayerName() const
{
	return m_ActivePlayerName;
}

FString AGS_Multiplayer::GetPlayerName(int PlayerID) const
{
	if (m_PlayerNameArray.Num() > PlayerID)
	{
		return m_PlayerNameArray[PlayerID];
	}

	return "None";
}

TArray<FString> AGS_Multiplayer::GetAllPlayerNames() const
{
	return m_PlayerNameArray;
}

int AGS_Multiplayer::GetPlayerID(FString PlayerName) const
{
	int ID = -1;

	for (int x = 0; x < m_PlayerNameArray.Num(); x++)
	{
		if (m_PlayerNameArray[x] == PlayerName)
		{
			ID = x;
		}
	}

	return ID;
}
