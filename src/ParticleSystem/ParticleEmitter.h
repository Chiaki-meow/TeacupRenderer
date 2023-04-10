#pragma once
#include "Particle.h"
#include <glm/glm.hpp>
#include <list>

namespace Particle2D
{
	class ParticleEffect;

	class ParticleEmitter
	{
	public:
		ParticleEmitter();
		~ParticleEmitter();

		void setDecription(const ParticleDescription& desc);
		void setParticleEffect(ParticleEffect* effect);

		void emitParticles(float dt);
		void addParticle();

		void update(float dt);

		void startEmitting();
		void stopEmitting();
		bool CanEmit();
		void setEmiterPosition(glm::vec2 pos)
		{
			setEmitPos(Particle2D::Vec2(pos.x, SCR_HEIGHT - pos.y));
		}
		std::list<Particle*>* getParticleList() { return &vParticleList; }
		ParticleEffect* getParticleEffect() { return pParticleEffect; }

		//===================== ���������� =====================

		/* ������λ�� */
		MEMBER_FUNCTION(Vec2, EmitPos, emitPos);
		MEMBER_FUNCTION(Vec2, EmitPosVar, emitPosVar);

		/* �������������ӽǶ� */
		MEMBER_FUNCTION(float, EmitAngle, emitAngle);
		MEMBER_FUNCTION(float, EmitAngleVar, emitAngleVar);

		/* �������������Ӹ����ӵĳ��ٶȴ�С */
		MEMBER_FUNCTION(float, EmitSpeed, emitSpeed);
		MEMBER_FUNCTION(float, EmitSpeedVar, emitSpeedVar);

		/* �������� */
		MEMBER_FUNCTION(int, ParticleCount, particleCount);

		/* ��������  */
		MEMBER_FUNCTION(float, EmitRate, emitRate);

		/* �������ʱ�䣬-1 ��ʾ��Զ���� */
		MEMBER_FUNCTION(float, Duration, duration);

	private:
		ParticleEffect* pParticleEffect;
		std::list<Particle*> vParticleList;

		bool bCanEmit;
		float fElapsed;
		float fEmitCounter;
	};
}