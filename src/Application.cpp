#include "Application.h"

#include "GLFW/glfw3.h"

#include <iostream>

#include "Util/GLFWEventWrapper.h"
#include "Util/Timer.h"

#include "Renderer/Renderer.h"

#include "UI/Widgets/Frame.h"
#include "UI/Widgets/FrameButton.h"
#include "UI/Widgets/TexturedButton.h"
#include "UI/Widgets/NineSliceButton.h"
#include "UI/Widgets/TexturedFrame.h"
#include "UI/Widgets/TextLabel.h"
#include "UI/Widgets/NineSliceFrame.h"

#include "UI/Behaviors/Focusable.h"

#include "UI/IWidgetGroup.h"
#include "UI/Animators.h"

Application::Application()
	: m_Window(nullptr), m_Surface(), m_Mesh(), m_Shader("res/Render.shader", false) {}

void Application::LoadResources()
{
	// Turn on blending
	Renderer::UseBlending(true, Renderer::BlendFunction::SrcAlpha, Renderer::BlendFunction::OneMinusSrcAlpha);

	// Build mesh

	float vertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	m_Mesh.SetVertices(vertices, 4 * 4 * 4);
	m_Mesh.SetIndices(indices, 6);
	m_Mesh.BufferLayout.Push<float>(2);
	m_Mesh.BufferLayout.Push<float>(2);
	m_Mesh.Construct();

	m_Shader.SetUniform1i("u_Texture", 0);

	// Window event callbacks
	GLFWEventWrapper* wrapper = GLFWEventWrapper::GetWrapper(m_Window);
	wrapper->ConnectFramebufferSize([&](GLFWwindow*, int32_t width, int32_t height) 
		{
			if (width > 0 && height > 0)
				m_Surface.SetResolution({ width, height });
			Renderer::SetViewport(width, height);

			Draw();
		}
	);

	// Setup surface
	int32_t width, height;
	glfwGetFramebufferSize(m_Window, &width, &height);
	m_Surface.SetResolution({ width, height });
	m_Surface.SetTimeFunction([]() 
		{ return glfwGetTime(); }
	);

	wrapper->ConnectMouseCursorPos(m_Surface.GLFWMouseCursorPosCallback());
	wrapper->ConnectMouseButton(m_Surface.GLFWMouseButtonCallback());
	wrapper->ConnectMouseScroll(m_Surface.GLFWMouseScrollCallback());
	wrapper->ConnectKey(m_Surface.GLFWKeyCallback());
	wrapper->ConnectChar(m_Surface.GLFWCharCallback());
}

glm::vec3 hsv2rgb(glm::vec3 HSV)
{
	glm::vec3 RGB;
	double H = HSV.x, S = HSV.y, V = HSV.z,
		P, Q, T,
		fract;

	(H == 360.0) ? (H = 0.0) : (H /= 60.0);
	fract = H - std::floor(H);

	P = V * (1.0 - S);
	Q = V * (1.0 - S * fract);
	T = V * (1.0 - S * (1.0 - fract));

	if (0.0 <= H && H < 1.0)
		RGB = { V, T, P };
	else if (1.0 <= H && H < 2.0)
		RGB = { Q, V, P };
	else if (2.0 <= H && H < 3.0)
		RGB = { P, V, T };
	else if (3.0 <= H && H < 4.0)
		RGB = { P, Q, V };
	else if (4.0 <= H && H < 5.0)
		RGB = { T, P, V };
	else if (5.0 <= H && H < 6.0)
		RGB = { V, P, Q };
	else
		RGB = { 0.0, 0.0, 0.0 };

	return RGB;
}

void Application::Draw()
{
	Renderer::Clear();

	m_Surface.Update();
	m_Surface.Render();

	m_Surface.GetColorBuffer()->Bind();
	Renderer::SubmitMesh(m_Mesh, m_Shader);

	Renderer::DrawFrame();

	glfwSwapBuffers(m_Window);
}

