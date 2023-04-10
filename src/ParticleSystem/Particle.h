#pragma once
#include "ParticleDescription.h"

namespace Particle2D
{
	struct Particle
	{
		Vec2 vPos;
		Vec2 vChangePos;
		Vec2 vStartPos;

		Color cColor;
		Color cDeltaColor;

		float fCurrentSize;
		float fSize;
		float fDeltaSize;

		float fRotation;
		float fDeltaRotation;

		float fRemainingLife;

		int textureID;

		/* 重力模式数据 */
		struct GravityModeData
		{
			Vec2  vInitialVelocity;		/* 初速度 */
			float fRadialAccel;			/* 径向加速度（法相加速度）， 与运动方向垂直 */
			float fTangentialAccel;		/* 切向加速度 */

		} gravityMode;

		/* 半径模式数据 */
		struct RadiusModeData
		{
			float fAngle;				/* 发射角度 */
			float fDegressPerSecond;	/* 每秒旋转角度 */
			float fRadius;				/* 半径 */
			float fDelatRadius;			/* 半径变化量 */

		} radiusMode;
	};
}