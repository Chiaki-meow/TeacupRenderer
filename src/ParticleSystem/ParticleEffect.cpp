#include "ParticleEffect.h"
#include "ParticleEmitter.h"
#include "ParticleSystem.h"
#include "ParticleMemory.h"

#include "Random.h"

const Particle2D::Vec2 GRAVITY(-5, 0);
namespace Particle2D
{
	ParticleEffect::ParticleEffect()
	{
		motionMode = MotionMode::MOTION_MODE_FREE;
	}

	void ParticleEffect::setDecription(const ParticleDescription& desc)
	{
		life = desc.fLife;
		lifeVar = desc.fLifeVar;

		beginColor = desc.cBeginColor;
		beginColorVar = desc.cBeginColorVar;
		endColor = desc.cEndColor;
		endColorVar = desc.cEndColorVar;

		beginSize = desc.fBeginSize;
		beginSizeVar = desc.fBeginSizeVar;
		endSize = desc.fEndSize;
		endSizeVar = desc.fEndSizeVar;

		beginSpin = desc.fBeginSpin;
		beginSpinVar = desc.fBeginSpinVar;
		endSpin = desc.fEndSpin;
		endSpinVar = desc.fEndSpinVar;

		motionMode = desc.motionMode;

		gravityMode = desc.gravityMode;
		radiusMode = desc.radiusMode;
	}

	void ParticleEffect::initParticle(ParticleEmitter* pe, Particle* particle)
	{
		particle->textureID = rand() % 3;

		/* ������ʼλ�� */
		particle->vPos.x = pe->getEmitPos().x + pe->getEmitPosVar().x * RANDOM_MINUS1_1();
		particle->vPos.y = pe->getEmitPos().y + pe->getEmitPosVar().y * RANDOM_MINUS1_1();

		particle->vStartPos = pe->getEmitPos();
		particle->vChangePos = particle->vPos;

		/* �������� */
		particle->fRemainingLife = MAX(0.1, life + lifeVar * RANDOM_MINUS1_1());

		/* ���ӵ���ɫ�仯ֵ */
		Color begin_color, end_color;
		begin_color.r = CLAMPF(beginColor.r + beginColorVar.r * RANDOM_MINUS1_1(), 0, 1);
		begin_color.g = CLAMPF(beginColor.g + beginColorVar.g * RANDOM_MINUS1_1(), 0, 1);
		begin_color.b = CLAMPF(beginColor.b + beginColorVar.b * RANDOM_MINUS1_1(), 0, 1);
		begin_color.a = CLAMPF(beginColor.a + beginColorVar.a * RANDOM_MINUS1_1(), 0, 1);

		end_color.r = CLAMPF(endColor.r + endColorVar.r * RANDOM_MINUS1_1(), 0, 1);
		end_color.g = CLAMPF(endColor.g + endColorVar.g * RANDOM_MINUS1_1(), 0, 1);
		end_color.b = CLAMPF(endColor.b + endColorVar.b * RANDOM_MINUS1_1(), 0, 1);
		end_color.a = CLAMPF(endColor.a + endColorVar.a * RANDOM_MINUS1_1(), 0, 1);

		float tmp = 1 / (particle->fRemainingLife);
		particle->cColor = begin_color;
		particle->cDeltaColor.r = (end_color.r - begin_color.r) * tmp;
		particle->cDeltaColor.g = (end_color.g - begin_color.g) * tmp;
		particle->cDeltaColor.b = (end_color.b - begin_color.b) * tmp;
		particle->cDeltaColor.a = (end_color.a - begin_color.a) * tmp;

		/* ���Ӵ�С */
		float begin_size = MAX(0, beginSize + beginSizeVar * RANDOM_MINUS1_1());
		float end_size = MAX(0, endSize + endSizeVar * RANDOM_MINUS1_1());

		particle->fSize = begin_size;
		particle->fDeltaSize = (end_size - begin_size) / particle->fRemainingLife;

		/* ������ת�Ƕ� */
		float begin_spin = toRadian(MAX(0, beginSpin + beginSpinVar * RANDOM_MINUS1_1()));
		float end_spin = toRadian(MAX(0, endSpin + endSpinVar * RANDOM_MINUS1_1()));

		particle->fRotation = begin_spin;
		particle->fDeltaRotation = (end_spin - begin_spin) / particle->fRemainingLife;
	}