class Slider : public UI::IWidgetGroup
{
public:
	Slider()
	{
		m_Root = std::make_shared<UI::Frame>();
		m_Root->Position = glm::vec4(0.5f, 0.0f, 0.5f, 0.0f);
		m_Root->Size = glm::vec4(0.3f, 0.0f, 0.1f, 0.0f);
		m_Root->Visible = false;
		m_Root->ClipsDescendants = false;

		m_Track = std::make_shared<UI::Frame>();
		m_Track->Position = glm::vec4(0.5f, 0.0f, 0.5f, 0.0f);
		m_Track->Size = glm::vec4(1.0f, 0.0f, 0.0f, 5.0f);
		m_Track->Color = glm::vec3(100.0f);
		m_Track->ZIndex = 2;
		m_Root->AddChild(m_Track);

		m_Slider = std::make_shared<UI::FrameButton>();
		m_Slider->Position = glm::vec4(0.5f, 0.0f, 0.5f, 0.0f);
		m_Slider->Size = glm::vec4(0.0f, 20.0f, 1.0f, 0.0f);
		m_Slider->Color = glm::vec3(160.0f);
		m_Slider->HoverColor = glm::vec3(150.0f);
		m_Slider->ClickColor = glm::vec3(130.0f);
		m_Root->AddChild(m_Slider);
	}

	UI::WidgetProperty<glm::vec4>& Position() { return m_Root->Position; }
	UI::WidgetProperty<glm::vec4>& Size() { return m_Root->Size; }
	UI::WidgetProperty<glm::vec2>& AnchorPoint() { return m_Root->AnchorPoint; }

	UI::WidgetProperty<glm::vec4>& SliderSize() { return m_Slider->Size; }

	UI::WidgetProperty<glm::vec3>& SliderColor() { return m_Slider->Color; }
	UI::WidgetProperty<glm::vec3>& TrackColor() { return m_Track->Color; }

	float GetValue() const { return m_Slider->Position->x; }

	void SetValue(float value)
	{
		float scale = std::min(1.0f, std::max(0.0f, value));
		m_Slider->Position->x = scale;
		m_Slider->Position.ManualTrigger();
	}

	virtual std::shared_ptr<UI::Widget> GetRoot() const override { return m_Root; }

protected:

	virtual void OnRootChanged(UI::Surface* root)
	{
		root->ConnectMouseButton([&](UI::MouseButtonArgs args)
			{
				m_IsClicked = args.Button == UI::MouseButton::Left ? 
					args.Target == m_Slider && args.Action == UI::InputAction::Press : m_IsClicked;
			}
		);

		root->ConnectMouseMove([&](UI::MouseMoveArgs args)
			{
				if (m_IsClicked)
					AdjustPosition(args.Position.x);
			}
		);
	}

private:
	bool m_IsClicked = false;

	std::shared_ptr<UI::Frame> m_Root;

	std::shared_ptr<UI::Frame> m_Track;
	std::shared_ptr<UI::FrameButton> m_Slider;

	void AdjustPosition(float mouseX)
	{
		glm::vec4 screenRect = m_Root->GetScreenRect();
		float adjustedX = mouseX - screenRect.x;
		float scale = std::min(1.0f, std::max(0.0f, adjustedX / (screenRect.z - screenRect.x)));
		m_Slider->Position->x = scale;
		m_Slider->Position.ManualTrigger();
	}
};

