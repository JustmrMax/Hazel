#pragma once

namespace Hazel
{
	// Time is usually in seconds
	class Timestep
	{
	public:
		Timestep(float frameTime = 0.0f);

		operator float() const { return m_FrameTime; }

		inline float GetTimeSeconds() const { return m_FrameTime; }
		inline float GetTimeMilliseconds() const { return m_FrameTime * 1000; }

	private:
		float m_FrameTime;
	};
}