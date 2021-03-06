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
#include "Stdafx.h"
#include "IptcProfile.h"
#include "IptcTag.h"
#include "..\..\Helpers\EnumHelper.h"
#include "..\..\Helpers\ByteConverter.h"

namespace GraphicsMagick
{
	//==============================================================================================
	void IptcProfile::Initialize()
	{
		if (_Values != nullptr)
			return;

		_Values = gcnew List<IptcValue^>();

		if (Data == nullptr)
			return;

		int i = -1;
		while (i < Data->Length)
			if (Data[++i] == 0x1c)
				break;

		while(i + 4 < Data->Length)
		{
			if (Data[i++] != 28)
				continue;

			i++;

			IptcTag tag = EnumHelper::Parse<IptcTag>((int)Data[i++], IptcTag::Unknown);

			short count = ByteConverter::ToShort(Data, i);
			if (count <= 0)
				continue;

			array<Byte>^ data = gcnew array<Byte>(count);
			if ((count > 0) && (i + count <= Data->Length))
				Buffer::BlockCopy(Data, i, data, 0, count);
			_Values->Add(gcnew IptcValue(tag, data));

			i += count;
		}
	}
	//==============================================================================================
	array<Byte>^ IptcProfile::GetData()
	{
		int length = 0;
		for each (IptcValue^ value in Values)
		{
			length += value->Length + 5;
		}

		array<Byte>^ result = gcnew array<Byte>(length);

		int i = 0;
		for each (IptcValue^ value in Values)
		{
			result[i++] = 28;
			result[i++] = 2;
			result[i++] = (Byte)value->Tag;
			result[i++] = (Byte)(value->Length >> 8);
			result[i++] = (Byte)value->Length;
			if (value->Length > 0)
			{
				Buffer::BlockCopy(value->ToByteArray(), 0, result, i, value->Length);
				i += value->Length;
			}
		}

		return result;
	}
	//==============================================================================================
	IEnumerable<IptcValue^>^ IptcProfile::Values::get()
	{
		Initialize();
		return _Values;
	}
	//==============================================================================================
	IptcValue^ IptcProfile::GetValue(IptcTag tag)
	{
		for each (IptcValue^ iptcValue in Values)
		{
			if (iptcValue->Tag == tag)
				return iptcValue;
		}

		return nullptr;
	}
	//==============================================================================================
	bool IptcProfile::RemoveValue(IptcTag tag)
	{
		Initialize();

		for (int i=0; i < _Values->Count; i++)
		{
			if (_Values[i]->Tag == tag)
			{
				_Values->RemoveAt(i);
				return true;
			}
		}

		return false;
	}
	//==============================================================================================
	void IptcProfile::SetEncoding(Encoding^ encoding)
	{
		Throw::IfNull("encoding", encoding);

		for each (IptcValue^ value in Values)
		{
			value->Encoding = encoding;
		}
	}
	//==============================================================================================
	void IptcProfile::SetValue(IptcTag tag, Encoding^ encoding, String^ value)
	{
		Throw::IfNull("encoding", encoding);

		for each (IptcValue^ iptcValue in Values)
		{
			if (iptcValue->Tag == tag)
			{
				iptcValue->Encoding = encoding;
				iptcValue->Value = value;
				return;
			}
		}

		_Values->Add(gcnew IptcValue(tag, encoding, value));
	}
	//==============================================================================================
	void IptcProfile::SetValue(IptcTag tag, String^ value)
	{
		SetValue(tag, Encoding::Default, value);
	}
	//==============================================================================================
}