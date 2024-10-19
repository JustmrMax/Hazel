#include "RenderCommand.h"

#include "Core/OrhographicCamera.h"
#include "Shader.h"

namespace Hazel
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrhographicCamera& camera);
		static void EndScene();

		static void DrawQuad(glm::vec2& position, glm::vec2& size, glm::vec4& color);
		static void DrawQuad(glm::vec3& position, glm::vec2& size, glm::vec4& color);
	};
}