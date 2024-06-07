#pragma once

#include <memory>
#include <functional>

#include "Arrowhead/IValueObserver.h"

namespace UI
{
	class Widget;
	class Surface;

	class IWidgetGroup
	{
	public:
		virtual std::shared_ptr<Widget> GetRoot() const = 0;

		// This isn't really WidgetGroup specific functionality
		template<typename T>
		class GroupProperty : public arwh::IValueObserver<T>
		{
		public:
			template<typename... Args>
			GroupProperty(std::function<void()> callback, Args&&... args)
				: arwh::IValueObserver<T>(T(std::forward<Args>(args)...)), m_Callback(callback) {}

			using arwh::IValueObserver<T>::operator=;

		private:
			std::function<void()> m_Callback;

			virtual void OnValueModified() override
			{
				m_Callback();
			}
		};

	protected:

		virtual void OnRootChanged(Surface* root) {}

		friend class IWidgetContainer; // Widget does this for ease of access too so IWidgetGroup will too
	};
}