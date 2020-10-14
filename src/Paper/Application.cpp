#include "p_pch.h"

#include "Application.h"
#include "Log.h"
#include <Paper/Events/ApplicationEvent.h>
#include "Paper/Input.h"

#include "Renderer/Renderer.h"

namespace Paper {

	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		// TODO: Only 1 instance of application is possible right now. Change it!
		PAPER_CORE_ASSERT(!s_Instance, "Paper Application already Exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(PAPER_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();

		m_Running = true;

		float vertecies[3*7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		m_VertexArray.reset(VertexArray::Create());

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertecies, sizeof(vertecies)));
		vertexBuffer->Bind();

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));
		indexBuffer->Bind();

		m_VertexArray->SetIndexBuffer(indexBuffer);


		m_SquareVAO.reset(VertexArray::Create());

		float square_vertecies[3*4] = {
			-0.75f, -0.75f, 0.0f,
			0.75f, -0.75f, 0.0f,
			0.75f, 0.75f, 0.0f,
			-0.75f, 0.75f, 0.0f
		};

		std::shared_ptr<VertexBuffer> squareVBO;
		squareVBO.reset(VertexBuffer::Create(square_vertecies, sizeof(square_vertecies)));
		BufferLayout SquareBufferLayout = {
			{ ShaderDataType::Float3, "a_Position" }
		};
		squareVBO->SetLayout(SquareBufferLayout);

		m_SquareVAO->AddVertexBuffer(squareVBO);

		uint32_t square_indices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIBO;
		squareIBO.reset(IndexBuffer::Create(square_indices, sizeof(square_indices)/sizeof(uint32_t)));
		m_SquareVAO->SetIndexBuffer(squareIBO);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_VP;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_VP * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);
		m_Shader->Bind();

		std::string vertexSrc2 = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			out vec3 v_Position;

			uniform mat4 u_VP;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_VP * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc2 = R"(
			#version 330 core

			out vec4 color;
			in vec3 v_Position;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				// color = vec4(0.0, 0.0, 1.0, 1.0);
			}
		)";

		m_SquareShader = std::make_unique<Shader>(vertexSrc2, fragmentSrc2);
		m_SquareShader->Bind();
	}

	Application::~Application()
	{

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(PAPER_BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.IsHandled())
				break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;

		return true;
	}

	void Application::Run()
	{
		m_Camera.SetPosition({ 0.5f, 0.5f, 0.0f });
		m_Camera.SetRotation(45.0f);

		while (m_Running)
		{
			RenderCommand::SetClearColor({0.2f, 0.2f, 0.2f, 1.0f});
			RenderCommand::Clear();

			Renderer::BeginScene(m_Camera);
			{
				Renderer::Submit(m_SquareShader, m_SquareVAO);
				Renderer::Submit(m_Shader, m_VertexArray);
			}
			Renderer::EndScene();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRenderer();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}
}