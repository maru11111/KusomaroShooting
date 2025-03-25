#pragma once

enum class AudioType {
	BGM,
	SE,
};

enum class PlayType {
	Play,
	PlayOneShot
};

struct AudioConfig {
	AudioType audioType;
	PlayType playType;
};

//シングルトンにしてみた
class AudioManager {
public:
	//コンストラクタの代わり
	static void Init() {
		if (instance != nullptr) return;
		instance = new AudioManager();
	}

	//インスタンスのゲッター
	static AudioManager* Instance() {
		return instance;
	}

	//audioの登録
	void setAudio(String name, String path, AudioType AudioType) {
		if(AudioType==AudioType::BGM)AudioAsset::Register(name, path, Loop::Yes);
		else AudioAsset::Register(name, path);
		AudioAsset::LoadAsync(name);
		audioConfigs[name].audioType = AudioType;
		if(AudioType == AudioType::BGM)audioConfigs[name].playType=PlayType::Play;
	}
	// + SEをループするかどうか
	void setAudio(String name, String path, AudioType AudioType, Loop loop) {
		if (AudioType == AudioType::BGM)AudioAsset::Register(name, path, Loop::Yes);
		else AudioAsset::Register(name, path, loop);
		AudioAsset::LoadAsync(name);
		audioConfigs[name].audioType = AudioType;
	}

	//audioを再生
	//BGMかSEかによって異なる音量で再生
	//(BGMはつねにplay)
	void playOneShot(String name) {
		switch (audioConfigs[name].audioType) {
		case AudioType::SE:
			audioConfigs[name].playType = PlayType::PlayOneShot;
			AudioAsset(name).playOneShot(seVolume);
			break;
		case AudioType::BGM:
			//bgmのループ開始位置
			 uint64 startSample;
			if (name == U"MidBoss") {
				startSample = (uint64)(39.724 * 44100);
			}
			else {
				throw Error{ U"ループ位置が指定されていない、またはSEが正しく再生されていません。" };
			}
			//AudioAsset(name).seekSamples((uint64)66.207 * 44100);
			AudioAsset(name).setLoopPoint(startSample);
			AudioAsset(name).setVolume(bgmVolume).play();
			currentBGMName = name;
			break;
		}
	}
	void play(String name) {
		switch (audioConfigs[name].audioType) {
		case AudioType::SE:
			audioConfigs[name].playType = PlayType::Play;
			AudioAsset(name).setVolume(seVolume).play();
			break;
		case AudioType::BGM:
			//bgmのループ開始位置
			uint64 startSample;
			if (name == U"MidBoss") {
				startSample = (uint64)(39.724 * 44100);
			}
			else {
				throw Error{ U"ループ位置が指定されていない、またはSEが正しく再生されていません。" };
			}
			//AudioAsset(name).seekSamples((uint64)66.207 * 44100);
			AudioAsset(name).setLoopPoint(startSample);
			AudioAsset(name).setVolume(bgmVolume).play();
			currentBGMName = name;
			break;
		}
	}

	void play(String name, SecondsF seconds) {
		switch (audioConfigs[name].audioType) {
		case AudioType::SE:
			audioConfigs[name].playType = PlayType::Play;
			AudioAsset(name).setVolume(0);
			AudioAsset(name).play();
			AudioAsset(name).fadeVolume(seVolume, seconds);
			break;
		case AudioType::BGM:
			//bgmのループ開始位置
			uint64 startSample = 0;
			if (name == U"MidBoss") {
				startSample = (uint64)(39.724 * 44100);
			}
			//else {
			//	throw Error{ U"ループ位置が指定されていない、またはSEが正しく再生されていません。" };
			//}
			//AudioAsset(name).seekSamples((uint64)66.207 * 44100);
			AudioAsset(name).setLoopPoint(startSample);
			AudioAsset(name).setVolume(0);
			AudioAsset(name).play();
			AudioAsset(name).fadeVolume(bgmVolume, seconds);
			currentBGMName = name;
			break;
		}
	}

	void stop(String name, SecondsF seconds) {
		AudioAsset(name).stop(seconds);
	}

	void stopAllAudio() {
		for (auto&& [key, value] : audioConfigs)
		{
			AudioAsset(key).stop();
		}
	}

	void stopAllSE() {
		// 構造化束縛を使う
		for (auto&& [key, value] : audioConfigs)
		{
			if (value.audioType == AudioType::SE) {
				AudioAsset(key).stop(1s);
			}
		}
	}

	void pause(String name, Duration seconds) {
		AudioAsset(name).pause(seconds);
	}

	void pauseAllSE() {
		// 構造化束縛を使う
		for (auto&& [key, value] : audioConfigs)
		{
			if(value.audioType==AudioType::SE)AudioAsset(key).pause();
		}
	}

	void pauseAllAudio() {
		for (auto&& [key, value] : audioConfigs)
		{
			AudioAsset(key).pause();
		}
	}

	void playAllPauseAudio(Duration seconds) {
		// 構造化束縛を使う
		for (auto&& [key, value] : audioConfigs)
		{
			if (AudioAsset(key).isPaused()) {
				switch (value.audioType) {
				case AudioType::BGM:
					AudioAsset(key).setVolume(0);
					AudioAsset(key).play();
					AudioAsset(key).fadeVolume(bgmVolume, seconds);
					break;

				case AudioType::SE:
					if(value.playType==PlayType::Play) AudioAsset(key).setVolume(seVolume).play();
					else AudioAsset(key).playOneShot(seVolume);
					break;
				}
			}
		}
	}

	double LinearToLog(double value)
	{
		if (value == 0.0)
		{
			return 0.0;
		}

		const double minDb = -40.0; // 必要に応じて調整
		const double db = (minDb * (1.0 - value));
		return Math::Pow(10.0, (db / 20.0));
	}

	void changeSeVol(int num) {
		seLevel += num;
		seVolume = LinearToLog((seLevel+5)/10.0);
	}

	void changeBgmVol(int num) {
		bgmLevel += num;
		bgmVolume = LinearToLog((bgmLevel + 5) / 10.0);
		if (AudioAsset(currentBGMName).isPlaying()) AudioManager::Instance()->play(currentBGMName,0s);
	}

	//音量レベル
	double seLevel = 0;
	double bgmLevel = 0;
	//現在のBGM名
	String currentBGMName;
private:
	AudioManager() {
		seVolume = LinearToLog((seLevel + 5) / 10.0);
		bgmVolume = LinearToLog((bgmLevel + 5) / 10.0);
	}
	static AudioManager* instance;
	//曲とその種類のハッシュテーブル
	HashTable<String, AudioConfig> audioConfigs;
	//音量
	double seVolume;
	double bgmVolume;
};
