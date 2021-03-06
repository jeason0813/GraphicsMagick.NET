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
#include "DrawableCompositeImage.h"

namespace GraphicsMagick
{
	//==============================================================================================
	DrawableCompositeImage::DrawableCompositeImage(double x, double y, MagickImage^ image)
	{
		Throw::IfNull("image", image);

		BaseValue = new Magick::DrawableCompositeImage(x, y, image->ReuseValue());
	}
	//==============================================================================================
	DrawableCompositeImage::DrawableCompositeImage(double x, double y, CompositeOperator compose, 
		MagickImage^ image)
	{
		Throw::IfNull("image", image);

		BaseValue = new Magick::DrawableCompositeImage(x, y, image->Width, image->Height,
			image->ReuseValue(), (Magick::CompositeOperator)compose);
	}
	//==============================================================================================
	DrawableCompositeImage::DrawableCompositeImage(MagickGeometry^ offset, MagickImage^ image)
	{
		Throw::IfNull("offset", offset);
		Throw::IfNull("image", image);

		BaseValue = new Magick::DrawableCompositeImage(offset->X, offset->Y, offset->Width,
			offset->Height, image->ReuseValue());
	}
	//==============================================================================================
	DrawableCompositeImage::DrawableCompositeImage(MagickGeometry^ offset, CompositeOperator compose,
		MagickImage^ image)
	{
		Throw::IfNull("offset", offset);
		Throw::IfNull("image", image);

		BaseValue = new Magick::DrawableCompositeImage(offset->X, offset->Y, offset->Width,
			offset->Height, image->ReuseValue(), (Magick::CompositeOperator)compose);
	}
	//==============================================================================================
	double DrawableCompositeImage::Height::get()
	{
		return Value->height();
	}
	//==============================================================================================
	void DrawableCompositeImage::Height::set(double value)
	{
		Value->height(value);
	}
	//==============================================================================================
	double DrawableCompositeImage::Width::get()
	{
		return Value->width();
	}
	//==============================================================================================
	void DrawableCompositeImage::Width::set(double value)
	{
		Value->width(value);
	}
	//==============================================================================================
	double DrawableCompositeImage::X::get()
	{
		return Value->x();
	}
	//==============================================================================================
	void DrawableCompositeImage::X::set(double value)
	{
		Value->x(value);
	}
	//==============================================================================================
	double DrawableCompositeImage::Y::get()
	{
		return Value->y();
	}
	//==============================================================================================
	void DrawableCompositeImage::Y::set(double value)
	{
		Value->y(value);
	}
	//==============================================================================================
}