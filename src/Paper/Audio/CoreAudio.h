#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Paper
{
	class AudioSource
	{
	public:
		~AudioSource();

		bool IsLoaded() const { return m_Loaded; }

		void SetPosition(float x, float y, float z);
		void SetGain(float gain);
		void SetPitch(float pitch);
		void SetSpatial(bool spatial);
		void SetLoop(bool loop);

		std::pair<uint32_t, uint32_t> GetLengthMinutesAndSeconds() const;

		static AudioSource LoadFromFile(const std::string& file, bool spatial = false);

		friend class Audio;
	private:
		AudioSource() = default;
		AudioSource(uint32_t handle, bool loaded, float length);

		uint32_t m_BufferHandle = 0;
		uint32_t m_SourceHandle = 0;
		bool m_Loaded = false;
		bool m_Spatial = false;

		float m_TotalDuration = 0;

		float m_Position[3] = { 0.0f, 0.0f, 0.0f };
		float m_Gain = 1.0f;
		float m_Pitch = 1.0f;
		bool m_Loop = false;
	};

	class Audio
	{
	public:
		static void Init();

		static AudioSource LoadAudioSource(const std::string& filename);
		static void Play(const AudioSource& source);

		static void SetDebugLogging(bool log);
	private:
		static AudioSource LoadAudioSourceOgg(const std::string& filename);
		static AudioSource LoadAudioSourceMP3(const std::string& filename);
	};
}