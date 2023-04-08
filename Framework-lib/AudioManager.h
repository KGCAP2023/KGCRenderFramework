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
    /// ����� ������ �ҷ��� map�� �����մϴ�.
    /// </summary>
    /// <param name="audioName">map�� ����� �̸��Դϴ�.</param>
    /// <param name="audioFilePath">����� ������ ����Դϴ�.</param>
    virtual void LoadAudio(const char* audioName, const char* audioFilePath) {};

    /// <summary>
    /// �ҷ��� ������� �����մϴ�.
    /// </summary>
    /// <param name="audioName">������ ����� �̸��Դϴ�.</param>
    virtual void DeleteAudio(const char* audioName) {};

    

    /// <summary>
    /// ������� ����մϴ�.
    /// </summary>
    /// <param name="audioName">����� ������� �̸��Դϴ�.</param>
    virtual void PlayAudio(const char* audioName) {};

    /// <summary>
    /// ����� ����� �����մϴ�.
    /// </summary>
    /// <param name="audioName">������ ������� �̸��Դϴ�.</param>
    virtual void StopAudio(const char* audioName) {};


    /// <summary>
    /// ����� ����� �Ͻ������մϴ�.
    /// </summary>
    /// <param name="audioName">�Ͻ� ������ ������� �̸��Դϴ�.</param>
    virtual void PauseAudio(const char* audioName) {};
    


    /// <summary>
    /// ����� ����� �Ͻ������� �����մϴ�.
    /// </summary>
    /// <param name="audioName">������ ������� �̸��Դϴ�.</param>
    virtual void ResumeAudio(const char* audioName) {};




    /// <summary>
    /// ������� ������ �����մϴ�. �� �Լ��� PlayAudio �Լ� ������ ����ؾ� ���� �۵� �մϴ�.
    /// </summary>
    /// <param name="audioName">map���� �ҷ��� ������� �̸��Դϴ�.</param>
    /// <param name="volume">�������Դϴ�. 1.0f�� �ִ밪�̸� 0.0f�� ���Ұ��Դϴ�. </param>
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

    //������
    Framework* framework;
    ResourceManager* res;

private:
    FMOD::System* m_system;
    std::map<std::string, FMOD::Sound*> m_sound;
    std::map<std::string, FMOD::Channel*> m_channel;
};
