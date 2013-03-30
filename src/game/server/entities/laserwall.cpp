/*
 * laserwall.cpp
 *
 *  Created on: 25 mars 2013
 *      Author: gwilherm
 */
#include <iostream>
#include <math.h>
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include "laserwall.h"

CLaserWall::CLaserWall(CGameWorld *pGameWorld, vec2 StartPoint, vec2 Direction, int Owner)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_LASER),
  m_Owner(Owner)
{
	m_Active = false;
	m_Pos = StartPoint;

	m_Delay = time_get() + LW_DELAY * time_freq();
	vec2 EndPoint = ComputeEndPoint(StartPoint, Direction);
	m_WallBody = new CLaserWallBody(pGameWorld, StartPoint, EndPoint, Owner);

	GameWorld()->InsertEntity(this);
}

vec2 CLaserWall::ComputeEndPoint(vec2 StartPoint, vec2 Direction)
{
	vec2 EndPoint;
	vec2 norm;
	float len;

	/* Prevent from normalize(0) bug */
	if(!(Direction == StartPoint))
	{
		/* Wall's direction */
		norm = normalize(Direction - StartPoint);
		/* Wall's length */
		len = distance(StartPoint, Direction);

		/* Limit the length */
		if(len > LW_MAX_LENGTH)	len = LW_MAX_LENGTH;

		/* Effective end of the wall */
		EndPoint = StartPoint + norm * len;
	}
	else
		EndPoint = StartPoint;

	return EndPoint;
}



bool CLaserWall::HitCharacter(vec2 From, vec2 To)
{
	vec2 At;
	CCharacter *OwnerChar = GameServer()->GetPlayerChar(m_Owner);
	CCharacter *Hit = GameServer()->m_World.IntersectCharacter(From, To, 0.f, At, OwnerChar);

	if(!Hit)
		return false;

	Hit->Die(m_Owner, WEAPON_WORLD);
	return true;
}

void CLaserWall::Reset()
{
	GameServer()->m_World.DestroyEntity(m_WallBody);
	GameServer()->m_World.DestroyEntity(this);
}

void CLaserWall::Tick()
{
	CCharacter *OwnerChar = GameServer()->GetPlayerChar(m_Owner);
	if(!OwnerChar || !OwnerChar->IsAlive())
		Reset();

	if(m_Active)
	{
		HitCharacter(m_WallBody->getStartPoint(), m_WallBody->getEndPoint());
	}
	else
	{
		if(time_get() >= m_Delay)
		{
			GameWorld()->InsertEntity(m_WallBody);
			m_Active = true;
		}
	}
}

void CLaserWall::Snap(int SnappingClient)
{
	if(NetworkClipped(SnappingClient))
		return;

	CNetObj_Laser *pObj = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_ID, sizeof(CNetObj_Laser)));
	if(!pObj)
		return;

	pObj->m_X = (int)m_Pos.x;
	pObj->m_Y = (int)m_Pos.y;
	pObj->m_FromX = (int)m_Pos.x;
	pObj->m_FromY = (int)m_Pos.y;
	pObj->m_StartTick = Server()->Tick()-1;
}

CLaserWall::CLaserWallBody::CLaserWallBody(CGameWorld *pGameWorld, vec2 StartPoint, vec2 EndPoint, int Owner)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_LASER)
{
	m_StartPoint = StartPoint;
	m_EndPoint = EndPoint;
}

void CLaserWall::CLaserWallBody::Reset()
{
	GameServer()->m_World.DestroyEntity(this);
}

void CLaserWall::CLaserWallBody::Snap(int SnappingClient)
{
	CNetObj_Laser *pObj = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_ID, sizeof(CNetObj_Laser)));
	if(!pObj)
		return;

	pObj->m_X = (int)m_EndPoint.x;
	pObj->m_Y = (int)m_EndPoint.y;
	pObj->m_FromX = (int)m_StartPoint.x;
	pObj->m_FromY = (int)m_StartPoint.y;
	pObj->m_StartTick = Server()->Tick()-1;
}

vec2 CLaserWall::CLaserWallBody::getStartPoint(){
	return m_StartPoint;
}

vec2 CLaserWall::CLaserWallBody::getEndPoint(){
	return m_EndPoint;
}
