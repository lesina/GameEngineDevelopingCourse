#pragma once

#include <Core/export.h>

namespace GameEngine::Core
{
	template <typename T, size_t N>
	struct array
	{
		T data[N];

		T& operator[](size_t i) { return data[i]; }
		const T& operator[](size_t i) const { return data[i]; }

		T* begin() { return data; }
		T* end() { return data + N; }

		const T* begin() const { return data; }
		const T* end() const { return data + N; }

		consteval size_t size() const { return N; }
	};
}