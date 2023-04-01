#include "pch.h"
#include "AudioManager.h"
#include "Framework.h"

//오디오 매니저 초기화
bool AudioManager::Initialize(Framework* framework)
{

	this->framework = framework;
	this->res = &framework->resourceManager;


	FMOD::System_Create(&this->m_system);
	this->m_system->init(32, FMOD_INIT_NORMAL, nullptr);

	return true;
}


void AudioManager::LoadAudio(const char* audioName, const char* audioFilePath) 
{
	if (this->m_sound.find(audioName) == this->m_sound.end())
	{
		FMOD::Sound* sound;
		this->m_system->createSound(audioFilePath, FMOD_DEFAULT, nullptr, &sound);
		this->m_sound.insert(std::make_pair<>(audioName, sound));
		this->m_channel[audioName]->setVolume(1.0f);
	}
	else
		std::cout << "Audio Name already exist" << std::endl;
}


void AudioManager::DeleteAudio(const char* audioName)
{
	this->m_sound.erase(audioName);
}


void AudioManager::PlayAudio(const char* audioName) 
{
	this->m_system->playSound(m_sound[audioName], nullptr, false, &m_channel[audioName]);
}


void AudioManager::PlayAudio(const char* audioName, const long frontdelay, const long backdelay)
{
	std::thread t([=] {
		std::this_thread::sleep_for(std::chrono::milliseconds(frontdelay));
		this->PlayAudio(audioName);
		std::cout << "sound!" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(backdelay));
		});

	t.detach();

}


void AudioManager::StopAudio(const char* audioName)
{
	this->m_channel[audioName]->stop();
}

void AudioManager::PauseAudio(const char* audioName)
{
	this->m_channel[audioName]->setPaused(true);
}

void AudioManager::PauseAll()
{
	this->m_system->mixerSuspend();
}



void AudioManager::ResumeAudio(const char* audioName)
{
	this->m_channel[audioName]->setPaused(false);
}


void AudioManager::ResumeAll()
{
	this->m_system->mixerResume();
}

void AudioManager::SetVolume(const char* audioName, float volume)
{
	this->m_channel[audioName]->setVolume(volume);
}