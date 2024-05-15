#pragma once
#include "IInput.h"

namespace mk
{
	class NullInput : public IInput
	{
	public:
		NullInput()				= default;
		virtual ~NullInput()	= default;
		NullInput(const NullInput& other)					= delete;
		NullInput(NullInput&& other) noexcept				= delete;
		NullInput& operator=(const NullInput& other)		= delete;
		NullInput& operator=(NullInput&& other) noexcept	= delete;

		bool ProcessInput() override { return false; }
		controller_id RegisterController() override { return  0; }
		void UnregisterController(controller_id id) override {}
	};
}
