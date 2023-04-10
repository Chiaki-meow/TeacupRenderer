#pragma once
#include "Math.h"

namespace Particle2D
{
	enum class EmitterType
	{
		EMITTER_TYPE_GRAVITY,
		EMITTER_TYPE_RADIUS
	};

	/* �����˶�ģʽ */
	enum class MotionMode
	{
		MOTION_MODE_FREE,
		MOTION_MODE_RELATIVE
	};

	/* ����ģʽ */
	struct GravityMode
	{
		Vec2 vGravity;

		float fTangentialAccel;
		float fTangentialAccelVar;

		float fRadialAccel;
		float fRadialAccelVar;
	};

	struct RadiusMode
	{
		float fBeginRadius;
		float fBeginRadiusVar;

		float fEndRadius;
		float fEndRadiusVar;

		float fSpinPerSecond;
		float fSpinPerSecondVar;
	};


	class ParticleDescription
	{
	public:
		ParticleDescription()
			: vEmitPos(0, 0)
			, vEmitPosVar(0, 0)
			, fEmitAngle(0)
			, fEmitAngleVar(0)
			, fEmitSpeed(0)
			, fEmitSpeedVar(0)
			, nParticleCount(0)
			, fEmitRate(0)
			, fDuration(-1)
			, emitterType(EmitterType::EMITTER_TYPE_GRAVITY)
			, motionMode(MotionMode::MOTION_MODE_FREE)
			, fLife(0)
			, fLifeVar(0)
			, cBeginColor(0, 0, 0, 0)
			, cBeginColorVar(0, 0, 0, 0)
			, cEndColor(0, 0, 0, 0)
			, cEndColorVar(0, 0, 0, 0)
			, fBeginSize(0)
			, fBeginSizeVar(0)
			, fEndSize(0)
			, fEndSizeVar(0)
			, fBeginSpin(0)
			, fBeginSpinVar(0)
			, fEndSpin(0)
			, fEndSpinVar(0)
		{
			gravityMode.fRadialAccel = 0;
			gravityMode.fRadialAccelVar = 0;
			gravityMode.fTangentialAccel = 0;
			gravityMode.fTangentialAccelVar = 0;
			gravityMode.vGravity.set(0, 0);

			radiusMode.fBeginRadius = 0;
			radiusMode.fBeginRadiusVar = 0;
			radiusMode.fEndRadius = 0;
			radiusMode.fEndRadiusVar = 0;
			radiusMode.fSpinPerSecond = 0;
			radiusMode.fSpinPerSecondVar = 0;
		}


		Vec2 vEmitPos;
		Vec2 vEmitPosVar;			

		float fEmitAngle;
		float fEmitAngleVar;
				
		float fEmitSpeed;
		float fEmitSpeedVar;

		int nParticleCount;
		float fEmitRate;
		float fDuration;
		EmitterType emitterType;
		MotionMode  motionMode;


		float fLife;
		float fLifeVar;

		Color cBeginColor;
		Color cBeginColorVar;
		Color cEndColor;
		Color cEndColorVar;

		float fBeginSize;
		float fBeginSizeVar;
		float fEndSize;
		float fEndSizeVar;

		float fBeginSpin;
		float fBeginSpinVar;
		float fEndSpin;
		float fEndSpinVar;

		GravityMode gravityMode;
		RadiusMode radiusMode;
	};
}