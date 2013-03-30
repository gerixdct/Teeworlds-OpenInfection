/*
 * laserwall.h
 *
 *  Created on: 25 mars 2013
 *      Author: gwilherm
 */

#ifndef GAME_SERVER_ENTITIES_LASERWALL_H
#define GAME_SERVER_ENTITIES_LASERWALL_H

/* TODO: SERVER TUNING */
#define LW_MAX_LENGTH 300
#define LW_DELAY 0.5

#include <game/server/entity.h>
#include <game/server/entities/laser.h>


class CLaserWall : public CEntity
{
	class CLaserWallBody : public CEntity
	{
	public:
		CLaserWallBody(CGameWorld *pGameWorld, vec2 StartPoint, vec2 EndPoint, int Owner);

		virtual void Reset();
		virtual void Snap(int SnappingClient);

		vec2 getStartPoint();
		vec2 getEndPoint();

	private:
		vec2 m_StartPoint;
		vec2 m_EndPoint;

		int m_Owner;
	};

public:
	CLaserWall(CGameWorld *pGameWorld, vec2 StartPoint, vec2 Direction, int Owner);

	virtual void Reset();
	virtual void Tick();
	virtual void Snap(int SnappingClient);

protected:
	bool HitCharacter(vec2 From, vec2 To);

private:

	bool m_Active;

	int m_Owner;

	int64 m_Delay;
	CLaserWallBody* m_WallBody;

	vec2 ComputeEndPoint(vec2 From, vec2 Dir);
};

#endif /* GAME_SERVER_ENTITIES_LASERWALL_H */
