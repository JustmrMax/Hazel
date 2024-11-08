#include "hzpch.h"
#include "OrhographicCameraController.h"

#include "Core/Input.h"
#include "Core/KeyCodes.h"
#include "Core/Core.h"

namespace Hazel
{
	OrhographicCameraController::OrhographicCameraController(float aspectRation, bool canRotate)
		: m_Camera(-aspectRation * 1.0f, aspectRation * 1.0f, -1.0f, 1.0f),
		m_ZoomLevel(1.0f), m_AspectRation(aspectRation), m_CanRotate(canRotate)
	{
	}

	void OrhographicCameraController::OnUpdate(Timestep ts)
	{
		// Handling Camera movement

		// When we zoom out we slow down the speedmovement YEAP
		ts = ts * m_ZoomLevel;

		if (Input::IsKeyPressed(HZ_KEY_D))
			m_CameraPosition.x += m_TranslationMovementSpeed * ts;
		else if (Input::IsKeyPressed(HZ_KEY_A))
			m_CameraPosition.x -= m_TranslationMovementSpeed * ts;
			
		if (Input::IsKeyPressed(HZ_KEY_W))
			m_CameraPosition.y += m_TranslationMovementSpeed * ts;
		else if (Input::IsKeyPressed(HZ_KEY_S))
			m_CameraPosition.y -= m_TranslationMovementSpeed * ts;

		// Special cases (for now)
		if (Input::IsKeyPressed(HZ_KEY_Z))
			SetZoomLevel(1.0f);
			
		// Handling Rotation Camera
		if (m_CanRotate)
		{
			if (Input::IsKeyPressed(HZ_KEY_Q))
				m_CameraRotation += m_TranslationRotationSpeed * ts;
			else if (Input::IsKeyPressed(HZ_KEY_E))
				m_CameraRotation -= m_TranslationRotationSpeed * ts;
			else if (Input::IsKeyPressed(HZ_KEY_C))
				m_CameraRotation = 0.0f;

			m_Camera.SetRotation(m_CameraRotation);
		}
			
		m_Camera.SetPosition(m_CameraPosition);
	}

	void OrhographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrhographicCameraController::OnScrolledEvent));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OrhographicCameraController::OnResizedWindowEvent));
	}

	bool OrhographicCameraController::OnScrolledEvent(MouseScrolledEvent& e)
	{
		float zoomLevel = m_ZoomLevel - e.GetYOffset() * 0.25;
		
		SetZoomLevel(zoomLevel);
		return true;
	}

	void OrhographicCameraController::SetZoomLevel(float zoomLevel)
	{
		if (zoomLevel <= 0.5f) // YEAP to do : add consts
			m_ZoomLevel = 0.5f;
		else if (zoomLevel >= 3.0f)
			m_ZoomLevel = 3.0f;
		else
			m_ZoomLevel = zoomLevel;

		m_Camera.SetProjection(-m_AspectRation * m_ZoomLevel, m_AspectRation * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	bool OrhographicCameraController::OnResizedWindowEvent(WindowResizeEvent& e)
	{
		m_AspectRation = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRation * m_ZoomLevel, m_AspectRation * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return true;
	}
}