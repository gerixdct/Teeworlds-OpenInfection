
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include "laserwall.h"

CLaserWall::CLaserWall(CGameWorld *pGameWorld, vec2 Pos, vec2 Direction, float Length, int Owner)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_LASER)
{
	m_Pos = Pos;
	m_Owner = Owner;
	m_Length = Length;
	m_Dir = Direction;
	m_Bounces = 0;
	m_EvalTick = 0;
	GameWorld()->InsertEntity(this);
	DoBounce();
}


bool CLaserWall::HitCharacter(vec2 From, vec2 To)
{
	vec2 At;
	CCharacter *OwnerChar = GameServer()->GetPlayerChar(m_Owner);
	CCharacter *Hit = GameServer()->m_World.IntersectCharacter(m_Pos, To, 0.f, At, OwnerChar);
	if(!Hit)
		return false;

	m_From = From;
	m_Pos = At;
	Hit->TakeDamage(vec2(0.f, 0.f), GameServer()->Tuning()->m_LaserDamage, m_Owner, WEAPON_RIFLE);
	return true;
}

void CLaserWall::DoBounce()
{
	m_EvalTick = Server()->Tick();

	vec2 To = m_Pos + m_Dir * m_Length;
	m_From = m_Pos;
	m_Pos = To;
}

void CLaserWall::Reset()
{
	GameServer()->m_World.DestroyEntity(this);
}

void CLaserWall::Tick()
{
	CCharacter *OwnerChar = GameServer()->GetPlayerChar(m_Owner);
	if(!OwnerChar || !OwnerChar->IsAlive())
		Reset();
}

void CLaserWall::Snap(int SnappingClient)
{
	if(NetworkClipped(SnappingClient))
		return;

	CNetObj_Laser *pObj1 = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_ID, sizeof(CNetObj_Laser)));
	if(!pObj1)
		return;

	pObj1->m_X = (int)m_Pos.x;
	pObj1->m_Y = (int)m_Pos.y;
	pObj1->m_FromX = (int)m_From.x;
	pObj1->m_FromY = (int)m_From.y;
	pObj1->m_StartTick = Server()->Tick()-1;

	CNetObj_Laser *pObj2 = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, Server()->SnapNewID(), sizeof(CNetObj_Laser)));
	if(!pObj2)
		return;

	pObj2->m_X = (int)m_From.x;
	pObj2->m_Y = (int)m_From.y;
	pObj2->m_FromX = (int)m_From.x;
	pObj2->m_FromY = (int)m_From.y;
	pObj2->m_StartTick = Server()->Tick()-1;
}
