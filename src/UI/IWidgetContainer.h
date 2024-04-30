#pragma once

#include "glm/glm.hpp"

#include "UI/IWidgetGroup.h"

#include <vector>
#include <memory>

namespace UI
{
	class Surface;

	class IWidgetContainer
	{
	public:
		~IWidgetContainer();

		virtual inline IWidgetContainer* GetParent() const = 0;
		virtual Surface* GetRootSurface() = 0;
		virtual inline bool IsSurface() const = 0;

		virtual glm::vec4 TransformPosition(const glm::vec4& childSpace, const glm::vec2& resolution) const = 0;
		virtual glm::vec4 TransformSize(const glm::vec4& childSpace, const glm::vec2& resolution) const = 0;

		virtual void AddChild(std::shared_ptr<Widget> widget);
		void AddChild(std::shared_ptr<IWidgetGroup> widgetGroup);
		void RemoveChild(std::shared_ptr<Widget> widget);
		inline const std::vector<std::shared_ptr<Widget>>& GetChildren() { return m_Children; }

		inline const std::shared_ptr<Widget> FirstChild() const { return m_Children.empty() ? nullptr : m_Children.front(); }
		inline const std::shared_ptr<Widget> LastChild() const { return m_Children.empty() ? nullptr : m_Children.back(); }

		auto begin() const { return m_Children.begin(); }
		auto begin() { return m_Children.begin(); }
		auto end() const { return m_Children.end(); }
		auto end() { return m_Children.end(); }

	protected:
		std::vector<std::shared_ptr<Widget>> m_Children;

		void DisassociateWidget(std::shared_ptr<Widget> widget);
	};
}