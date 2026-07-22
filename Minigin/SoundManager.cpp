#include "SoundManager.h"
#include <queue>
#include <iostream>
#include <thread>

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

class dae::SoundSystem::Impl
{
	std::queue<SoundRequest> m_SoundQueue{};
	std::vector<MIX_Audio*> m_SoundObjects{};

	MIX_Mixer* m_Mixer;
	MIX_Track* m_Track;

	float m_MasterVolume = 0.5f;
	bool m_IsMuted{false};

	void CheckQueue(); //function to avoid recursive form of PlayOldestSound
	void playOldestSound();

public:

	explicit Impl();
	~Impl();

	void AddToSoundQueue(const SoundRequest& soundRequest);
	MIX_Audio* GetSoundObjectById(int id);
	void AddToSoundObjects(const SoundObject& soundObject);

	void AddVolume(bool increase, float volume = 0.01f);
	void MuteVolume();
	bool GetIsMuted();
};

dae::SoundSystem::Impl::Impl()
{
	MIX_Init(); //can be called multiple times safely
	m_Mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
	m_Track = MIX_CreateTrack(m_Mixer);
}

dae::SoundSystem::Impl::~Impl()
{

	MIX_StopAllTracks(m_Mixer, 0);

	MIX_Quit();
}

void dae::SoundSystem::Impl::CheckQueue()
{
	//keep looping until the queue is empty
	while (true)
	{
		if (m_SoundQueue.empty())
		{
			return;
		}

		playOldestSound();
	}
}

void dae::SoundSystem::Impl::playOldestSound()
{
	SoundRequest sound = m_SoundQueue.back();

	MIX_Audio* audioclip = GetSoundObjectById(m_SoundQueue.front().Id);

	// !m_IsMuted returns 0 if muted, 1 if unmuted
	const float volume = m_MasterVolume * m_SoundQueue.front().Volume * !m_IsMuted;

	MIX_SetTrackAudio(m_Track, audioclip);
	MIX_SetTrackGain(m_Track, volume);
	MIX_PlayTrack(m_Track, 0);

	m_SoundQueue.pop();
}

void dae::SoundSystem::Impl::AddToSoundQueue(const SoundRequest& sound)
{
	m_SoundQueue.push(sound);

	std::thread myThread(&Impl::CheckQueue, this);
	myThread.detach(); //keep running until queue is empty
}

MIX_Audio* dae::SoundSystem::Impl::GetSoundObjectById(int id)
{
	if (m_SoundObjects[id] == nullptr)
	{
		std::cout << "requested sound id does not exist!";
	}
	return m_SoundObjects[id];
}

void dae::SoundSystem::Impl::AddToSoundObjects(const SoundObject& soundObject)
{
	if (m_SoundObjects.size() <= soundObject.Id)
	{
		m_SoundObjects.reserve(soundObject.Id + 1);
	}

	for (int index{ (int)m_SoundObjects.size() }; index < m_SoundObjects.capacity(); index++)
	{
		m_SoundObjects.emplace_back(nullptr);
	}

	MIX_Audio* audioclip = MIX_LoadAudio(m_Mixer, soundObject.Path, false);
	m_SoundObjects[soundObject.Id] = audioclip;

}

void dae::SoundSystem::Impl::AddVolume(bool increase, float volume)
{
	if (increase)
	{
		m_MasterVolume += volume;
	}
	else
	{
		m_MasterVolume -= volume;
	}
}

void dae::SoundSystem::Impl::MuteVolume()
{
	m_IsMuted = !m_IsMuted;
}

bool dae::SoundSystem::Impl::GetIsMuted()
{
	return m_IsMuted;
}

dae::SoundSystem::SoundSystem()
	: m_pImpl{ new Impl{} }
{
}

dae::SoundSystem::~SoundSystem()
{
	delete m_pImpl;
}

void dae::SoundSystem::play(const SoundRequest& sound)
{
	m_pImpl->AddToSoundQueue(sound);
}

void dae::SoundSystem::RegisterSound(const SoundObject& soundObject)
{
	m_pImpl->AddToSoundObjects(soundObject);
}

void dae::SoundSystem::AddVolume(bool increase, float volume)
{
	m_pImpl->AddVolume(increase, volume);
}

void dae::SoundSystem::MuteVolume()
{
	m_pImpl->MuteVolume();
}

bool dae::SoundSystem::GetIsMuted()
{
	return m_pImpl->GetIsMuted();
}


std::unique_ptr<dae::SoundSystem> dae::ServiceLocator::ServiceInstance = nullptr;

dae::SoundSystem& dae::ServiceLocator::GetService()
{
	return *ServiceInstance;
}

void dae::ServiceLocator::RegisterService(std::unique_ptr<dae::SoundSystem>&& service)
{
	ServiceInstance = std::move(service);
}
