#pragma once

namespace mk
{
	class Mouse;
	using controller_id = int32_t;

	class IInput
	{
	public:
		virtual ~IInput()							= default;
		IInput(const IInput& other)					= delete;
		IInput(IInput&& other) noexcept				= delete;
		IInput& operator=(const IInput& other)		= delete;
		IInput& operator=(IInput&& other) noexcept	= delete;

		virtual bool ProcessInput() = 0;
		virtual controller_id RegisterController() = 0;
		virtual void UnregisterController(controller_id id)	= 0;
		virtual const Mouse* GetMouse() const = 0;
			
	protected:
		IInput() = default;
	};
}