	//---------------------------------------------------------------------
	// GravityParticleEffect
	//---------------------------------------------------------------------
	void GravityParticleEffect::initParticle(ParticleEmitter* pe, Particle* particle)
	{
		ParticleEffect::initParticle(pe, particle);

		/* ���������ܵ����������ĳ��ٶȴ�С */
		float particleSpeed = pe->getEmitSpeed() + pe->getEmitSpeedVar() * RANDOM_MINUS1_1();

		/* �������ӳ��ٶȵķ��򣬼��������������ӵķ��䷽�� */
		float angle = pe->getEmitAngle() + pe->getEmitAngleVar() * RANDOM_MINUS1_1();
		Vec2 particleDirection = Vec2(cosf(toRadian(angle)), sinf(toRadian(angle)));
		
		/* �������ӵ���ʼ���ٶȣ�������С������*/
		particle->gravityMode.vInitialVelocity = particleDirection * particleSpeed;

		/* ����������ٶȡ�������ٶ� */
		particle->gravityMode.fTangentialAccel = gravityMode.fTangentialAccel + gravityMode.fTangentialAccelVar * RANDOM_MINUS1_1();
		particle->gravityMode.fRadialAccel = gravityMode.fRadialAccel + gravityMode.fRadialAccelVar * RANDOM_MINUS1_1();
	}

	void GravityParticleEffect::update(ParticleEmitter* pe, float dt)
	{
		std::list<Particle*>* indexList = pe->getParticleList();

		for ( auto it = indexList->begin(); it != indexList->end(); ) {
			Particle* p = (*it);

			p->fRemainingLife -= dt;

			if ( p->fRemainingLife > 0 ) {
				static Vec2 offset, radial, tangential;

				/* ������ٶ� */
				if ( p->vChangePos.x || p->vChangePos.y ) {
					offset = p->gravityMode.vInitialVelocity;
					radial = offset.normalize();
				}
				tangential = radial;
				radial = radial * p->gravityMode.fRadialAccel;

				float newy = tangential.x;
				tangential.x = -tangential.y;
				tangential.y = newy;
				tangential = tangential * p->gravityMode.fTangentialAccel;

				offset = (radial + tangential + gravityMode.vGravity + GRAVITY) * dt;

				p->gravityMode.vInitialVelocity = p->gravityMode.vInitialVelocity + offset;
				p->vChangePos = p->vChangePos + p->gravityMode.vInitialVelocity * dt;

				p->cColor = p->cColor + p->cDeltaColor * dt;
				p->fSize = MAX(0, p->fSize + p->fDeltaSize * dt);
				p->fRotation = p->fRotation + p->fDeltaRotation * dt;
				p->gravityMode.vInitialVelocity;

				if ( motionMode == MotionMode::MOTION_MODE_RELATIVE ) {
					Vec2 diff = pe->getEmitPos() - p->vStartPos;
					p->vPos = p->vChangePos + diff;
				}
				else {
					p->vPos = p->vChangePos;
				}
				++it;
			}
			else {
				ParticleMemory::freeParticle(*it);
				it = indexList->erase(it);
			}
		}
	}

	//---------------------------------------------------------------------
	// GravityParticleEffect
	//---------------------------------------------------------------------
	void RadialParticleEffect::initParticle(ParticleEmitter* pe, Particle* particle)
	{
		ParticleEffect::initParticle(pe, particle);

		float begin_radius = radiusMode.fBeginRadius + radiusMode.fBeginRadiusVar * RANDOM_MINUS1_1();
		float end_radius = radiusMode.fEndRadius + radiusMode.fEndRadiusVar * RANDOM_MINUS1_1();
		
		particle->radiusMode.fRadius = begin_radius;
		particle->radiusMode.fDelatRadius = (end_radius - begin_radius) / particle->fRemainingLife;

		float degress = pe->getEmitAngle() + pe->getEmitAngleVar() * RANDOM_MINUS1_1();
		particle->radiusMode.fAngle = toRadian(degress);

		degress = radiusMode.fSpinPerSecond + radiusMode.fSpinPerSecondVar * RANDOM_MINUS1_1();
		particle->radiusMode.fDegressPerSecond = toRadian(degress);
	}

	void RadialParticleEffect::update(ParticleEmitter* pe, float dt)
	{
		std::list<Particle*>* indexList = pe->getParticleList();

		for ( auto it = indexList->begin(); it != indexList->end(); ) {
			Particle* p = (*it);

			p->fRemainingLife -= dt;

			if ( p->fRemainingLife > 0 ) {
				p->radiusMode.fAngle += p->radiusMode.fDegressPerSecond * dt;
				p->radiusMode.fRadius += p->radiusMode.fDelatRadius * dt;

				p->vChangePos.x = cosf(p->radiusMode.fAngle) * p->radiusMode.fRadius;
				p->vChangePos.y = sinf(p->radiusMode.fAngle) * p->radiusMode.fRadius;
				
				if ( motionMode == MotionMode::MOTION_MODE_FREE ) {
					p->vPos = p->vChangePos + pe->getEmitPos();
				}
				else {
					p->vPos = p->vChangePos + p->vStartPos;
				}

				p->cColor = p->cColor + p->cDeltaColor * dt;
				p->fSize = MAX(0, p->fSize + p->fDeltaSize * dt);
				p->fRotation = p->fRotation + p->fDeltaRotation * dt;
				++it;
			}
			else {
				ParticleMemory::freeParticle(*it);
				it = indexList->erase(it);
			}
		}
	}
}