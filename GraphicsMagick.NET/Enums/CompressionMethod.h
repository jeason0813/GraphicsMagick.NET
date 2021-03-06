//=================================================================================================
// Copyright 2014-2015 Dirk Lemstra <https://graphicsmagick.codeplex.com/>
//
// Licensed under the ImageMagick License (the "License"); you may not use this file except in 
// compliance with the License. You may obtain a copy of the License at
//
//   http://www.imagemagick.org/script/license.php
//
// Unless required by applicable law or agreed to in writing, software distributed under the
// License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
// express or implied. See the License for the specific language governing permissions and
// limitations under the License.
//=================================================================================================
#pragma once

#include "Stdafx.h"

namespace GraphicsMagick
{
	///=============================================================================================
	///<summary>
	/// Specifies compression methods.
	///</summary>
	public enum class CompressionMethod
	{
		Undefined = Magick::UndefinedCompression,
		NoCompression = Magick::NoCompression,
		BZip = Magick::BZipCompression,
		Faxo = Magick::FaxCompression,
		Group4 = Magick::Group4Compression,
		JPEG = Magick::JPEGCompression,
		JPEG2000 = Magick::JPEG2000Compression,
		LZW = Magick::LZWCompression,
		RLE = Magick::RLECompression,
		Zip = Magick::ZipCompression,
		LZMA = Magick::LZMACompression,
		JBIG1 = Magick::JBIG1Compression,
		JBIG2 = Magick::JBIG2Compression
	};
	//==============================================================================================
}