#pragma once
#ifndef _TAGLIBDRIVER_H_
#define _TAGLIBDRIVER_H_
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

namespace Native {

	using namespace System;
	using namespace System::Drawing;
	using namespace msclr::interop;

	typedef unsigned int uint;



	class TaglibDriver
	{
	public:
		struct SongDetails {
			std::wstring name;
			std::wstring artist;
			std::wstring album;
			std::wstring comments;
			std::wstring genre;
			uint year;
			uint track;
		};

		struct AudioProperties {
			uint bitrate;
			uint sampleRate;
			uint channels;
			uint lenght;
			uint lenghtPrecise;
		};

		TaglibDriver(String^ filePath);
		~TaglibDriver();


		/// Gets the first image in the embeded metadata;
		/// \ERROR This function might return null if an image was not found or if the parsing failed
		/// \RETURN Image^
		Image^ metadataGetImage();

		/// Returns audio metadata;
		/// \ERROR The metadata might be empty if parsing failed
		/// \RETURN SongDetails
		SongDetails metadataGet() const;

		/// Gets the first image in the embeded metadata;
		/// \ERROR The properties might be 0 if the parsing failed
		/// \RETURN AudioProperties
		AudioProperties audioPropertiesGet() const;

	private:

		/// Converts from System::String^ to std::wstring;
		/// \ERROR This function might return null if the conversion fails
		/// \RETURN std::wstring
		std::wstring systemStringToStdWString(String^ systemString);

		/// Converts from std::wstring& to String^;
		/// \ERROR This function might return null if the conversion fails
		/// \RETURN String^
		String^ stdWStringToSystemString(std::wstring& stdString);

		TagLib::FileRef m_fileRef;
		std::wstring m_filePath;

		SongDetails m_metadata;
		AudioProperties m_audioProperties;

		/// \Note MPEG-3 - specifics

		Image^ m_mpeg3_metadataGetImage(TagLib::MPEG::File* mpegFile);

		/// \Note FLAC - speficics

		Image^ m_flac_metadataGetImage(TagLib::FLAC::File* flacFile);

	};
}

#endif // !_TAGLIBDRIVER_H_