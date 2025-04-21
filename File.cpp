#include "File.h"
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/tvariant.h>
#include <taglib/tstringlist.h>
#include <taglib/oggfile.h>
#include <taglib/oggflacfile.h>
#include <taglib/flacpicture.h>
#include <taglib/flacfile.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v1tag.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/unsynchronizedlyricsframe.h>
#include <taglib/synchronizedlyricsframe.h>
#include <taglib/tpropertymap.h>
#include <msclr/marshal_cppstd.h>

SAP::File::File(String^ filePath)
{
	TagLib::FileRef f(msclr::interop::marshal_as<std::wstring>(filePath).c_str());

	if (!f.isNull() && f.tag())
	{
		TagLib::Tag* tag = f.tag();

		if (tag->title() == "")
		{
			title = gcnew String(f.file()->name());
		}
		else {
			title = gcnew String(tag->title().toCWString());
		}

		if (tag->artist() == "")
		{
			artist = gcnew String("Unknown artist");
		}
		else {
			artist = gcnew String(tag->artist().toCWString());
		}

		if (tag->album() == "")
		{
			album = gcnew String("Unknown album");
		}
		else {
			album = gcnew String(tag->album().toCWString());
		}

		if (!f.isNull() && f.audioProperties()) {

			TagLib::AudioProperties* properties = f.audioProperties();

			seconds = properties->lengthInSeconds() % 60;
			minutes = (properties->lengthInSeconds() - (properties->lengthInSeconds() % 60)) / 60;

		}
	}
	else {
		title = gcnew String("Unknown audio");
		artist = gcnew String("Unknown artist");
		album = gcnew String("Unknown album");
		seconds = 0;
		minutes = 0;
		isValid = false;
	}

	this->filePath = filePath;

}

SAP::File::~File()
{
	delete title;
	delete artist;
	delete album;
	delete filePath;
}

System::String^ SAP::File::getArtist()
{
	return artist;
}

System::String^ SAP::File::getAlbum()
{
	return album;
}

System::String^ SAP::File::getTitle()
{
	return title;
}

int SAP::File::getSeconds()
{
	return seconds;
}

int SAP::File::getMinutes()
{
	return minutes;
}

System::String^ SAP::File::ToString()
{
	return title + " - " + album;
}

System::String^ SAP::File::getFilePath()
{
	return filePath;
}

bool SAP::File::getIsValid()
{
	return isValid;
}



System::String^ SAP::File::getDetailedInfo()
{
	TagLib::FileRef f(msclr::interop::marshal_as<std::wstring>(getFilePath()).c_str());

	if (!f.isNull() && f.tag())
	{
		TagLib::Tag* tg = f.tag();

		String^ resStr = gcnew String("");

		resStr += "Title: " + gcnew String(tg->title().toCWString()) + "\r\n";
		resStr += "Artist: " + gcnew String(tg->artist().toCWString()) + "\r\n";
		resStr += "Year: " + gcnew String(tg->year().ToString()) + "\r\n";
		resStr += "Comment: " + gcnew String(tg->comment().toCWString()) + "\r\n";
		resStr += "Track: " + gcnew String(tg->track().ToString()) + "\r\n";
		resStr += "Genre: " + gcnew String(tg->genre().toCWString()) + "\r\n";
		resStr += "Album: " + gcnew String(tg->album().toCWString()) + "\r\n";
		resStr += "----- All information -----\r\n";

		TagLib::PropertyMap tags = f.properties();

		if (!tags.isEmpty())
		{
			for (auto j = tags.cbegin(); j != tags.cend(); ++j) {
				for (auto k = j->second.begin(); k != j->second.end(); ++k) {
					resStr += gcnew String(j->first.toCWString()) + ": " + "\"" + gcnew String(tags.value(j->first).toString().toCWString()) + "\"" + "\r\n";
				}
			}
		}

		TagLib::StringList names = f.complexPropertyKeys();
		for (const auto& name : names) {
			const auto& properties = f.complexProperties(name);
			for (const auto& property : properties) {
				resStr += gcnew String(name.toCWString()) + ":" + "\r\n";
				for (const auto& a : property) {

					resStr += "  " + gcnew String(a.first.toCWString()) + " - ";

					if (a.second.type() == TagLib::Variant::ByteVector) {
						resStr += "(" + a.second.value<TagLib::ByteVector>().size() + " bytes)" + "\r\n";
					}
					else {
						resStr += gcnew String(a.second.toString().toCWString()) + "\r\n";
					}

				}
			}
		}

		if (!f.isNull() && f.audioProperties()) {

			TagLib::AudioProperties* properties = f.audioProperties();

			int seconds = properties->lengthInSeconds() % 60;
			int minutes = (properties->lengthInSeconds() - seconds) / 60;

			resStr += "----- Audio Properties -----" + "\r\n";
			resStr += "bitrate: " + properties->bitrate() + "\r\n";
			resStr += "sample rate: " + properties->sampleRate() + "\r\n";
			resStr += "channels: " + properties->channels() + "\r\n";
			resStr += "length: " + minutes + ":" + seconds + "\r\n";
		}

		return resStr;
	}

	return "";
}

