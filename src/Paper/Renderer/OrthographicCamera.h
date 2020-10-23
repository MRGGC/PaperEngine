#pragma once

#include <glm/glm.hpp>

namespace Paper
{

	struct OrthographicCameraBounds
	{
		OrthographicCameraBounds(float l, float r, float b, float t)
			: Left(l), Right(r), Bottom(b), Top(t) {}

		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float l, float r, float b, float t);

		void SetBounds(const OrthographicCameraBounds& bounds) { m_Bounds = bounds; };
		const OrthographicCameraBounds& GetBounds() { return m_Bounds; }

		void SetProjection(float l, float r, float b, float t);

		void SetPosition(const glm::vec3& pos) { m_Position = pos; RecalculateVP(); }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateVP(); }

		const glm::vec3& GetPosition() const { return m_Position; }
		const float& GetRotation() const { return m_Rotation; }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	private:
		void RecalculateVP();
	private:
		OrthographicCameraBounds m_Bounds;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
	};
}