#include "p_pch.h"
#include "Layer.h"

namespace Paper
{
	Layer::Layer(const std::string& d_name) 
		: m_DebugName(d_name) {}

	Layer::~Layer() {}
}