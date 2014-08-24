//=================================================================================================
// Copyright 2014 Dirk Lemstra <https://graphicsmagick.codeplex.com/>
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
#include "Stdafx.h"
#include "..\..\Helpers\EnumHelper.h"
#include "ExifReader.h"

using namespace System::Text;

namespace GraphicsMagick
{
	//==============================================================================================
	void ExifReader::AddValues(List<ExifValue^>^ values, unsigned int index)
	{
		_Index = _StartIndex + index;
		unsigned short count = GetUInt16();

		for (unsigned short i = 0; i < count; i++)
		{
			ExifValue^ value = CreateValue();
			if (value == nullptr)
				continue;

			bool duplicate = false;
			for each (ExifValue^ val in values)
			{
				if (val->Tag == value->Tag)
				{
					duplicate = true;
					break;
				}
			}

			if (duplicate)
				continue;

			if (value->Tag == ExifTag::SubIFDOffset)
			{
				if (value->DataType == ExifDataType::Long)
					_ExifOffset = (unsigned int)value->Value;
			}
			else if (value->Tag == ExifTag::GPSIFDOffset)
			{
				if (value->DataType == ExifDataType::Long)
					_GPSOffset =  (unsigned int)value->Value;
			}
			else
				values->Add(value);
		}
	}
	//==============================================================================================
	Object^ ExifReader::ConvertValue(ExifDataType dataType, array<Byte>^ data, int numberOfComponents)
	{
		if (data == nullptr || data->Length == 0)
			return nullptr;

		switch (dataType)
		{
		case ExifDataType::Unknown:
			return nullptr;
		case ExifDataType::Ascii:
			return ToString(data);
		case ExifDataType::Byte:
			if (numberOfComponents == 1)
				return ToByte(data);
			else 
				return data;
		case ExifDataType::DoubleFloat:
			if (numberOfComponents == 1)
				return ToDouble(data);
			else
				return ToArray<double>(dataType, data, gcnew ConverterMethod<double>(this, &ExifReader::ToDouble));
		case ExifDataType::Long:
			if (numberOfComponents == 1)
				return ToUInt32(data);
			else
				return ToArray<unsigned int>(dataType, data, gcnew ConverterMethod<unsigned int>(this, &ExifReader::ToUInt32));
		case ExifDataType::Rational:
			if (numberOfComponents == 1)
				return ToURational(data);
			else
				return ToArray<double>(dataType, data, gcnew ConverterMethod<double>(this, &ExifReader::ToURational));
		case ExifDataType::Short:
			if (numberOfComponents == 1)
				return ToUInt16(data);
			else
				return ToArray<unsigned short>(dataType, data, gcnew ConverterMethod<unsigned short>(this, &ExifReader::ToUInt16));
		case ExifDataType::SignedByte:
			if (numberOfComponents == 1)
				return ToSByte(data);
			else
				return ToArray<SByte>(dataType, data, gcnew ConverterMethod<SByte>(this, &ExifReader::ToSByte));
		case ExifDataType::SignedLong:
			if (numberOfComponents == 1)
				return ToInt32(data);
			else
				return ToArray<int>(dataType, data, gcnew ConverterMethod<int>(this, &ExifReader::ToInt32));
		case ExifDataType::SignedRational:
			if (numberOfComponents == 1)
				return ToRational(data);
			else
				return ToArray<double>(dataType, data, gcnew ConverterMethod<double>(this, &ExifReader::ToRational));
		case ExifDataType::SignedShort:
			if (numberOfComponents == 1)
				return ToInt16(data);
			else
				return ToArray<short>(dataType, data, gcnew ConverterMethod<short>(this, &ExifReader::ToInt16));
		case ExifDataType::SingleFloat:
			if (numberOfComponents == 1)
				return ToSingle(data);
			else
				return ToArray<float>(dataType, data, gcnew ConverterMethod<float>(this, &ExifReader::ToSingle));
		case ExifDataType::Undefined:
			if (numberOfComponents == 1)
				return ToByte(data);
			else
				return data;
		default:
			throw gcnew NotImplementedException();
		}
	}
	//==============================================================================================
	ExifValue^ ExifReader::CreateValue()
	{
		ExifTag tag = EnumHelper::Parse(GetUInt16(), ExifTag::Unknown);
		ExifDataType dataType = EnumHelper::Parse(GetUInt16(), ExifDataType::Unknown);
		Object^ value = nullptr;

		if (dataType == ExifDataType::Unknown)
			return gcnew ExifValue(tag, dataType, value, false);

		unsigned int numberOfComponents = GetUInt32();

		int size = (int)(numberOfComponents*ExifValue::GetSize(dataType));
		array<Byte>^ data = GetBytes(4);

		if (size > 4)
		{			
			int oldIndex = _Index;
			_Index = ToUInt32(data) + _StartIndex;
			value = ConvertValue(dataType, GetBytes(size), numberOfComponents);
			_Index = oldIndex;
		}
		else
		{
			value = ConvertValue(dataType, data, numberOfComponents);
		}

		bool isArray = value != nullptr && numberOfComponents > 1;
		return gcnew ExifValue(tag, dataType, value, isArray);
	}
	//==============================================================================================
	array<Byte>^ ExifReader::GetBytes(unsigned int length)
	{
		if (_Index + length > (unsigned int)_Data->Length)
			return nullptr;

		array<Byte>^ data = gcnew array<Byte>(length);
		Array::Copy(_Data, _Index, data, 0, length);
		_Index += length;

		return data;
	}
	//==============================================================================================
	String^ ExifReader::GetString(unsigned int length)
	{
		return ToString(GetBytes(length));
	}
	//==============================================================================================
	void ExifReader::GetThumbnail(unsigned int offset)
	{
		List<ExifValue^>^ values = gcnew List<ExifValue^>();
		AddValues(values, offset);

		for each(ExifValue^ value in values)
		{
			if (value->Tag == ExifTag::JPEGInterchangeFormat && (value->DataType == ExifDataType::Long))
				_ThumbnailOffset = (unsigned int)value->Value + _StartIndex;
			else if (value->Tag == ExifTag::JPEGInterchangeFormatLength && value->DataType == ExifDataType::Long)
				_ThumbnailLength = (unsigned int)value->Value;
		}
	}
	//==============================================================================================
	unsigned short ExifReader::GetUInt16()
	{
		return ToUInt16(GetBytes(2));
	}
	//==============================================================================================
	unsigned int ExifReader::GetUInt32()
	{
		return ToUInt32(GetBytes(4));
	}
	//==============================================================================================
	generic<typename TDataType>
	where TDataType : value class
		array<TDataType>^ ExifReader::ToArray(ExifDataType dataType, array<Byte>^ data, ConverterMethod<TDataType>^ converter)
	{
		int dataTypeSize = ExifValue::GetSize(dataType);
		int length = data->Length / dataTypeSize;

		array<TDataType>^ result = gcnew array<TDataType>(length);
		array<Byte>^ buffer = gcnew array<Byte>(dataTypeSize);

		for (int i = 0; i < length; i++)
		{
			Array::Copy(data, i * dataTypeSize, buffer, 0, dataTypeSize);

			result->SetValue(converter(buffer), i);
		}

		return result;
	}
	//==============================================================================================
	Byte ExifReader::ToByte(array<Byte>^ data)
	{
		return data[0];
	}
	//==============================================================================================
	double ExifReader::ToDouble(array<Byte>^ data)
	{
		if (!ValidateArray(data, 8))
			return double();

		return BitConverter::ToDouble(data, 0);
	}
	//==============================================================================================
	short ExifReader::ToInt16(array<Byte>^ data)
	{
		if (!ValidateArray(data, 2))
			return short();

		return BitConverter::ToInt16(data, 0);
	}
	//==============================================================================================
	int ExifReader::ToInt32(array<Byte>^ data)
	{
		if (!ValidateArray(data, 4))
			return int();

		return BitConverter::ToInt32(data, 0);
	}
	//==============================================================================================
	double ExifReader::ToRational(array<Byte>^ data)
	{
		if (!ValidateArray(data, 8))
			return double();

		int numerator = BitConverter::ToInt32(data, 0);
		int denominator = BitConverter::ToInt32(data, 4);

		return numerator/(double) denominator;
	}
	//==============================================================================================
	SByte ExifReader::ToSByte(array<Byte>^ data)
	{
		return (SByte) (data[0] - Byte::MaxValue);
	}
	//==============================================================================================
	float ExifReader::ToSingle(array<Byte>^ data)
	{
		if (!ValidateArray(data, 4))
			return float();

		return BitConverter::ToSingle(data, 0);
	}
	//==============================================================================================
	String^ ExifReader::ToString(array<Byte>^ data)
	{		
		String^ result = Encoding::UTF8->GetString(data, 0, data->Length);
		int nullCharIndex = result->IndexOf('\0');
		if (nullCharIndex != -1)
			result = result->Substring(0, nullCharIndex);

		return result;
	}
	//==============================================================================================
	unsigned short ExifReader::ToUInt16(array<Byte>^ data)
	{
		if (!ValidateArray(data, 2))
			return unsigned short();

		return BitConverter::ToUInt16(data, 0);
	}
	//==============================================================================================
	unsigned int ExifReader::ToUInt32(array<Byte>^ data)
	{
		if (!ValidateArray(data, 4))
			return unsigned int();

		return BitConverter::ToUInt32(data, 0);
	}
	//==============================================================================================
	double ExifReader::ToURational(array<Byte>^ data)
	{
		if (!ValidateArray(data, 8))
			return double();

		unsigned int numerator = BitConverter::ToUInt32(data, 0);
		unsigned int denominator = BitConverter::ToUInt32(data, 4);

		return numerator/(double) denominator;
	}
	//==============================================================================================
	bool ExifReader::ValidateArray(array<Byte>^ data, int size)
	{
		if (data == nullptr || data->Length < size)
			return false;

		if (_IsLittleEndian == BitConverter::IsLittleEndian)
			return true;

		for (int i=0; i < data->Length; i+=size)
		{
			Array::Reverse(data, i, size);
		}

		return true;
	}
	//==============================================================================================
	unsigned int ExifReader::ThumbnailLength::get()
	{
		return _ThumbnailLength;
	}
	//==============================================================================================
	unsigned int ExifReader::ThumbnailOffset::get()
	{
		return _ThumbnailOffset;
	}
	//==============================================================================================
	List<ExifValue^>^ ExifReader::Read(array<Byte>^ data)
	{
		List<ExifValue^>^ result = gcnew List<ExifValue^>();

		_Data = data;

		if (GetString(4) == "Exif")
		{
			if (GetUInt16() != 0)
				return result;

			_StartIndex = 6;
		}
		else
		{
			_Index = 0;
		}

		_IsLittleEndian = GetString(2) == "II";

		if (GetUInt16() != 0x002A)
			return result;

		unsigned int ifdOffset = GetUInt32();
		AddValues(result, ifdOffset);

		unsigned int thumbnailOffset = GetUInt32();
		GetThumbnail(thumbnailOffset);

		if (_ExifOffset != 0)
			AddValues(result, _ExifOffset);

		if (_GPSOffset != 0)
			AddValues(result, _GPSOffset);

		return result;
	}
	//==============================================================================================
}