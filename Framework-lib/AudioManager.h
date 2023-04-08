#pragma once
#include "pch.h"
#include <Fmod/fmod.h>
#include <map>


class Framework;
class ResourceManager;

class IAudioManager
{
public:


    virtual bool Initialize(Framework* framework) { return false; };

    /// <summary>
    /// 오디오 파일을 불러와 map에 저장합니다.
    /// </summary>
    /// <param name="audioName">map에 저장될 이름입니다.</param>
    /// <param name="audioFilePath">오디오 파일의 경로입니다.</param>
    virtual void LoadAudio(const char* audioName, const char* audioFilePath) {};

    /// <summary>
    /// 불러온 오디오를 삭제합니다.
    /// </summary>
    /// <param name="audioName">삭제할 오디오 이름입니다.</param>
    virtual void DeleteAudio(const char* audioName) {};

    

    /// <summary>
    /// 오디오를 재생합니다.
    /// </summary>
    /// <param name="audioName">재생할 오디오의 이름입니다.</param>
    virtual void PlayAudio(const char* audioName) {};

    /// <summary>
    /// 오디오 재생을 중지합니다.
    /// </summary>
    /// <param name="audioName">중지할 오디오의 이름입니다.</param>
    virtual void StopAudio(const char* audioName) {};


    /// <summary>
    /// 오디오 재생을 일시중지합니다.
    /// </summary>
    /// <param name="audioName">일시 중지할 오디오의 이름입니다.</param>
    virtual void PauseAudio(const char* audioName) {};
    


    /// <summary>
    /// 오디오 재생을 일시중지를 해제합니다.
    /// </summary>
    /// <param name="audioName">해제할 오디오의 이름입니다.</param>
    virtual void ResumeAudio(const char* audioName) {};




    /// <summary>
    /// 오디오의 볼륨을 조절합니다. 이 함수는 PlayAudio 함수 다음에 사용해야 정상 작동 합니다.
    /// </summary>
    /// <param name="audioName">map에서 불러올 오디오의 이름입니다.</param>
    /// <param name="volume">볼륨값입니다. 1.0f가 최대값이며 0.0f는 음소거입니다. </param>
    virtual void SetVolume(const char* audioName, float volume) {};


	
}; 

class AudioManager : public IAudioManager {
public:
    virtual bool Initialize(Framework* framework) override;
    virtual void LoadAudio(const char* audioName, const char* audioFilePath) override;
    virtual void DeleteAudio(const char* audioName) override;
    virtual void PlayAudio(const char* audioName) override;
    virtual void StopAudio(const char* audioName) override;
    virtual void PauseAudio(const char* audioName) override;
    virtual void ResumeAudio(const char* audioName) override;
    virtual void SetVolume(const char* audioName, float volume) override;

    //의존성
    Framework* framework;
    ResourceManager* res;

private:
    FMOD::System* m_system;
    std::map<std::string, FMOD::Sound*> m_sound;
    std::map<std::string, FMOD::Channel*> m_channel;
};
