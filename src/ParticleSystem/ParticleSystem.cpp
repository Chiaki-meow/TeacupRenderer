#pragma once
#include "ParticleSystem.h"
#include "ParticleMemory.h"
#include "tinyxml2.h"
#include "../ResourceManager.h"

#define GET_F(map, name) atof((map)[name].c_str())
#define GET_I(map, name) atoi((map)[name].c_str())
const int MEMORY_SIZE = 50000;
const float TOLERANCE = 30;
const float DEL_TIME = 2.5;

namespace Particle2D
{
	ParticleSystem::ParticleSystem(const std::string& texturename,Shader _shader, const int num): deathtime(0), shader(_shader)
	{
		GLfloat particle_quad[] = {
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(this->VAO);
		// Fill mesh buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
		// Set mesh attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		pEmitter = new ParticleEmitter();

		textures.push_back(ResourceManager::loadTextureFromFile(("../resources/textures/Petal1.png"),GL_TRUE));
		textures.push_back(ResourceManager::loadTextureFromFile(("../resources/textures/Petal2.png"),GL_TRUE));
		textures.push_back(ResourceManager::loadTextureFromFile(("../resources/textures/Petal3.png"),GL_TRUE));
	}

	ParticleSystem::~ParticleSystem()
	{
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &VBO);
		delete pEmitter;
	}

	void ParticleSystem::setTexture(const std::string& filename)
	{
	}

	void ParticleSystem::initWithPlist(const std::string& filename)
	{
		ParticleConfigMap map = this->parseParticlePlistFile(filename);
		ParticleDescription desc = this->createParticleDescription(map);
		this->setDescription(desc);
	}

	void ParticleSystem::setDescription(const ParticleDescription& desc)
	{
		pEmitter->setDecription(desc);
	}

	void ParticleSystem::update(float dt)
	{
		pEmitter->update(dt);
	}

