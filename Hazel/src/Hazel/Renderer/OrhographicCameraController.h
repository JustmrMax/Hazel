#include "Core/OrhographicCamera.h"

#include "Core/Timestep.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"

namespace Hazel
{
	class OrhographicCameraController
	{
	public:
		OrhographicCameraController(float aspectRation, bool canRotate = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		inline OrhographicCamera& GetCamera() { return m_Camera; }
		inline const OrhographicCamera& GetCamera() const { return m_Camera; }

	private:
		bool OnScrolledEvent(MouseScrolledEvent& e);
		bool OnResizedWindowEvent(WindowResizeEvent& e);

	private:
		OrhographicCamera m_Camera;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_TranslationMovementSpeed = 5.0f;
		float m_TranslationRotationSpeed = 180.0f; // in degrees
		float m_AspectRation;
		float m_ZoomLevel;
		bool m_CanRotate;
	};
}