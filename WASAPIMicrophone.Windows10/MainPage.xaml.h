//*********************************************************
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
// This code is based on: 
// https://github.com/Microsoft/Windows-universal-samples.git
// Copyright (c) Microsoft. All rights reserved.
//*********************************************************

#pragma once

#include "MainPage.g.h"
#include "DeviceState.h"
#include "AudioData.h"
#include "WASAPICapture.h"

using namespace WASAPIAudio;

namespace App1
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class MainPage sealed
	{
	public:
		MainPage();

	private:
		// Handlers
		void OnDeviceStateChange(Object^ sender, DeviceStateChangedEventArgs^ e);
		void OnAudioDataReady(Object^ sender, AudioDataReadyEventArgs^ e);

	private:
		void Start_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Stop_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void Finish_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		Windows::Foundation::EventRegistrationToken     m_deviceStateChangeToken;
		Windows::Foundation::EventRegistrationToken     m_audioDataReadyToken;

		DeviceStateChangedEvent^    m_StateChangedEvent;
		ComPtr<WASAPICapture>       m_spCapture;
    };
}
