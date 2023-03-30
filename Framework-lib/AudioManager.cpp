#include "pch.h"
#include "AudioManager.h"
#include "Framework.h"

//오디오 매니저 초기화
bool AudioManager::Initialize(Framework* framework)
{

	this->framework = framework;
	this->res = &framework->resourceManager;


	FMOD::System_Create(&m_system);
	m_system->init(32, FMOD_INIT_NORMAL, nullptr);

	return true;
}


void AudioManager::LoadAudio(const char* audioName, const char* audioFilePath) {
	FMOD::Sound* sound;
	m_system->createSound(audioFilePath, FMOD_DEFAULT, nullptr, &sound);
	m_sound[audioName] = sound;
	m_channel[audioName]->setVolume(1.0f);
}


void AudioManager::PlayAudio(const char* audioName) {
	m_system->playSound(m_sound[audioName], nullptr, false, &m_channel[audioName]);
}


void AudioManager::StopAudio(const char* audioName) {
	m_channel[audioName]->stop();
}


void AudioManager::SetVolume(const char* audioName, float volume) {
	m_channel[audioName]->setVolume(volume);
}