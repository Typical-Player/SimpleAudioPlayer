#include "Player.h"

SAP::Player::Player()
{
	result = new ma_result();
	engine = new ma_engine();
	sound = new ma_sound();

	*result = ma_engine_init(NULL, engine);
	if (*result != MA_SUCCESS) {
		Debug::WriteLine("MA: ER: Failed to initialize audio engine.");
		engineIsValid = false;
		return;
	}

	engineIsValid = true;

	Debug::WriteLine("MA: OK: Audio engine initialized!");
}

SAP::Player::~Player()
{
	ma_engine_uninit(engine);
}

void SAP::Player::loadFile(String^ path)
{
	if (fileIsValid)
	{
		ma_sound_uninit(sound);
	}
	*result = ma_sound_init_from_file_w(engine, msclr::interop::marshal_as<std::wstring>(path).c_str(), MA_SOUND_FLAG_ASYNC | MA_SOUND_FLAG_STREAM, NULL, NULL, sound);
	if (*result != MA_SUCCESS) {
		Debug::WriteLine("MA: ER: Failed to load file.");
		fileIsValid = false;
		return;
	}

	Debug::WriteLine("MA: OK: File loaded!");
	fileIsValid = true;
}

void SAP::Player::play()
{
	*result = ma_sound_start(sound);
	if (*result != MA_SUCCESS) {
		Debug::WriteLine("MA: ER: Failed start.");
	}
	Debug::WriteLine("MA: OK: Start!");
}

void SAP::Player::pause()
{
	*result = ma_sound_stop(sound);
	if (*result != MA_SUCCESS) {
		Debug::WriteLine("MA: ER: Failed to stop.");
	}
	Debug::WriteLine("MA: OK: Pause!");
}

bool SAP::Player::paused()
{
	return !ma_sound_is_playing(sound);
}

bool SAP::Player::fileValid()
{
	return fileIsValid;
}

bool SAP::Player::engineValid()
{
	return engineIsValid;
}

uint64_t SAP::Player::getTimeInMiliseconds()
{
	return ma_sound_get_time_in_milliseconds(sound);
}

uint64_t SAP::Player::getTimeInPcmFrames()
{
	return ma_sound_get_time_in_pcm_frames(sound);
}

uint64_t SAP::Player::getTotalTimeInPcmFrames()
{
	uint64_t res;
	*result = ma_sound_get_length_in_pcm_frames(sound, &res);

	if (*result != MA_SUCCESS) {
		return -1;
	}

	return res;
}

float SAP::Player::getTotalTimeInSeconds()
{
	float res;
	*result = ma_sound_get_length_in_seconds(sound, &res);

	if (*result != MA_SUCCESS) {
		return -1.f;
	}

	return res;
}

void SAP::Player::setTimeInPcmFrames(uint64_t second)
{
	ma_sound_seek_to_pcm_frame(sound, second);
}

void SAP::Player::setVolume(float vol)
{
	*result = ma_engine_set_volume(engine, vol);
	if (*result != MA_SUCCESS) {
		Debug::WriteLine("MA: ER: Failed to set volume!");
	}

	Debug::WriteLine("MA: OK: New volume set!");
}
