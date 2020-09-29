#pragma once

#include "p_pch.h"
#include "Paper/Core.h"
#include "Layer.h"

namespace Paper
{
	class PAPER_API LayerStack
	{
	public:
		LayerStack();
		virtual ~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlayer);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		inline std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		inline std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*>::iterator m_LayerInsert;
	};
}