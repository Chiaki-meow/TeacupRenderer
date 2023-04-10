#pragma once
#include "Particle.h"

namespace Particle2D
{
	class ParticleEmitter;
	class ParticleSystem;

	class ParticleEffect
	{
	public:
		ParticleEffect();
		virtual ~ParticleEffect() {}

		void setDecription(const ParticleDescription& desc);

		/* 初始化粒子 */
		virtual void initParticle(ParticleEmitter* pe, Particle* particle);

		/* 更新粒子 */
		virtual void update(ParticleEmitter* pe, float dt) = 0;

		void setMotionMode(MotionMode mode) { motionMode = mode; }

		
	public:
		/* 粒子生命周期 */
		VIR_MEMBER_FUNCTION(float, Life, life);
		VIR_MEMBER_FUNCTION(float, LifeVar, lifeVar);

		/* 粒子起始颜色  */
		VIR_MEMBER_FUNCTION(Color, BeginColor, beginColor);
		VIR_MEMBER_FUNCTION(Color, BeginColorVar, beginColorVar);

		/* 粒子结束颜色 */
		VIR_MEMBER_FUNCTION(Color, EndColor, endColor);
		VIR_MEMBER_FUNCTION(Color, EndColorVar, endColorVar);

		/* 粒子起始大小 */
		VIR_MEMBER_FUNCTION(float, BeginSize, beginSize);
		VIR_MEMBER_FUNCTION(float, BeginSizeVar, beginSizeVar);

		/* 粒子结束大小 */
		VIR_MEMBER_FUNCTION(float, EndSize, endSize);
		VIR_MEMBER_FUNCTION(float, EndSizeVar, endSizeVar);

		/* 粒子起始旋转角度 */
		VIR_MEMBER_FUNCTION(float, BeginSpin, beginSpin);
		VIR_MEMBER_FUNCTION(float, BeginSpinVar, beginSpinVar);

		/* 粒子结束旋转角度 */
		VIR_MEMBER_FUNCTION(float, EndSpin, endSpin);
		VIR_MEMBER_FUNCTION(float, EndSpinVar, endSpinVar);

	public:
		MotionMode motionMode;

		GravityMode gravityMode;
		RadiusMode radiusMode;
	};

	//---------------------------------------------------------------------
	// GravityParticleEffect
	//---------------------------------------------------------------------
	class GravityParticleEffect : public ParticleEffect
	{
	public:
		void initParticle(ParticleEmitter* pe, Particle* particle) override;
		void update(ParticleEmitter* pe, float dt) override;
	};

	//---------------------------------------------------------------------
	// RadialParticleEffect
	//---------------------------------------------------------------------
	class RadialParticleEffect : public ParticleEffect
	{
	public:
		void initParticle(ParticleEmitter* pe, Particle* particle) override;
		void update(ParticleEmitter* pe, float dt) override;
	};
}