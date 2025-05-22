#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "MiniaudioDriver.h"
#include <msclr/marshal_cppstd.h>
namespace SAP {
	using namespace System;
	using namespace System::Diagnostics;
	ref class Player {
	public:
		Player();
		~Player();

		void loadFile(String^ path);

		void play();
		void pause();
		bool paused();
		uint64_t getTimeInMiliseconds();
		uint64_t getTimeInSeconds();
		float getLenghtInSeconds();
		void setTimeInMiliseconds(uint64_t time);
		void setVolume(float vol);

	private:
		Native::MiniaudioDriver* m_driver;
	};
}

#endif // !_PLAYER_H_