#pragma once
#include "pch.h"
#include <Fmod/fmod.h>
#include <map>


class Framework;
class ResourceManager;

class AudioManager
{
public:

	Framework* framework;
	ResourceManager* res;

    bool Initialize(Framework* framework);

    /// <summary>
    /// 오디오 파일을 불러와 map에 저장합니다.
    /// </summary>
    /// <param name="audioName">map에 저장될 이름입니다.</param>
    /// <param name="audioFilePath">오디오 파일의 경로입니다.</param>
    void LoadAudio(const char* audioName, const char* audioFilePath);

    /// <summary>
    /// 오디오를 재생합니다.
    /// </summary>
    /// <param name="audioName">map에서 불러올 오디오의 이름입니다.</param>
    void PlayAudio(const char* audioName);

    /// <summary>
    /// 오디오 재생을 중단합니다.
    /// </summary>
    /// <param name="audioName">map에서 불러올 오디오의 이름입니다.</param>
    void StopAudio(const char* audioName);

    /// <summary>
    /// 오디오의 볼륨을 조절합니다.
    /// </summary>
    /// <param name="audioName">map에서 불러올 오디오의 이름입니다.</param>
    /// <param name="volume">볼륨값입니다. 1.0f가 최대값이며 0.0f는 음소거입니다. </param>
    void SetVolume(const char* audioName, float volume);

private:
    FMOD::System* m_system;
    std::map<std::string, FMOD::Sound*> m_sound;
    std::map<std::string, FMOD::Channel*> m_channel;
	
}; 

