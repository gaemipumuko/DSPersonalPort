#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "fmod.h"
#include "fmod.hpp"

BEGIN(Client)

class CSound_Manager final : public CBase
{
	DECLARE_SINGLETON(CSound_Manager)

public:
	enum CHANNELID
	{
		SOUND_BGM,
		SOUND_PLAYER_EFFECT1,
		SOUND_PLAYER_EFFECT2,
		SOUND_PLAYER_EFFECT3,
		SOUND_PLAYER_EFFECT4,
		SOUND_PLAYER_HIT1,
		SOUND_BACKGROUND_EFFECT1,
		SOUND_BACKGROUND_EFFECT2,
		SOUND_BACKGROUND_EFFECT3,
		SOUND_MONSTER_EFFECT,
		SOUND_MONSTER_EFFECT1,
		SOUND_MONSTER_EFFECT2,
		SOUND_MONSTER_EFFECT3,
		SOUND_MONSTER_EFFECT4,
		SOUND_MONSTER_EFFECT5,
		SOUND_MONSTER_EFFECT6,
		SOUND_MONSTER_EFFECT7,
		SOUND_MONSTER_LF,
		SOUND_MONSTER_RF,
		SOUND_NPC_DIALOGUE,
		SOUND_ITEM,
		SOUND_WALK,
		SOUND_TALK,
		SOUND_OBJECT,
		SOUND_DEVOTION,
		SOUND_EXP_MAX,
		SOUND_CURSE,
		SOUND_ROLL,
		SOUND_KNOCKBACK,
		SOUND_TREE_HIT1,
		SOUND_TREE_HIT2,
		SOUND_TREE_HIT3,
		SOUND_TREE_DESTROY,
		SOUND_STONE_HIT1,
		SOUND_STONE_HIT2,
		SOUND_STONE_HIT3,
		SOUND_STONE_DESTROY,
		SOUND_GET_CURSE,
		SOUND_GET_WEAPON,
		SOUND_LAMB_REBIRTH,
		SOUND_HAMMER1,
		SOUND_HAMMER2,
		SOUND_SERMON,
		MAX_CHANNEL
	};

private:
	CSound_Manager();
	virtual ~CSound_Manager() = default;

public:
	void PlaySound(const TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void PlayBGM(const TCHAR* pSoundKey, float fVolume);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, float fVolume);

private:
	void LoadSoundFile();

private:
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	FMOD_CHANNEL* m_pChannelArr[MAX_CHANNEL];
	FMOD_SYSTEM* m_pSystem;

public:
	virtual void Free() override;
};

END

