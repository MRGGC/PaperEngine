#pragma once

#include "p_pch.h"
#include "Paper/Events/Event.h"
#include "Paper/Layer.h"

namespace Paper
{
	class PAPER_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);
	private:
		float m_Time;
	};
}