System::Drawing::Image^ SAP::File::tryGetImage()
{
	TagLib::FileRef f(msclr::interop::marshal_as<std::wstring>(getFilePath()).c_str());

	if (!f.isNull() && f.tag())
	{
		if (auto file = dynamic_cast<TagLib::FLAC::File*>(f.file())) {
			const TagLib::List<TagLib::FLAC::Picture*>& plist = file->pictureList();
			if (plist.size() > 0)
			{
				TagLib::FLAC::Picture* pic = plist[0];

				System::Diagnostics::Debug::WriteLine("Processing Image (Status: IMG FOUND, SIZE: " + pic->data().size() + ")");

				array<Byte>^ ab = gcnew array<Byte>(pic->data().size());

				System::Diagnostics::Debug::WriteLine("Processing Image (Status: COPYING)");
				System::Runtime::InteropServices::Marshal::Copy(IntPtr((void*)pic->data().data()), ab, 0, pic->data().size());
				System::Diagnostics::Debug::WriteLine("Processing Image (Status: COPIED)");

				System::IO::MemoryStream^ memoryStream = gcnew System::IO::MemoryStream(ab);

				System::Drawing::Image^ coverImage = nullptr;

				System::Diagnostics::Debug::WriteLine("Processing Image (Status: FINISHING)");

				try {
					coverImage = System::Drawing::Image::FromStream(memoryStream);
					System::Diagnostics::Debug::WriteLine("Processing Image (Status: VERYFYING)");
					if (coverImage == nullptr) {
						System::Diagnostics::Debug::WriteLine("Failed to create Image from stream.");
					}
					else {
						System::Diagnostics::Debug::WriteLine("Image created successfully.");
					}
				}
				catch (Exception^ ex) {
					System::Diagnostics::Debug::WriteLine("Exception while creating Image: {0}", ex->Message);
					return nullptr;
				}

				return coverImage;

			}
		}

		if (auto file = dynamic_cast<TagLib::MPEG::File*>(f.file()))
		{
			if (file->hasID3v2Tag()) {
				if (auto id3v2tag = file->ID3v2Tag())
				{
					TagLib::ID3v2::FrameList flist = id3v2tag->frameList("APIC");
					if (flist.isEmpty())
					{
						return nullptr;
					}
					TagLib::ID3v2::AttachedPictureFrame* coverImg = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(flist.front());


					System::Diagnostics::Debug::WriteLine("Processing Image (Status: IMG FOUND, SIZE: " + coverImg->picture().size() + ")");

					array<Byte>^ ab = gcnew array<Byte>(coverImg->picture().size());

					System::Diagnostics::Debug::WriteLine("Processing Image (Status: COPYING)");
					System::Runtime::InteropServices::Marshal::Copy(IntPtr((void*)coverImg->picture().data()), ab, 0, coverImg->picture().size());
					System::Diagnostics::Debug::WriteLine("Processing Image (Status: COPIED)");

					System::IO::MemoryStream^ memoryStream = gcnew System::IO::MemoryStream(ab);

					System::Drawing::Image^ coverImage = nullptr;

					System::Diagnostics::Debug::WriteLine("Processing Image (Status: FINISHING)");

					try {
						coverImage = System::Drawing::Image::FromStream(memoryStream);
						System::Diagnostics::Debug::WriteLine("Processing Image (Status: VERYFYING)");
						if (coverImage == nullptr) {
							System::Diagnostics::Debug::WriteLine("Failed to create Image from stream.");
						}
						else {
							System::Diagnostics::Debug::WriteLine("Image created successfully.");
						}
					}
					catch (Exception^ ex) {
						System::Diagnostics::Debug::WriteLine("Exception while creating Image: {0}", ex->Message);
						return nullptr;
					}

					return coverImage;

				}
			}
		}
	}
	return nullptr;
}

