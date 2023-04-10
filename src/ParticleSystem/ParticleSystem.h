#pragma once

#include "Particle.h"
#include "ParticleEmitter.h"
#include "ParticleEffect.h"
#include "ParticleDescription.h"

#include "shader.h"
#include "../Texture2D.h"
#include <vector>
#include <map>
#include <unordered_set>

namespace Particle2D
{
	class ParticleSystem
	{
		typedef std::map<std::string, std::string> ParticleConfigMap;

	public:
		ParticleSystem(const std::string& texturename, Shader _shader, const int num = 1);
		~ParticleSystem();

		void initWithPlist(const std::string& filename);
		void setDescription(const ParticleDescription& desc);

		void setTexture(const std::string& filename);

		void update(float dt);
		void render();

		ParticleEffect* getEffect() { return pEmitter->getParticleEffect(); }
		ParticleEmitter* getEmitter() { return pEmitter; }

		float deathtime;

	private:
		/* ������������ϵͳ�������ļ� */
		static ParticleConfigMap parseParticlePlistFile(const std::string& filename);

		/* ������������ */
		static ParticleDescription createParticleDescription(ParticleConfigMap& map);

		

	private:
		ParticleEmitter* pEmitter;
//		Texture* texture;

		std::vector<Vec3> vPositions;
		std::vector<Color> vColors;
		int nPositionIndex;

		Shader shader;
		vector<Texture2D> textures;
		GLuint VAO;
		GLuint VBO;
		
	};


	//-------------------------------------------------------------------------------
	// ����ϵͳ������
	// ParticleSystemManager
	//-------------------------------------------------------------------------------
	class ParticleSystemManager
	{
	public:
		ParticleSystemManager();
		~ParticleSystemManager();

		void update(float dt);
		void render();

		void appendParticleSystem(ParticleSystem* ps) { vParticleSystems.push_back(ps); }
		std::vector<ParticleSystem*> vParticleSystems;

	};
}