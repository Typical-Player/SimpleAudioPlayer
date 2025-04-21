#include "LyricsLine.h"

LRCService::LyricsLine::LyricsLine(String^ lrcContent, Int64 timeMiliseconds, Int64 lrcId)
{
	this->lrcContent = lrcContent;
	this->pTimeMiliseconds = timeMiliseconds;
	this->lrcId = lrcId;
}

System::String^ LRCService::LyricsLine::ToString()
{
	return lrcContent;
}

void LRCService::LyricsLine::setIsActive(bool active)
{
	isActive = active;
}

bool LRCService::LyricsLine::getIsActive()
{
	return isActive;
}

System::Int64 LRCService::LyricsLine::getId()
{
	return lrcId;
}

System::Int64 LRCService::LyricsLine::getMiliseconds()
{
	return pTimeMiliseconds;
}
