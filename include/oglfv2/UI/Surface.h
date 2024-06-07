#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <array>
#include <optional>

#include "oglfv2/UI/IWidgetContainer.h"
#include "oglfv2/UI/InputEvents.h"

#include "oglfv2/Renderer/Framebuffer.h"
#include "oglfv2/Renderer/Texture.h"

#include "Arrowhead/Events.h"

class Shader;
struct GLFWwindow;

namespace UI
{
	class Surface : public IWidgetContainer
	{
	public:
		Surface();
		Surface(glm::vec2 resolution);

		// IWidgetContainer functionality implementation

		virtual inline IWidgetContainer* GetParent() const override { return nullptr; }
		virtual Surface* GetRootSurface() override { return this; }
		virtual inline bool IsSurface() const override { return true; }

		virtual void AddChild(std::shared_ptr<Widget> widget) override;
		using IWidgetContainer::AddChild; // For the widget group version. Not sure why it doesn't work automatically

		virtual glm::vec4 TransformPosition(const glm::vec4& childSpace, const glm::vec2& resolution) const override { return childSpace; }
		virtual glm::vec4 TransformSize(const glm::vec4& childSpace, const glm::vec2& resolution) const override { return childSpace; }

		// Update gets called by SetResolution so calling it manually shouldn't typically be necessary
		void Update();

		void Render();
		bool ShouldRender = true;

		Texture* GetColorBuffer();

		// Resizing functions

		inline glm::vec2 GetResolution() const { return m_Resolution; }
		void SetResolution(glm::vec2 resolution);
		

		// Input passthrough functions:
		// It works this way because the widget class is pretty flexible and isn't limited to drawing directly on the window.
		// A surface doesn't know where it is and may require a conversion from screen coordinates. So connecting directly
		// to GLFW callbacks doesn't make sense in this case. An added benifit is that to lock input you just stop calling
		// these functions as well as input not being tied to GLFW

		bool UpdateCursorPos(glm::vec2 pos);
		bool UpdateMouseButtonState(MouseButton button, InputAction action);

		inline void UpdateScrollInput(glm::vec2 offset)
		{ m_MouseScrollCallbacks.Call({ false, nullptr, m_MousePos, offset }); }

		inline void UpdateKeyInput(Key key, InputAction action) 
		{ m_KeyInputCallbacks.Call({ false, nullptr, key, action }); }

		inline void UpdateCharInput(wchar_t value) 
		{ m_CharInputCallbacks.Call({ false, nullptr, value }); }

		// Given the comment above I will give it 'native' support for GLFW which
		// will make it work seamlessly with my GLFW event wrapper

		std::function<void(GLFWwindow*, double, double)> GLFWMouseCursorPosCallback();
		std::function<void(GLFWwindow*, int, int, int)> GLFWMouseButtonCallback();
		std::function<void(GLFWwindow*, double, double)> GLFWMouseScrollCallback();
		std::function<void(GLFWwindow*, int, int, int, int)> GLFWKeyCallback();
		std::function<void(GLFWwindow*, unsigned int)> GLFWCharCallback();

		// Input callback connection functions

		inline std::shared_ptr<arwh::EventConnection<MouseMoveArgs>> ConnectMouseMove(std::function<void(MouseMoveArgs)> callback)
		{ return m_MouseMoveCallbacks.Connect(callback); }

		inline std::shared_ptr<arwh::EventConnection<MouseButtonArgs>> ConnectMouseButton(std::function<void(MouseButtonArgs)> callback)
		{ return m_MouseButtonCallbacks.Connect(callback); }

		inline std::shared_ptr<arwh::EventConnection<MouseScrollArgs>> ConnectMouseScroll(std::function<void(MouseScrollArgs)> callback)
		{ return m_MouseScrollCallbacks.Connect(callback); }

		inline std::shared_ptr<arwh::EventConnection<KeyUpdateArgs>> ConnectKeyInput(std::function<void(KeyUpdateArgs)> callback)
		{ return m_KeyInputCallbacks.Connect(callback); }

		inline std::shared_ptr<arwh::EventConnection<CharInputArgs>> ConnectCharInput(std::function<void(CharInputArgs)> callback)
		{ return m_CharInputCallbacks.Connect(callback); }

		// Timed update interface
		inline static void SetTimeFunction(std::function<double()> func) { s_TimeFunction = func; }
		inline double GetTime() { s_TimeFunction.has_value() ? s_TimeFunction->operator()() : 0.0; }

		inline std::shared_ptr<arwh::EventConnection<double>> ConnectTimedUpdate(std::function<void(double)> callback)
		{ return m_TimedUpdateCallbacks.Connect(callback); }

	private:
		glm::vec2 m_Resolution;

		std::unique_ptr<Framebuffer> m_Framebuffer;
		void CreateFramebuffer();

		void UpdateRecursive(std::shared_ptr<Widget> widget);

		// Render algorithms

		glm::vec4 RenderWidget(std::shared_ptr<Widget> widget, glm::vec4 bounds);
		void RenderRecursive(std::shared_ptr<Widget> widget, glm::vec4 bounds);

		// Widget shader storage and retrieval

		static std::shared_ptr<Shader> GetWidgetShader(const char* shaderPath);
		inline static std::unordered_map<const char*, std::shared_ptr<Shader>> s_WidgetShaders =
			std::unordered_map<const char*, std::shared_ptr<Shader>>();

		// Timed update related values

		arwh::CallbackList<double> m_TimedUpdateCallbacks;
		double m_LastUpdate = -1.0f;
		inline static std::optional<std::function<double()>> s_TimeFunction = std::nullopt;

		// Input

		std::shared_ptr<Widget> FindWidget(glm::vec2 pos, std::shared_ptr<IWidgetContainer> parent = nullptr);

		glm::vec2 m_MousePos = { 0.0f, 0.0f };
		std::array<bool, 8> m_MouseButtonsPressed = { false, false, false, false, false, false, false, false };

		arwh::CallbackList<MouseMoveArgs> m_MouseMoveCallbacks;
		arwh::CallbackList<MouseButtonArgs> m_MouseButtonCallbacks;
		arwh::CallbackList<MouseScrollArgs> m_MouseScrollCallbacks;
		arwh::CallbackList<KeyUpdateArgs> m_KeyInputCallbacks;
		arwh::CallbackList<CharInputArgs> m_CharInputCallbacks;
	};
}