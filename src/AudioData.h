#pragma once
#include <Windows.h>

namespace WASAPIAudio
{
	public ref class AudioDataReadyEventArgs sealed
	{
	internal:
		AudioDataReadyEventArgs(Platform::Array<int, 1>^ points, UINT32 size) :
			m_PointArray(points),
			m_Size(size)
		{};

		property Platform::Array<int, 1>^ Points
		{
			Platform::Array<int, 1>^ get() { return m_PointArray; }
		};

		property UINT32 Size
		{
			UINT32 get() { return m_Size; }
		};

	private:
		Platform::Array<int, 1>^     m_PointArray;
		UINT32                      m_Size;
	};

	// AudioDataReady delegate
	public delegate void AudioDataReadyHandler(Platform::Object^ sender, AudioDataReadyEventArgs^ e);

	// AudioDataReady Event
	public ref class AudioDataReadyEvent sealed
	{
	public:
		AudioDataReadyEvent() {};

	internal:
		void SendEvent(Object^ obj, Platform::Array<int, 1>^ points, UINT32 size)
		{
			AudioDataReadyEventArgs^ e = ref new AudioDataReadyEventArgs(points, size);
			AudioDataReady(obj, e);
		}

	public:
		static event AudioDataReadyHandler^    AudioDataReady;
	};
}