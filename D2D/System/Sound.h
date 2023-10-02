#pragma once

#define SOUND Sound::Get()
class Sound
{
	DECLARE_SINGLETON(Sound);

	struct SoundNode
	{
		FMOD::Sound* sound = nullptr;
		FMOD::Channel* channel = nullptr;
		wstring path = L"";
		float channelVolume = 0.0f;
		bool bLoop = false;
		bool bMute = false;
		bool canMulti = false;
	};

public:
	bool AddSound(const string& key, const wstring& path, bool bLoop = false, bool canMultiPlay = false);
	bool DeleteSound(const string& key);

	void Play(const string& key);
	void Stop(const string& key);
	void Pause(const string& key);
	void Resume(const string& key);

	void SetVolume(const string& key, float channelVolume);
	void SetMasterVolume(float volume);

	void Update();

//	void GUI(const string& key);

private:
	void ChangeSoundFunc(const string& key, const wstring& path = L"");

private:
	map<string, shared_ptr<SoundNode>> soundList;
	FMOD::System* system = nullptr;
	float _volume = 0.2f; //½Ã½ºÅÛ ÀüÃ¼ À½¾Ç º¼·ý


};