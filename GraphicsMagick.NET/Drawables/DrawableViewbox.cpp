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
#include "DrawableViewbox.h"

namespace GraphicsMagick
{
	//==============================================================================================
	void DrawableViewbox::Initialize(int upperLeftX, int upperLeftY, int lowerRightX,
		int lowerRightY)
	{
		BaseValue = new Magick::DrawableViewbox(upperLeftX, upperLeftY, lowerRightX,
			lowerRightY);
	}
	//==============================================================================================
	DrawableViewbox::DrawableViewbox(int upperLeftX, int upperLeftY, int lowerRightX,
		int lowerRightY)
	{
		Initialize(upperLeftX, upperLeftY, lowerRightX, lowerRightY);
	}
	//==============================================================================================
	DrawableViewbox::DrawableViewbox(Rectangle rectangle)
	{
		Initialize(rectangle.X, rectangle.Y, rectangle.X + rectangle.Width,
			rectangle.Y + rectangle.Height);
	}
	//==============================================================================================
	double DrawableViewbox::LowerRightX::get()
	{
		return Convert::ToDouble((int)Value->x1());
	}
	//==============================================================================================
	void DrawableViewbox::LowerRightX::set(double value)
	{
		Value->x1((unsigned long)value);
	}
	//==============================================================================================
	double DrawableViewbox::LowerRightY::get()
	{
		return Convert::ToDouble((int)Value->y1());
	}
	//==============================================================================================
	void DrawableViewbox::LowerRightY::set(double value)
	{
		Value->y1((unsigned long)value);
	}
	//==============================================================================================
	double DrawableViewbox::UpperLeftX::get()
	{
		return Convert::ToDouble((int)Value->x2());
	}
	//==============================================================================================
	void DrawableViewbox::UpperLeftX::set(double value)
	{
		Value->x2((unsigned long)value);
	}
	//==============================================================================================
	double DrawableViewbox::UpperLeftY::get()
	{
		return Convert::ToDouble((int)Value->y2());
	}
	//==============================================================================================
	void DrawableViewbox::UpperLeftY::set(double value)
	{
		Value->y2((unsigned long)value);
	}
	//==============================================================================================
}