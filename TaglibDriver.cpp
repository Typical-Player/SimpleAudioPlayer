#include "TaglibDriver.h"

Native::TaglibDriver::TaglibDriver(String^ filePath)
{
	m_filePath = systemStringToStdWString(filePath);
	m_fileRef = TagLib::FileRef(m_filePath.c_str());

	if (m_fileRef.isNull() || !m_fileRef.tag())
		return; /// \Error fileRef isnt valid

	TagLib::Tag* tag = m_fileRef.tag();
	m_metadata.name = tag->title() != "" ? tag->title().toWString() : m_fileRef.file()->name().wstr();
	m_metadata.artist = tag->artist() != "" ? tag->title().toWString() : L"Unknown";
	m_metadata.album = tag->album() != "" ? tag->album().toWString() : L"Unknown";
	m_metadata.comments = tag->comment().toWString();
	m_metadata.genre = tag->genre().toWString();
	m_metadata.year = tag->year();
	m_metadata.track = tag->track();

	if (!m_fileRef.audioProperties())
		return; /// \ERROR No audio properites

	TagLib::AudioProperties* properites = m_fileRef.audioProperties();

	m_audioProperties.bitrate = properites->bitrate();
	m_audioProperties.sampleRate = properites->sampleRate();
	m_audioProperties.channels = properites->channels();
	m_audioProperties.lenght = properites->lengthInSeconds();
	m_audioProperties.lenghtPrecise = properites->lengthInMilliseconds();
}

Native::TaglibDriver::~TaglibDriver()
{
}

System::Drawing::Image^ Native::TaglibDriver::metadataGetImage()
{
	if (m_fileRef.isNull() || !m_fileRef.tag())
		return nullptr; /// \Error fileRef isnt valid

	if (auto flacFile = dynamic_cast<TagLib::FLAC::File*>(m_fileRef.file())) {
		return m_flac_metadataGetImage(flacFile);
	}
	else if (auto mpegFile = dynamic_cast<TagLib::MPEG::File*>(m_fileRef.file())) {
		return m_mpeg3_metadataGetImage(mpegFile);
	}
	return nullptr; /// \ERROR File format unknown
}

Native::TaglibDriver::SongDetails Native::TaglibDriver::metadataGet() const
{
	return m_metadata;
}

Native::TaglibDriver::AudioProperties Native::TaglibDriver::audioPropertiesGet() const
{
	return m_audioProperties;
}

std::wstring Native::TaglibDriver::systemStringToStdWString(String^ systemString)
{
	return marshal_as<std::wstring>(systemString);
}

System::String^ Native::TaglibDriver::stdWStringToSystemString(std::wstring& stdString)
{
	return marshal_as<String^>(stdString);
}

System::Drawing::Image^ Native::TaglibDriver::m_mpeg3_metadataGetImage(TagLib::MPEG::File* mpegFile)
{
	if (mpegFile == nullptr)
		return nullptr; /// \ERROR Invalid parameter

	if (!mpegFile->hasID3v2Tag())
		return nullptr; /// \ERROR No ID3v2 tag found

	auto& pictureList = mpegFile->ID3v2Tag()->frameList("APIC");
	if (pictureList.isEmpty())
		return nullptr; /// \ERROR No image found

	auto coverImage = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(pictureList.front());
	if (coverImage == nullptr)
		return nullptr; /// \ERROR No front image found

	/// \Note Byte array for image data
	array<Byte>^ pictureByteArray = gcnew array<Byte>(coverImage->picture().size());

	/// \Note Copy all from flacPicture to pictureByteArray
	System::Runtime::InteropServices::Marshal::Copy(IntPtr((void*)coverImage->picture().data()), pictureByteArray, 0, coverImage->picture().size());

	IO::MemoryStream^ imageStream = gcnew IO::MemoryStream(pictureByteArray);

	Image^ fileCoverImage;

	try {
		fileCoverImage = Image::FromStream(imageStream);
	}
	catch (const std::exception&) {
		return nullptr; /// \ERROR Failed to convert byteArray into Image
	}

	return fileCoverImage;
}

System::Drawing::Image^ Native::TaglibDriver::m_flac_metadataGetImage(TagLib::FLAC::File* flacFile)
{
	if (flacFile == nullptr)
		return nullptr; /// \ERROR Invalid parameter

	auto pictureList = flacFile->pictureList();
	if (pictureList.isEmpty())
		return nullptr; /// \ERROR No image found

	auto flacPicture = pictureList[0];

	/// \Note Byte array for image data
	array<Byte>^ pictureByteArray = gcnew array<Byte>(flacPicture->data().size());

	/// \Note Copy all from flacPicture to pictureByteArray
	System::Runtime::InteropServices::Marshal::Copy(IntPtr((void*)flacPicture->data().data()), pictureByteArray, 0, flacPicture->data().size());

	IO::MemoryStream^ imageStream = gcnew IO::MemoryStream(pictureByteArray);

	Image^ fileCoverImage;

	try {
		fileCoverImage = Image::FromStream(imageStream);
	}
	catch (const std::exception&) {
		return nullptr; /// \ERROR Failed to convert byteArray into Image
	}

	return fileCoverImage;
}
