#pragma once

#include "p_pch.h"

#include "Paper/Events/Event.h"
#include "Paper/Events/ApplicationEvent.h"
#include "Paper/Events/KeyEvent.h"
#include "Paper/Events/MouseEvent.h"

#include "Paper/Core/Layer.h"

namespace Paper
{
	class PAPER_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRenderer() override;

		void Begin();
		void End();
	private:
		float m_Time;
	};
}