	void ParticleSystem::render()
	{
		// Use additive blending to give it a 'glow' effect
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(this->VAO);
		glDisable(GL_DEPTH_TEST);
		this->shader.use();

		int begin_index = 0;
		float s = 0, c = 0, x = 0, y = 0;

		auto particleIndex = pEmitter->getParticleList();
		Particle* particle = nullptr;

		nPositionIndex = 0;
		for ( auto it = particleIndex->begin(); it != particleIndex->end(); ++it ) {
			particle = (*it);

			x = particle->vPos.x;
			y = particle->vPos.y;
			y = SCR_HEIGHT - y;

			glm::mat4 model(1.0f);

			// First translate 
			// (transformations are: scale happens first, then rotation
			// and then finall translation happens; reversed order)
			model = glm::translate(model, glm::vec3(x, y, 0.0f));

			// Move origin of rotation to center of quad
			model = glm::translate(model, glm::vec3(0.5f * particle->fSize, 0.5f * particle->fSize, 0.0f));
			// Then rotate
			model = glm::rotate(model, particle->fRotation, glm::vec3(0.0f, 0.0f, 1.0f));
			// Move origin back
			model = glm::translate(model, glm::vec3(-0.5f * particle->fSize, -0.5f * particle->fSize, 0.0f));
			// Last scale
			model = glm::scale(model, glm::vec3(particle->fSize, particle->fSize, 1.0f));

			this->shader.setMat4("model", model);
			this->shader.setVec4("ParticleColor", glm::clamp(glm::vec4(particle->cColor.r, particle->cColor.g,
				particle->cColor.b, particle->cColor.a), glm::vec4(0), glm::vec4(1)));
			this->textures[particle->textureID].Bind();
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		
		glBindVertexArray(0);
		glUseProgram(0);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// Reset to default blending mode
		glEnable(GL_DEPTH_TEST);
	}

	ParticleSystem::ParticleConfigMap ParticleSystem::parseParticlePlistFile(const std::string& filename)
	{
		ParticleConfigMap particleConfigMap;

		tinyxml2::XMLDocument doc;
		doc.LoadFile(filename.c_str());

		tinyxml2::XMLElement* root = doc.RootElement();
		tinyxml2::XMLNode* dict = root->FirstChildElement("dict");
		tinyxml2::XMLElement* ele = dict->FirstChildElement();

		std::string tmpstr1, tmpstr2;
		while ( ele ) {
			if ( ele->GetText() != nullptr && strcmp("textureImageData", ele->GetText()) == 0 ) {
				ele = ele->NextSiblingElement()->NextSiblingElement();
			}
			else {
				tmpstr1 = ele->GetText();
				ele = ele->NextSiblingElement();
				tmpstr2 = ele->GetText() == nullptr ? "0" : ele->GetText();
				ele = ele->NextSiblingElement();

				particleConfigMap.insert(std::make_pair(tmpstr1, tmpstr2));
			}
		}
		return particleConfigMap;
	}

	ParticleDescription ParticleSystem::createParticleDescription(ParticleConfigMap& map)
	{
		ParticleDescription desc;

		//================================== ���������� ========================================

		/* �������Ƕ� */
		desc.fEmitAngle	   = float(GET_I(map, "angle"));
		desc.fEmitAngleVar = float(GET_I(map, "angleVariance"));

		/* �������ٶ� */
		desc.fEmitSpeed	   = float(GET_I(map, "speed"));
		desc.fEmitSpeedVar = float(GET_I(map, "speedVariance"));

		// ����������ʱ��
		desc.fDuration     = GET_F(map, "duration");

		// ������ģʽ������������
		if ( GET_I(map, "emitterType") ) {
			desc.emitterType = EmitterType::EMITTER_TYPE_RADIUS;
		}
		else {
			desc.emitterType = EmitterType::EMITTER_TYPE_GRAVITY;
		}
		

		/* ����������� */
		desc.nParticleCount = int(GET_F(map, "maxParticles"));

		/* ���������� */
		desc.vEmitPos.set(GET_F(map, "sourcePositionx"), GET_F(map, "sourcePositiony"));
		desc.vEmitPosVar.set(GET_F(map, "sourcePositionVariancex"), GET_F(map, "sourcePositionVariancey"));

		/* ������������ */
		desc.fLife    = GET_F(map, "particleLifespan");
		desc.fLifeVar = GET_F(map, "particleLifespanVariance");

		/* �������� */
		desc.fEmitRate = desc.nParticleCount / desc.fLife;

		//================================== �������� ========================================

		/* ������ʼ��ɫ */
		desc.cBeginColor.set(
			GET_F(map, "startColorRed"), 
			GET_F(map, "startColorGreen"), 
			GET_F(map, "startColorBlue"), 
			GET_F(map, "startColorAlpha"));

		desc.cBeginColorVar.set(
			GET_F(map, "startColorVarianceRed"),
			GET_F(map, "startColorVarianceGreen"),
			GET_F(map, "startColorVarianceBlue"),
			GET_F(map, "startColorVarianceAlpha"));

		/* ���ӽ�����ɫ */
		desc.cEndColor.set(
			GET_F(map, "finishColorRed"),
			GET_F(map, "finishColorGreen"),
			GET_F(map, "finishColorBlue"),
			GET_F(map, "finishColorAlpha"));

		desc.cEndColorVar.set(
			GET_F(map, "finishColorVarianceRed"),
			GET_F(map, "finishColorVarianceGreen"),
			GET_F(map, "finishColorVarianceBlue"),
			GET_F(map, "finishColorVarianceAlpha"));

		/* ���Ӵ�С */
		desc.fBeginSize		= GET_F(map, "startParticleSize");
		desc.fBeginSizeVar	= GET_F(map, "startParticleSizeVariance");
		desc.fEndSize		= GET_F(map, "finishParticleSize");
		desc.fEndSizeVar	= GET_F(map, "finishParticleSizeVariance");
									
		/* ������ת */				
		desc.fBeginSpin		= GET_F(map, "rotationStart");
		desc.fBeginSpinVar	= GET_F(map, "rotationStartVariance");
		desc.fEndSpin		= GET_F(map, "rotationEnd");
		desc.fEndSpinVar	= GET_F(map, "rotationEndVariance");

		/* �����˶�ģʽ */
		MotionMode motionModes[2] = {
			MotionMode::MOTION_MODE_FREE,
			MotionMode::MOTION_MODE_RELATIVE
		};

		desc.motionMode = motionModes[GET_I(map, "positionType")];

		/* GravityMode ����ģʽ */
		desc.gravityMode.vGravity.set(GET_F(map, "gravityx"), GET_F(map, "gravityy"));

		desc.gravityMode.fRadialAccel	 = GET_F(map, "radialAcceleration");
		desc.gravityMode.fRadialAccelVar = GET_F(map, "radialAccelVariance");

		desc.gravityMode.fTangentialAccel	 = GET_F(map, "tangentialAcceleration");
		desc.gravityMode.fTangentialAccelVar = GET_F(map, "tangentialAccelVariance");

		// RadiusMode �뾶ģʽ
		desc.radiusMode.fEndRadius = atof((map)["minRadius"].c_str());
		desc.radiusMode.fEndRadiusVar = atof((map)["minRadiusVariance"].c_str());

		desc.radiusMode.fBeginRadius = atof((map)["maxRadius"].c_str());
		desc.radiusMode.fBeginRadiusVar = atof((map)["maxRadiusVariance"].c_str());

		desc.radiusMode.fSpinPerSecond = atof((map)["rotatePerSecond"].c_str());
		desc.radiusMode.fSpinPerSecondVar = atof((map)["rotatePerSecondVariance"].c_str());

		return desc;
	}


	//-------------------------------------------------------------------------------
	// ����ϵͳ������
	// ParticleSystemManager
	//-------------------------------------------------------------------------------
	ParticleSystemManager::ParticleSystemManager()
	{
		ParticleMemory::initParticleMemory(MEMORY_SIZE);
	}

	ParticleSystemManager::~ParticleSystemManager()
	{
		for ( auto& ele : vParticleSystems ) {
			delete ele;
		}
		vParticleSystems.clear();

		ParticleMemory::freeParticleMemory();
	}

	void ParticleSystemManager::update(float dt)
	{
		for (auto ele = vParticleSystems.begin(); ele != vParticleSystems.end(); )
		{
			if (vParticleSystems.empty())	return;
			(*ele)->update(dt);
			if (!(*ele)->getEmitter()->CanEmit())
			{
				(*ele)->deathtime += dt;
				if ((*ele)->deathtime > DEL_TIME)
				{
					delete (*ele);
					ele = vParticleSystems.erase(ele);
				}
				else ele++;
			}
			else ele++;
		}
	}

	void ParticleSystemManager::render()
	{
		for ( auto& ele : vParticleSystems ) {
			ele->render();
		}
	}
}