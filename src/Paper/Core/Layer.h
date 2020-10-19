#pragma once

#include "p_pch.h"
#include "Paper/Core/Core.h"
#include "Paper/Core/Timestep.h"
#include "Paper/Events/Event.h"

namespace Paper
{
	class PAPER_API Layer
	{
	public:
		Layer(const std::string& name);
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep dt) {}
		virtual void OnEvent(Event& e) {}
		virtual void OnImGuiRenderer() {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}