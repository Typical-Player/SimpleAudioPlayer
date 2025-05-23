#define MINIAUDIO_IMPLEMENTATION
#include "MiniaudioDriver.h"

Native::MiniaudioDriver::MiniaudioDriver() {
	m_result = ma_engine_init(nullptr, &m_engine);
	if (m_result != MA_SUCCESS) {
		return;
	}

}

Native::MiniaudioDriver::~MiniaudioDriver() {
	if (soundIsInitialized) {
		ma_sound_uninit(&m_sound);
	}
	ma_engine_uninit(&m_engine);
}

bool Native::MiniaudioDriver::loadFile(std::wstring& filePath) {
	if (soundIsInitialized) {
		ma_sound_uninit(&m_sound);
	}

	m_result = ma_sound_init_from_file_w(&m_engine, filePath.c_str(), MA_SOUND_FLAG_ASYNC | MA_SOUND_FLAG_STREAM, NULL, NULL, &m_sound);

	if (m_result != MA_SUCCESS) {
		soundIsInitialized = false;
		return false;
	}

	soundIsInitialized = true;
	return true;
}

bool Native::MiniaudioDriver::playSound() {
	m_result = ma_sound_start(&m_sound);
	if (m_result != MA_SUCCESS) {
		return false;
	}
	return true;
}

bool Native::MiniaudioDriver::pauseSound() {
	m_result = ma_sound_stop(&m_sound);
	if (m_result != MA_SUCCESS) {
		return false;
	}
	return true;
}

bool Native::MiniaudioDriver::paused() {
	return !ma_sound_is_playing(&m_sound);
}

float Native::MiniaudioDriver::getSoundLenght() {
	float res;
	m_result = ma_sound_get_length_in_seconds(&m_sound, &res);
	if (m_result != MA_SUCCESS) {
		return -1.f;
	}
	return res;
}

uint64_t Native::MiniaudioDriver::getMilisecondTime() {
	return ma_sound_get_time_in_milliseconds(&m_sound);
}

void Native::MiniaudioDriver::setMilisecondTime(uint64_t time) {
	ma_engine_set_time_in_milliseconds(&m_engine, time);
}

bool Native::MiniaudioDriver::setEngineVolume(float volume) {
	m_result = ma_engine_set_volume(&m_engine, volume);
	if (m_result != MA_SUCCESS) {
		return false;
	}

	return true;
}