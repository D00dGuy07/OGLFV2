#include "Widget.h"

//#include "UI/Surface.h"

using namespace UI;

glm::vec4 Widget::TransformPosition(const glm::vec4& childSpace, const glm::vec2& resolution) const
{
	glm::vec2 sizeAdjusted = glm::vec2(
		(Size->x * resolution.x + Size->y) / resolution.x,
		(Size->z * resolution.y + Size->w) / resolution.y
	);

	return glm::vec4(
		childSpace.x * sizeAdjusted.x + (Position->x - AnchorPoint->x * sizeAdjusted.x),
		childSpace.y + Position->y,
		childSpace.z * sizeAdjusted.y + (Position->z - AnchorPoint->y * sizeAdjusted.y),
		childSpace.w + Position->w
	);
}

glm::vec4 Widget::TransformSize(const glm::vec4& childSpace, const glm::vec2& resolution) const
{
	// This function converts the exact pixel sizes to scale because it's easier
	glm::vec2 resolutionConstrained;
	switch (SizeConstraint)
	{
	case WidgetSizeConstraint::RelativeXX:
		resolutionConstrained = glm::vec2(resolution.x, resolution.x);
		break;
	case WidgetSizeConstraint::RelativeYY:
		resolutionConstrained = glm::vec2(resolution.y, resolution.y);
		break;
	default:
		resolutionConstrained = resolution;
		break;
	}

	glm::vec2 transformedScale = glm::vec2(
		(childSpace.x * resolution.x / resolution.x) * ((Size->x * resolution.x + Size->y) / resolution.x),
		(childSpace.z * resolution.y / resolution.y) * ((Size->z * resolution.y + Size->w) / resolution.y)
	) * resolutionConstrained / resolution;

	return glm::vec4(
		transformedScale.x,
		childSpace.y,
		transformedScale.y,
		childSpace.w
	);
}

void Widget::Update(glm::vec2 resolution)
{
	CalculateScreenRect(resolution);
}

void Widget::SetRootReferences(Surface* root)
{
	// Get the root surface
	m_Root = root;
	if (m_Root == nullptr) 
	{ 
		IWidgetContainer* ancestor = m_Parent;
		while (ancestor != nullptr && !ancestor->IsSurface())
			ancestor = ancestor->GetParent();
		m_Root = reinterpret_cast<Surface*>(ancestor);
	}

	// Set root surface pointers
	AnchorPoint.m_WidgetRoot = m_Root;
	Position.m_WidgetRoot = m_Root;
	Rotation.m_WidgetRoot = m_Root;

	Size.m_WidgetRoot = m_Root;
	SizeConstraint.m_WidgetRoot = m_Root;

	Visible.m_WidgetRoot = m_Root;
	Alpha.m_WidgetRoot = m_Root;
	ZIndex.m_WidgetRoot = m_Root;

	OnSetRootReferences(m_Root);

	// Update children recursively
	for (auto& child : m_Children)
		child->SetRootReferences(m_Root);
}

glm::vec4 Widget::GetBounds(glm::vec4 position, glm::vec4 size, WidgetSizeConstraint constraint,
	glm::vec2 anchorPoint, glm::vec2 resolution, glm::vec2& sizeScale, glm::vec2& positionScale)
{
	// I could "simplify" this by solving for the exact values from the start but this way is easier to understand
	positionScale = glm::vec2(position.x, position.z) + glm::vec2(position.y, position.w) / resolution;

	glm::vec2 resolutionConstrained;
	switch (constraint)
	{
	case WidgetSizeConstraint::RelativeXX:
		sizeScale = glm::vec2(size.x, size.x) + glm::vec2(size.y, size.w) / resolution.x;
		resolutionConstrained = glm::vec2(resolution.x, resolution.x);
		break;
	case WidgetSizeConstraint::RelativeYY:
		sizeScale = glm::vec2(size.z, size.z) + glm::vec2(size.y, size.w) / resolution.y;
		resolutionConstrained = glm::vec2(resolution.y, resolution.y);
		break;
	default:
		sizeScale = glm::vec2(size.x, size.z) + glm::vec2(size.y, size.w) / resolution;
		resolutionConstrained = resolution;
		break;
	}

	glm::vec2 screenSize = sizeScale * resolutionConstrained;
	sizeScale = screenSize / resolution;
	glm::vec2 screenPosition = (positionScale - sizeScale * anchorPoint) * resolution;

	return glm::vec4(screenPosition, screenPosition + screenSize);
}

void Widget::CalculateScreenRect(glm::vec2 resolution)
{
	if (m_Parent == nullptr)
	{
		m_ScreenRect = GetBounds(Position, Size, SizeConstraint, AnchorPoint, resolution, 
			m_AbsoluteSize, m_AbsolutePosition);
		return;
	}

	glm::vec4 transformedPosition = Position;
	glm::vec4 transformedSize = Size;

	IWidgetContainer* ancestor = m_Parent;
	while (!ancestor->IsSurface())
	{
		transformedPosition = ancestor->TransformPosition(transformedPosition, resolution);
		transformedSize = ancestor->TransformSize(transformedSize, resolution);
		ancestor = ancestor->GetParent();
	}

	m_ScreenRect = GetBounds(transformedPosition, transformedSize, SizeConstraint, AnchorPoint, resolution, 
		m_AbsoluteSize, m_AbsolutePosition);
}