#pragma once
namespace mk
{
	template <typename T>
	class ISingleton
	{
	public:
		static T& GetInstance()
		{
			static T instance{};
			return instance;
		}

		virtual ~ISingleton() = default;
		ISingleton(const ISingleton& other) = delete;
		ISingleton(ISingleton&& other) = delete;
		ISingleton& operator=(const ISingleton& other) = delete;
		ISingleton& operator=(ISingleton&& other) = delete;

	protected:
		ISingleton() = default;
	};
}