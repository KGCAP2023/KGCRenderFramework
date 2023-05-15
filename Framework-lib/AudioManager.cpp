#include "pch.h"
#include "AudioManager.h"
#include "Framework.h"



//잠재적 오류사항
// 오디오 33개 동시 재생시 32번째 재생한 오디오 pop and 33번째가 push 됨(스택)
// 
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
	if (_access(audioFilePath, 0) != -1) 
	{
		if (res->m_sound.find(audioName) == res->m_sound.end())
		{
			FMOD::Sound* sound;
				this->m_system->createSound(audioFilePath, FMOD_DEFAULT, nullptr, &sound);
				res->m_sound.insert(std::make_pair<>(audioName, sound));
				this->m_channel[audioName]->setVolume(1.0f);
		}
		else
			std::cout << "Audio Name already exist" << std::endl;
	}
	else 
		std::cout << "Audio File NOT exist" << std::endl;
}


void AudioManager::DeleteAudio(const char* audioName)
{
	if(res->m_sound.find(audioName) != res->m_sound.end())
		res->m_sound.erase(audioName);
}




void AudioManager::PlayAudio(const char* audioName)
{
	if (res->m_sound.find(audioName) != res->m_sound.end())
		this->m_system->playSound(res->m_sound[audioName], nullptr, false, &m_channel[audioName]);
}


void AudioManager::StopAudio(const char* audioName)
{
	if(this->m_channel.find(audioName) != this->m_channel.end())
		this->m_channel[audioName]->stop();
}

void AudioManager::PauseAudio(const char* audioName)
{
	if (this->m_channel.find(audioName) != this->m_channel.end())
		this->m_channel[audioName]->setPaused(true);
}




void AudioManager::ResumeAudio(const char* audioName)
{
	if (this->m_channel.find(audioName) != this->m_channel.end())
		this->m_channel[audioName]->setPaused(false);
}



void AudioManager::SetVolume(const char* audioName, float volume)
{
	if (this->m_channel.find(audioName) != this->m_channel.end())
		this->m_channel[audioName]->setVolume(volume);
}