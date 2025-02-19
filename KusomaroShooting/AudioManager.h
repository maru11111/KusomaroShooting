#pragma once

enum class AudioType {
	BGM,
	SE
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
		AudioAsset::Load(name);
		AudioTypes[name] = AudioType;
	}

	//audioを再生
	//BGMかSEかによって異なる音量で再生
	void play(String name) {
		switch (AudioTypes[name]) {
		case AudioType::SE: AudioAsset(name).setVolume(seVolume).playOneShot(); break;
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
			break;
		}
	}

private:
	AudioManager() {}
	static AudioManager* instance;
	//曲とその種類のハッシュテーブル
	HashTable<String, AudioType> AudioTypes;
	//音量
	double seVolume=1.0;
	double bgmVolume=1.0;
};