void SAP::File::getLyricsSynced(LRCService::Lyrics^ lyricsObj)
{
	TagLib::FileRef f(msclr::interop::marshal_as<std::wstring>(getFilePath()).c_str());

	if (!f.isNull() && f.tag())
	{
		System::Diagnostics::Debug::WriteLine("Valid file loaded");
		if (auto file = dynamic_cast<TagLib::MPEG::File*>(f.file())) {
			if (auto id3v2Tag = file->ID3v2Tag()) {
				auto frames = id3v2Tag->frameList("SYLT");
				if (!frames.isEmpty()) {
					if (auto syltFrame = dynamic_cast<TagLib::ID3v2::SynchronizedLyricsFrame*>(
						frames.front())) {
						auto text = syltFrame->synchedText();
						lyricsObj->setIsSynced(true);
						Int64 id = 0;
						for each (auto var in text)
						{
							lyricsObj->AddSyncedLine(gcnew LRCService::LyricsLine(gcnew String(var.text.toCWString()), var.time, id));
							id++;
						}
						Diagnostics::Debug::WriteLine("LyricsService: OK: MPEG3 SYNCED LYRICS DETECTED!");
						return;
					}
				}
			}
		}


		TagLib::PropertyMap tags = f.properties();

		if (!tags.isEmpty())
		{
			for (auto j = tags.cbegin(); j != tags.cend(); ++j) {
				for (auto k = j->second.begin(); k != j->second.end(); ++k) {
					if (gcnew String(j->first.toCWString()) == L"LYRICS")
					{
						Diagnostics::Debug::WriteLine("LyricsService: OK: GENERIC 'LYRICS' TAG DETECTED!");
						String^ tagVal = gcnew String(tags.value(j->first).toString().toCWString());

						Diagnostics::Debug::WriteLine(gcnew String(tags.value(j->first).toString().toCWString()));

						System::String^ rgxMatch = gcnew String(R"(^\[([0-9]*)\:([0-9]*)\.([0-9]*)\]([^\[]*))");
						Diagnostics::Debug::WriteLine("INITIALIZATING REGEX ENGINE WITH: " + rgxMatch);

						auto regex = gcnew System::Text::RegularExpressions::Regex(rgxMatch, System::Text::RegularExpressions::RegexOptions::Multiline | System::Text::RegularExpressions::RegexOptions::IgnoreCase);

						auto ok = regex->Matches(tagVal);

						if (ok->Count > 0)
						{
							lyricsObj->setIsSynced(true);
							Diagnostics::Debug::WriteLine("REGEX: OK: MATCHED");
							Int64 id = 0;
							for (size_t i = 0; i < ok->Count; i++)
							{
								if (ok[i]->Groups->Count == 5) {
									int minutes = 0;
									int seconds = 0;
									int miliseconds = 0;
									try
									{
										minutes = System::Convert::ToInt32(ok[i]->Groups[1]->Value) * 60000;
										seconds = System::Convert::ToInt32(ok[i]->Groups[2]->Value) * 1000;
										miliseconds = System::Convert::ToInt32(ok[i]->Groups[3]->Value);
										LRCService::LyricsLine^ lrcLine = gcnew LRCService::LyricsLine(ok[i]->Groups[4]->Value->Trim(), minutes + seconds + miliseconds, id);
										lyricsObj->AddSyncedLine(lrcLine);
										id++;
									}
									catch (Exception^ ex)
									{
										Diagnostics::Debug::Write("REGEX: ERR: REGEX OR FORMAT EXCEPTION: " + ex->Message);
									}
								}
								else {
									Diagnostics::Debug::Write("REGEX: ERR: Capture group isnt of correct size!");
								}
							}
							Diagnostics::Debug::WriteLine("REGEX: OK: ADDED!");
							Diagnostics::Debug::WriteLine("LyricsService: OK: LYRICS MATCHED!");
						}
						else {
							Diagnostics::Debug::WriteLine("LyricsService: WARN: COULDNT FIND ANY VALID LRC!");
							lyricsObj->setIsSynced(false);
						}
						return;
					}
				}
			}
		}
		Diagnostics::Debug::WriteLine("LyricsService: WARN: COULDNT FIND ANY VALID LRC!");
		lyricsObj->setIsSynced(false);
	}

	lyricsObj->setIsSynced(false);
}
