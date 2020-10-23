#include "p_pch.h"
#include "ParticleSystem.h"

#include "Paper/Core/Random.h"
#include "Paper/Renderer/Renderer2D.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

namespace Paper {
	ParticleSystem::ParticleSystem(uint32_t poolSize)
		: m_PoolIndex(poolSize - 1)
	{
		m_ParticlePool.resize(poolSize);
	}

	void ParticleSystem::OnUpdate(Timestep dt)
	{
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			if (particle.LifeRemaining <= 0.0f)
			{
				particle.Active = false;
				continue;
			}

			particle.LifeRemaining -= dt;
			particle.Position += particle.Velocity * (float)dt;
			particle.Rotation += particle.RotationSpeed * dt;
		}
	}

	void ParticleSystem::OnRender(OrthographicCamera& camera)
	{
		Renderer2D::BeginScene(camera);
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			// Fade away particles
			float life = particle.LifeRemaining / particle.LifeTime;
			glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
			color.a = particle.EnableFading ? color.a * life : color.a;

			float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

			if (particle.Sprite) {
				Renderer2D::DrawRotatedQuad(particle.Position, {size, size}, particle.Sprite, particle.Rotation, 1.0f, color);
			} else {
				Renderer2D::DrawRotatedQuad(particle.Position, {size, size}, color, particle.Rotation);
			}
		}
		Renderer2D::EndScene();
	}

	void ParticleSystem::Emit(const ParticleProps& particleProps)
	{
		Particle& particle = m_ParticlePool[m_PoolIndex];
		particle.Active = true;
		particle.Position = particleProps.Position;
		particle.Rotation = particleProps.Rotated ? Random::Float() * 2.0f * glm::pi<float>() : 0.0f;
		particle.RotationSpeed = particleProps.RotationSpeed;

		// Velocity
		particle.Velocity = particleProps.Velocity;
		particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
		particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

		// Color
		particle.ColorBegin = particleProps.ColorBegin;
		particle.ColorEnd = particleProps.ColorEnd;

		// Texture
		particle.Sprite = particleProps.Sprite;

		particle.LifeTime = particleProps.LifeTime;
		particle.LifeRemaining = particleProps.LifeTime;
		particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
		particle.SizeEnd = particleProps.SizeEnd;

		particle.EnableFading = particleProps.EnableFading;

		m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
	}
}