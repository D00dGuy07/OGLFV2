#pragma once

#include "UI/Surface.h"
#include "UI/InputEvents.h"

namespace UI
{
	// Kind of a basic implementation. Ideally the surface would be aware of which widget has the focus but as
	// far as I know there isn't a great way of doing that without stripping type information and casting to
	// a plain widget pointer.

	template<typename T>
	class Focusable : public T
	{
	public:
		~Focusable() { m_EventConnection->Disconnect; }

		inline bool IsFocused() const { return m_IsFocused; }

	protected:

		bool m_IsFocused;

		virtual void OnSetRootReferences(Surface* rootSurface) override
		{
			// Connect or reconnect the event
			if (m_EventConnection)
				m_EventConnection->Disconnect();
			m_EventConnection = T::GetRootSurface()->ConnectMouseButton([&](MouseButtonArgs args)
				{
					m_IsFocused = args.Target.get() == static_cast<Widget*>(this);
				}
			);

			// Pass on the event
			T::OnSetRootReferences(rootSurface);
		}

	private:

		bool m_IsEventConnected;
		std::shared_ptr<EventConnection<MouseButtonArgs>> m_EventConnection;
	};
}