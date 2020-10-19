#pragma once

#include "Paper/Core/Input.h"

namespace Paper
{
	class LinuxInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
	};
}