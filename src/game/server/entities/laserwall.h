#ifndef GAME_SERVER_ENTITIES_LASERWALL_H
#define GAME_SERVER_ENTITIES_LASERWALL_H

#include <game/server/entity.h>
#include <game/server/entities/laser.h>

class CLaserWall : public CEntity
{
public:
	CLaserWall(CGameWorld *pGameWorld, vec2 Pos, vec2 Direction, float StartEnergy, int Owner);

	virtual void Reset();
	virtual void Tick();
	virtual void Snap(int SnappingClient);

protected:
	bool HitCharacter(vec2 From, vec2 To);
	void DoBounce();

private:
	vec2 m_From;
	vec2 m_Dir;
	float m_Length;
	float m_Delay;
	int m_Bounces;
	int m_EvalTick;
	int m_Owner;
};

#endif /* GAME_SERVER_ENTITIES_LASERWALL_H */
