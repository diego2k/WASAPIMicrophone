//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace App1;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// Don't forget  to enable the Microphone and Music Library access in the manifest
MainPage::MainPage() :
	m_StateChangedEvent(nullptr),
	m_spCapture(nullptr)
{
	InitializeComponent();

	// Initialize MF
	HRESULT hr = MFStartup(MF_VERSION, MFSTARTUP_LITE);
	if (SUCCEEDED(hr))
	{
		//m_IsMFLoaded = true;
	}
	else
	{
		ThrowIfFailed(hr);
	}

}

void App1::MainPage::OnDeviceStateChange(Object^ sender, DeviceStateChangedEventArgs^ e)
{
	// Handle state specific messages
	switch (e->State)
	{
	case DeviceState::DeviceStateInitialized:
		m_spCapture->StartCaptureAsync();
		break;

	case DeviceState::DeviceStateCapturing:
		break;

	case DeviceState::DeviceStateDiscontinuity:
		break;

	case DeviceState::DeviceStateFlushing:
		break;

	case DeviceState::DeviceStateStopped:
		// For the stopped state, completely tear down the audio device
		m_spCapture = nullptr;

		if (m_deviceStateChangeToken.Value != 0)
		{
			m_StateChangedEvent->StateChangedEvent -= m_deviceStateChangeToken;
			m_StateChangedEvent = nullptr;
			m_deviceStateChangeToken.Value = 0;
		}
		break;

	case DeviceState::DeviceStateInError:
		HRESULT hr = e->hr;

		if (m_deviceStateChangeToken.Value != 0)
		{
			m_StateChangedEvent->StateChangedEvent -= m_deviceStateChangeToken;
			m_StateChangedEvent = nullptr;
			m_deviceStateChangeToken.Value = 0;
		}

		m_spCapture = nullptr;

		wchar_t hrVal[11];
		swprintf_s(hrVal, 11, L"0x%08x\0", hr);
		String^ strHRVal = ref new String(hrVal);

		// Specifically handle a couple of known errors
		switch (hr)
		{
		case __HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND):
			break;

		case AUDCLNT_E_RESOURCES_INVALIDATED:
			break;

		case E_ACCESSDENIED:
			break;

		default:
			break;
		}
	}
}

void App1::MainPage::OnAudioDataReady(Object^ sender, AudioDataReadyEventArgs^ e)
{

}

void App1::MainPage::Test_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	HRESULT hr = S_OK;

	if (m_spCapture)
	{
		m_spCapture = nullptr;
	}

	// Create a new WASAPI capture instance
	m_spCapture = Make<WASAPICapture>();

	if (nullptr == m_spCapture)
	{
		OnDeviceStateChange(this, ref new DeviceStateChangedEventArgs(DeviceState::DeviceStateInError, E_OUTOFMEMORY));
		return;
	}

	// Get a pointer to the device event interface
	m_StateChangedEvent = m_spCapture->GetDeviceStateEvent();

	if (nullptr == m_StateChangedEvent)
	{
		OnDeviceStateChange(this, ref new DeviceStateChangedEventArgs(DeviceState::DeviceStateInError, E_FAIL));
		return;
	}

	// Register for events
	m_deviceStateChangeToken = m_StateChangedEvent->StateChangedEvent += ref new DeviceStateChangedHandler(this, &App1::MainPage::OnDeviceStateChange);
	m_audioDataReadyToken = AudioDataReadyEvent::AudioDataReady += ref new AudioDataReadyHandler(this, &App1::MainPage::OnAudioDataReady);

	// Reset discontinuity counter
	//m_DiscontinuityCount = 0;

	// Configure user based properties
	CAPTUREDEVICEPROPS props;

	auto m_IsLowLatency = static_cast<Platform::Boolean>(true);
	props.IsLowLatency = m_IsLowLatency;
	m_spCapture->SetProperties(props);

	// Perform the initialization
	m_spCapture->InitializeAudioDeviceAsync();
}
