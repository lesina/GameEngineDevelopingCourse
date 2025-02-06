#include <PackedVariables.h>

namespace GameEngine::Core
{
	PackedVariables::PackedVariables()
	{
		// No more than 4 8-bytes variables usually?
		m_dataSize = 32;
		m_data = new std::byte[m_dataSize];
	}

	PackedVariables::~PackedVariables()
	{
		delete m_data;
		m_data = nullptr;
		m_dataSize = 0;
	}

	void PackedVariables::Resize()
	{
		std::byte* data = new std::byte[m_dataSize * 2];
		std::memcpy(data, m_data, m_dataSize);
		m_dataSize *= 2;

		delete m_data;
		m_data = data;
	}
}