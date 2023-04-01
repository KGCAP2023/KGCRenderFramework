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
    /// �ҷ��� ������� �����մϴ�.
    /// </summary>
    /// <param name="audioName">������ ����� �̸��Դϴ�.</param>
    void DeleteAudio(const char* audioName);

    

    /// <summary>
    /// ������� �Էµ� �ð� ���� ���� �����Ǿ� ����մϴ�. ������ ms�Դϴ�.
    /// </summary>
    /// <param name="audioName">����� ������� �̸��Դϴ�.</param>
    /// <param name="frontdelay">��� �� ������ �ð��Դϴ�.</param>
    /// <param name="backdelay">��� �� ������ �ð��Դϴ�.</param>
    void PlayAudio(const char* audioName, const long frontdelay, const long backdelay);

    /// <summary>
    /// ����� ����� �����մϴ�.
    /// </summary>
    /// <param name="audioName">������ ������� �̸��Դϴ�.</param>
    /// <param name="frontdelay">��� �� ������ �ð��Դϴ�.</param>
    /// <param name="backdelay">��� �� ������ �ð��Դϴ�.</param>
    void StopAudio(const char* audioName, const long frontdelay, const long backdelay);


    /// <summary>
    /// ����� ����� �Ͻ������մϴ�.
    /// </summary>
    /// <param name="audioName">�Ͻ� ������ ������� �̸��Դϴ�.</param>
    /// <param name="frontdelay">��� �� ������ �ð��Դϴ�.</param>
    /// <param name="backdelay">��� �� ������ �ð��Դϴ�.</param>
    void PauseAudio(const char* audioName, const long frontdelay, const long backdelay);
    


    /// <summary>
    /// ����� ����� �Ͻ������� �����մϴ�.
    /// </summary>
    /// <param name="audioName">������ ������� �̸��Դϴ�.</param>
    /// <param name="frontdelay">��� �� ������ �ð��Դϴ�.</param>
    /// <param name="backdelay">��� �� ������ �ð��Դϴ�.</param>
    void ResumeAudio(const char* audioName, const long frontdelay, const long backdelay);




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

