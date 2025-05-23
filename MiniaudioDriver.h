#pragma once
#ifndef _MINIAUDIODRIVER_H_
#define _MINIAUDIODRIVER_H_
#include "miniaudio.h"
#include <string>

namespace Native {

	class MiniaudioDriver {
	public:
		MiniaudioDriver();
		~MiniaudioDriver();

		bool loadFile(std::wstring& filePath);
		bool playSound();
		bool pauseSound();
		bool paused();

		float getSoundLenght();
		uint64_t getMilisecondTime();
		bool setMilisecondTime(uint64_t time);
		bool setEngineVolume(float volume);
	private:
		ma_result m_result;
		ma_engine m_engine;
		ma_sound m_sound;

		bool soundIsInitialized = false;
	};
}

#endif // !_MINIAUDIODRIVER_H_


