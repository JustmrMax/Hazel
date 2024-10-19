#include "Hazel.h"

class SandBox2D : public Hazel::Layer
{
public:
	SandBox2D();
	virtual ~SandBox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDettach() override;

	virtual void OnUpdate(Hazel::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Hazel::Event& event) override;

private:
	Hazel::OrhographicCameraController m_CameraController;

	// YEAP temp
	Hazel::Ref<Hazel::VertexArray> m_SquareVA;
	Hazel::Ref<Hazel::Shader> m_FlatColorShader;

	glm::vec3 m_SquareColor = glm::vec3(0.3f, 1.0f, 0.5f);
};