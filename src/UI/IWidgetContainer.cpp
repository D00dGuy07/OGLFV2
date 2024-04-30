#include "UI/IWidgetContainer.h"

#include "UI/Widget.h"

using namespace UI;

IWidgetContainer::~IWidgetContainer()
{
	for (auto& child : m_Children)
		DisassociateWidget(child);
}

void IWidgetContainer::AddChild(std::shared_ptr<Widget> widget)
{
	if (widget->m_Parent != nullptr)
		widget->m_Parent->RemoveChild(widget);

	widget->m_Parent = this;
	widget->SetRootReferences();

	m_Children.push_back(widget);
}

void IWidgetContainer::AddChild(std::shared_ptr<IWidgetGroup> widgetGroup) 
{ 
	AddChild(widgetGroup->GetRoot());
	widgetGroup->OnRootChanged(GetRootSurface());
}

void IWidgetContainer::RemoveChild(std::shared_ptr<Widget> widget)
{
	if (widget->m_Parent != this)
		return;

	DisassociateWidget(widget);

	for (auto iterator = m_Children.begin(); iterator != m_Children.end(); iterator++)
	{
		if (*iterator == widget)
		{
			m_Children.erase(iterator);
			break;
		}
	}
}

void IWidgetContainer::DisassociateWidget(std::shared_ptr<Widget> widget)
{
	widget->m_Parent = nullptr;
	widget->SetRootReferences();
}