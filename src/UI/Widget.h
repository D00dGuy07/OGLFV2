#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "Util/Helpers.h"

#include "UI/IWidgetContainer.h"
#include "UI/Surface.h"
#include "UI/InputEvents.h"

#include "Renderer/Mesh.h"
#include "Renderer/Shader.h"

namespace UI
{
	// Functions exactly like SizeConstraint in Roblox
	enum class WidgetSizeConstraint
	{
		RelativeXY,
		RelativeXX,
		RelativeYY
	};

	template<typename T>
	class WidgetProperty : public IValueObserver<T>
	{
	public:
		template<typename... Args>
		WidgetProperty(Surface* root, Args&&... args)
			: IValueObserver<T>(T(std::forward<Args>(args)...)), m_WidgetRoot(root) {}

		using IValueObserver<T>::operator=;

	private:
		Surface* m_WidgetRoot;

		virtual void OnValueModified() override
		{
			if (m_WidgetRoot)
				m_WidgetRoot->ShouldRender = true;
		}

		// This class is an abstraction for widget properties so I think friend class here is justified
		friend class Widget;
	};

	class Widget : public IWidgetContainer
	{
	public:
		virtual Surface* GetRootSurface() override { return m_Root; }

		// IWidgetContainer API
		inline IWidgetContainer* GetParent() const override { return m_Parent; }
		inline bool IsSurface() const override { return false; }

		glm::vec4 TransformPosition(const glm::vec4& childSpace, const glm::vec2& resolution) const override;
		glm::vec4 TransformSize(const glm::vec4& childSpace, const glm::vec2& resolution) const override;

		// Render public API
		virtual const char* GetShaderPath() const = 0;
		virtual int32_t BuildRenderMesh(Mesh& renderMesh, Shader& renderShader, glm::vec2 resolution) = 0;
		virtual void Update(glm::vec2 resolution);

		// Widget public API
		inline glm::vec4 GetScreenRect() { return m_ScreenRect; }
		inline glm::vec2 GetAbsolutePosition() { return m_AbsolutePosition; }
		inline glm::vec2 GetAbsoluteSize() { return m_AbsoluteSize; }

		inline bool Intersects(glm::vec2 point)
		{ return point.x >= m_ScreenRect.x && point.x < m_ScreenRect.z && point.y >= m_ScreenRect.y && point.y < m_ScreenRect.w; }
		inline bool Intersects(glm::vec4 rect) 
		{ return !(rect.x > m_ScreenRect.z || rect.z < m_ScreenRect.x || rect.y > m_ScreenRect.w || rect.w < m_ScreenRect.y); }

		WidgetProperty<glm::vec2> AnchorPoint = WidgetProperty<glm::vec2>(nullptr, 0.5f);
		WidgetProperty<glm::vec4> Position = WidgetProperty<glm::vec4>(nullptr, 0.0f);
		// Rotation is 100% visual and has no effect on children at the moment
		WidgetProperty<float> Rotation = WidgetProperty<float>(nullptr, 0.0f);

		WidgetProperty<glm::vec4> Size = WidgetProperty<glm::vec4>(nullptr, 0.0f);
		WidgetProperty<WidgetSizeConstraint> SizeConstraint = 
			WidgetProperty<WidgetSizeConstraint>(nullptr, WidgetSizeConstraint::RelativeXY);

		WidgetProperty<bool> Visible = WidgetProperty<bool>(nullptr, true);
		WidgetProperty<bool> ClipsDescendants = WidgetProperty<bool>(nullptr, true);
		WidgetProperty<float> Alpha = WidgetProperty<float>(nullptr, 1.0f);
		WidgetProperty<int32_t> ZIndex = WidgetProperty<int32_t>(nullptr, 1);

		// Input
		bool SinksInput = true; // Doesn't have to be a widget property because it isn't a visual property

		// These callbacks are public so that they can be manipulated through code for whatever reason
		virtual void OnMouseMove(MouseMoveArgs& args) {}
		virtual void OnMouseEnter(MouseEnterArgs& args) {}
		virtual void OnMouseExit(MouseExitArgs& args) {}
		virtual void OnMouseButton(MouseButtonArgs& args) {}

	protected:
		IWidgetContainer* m_Parent = nullptr;
		Surface* m_Root = nullptr;
		glm::vec4 m_ScreenRect = glm::vec4(0.0f);
		glm::vec2 m_AbsolutePosition = glm::vec2(0.0f);
		glm::vec2 m_AbsoluteSize = glm::vec2(0.0f);

		void SetRootReferences(Surface* root = nullptr);
		virtual void OnSetRootReferences(Surface* rootSurface) {}

		// Since the WidgetProperty class is only "friends" with this widget base
		// a helper function to set the root is cleanest without exposing the root publicly
		template<typename T>
		void SetPropertyRootReference(WidgetProperty<T>& prop, Surface* rootSurface) { prop.m_WidgetRoot = rootSurface; }
		
		static glm::vec4 GetBounds(glm::vec4 position, glm::vec4 size, WidgetSizeConstraint constraint, 
			glm::vec2 anchorPoint, glm::vec2 resolution, glm::vec2& sizeScale, glm::vec2& positionScale);

		void CalculateScreenRect(glm::vec2 resolution);

		friend class IWidgetContainer;
	};
}