void Application::Run(GLFWwindow* window)
{
	m_Window = window;

	LoadResources();

	auto springButton = std::make_shared<UI::FrameButton>();
	springButton->Position = glm::vec4(0.5f, 0.0f, 0.2f, 0.0f);
	springButton->Size = glm::vec4(0.0f, 40.0f, 0.0f, 40.0f);
	springButton->Color = glm::vec3(0.0f, 255.0f, 0.0f);
	springButton->HoverColor = glm::vec3(128.0f, 255.0f, 128.0f);
	springButton->ClickColor = glm::vec3(192.0f, 255.0f, 192.0f);
	m_Surface.AddChild(springButton);

	auto spring = UI::Spring<glm::vec4, float>(&springButton->Position.Value(), springButton->Position.Value(), &m_Surface, 30.0, 4.0);
	spring.Enable();

	bool clicked = false;
	m_Surface.ConnectMouseButton([&](UI::MouseButtonArgs args)
		{ 
			clicked = args.Button == UI::MouseButton::Left ? 
				args.Action == UI::InputAction::Press && args.Target == springButton : clicked;
		}
	);

	m_Surface.ConnectMouseMove([&](UI::MouseMoveArgs args)
		{
			if (clicked)
				spring.Goal = glm::vec4(0.0f, args.Position.x, 0.0f, args.Position.y);
		}
	);

	auto button = std::make_shared<UI::FrameButton>();
	button->Position = glm::vec4(0.2f, 0.0f, 0.2f, 0.0f);
	button->Size = glm::vec4(0.2f, 0.0f, 0.2f, 0.0f);
	button->Color = glm::vec3(255.0f, 0.0f, 0.0f);
	button->AutoColor = false;
	button->ClipsDescendants = true;
	m_Surface.AddChild(button);

	button->ConnectMouseEnter([&](UI::MouseEnterArgs)
		{
			UI::AnimationScheduler::CreateAnimation<UI::EasingType::InQuad, glm::vec3, float>(
				&button->Color.Value(),
				glm::vec3(255.0f, 128.0f, 128.0f),
				&m_Surface,
				0.2, 0.0
			);
		}
	);

	button->ConnectMouseExit([&](UI::MouseExitArgs)
		{
			UI::AnimationScheduler::CreateAnimation<UI::EasingType::OutQuad, glm::vec3, float>(
				&button->Color.Value(),
				glm::vec3(255.0f, 0.0f, 0.0f),
				&m_Surface,
				0.2, 0.0
			);
		}
	);
	
	bool position = false;
	button->ConnectMouseButton([&](UI::MouseButtonArgs args)
		{
			if (args.Button == UI::MouseButton::Left && args.Action == UI::InputAction::Press)
			{
				UI::AnimationScheduler::CreateAnimation<UI::EasingType::OutBounce, glm::vec4, float>(
					&button->Position.Value(),
					position ? glm::vec4(0.2f, 0.0f, 0.2f, 0.0f) : glm::vec4(0.8f, 0.0f, 0.8f, 0.0f),
					&m_Surface,
					0.8, 0.0
				);

				UI::AnimationScheduler::CreateAnimation<UI::EasingType::OutQuad, glm::vec3, float>(
					&button->Color.Value(),
					glm::vec3(255.0f, 0.0f, 0.0f),
					&m_Surface,
					0.2, 0.0
				);

				position = !position;
			}
		}
	);

	TextureSpec spec = TextureSpec();
	spec.MagFilter = TextureMagFilter::Nearest;
	spec.WrapS = TextureWrapMode::ClampToBorder;
	spec.WrapT = TextureWrapMode::ClampToBorder;
	auto fontTexture = std::make_shared<Texture>("res/Fonts/default-font.png", spec);

	auto fontImage = std::make_shared<UI::TexturedFrame>();
	fontImage->Position = glm::vec4(0.5f, 0.0f, 0.5f, 0.0f);
	fontImage->Size = glm::vec4(1.0f, 0.0f, 1.0f, 0.0f);
	fontImage->ScaleType = UI::TextureScaleType::Fit;
	fontImage->SinksInput = false;
	fontImage->TextureObject = fontTexture;
	m_Surface.AddChild(fontImage);

	auto slider = std::make_shared<Slider>();
	slider->Size() = glm::vec4(0.3f, 0.0f, 0.0f, 30.0f);
	slider->SliderSize() = glm::vec4(0.0f, 10.0f, 1.0f, 0.0f);
	m_Surface.AddChild(slider);

	m_Surface.ConnectMouseScroll([&](UI::MouseScrollArgs args)
		{
			slider->SetValue(slider->GetValue() + args.Offset.y / 20.0f);
		}
	);

	auto text = std::make_shared<UI::TextLabel>();
	text->Position = glm::vec4(0.5f, 0.0f, 1.0f, 0.0f);
	text->AnchorPoint = glm::vec2(0.5, 1.0f);
	text->Size = glm::vec4(0.5f, 0.0f, 0.3f, 0.0f);
	text->Text = L"0.5";
	text->VerticalAlignment = UI::TextAlignY::Top;
	text->Scale = 5.0f;
	m_Surface.AddChild(text);

	int32_t frameNumber = 0;
	while (!glfwWindowShouldClose(window))
	{
		text->Color = hsv2rgb(glm::vec3(slider->GetValue() * 360.0f, 1.0f, 1.0f)) * 255.0f;
		text->Scale = slider->GetValue() * 9.0f + 1.0f;
		text->Text = std::to_wstring(slider->GetValue()).substr(0, 4);

		Draw();
		UI::AnimationScheduler::CleanupFinishedAnimations();

		frameNumber++;

		glfwPollEvents();
	}
}