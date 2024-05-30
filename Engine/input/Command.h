#pragma once
#include "glm/vec2.hpp"

namespace mk
{
	class GameObject;

	class Command
	{
	public:
		Command() = default;
		virtual ~Command() = default;

		Command(const Command& other)					= delete;
		Command(Command&& other) noexcept				= delete;
		Command& operator=(const Command& other)		= delete;
		Command& operator=(Command&& other)	noexcept	= delete;

		virtual void Execute() = 0;
	};

	class GameObjectCommand : public Command
	{
	public:
		GameObjectCommand(GameObject* object) : m_GameObject{ object }{}
		~GameObjectCommand() override = default;

		GameObjectCommand(const GameObjectCommand& other)					= delete;
		GameObjectCommand(GameObjectCommand&& other) noexcept				= delete;
		GameObjectCommand& operator=(const GameObjectCommand& other)		= delete;
		GameObjectCommand& operator=(GameObjectCommand&& other) noexcept	= delete;

	protected:
		GameObject& GetGameObject() const { return *m_GameObject; }

	private:
		GameObject* m_GameObject{};
	};

	class EditorCommand : public Command
	{
	public:
		EditorCommand() = default;
		~EditorCommand() override = default;

		EditorCommand(const EditorCommand& other) = delete;
		EditorCommand(EditorCommand&& other) noexcept = delete;
		EditorCommand& operator=(const EditorCommand& other) = delete;
		EditorCommand& operator=(EditorCommand&& other) noexcept = delete;
	};

	class DirectionCommand : public GameObjectCommand
	{
	public:
		DirectionCommand(GameObject* object) : GameObjectCommand(object) {}
		~DirectionCommand() override = default;

		DirectionCommand(const DirectionCommand& other) = delete;
		DirectionCommand(DirectionCommand&& other) noexcept = delete;
		DirectionCommand& operator=(const DirectionCommand& other) = delete;
		DirectionCommand& operator=(DirectionCommand&& other) noexcept = delete;

		void Execute() override {}
		virtual void Execute(const glm::vec2& direction) = 0;
    };
}
