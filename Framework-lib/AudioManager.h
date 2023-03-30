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
    /// ����� ������ �ҷ��� map�� �����մϴ�.
    /// </summary>
    /// <param name="audioName">map�� ����� �̸��Դϴ�.</param>
    /// <param name="audioFilePath">����� ������ ����Դϴ�.</param>
    void LoadAudio(const char* audioName, const char* audioFilePath);

    /// <summary>
    /// ������� ����մϴ�.
    /// </summary>
    /// <param name="audioName">map���� �ҷ��� ������� �̸��Դϴ�.</param>
    void PlayAudio(const char* audioName);

    /// <summary>
    /// ����� ����� �ߴ��մϴ�.
    /// </summary>
    /// <param name="audioName">map���� �ҷ��� ������� �̸��Դϴ�.</param>
    void StopAudio(const char* audioName);

    /// <summary>
    /// ������� ������ �����մϴ�.
    /// </summary>
    /// <param name="audioName">map���� �ҷ��� ������� �̸��Դϴ�.</param>
    /// <param name="volume">�������Դϴ�. 1.0f�� �ִ밪�̸� 0.0f�� ���Ұ��Դϴ�. </param>
    void SetVolume(const char* audioName, float volume);

private:
    FMOD::System* m_system;
    std::map<std::string, FMOD::Sound*> m_sound;
    std::map<std::string, FMOD::Channel*> m_channel;
	
}; 

