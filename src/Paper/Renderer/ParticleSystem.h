#pragma once

#include "Paper/Core/Core.h"
#include "Paper/Core/Timestep.h"
#include "Paper/Renderer/OrthographicCamera.h"
#include "Texture.h"

#include <vector>
#include <glm/glm.hpp>

namespace Paper
{
	struct ParticleProps
	{
		glm::vec2 Position;
		glm::vec2 Velocity, VelocityVariation;
		glm::vec4 ColorBegin, ColorEnd;
		Ref<Texture2D> Sprite = nullptr;
		float SizeBegin, SizeEnd, SizeVariation;
		bool Rotated; float RotationSpeed;
		bool EnableFading;
		float LifeTime = 1.0f;
	};

	class ParticleSystem
	{
	public:
		ParticleSystem(uint32_t poolSize);

		void OnUpdate(Timestep dt);
		void OnRender(OrthographicCamera& camera);
		void Emit(const ParticleProps& particleProps);
	private:
		struct Particle
		{
			glm::vec2 Position;
			glm::vec2 Velocity;
			glm::vec4 ColorBegin, ColorEnd;

			Ref<Texture2D> Sprite;

			float RotationSpeed;

			float Rotation = 0.0f;
			float SizeBegin, SizeEnd;

			float LifeTime = 1.0f;
			float LifeRemaining = 0.0f;

			bool EnableFading;

			bool Active = false;
		};

		std::vector<Particle> m_ParticlePool;
		uint32_t m_PoolIndex;
	};
}