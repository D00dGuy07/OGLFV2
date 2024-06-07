#include "oglfv2/UI/Surface.h"

#include "Arrowhead/Sort.h"

#include "oglfv2/Renderer/Renderer.h"
#include "oglfv2/Renderer/Framebuffer.h"
#include "oglfv2/Renderer/Shader.h"
#include "oglfv2/Renderer/UniformBuffer.h"

#include "oglfv2/UI/Widget.h"

using namespace UI;

Surface::Surface()
	: m_Resolution(glm::vec2(0, 0)), m_Framebuffer() {}

Surface::Surface(glm::vec2 resolution)
	: m_Resolution(resolution)
{ CreateFramebuffer(); }

void Surface::CreateFramebuffer()
{
	m_Framebuffer = std::make_unique<Framebuffer>(FramebufferSpec{
		static_cast<uint32_t>(m_Resolution.x), static_cast<uint32_t>(m_Resolution.y),
		{{ImageBufferType::Texture, FBOAttachmentType::Color, InternalImageFormat::RGBA8}},
		{{ImageBufferType::RenderBuffer, FBOAttachmentType::DepthStencil, InternalImageFormat::Depth24_Stencil8}}
	});
}

Texture* Surface::GetColorBuffer()
{
	if (!m_Framebuffer)
		return nullptr;
	return static_cast<Texture*>(m_Framebuffer->GetColorAttachment(0).Buffer);
}

void Surface::AddChild(std::shared_ptr<Widget> widget)
{
	IWidgetContainer::AddChild(widget);
	widget->Update(m_Resolution);
}

glm::vec4 Surface::RenderWidget(std::shared_ptr<Widget> widget, glm::vec4 bounds)
{
	// Get the right shader ready to render
	auto renderShader = GetWidgetShader(widget->GetShaderPath());
	renderShader->Bind(); // So that uniform changes can not be cached

	// Generate the mesh for the widget
	Mesh* renderMesh = Renderer::CreateGarbage<Mesh>();
	int32_t instanceCount = widget->BuildRenderMesh(*renderMesh, *renderShader, m_Resolution);
	if (instanceCount == -1) { return glm::vec4(0.0f, 0.0f, 0.0f, 0.0f); }
	renderMesh->Construct();

	// Set common appearance uniforms
	renderShader->SetUniform1f("u_Alpha", widget->Alpha);
	renderShader->SetUniform1f("u_Rotation", widget->Rotation);

	// Set other uniforms
	glm::vec4 screenRect = widget->GetScreenRect();
	glm::vec2 center = glm::vec2((screenRect.x + screenRect.z) / 2.0f, (screenRect.y + screenRect.w) / 2.0f);
	renderShader->SetUniform2f("u_Center", center);
	renderShader->SetUniform4f("u_Bounds", bounds);
	renderShader->SetUniform2f("u_Resolution", m_Resolution);

	// Render mesh
	if (instanceCount < 1)
		Renderer::SubmitMesh(*renderMesh, *renderShader);
	else
		Renderer::SubmitMeshInstanced(*renderMesh, *renderShader, instanceCount);

	return screenRect;
}

void Surface::RenderRecursive(std::shared_ptr<Widget> widget, glm::vec4 bounds)
{
	glm::vec4 newBounds = glm::vec4(0.0f, 0.0f, m_Resolution);
	if (widget->Visible)
	{
		glm::vec4 widgetRect = RenderWidget(widget, bounds);
		newBounds = widget->ClipsDescendants ? widgetRect : newBounds;
	}
	else if (widget->ClipsDescendants)
		return;
	
	std::vector<std::shared_ptr<Widget>> sortedChildren = widget->GetChildren();
	arwh::Sort<arwh::SortingAlgorithm::Introsort>(sortedChildren.data(), 0, static_cast<int32_t>(sortedChildren.size() - 1),
	[](std::shared_ptr<Widget> a, std::shared_ptr<Widget> b) {
		return a->ZIndex.Value() < b->ZIndex.Value();
	});

	for (auto& widget : sortedChildren)
		RenderRecursive(widget, newBounds);
}

void Surface::Render()
{
	// Shouldn't always render
	if (!m_Framebuffer || !ShouldRender)
		return;
	ShouldRender = false;

	// Sort widgets in render order
	std::vector<std::shared_ptr<Widget>> sortedChildren = m_Children;
	arwh::Sort<arwh::SortingAlgorithm::Introsort>(sortedChildren.data(), 0, static_cast<int32_t>(sortedChildren.size() - 1),
	[](std::shared_ptr<Widget> a, std::shared_ptr<Widget> b) {
		return a->ZIndex.Value() < b->ZIndex.Value();
	});

	// Recursively render each widget and it's children
	m_Framebuffer->Bind();
	Renderer::Clear();
	for (auto& widget : sortedChildren)
		RenderRecursive(widget, glm::vec4(0.0f, 0.0f, m_Resolution));
	m_Framebuffer->Unbind();
}

void Surface::UpdateRecursive(std::shared_ptr<Widget> widget)
{
	widget->Update(m_Resolution);

	for (auto& widget : *widget)
		UpdateRecursive(widget);
}

void Surface::Update()
{
	// Timed update callback
	double time = s_TimeFunction->operator()();
	double deltaTime = time - m_LastUpdate;
	m_LastUpdate = time;

	if (s_TimeFunction && m_LastUpdate != -1.0f)
		m_TimedUpdateCallbacks.Call(deltaTime);

	// Update widgets
	if (ShouldRender)
		for (auto& widget : m_Children)
			UpdateRecursive(widget);
}

