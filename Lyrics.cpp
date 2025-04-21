#include "Lyrics.h"

LRCService::Lyrics::Lyrics()
{
	LRCArray = gcnew System::Collections::Generic::List<LyricsLine^>();
	unsyncedLyrics = gcnew System::String("");
}

void LRCService::Lyrics::AddSyncedLine(LyricsLine^ lrcLine)
{
	if (isSynced)
	{
		LRCArray->Add(lrcLine);
	}
	else {
		System::Diagnostics::Debug::WriteLine("LyricsService: WARN: Tried to add LyricsLine on an unsynced Lyrics object!");
		setIsSynced(false);
	}
}

void LRCService::Lyrics::AddUnsyncedLyrics(String^ lyrics)
{
	unsyncedLyrics = lyrics;
	isSynced = false;
}

bool LRCService::Lyrics::getIsSynced()
{
	return isSynced;
}

void LRCService::Lyrics::setIsSynced(bool isSynced)
{
	this->isSynced = isSynced;
}

void LRCService::Lyrics::updateActives(Int64 currentMilisecond, System::Windows::Forms::ListBox^ lstViewTarget)
{
	if (isSynced && lstViewTarget->Items->Count == LRCArray->Count)
	{
		//Reverse loop
		for (int i = LRCArray->Count - 1; i >= 0; i--)
		{
			//Is the miliseconds of the current item are LESS OR EQUAL than the current miliseconds then we assign that element to active
			if (LRCArray[i]->getMiliseconds() <= currentMilisecond) {
				LRCArray[i]->setIsActive(true);
				if (lstViewTarget->SelectedIndex != i)
				{
					lstViewTarget->SelectedIndex = i;
				}

				//Now we set the previous element to inactive (If any)
				if (i > 0)
				{
					LRCArray[i - 1]->setIsActive(false);
				}
				//And we exit the function
				return;
			}
		}
	}
	else {
		System::Diagnostics::Debug::WriteLine("LyricsService: WARN: Tried to update actives for an unsynced lyrics object!");
		setIsSynced(false);
	}
}

System::Collections::Generic::List<LRCService::LyricsLine^>^ LRCService::Lyrics::getArray()
{
	return LRCArray;
}

void LRCService::Lyrics::populateList(System::Windows::Forms::ListBox^ lstViewTarget)
{
	for (size_t i = 0; i < LRCArray->Count; i++)
	{
		lstViewTarget->Items->Add(LRCArray[i]->ToString());
	}
}

void LRCService::Lyrics::clear()
{
	LRCArray->Clear();
}

void LRCService::Lyrics::updateList(System::Windows::Forms::ListBox^ lstViewTarget)
{
	if (lstViewTarget->Items->Count == LRCArray->Count)
	{
		for (size_t i = 0; i < lstViewTarget->Items->Count; i++)
		{
			if (lstViewTarget->SelectedItems->Contains(lstViewTarget->Items[i]) != LRCArray[i]->getIsActive()) {
				//Lyrics is active here
				lstViewTarget->SelectedItem = lstViewTarget->Items[i];
			}
			else {
				//Lyrics is no longer active here
			}
		}
	}
	else {
		Diagnostics::Debug::WriteLine("LyricsService: ERR: Tried to update list that doesnt match internal array size!");
	}
}
