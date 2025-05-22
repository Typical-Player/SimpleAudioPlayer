#include "Player.h"

SAP::Player::Player() {
	m_driver = new Native::MiniaudioDriver();
}

SAP::Player::~Player() {
	delete m_driver;
}

void SAP::Player::loadFile(String^ path) {
	std::wstring convertedPath = msclr::interop::marshal_as<std::wstring>(path);
	if (!m_driver->loadFile(convertedPath)) {
		Debug::WriteLine("Player: ERROR: Failed to load file");
	}
	Debug::WriteLine("Player: OK: File loaded");
}

void SAP::Player::play() {
	if (!m_driver->playSound()) {
		Debug::WriteLine("Player: ERROR: Failed to play sound");
	}
	Debug::WriteLine("Player: OK: Playing Sound");
}

void SAP::Player::pause() {
	if (!m_driver->pauseSound()) {
		Debug::WriteLine("Player: ERROR: Failed to stop sound");
	}
	Debug::WriteLine("Player: OK: Stopping Sound");
}

bool SAP::Player::paused() {
	return m_driver->paused();
}

uint64_t SAP::Player::getTimeInMiliseconds() {
	return m_driver->getMilisecondTime();
}

uint64_t SAP::Player::getTimeInSeconds() {
	return m_driver->getMilisecondTime() / 1000;
}

float SAP::Player::getLenghtInSeconds() {
	return m_driver->getSoundLenght();
}

void SAP::Player::setTimeInMiliseconds(uint64_t time) {
	m_driver->setMilisecondTime(time);
}

void SAP::Player::setVolume(float vol) {
	if (m_driver->setEngineVolume(vol)) {
		Debug::WriteLine("Player: ERROR: Failed set new volume level");
	}
	Debug::WriteLine("Player: OK: New volume level set");
}