void Surface::SetResolution(glm::vec2 resolution)
{
	m_Resolution = resolution;

	// Ensure that the framebuffer is the right size
	if (!m_Framebuffer)
		CreateFramebuffer();
	else
		m_Framebuffer->Resize(static_cast<int32_t>(resolution.x), static_cast<int32_t>(resolution.y));

	// Framebuffer is empty now so we need to re-render it
	ShouldRender = true;
}

std::shared_ptr<Shader> Surface::GetWidgetShader(const char* shaderPath)
{
	if (s_WidgetShaders.find(shaderPath) == s_WidgetShaders.end())
		s_WidgetShaders[shaderPath] = std::make_shared<Shader>(shaderPath, false);
	return s_WidgetShaders[shaderPath];
}

// Input

bool Surface::UpdateCursorPos(glm::vec2 pos)
{
	MouseMoveArgs globalArgs = { false, nullptr, pos, m_MousePos };

	// Intersection calculations aren't cheap!
	if (pos != m_MousePos)
	{
		// Find intersected widgets
		std::shared_ptr<Widget> lastHover = FindWidget(m_MousePos);
		std::shared_ptr<Widget> currentHover = FindWidget(pos);

		// Handle all hover scenarios
		if (currentHover != nullptr || lastHover != nullptr)
		{
			if (currentHover == lastHover) // Both are the same widget
			{
				MouseMoveArgs args = { false, currentHover, pos, m_MousePos };
				currentHover->OnMouseMove(args);
				globalArgs.IsHandled = args.IsHandled;
			}
			else if (currentHover != lastHover && currentHover != nullptr && lastHover != nullptr) // Both are different widgets
			{
				MouseEnterArgs enterArgs = { false, lastHover, pos, m_MousePos };
				MouseExitArgs exitArgs = { false, currentHover, pos, m_MousePos };
				currentHover->OnMouseEnter(enterArgs);
				lastHover->OnMouseExit(exitArgs);
				globalArgs.IsHandled = enterArgs.IsHandled || exitArgs.IsHandled;
			}
			else if (lastHover == nullptr) // Only current is a widget
			{
				MouseEnterArgs enterArgs = { false, nullptr, pos, m_MousePos };
				currentHover->OnMouseEnter(enterArgs);
				globalArgs.IsHandled = enterArgs.IsHandled;
			}
			else // Only last is a widget
			{
				MouseExitArgs exitArgs = { false, nullptr, pos, m_MousePos };
				lastHover->OnMouseExit(exitArgs);
				globalArgs.IsHandled = exitArgs.IsHandled;
			}
		}

		globalArgs.Target = currentHover;
	}
	

	// Invoke events
	m_MouseMoveCallbacks.Call(globalArgs);
	
	m_MousePos = pos;
	return globalArgs.IsHandled;
}

bool Surface::UpdateMouseButtonState(MouseButton button, InputAction action)
{
	MouseButtonArgs args = { false, nullptr, m_MousePos, button, action };

	// Don't wanna do an intersection test unless completely necessary
	bool isPressed = action == InputAction::Press;
	if (m_MouseButtonsPressed[static_cast<int32_t>(button)] != isPressed)
	{
		m_MouseButtonsPressed[static_cast<int32_t>(button)] = isPressed;

		// Find a widget that the mouse is over
		std::shared_ptr<Widget> currentHover = FindWidget(m_MousePos);
		args.Target = currentHover;

		// Call the event if there is a widget
		if (currentHover != nullptr)
			currentHover->OnMouseButton(args);
	}

	// Invoke callbacks
	m_MouseButtonCallbacks.Call(args);

	return args.IsHandled;
}

std::shared_ptr<Widget> Surface::FindWidget(glm::vec2 pos, std::shared_ptr<IWidgetContainer> parent)
{
	// Find the child that intersects the point and has the lowest ZIndex
	std::shared_ptr<Widget> topChild = nullptr;
	for (auto& child : (parent == nullptr ? m_Children : parent->GetChildren()))
	{
		if (child->Intersects(pos) && child->SinksInput)
		{
			if (topChild == nullptr)
				topChild = child;
			else if (topChild != nullptr && child->ZIndex.Value() < topChild->ZIndex.Value())
			{
				topChild = child;
			}
		}
	}

	// Return nullptr if there is none
	if (topChild == nullptr)
		return topChild;

	// Recursively check for a deeper top descendant and return that if it exists otherwise return the top child
	std::shared_ptr<Widget> topDescendant = FindWidget(pos, topChild);
	if (topDescendant != nullptr)
		return topDescendant;
	return topChild;
}

std::function<void(GLFWwindow*, double, double)> Surface::GLFWMouseCursorPosCallback()
{
	return [&](GLFWwindow*, double x, double y) {
		UpdateCursorPos({ static_cast<float>(x), static_cast<float>(y) });
	}; 
}

std::function<void(GLFWwindow*, int, int, int)> Surface::GLFWMouseButtonCallback()
{
	return [&](GLFWwindow*, int button, int action, int) {
		UpdateMouseButtonState(static_cast<UI::MouseButton>(button), static_cast<UI::InputAction>(action));
	};
}

std::function<void(GLFWwindow*, double, double)> Surface::GLFWMouseScrollCallback()
{
	return [&](GLFWwindow*, double XOffset, double YOffset) {
		UpdateScrollInput({ static_cast<float>(XOffset), static_cast<float>(YOffset) });
	};
}

std::function<void(GLFWwindow*, int, int, int, int)> Surface::GLFWKeyCallback()
{
	return [&](GLFWwindow*, int key, int, int action, int) {
		UpdateKeyInput(static_cast<Key>(key), static_cast<InputAction>(action));
	};
}

std::function<void(GLFWwindow*, unsigned int)> Surface::GLFWCharCallback()
{
	return [&](GLFWwindow*, unsigned int codepoint) {
		UpdateCharInput(static_cast<wchar_t>(codepoint));
	};
}