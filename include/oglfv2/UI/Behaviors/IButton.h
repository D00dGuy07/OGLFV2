#pragma once

#include "oglfv2/UI/Widget.h"

#include "Arrowhead/Events.h"

namespace UI
{
#pragma warning( push )
#pragma warning( disable: 4250 )
	template<typename T>
	class IButton : virtual protected T, virtual public Widget
	{
	public:
		using Widget::AnchorPoint;
		using Widget::Position;
		using Widget::Rotation;
		using Widget::Size;
		using Widget::SizeConstraint;
		using Widget::Visible;
		using Widget::ClipsDescendants;
		using Widget::Alpha;
		using Widget::ZIndex;

		std::shared_ptr<arwh::EventConnection<MouseMoveArgs>> ConnectMouseMove(std::function<void(MouseMoveArgs)> callback)
		{ return m_MoveCallbacks.Connect(callback); }
		std::shared_ptr<arwh::EventConnection<MouseEnterArgs>> ConnectMouseEnter(std::function<void(MouseEnterArgs)> callback)
		{ return m_EnterCallbacks.Connect(callback); }
		std::shared_ptr<arwh::EventConnection<MouseExitArgs>> ConnectMouseExit(std::function<void(MouseExitArgs)> callback)
		{ return m_ExitCallbacks.Connect(callback); }
		std::shared_ptr<arwh::EventConnection<MouseButtonArgs>> ConnectMouseButton(std::function<void(MouseButtonArgs)> callback)
		{ return m_ButtonCallbacks.Connect(callback); }

		bool IsHovered() const { return m_IsHovered; }
		bool IsClicked() const { return m_IsClicked; }

	protected:
		virtual void OnMouseMove(MouseMoveArgs& args) override 
		{ 
			m_MoveCallbacks.Call(args);
			args.IsHandled = true;
		}

		virtual void OnMouseEnter(MouseEnterArgs& args) override 
		{
			m_IsHovered = true;
			m_EnterCallbacks.Call(args);
			args.IsHandled = true;
		}

		virtual void OnMouseExit(MouseExitArgs& args) override 
		{
			m_IsHovered = false;
			m_IsClicked = false;
			m_ExitCallbacks.Call(args);
			args.IsHandled = true;
		}

		virtual void OnMouseButton(MouseButtonArgs& args) override 
		{
			m_IsClicked = args.Button == MouseButton::Left ? args.Action == InputAction::Press : m_IsClicked;
			m_ButtonCallbacks.Call(args);
			args.IsHandled = true;
		}

		bool m_IsHovered = false;
		bool m_IsClicked = false;

	private: // This line can be deleted if button implementees want to use the callback lists
		arwh::CallbackList<MouseMoveArgs> m_MoveCallbacks;
		arwh::CallbackList<MouseEnterArgs> m_EnterCallbacks;
		arwh::CallbackList<MouseExitArgs> m_ExitCallbacks;
		arwh::CallbackList<MouseButtonArgs> m_ButtonCallbacks;
	};
#pragma warning( pop )
}