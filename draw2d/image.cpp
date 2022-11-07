#include "image.hpp"

#include <memory>
#include <algorithm>

#include <cstdio>
#include <cassert>

#include <stb_image.h>

#include "surface.hpp"

#include "../support/error.hpp"

using std::max; using std::min;

namespace
{
	struct STBImageRGBA_ : public ImageRGBA
	{
		STBImageRGBA_( std::size_t, std::size_t, std::uint8_t* );
		virtual ~STBImageRGBA_();
	};
}

ImageRGBA::ImageRGBA()
	: mWidth( 0 )
	, mHeight( 0 )
	, mData( nullptr )
{}

ImageRGBA::~ImageRGBA() = default;


std::unique_ptr<ImageRGBA> load_image( char const* aPath )
{
	assert( aPath );

	stbi_set_flip_vertically_on_load( true );

	int w, h, channels;
	stbi_uc* ptr = stbi_load( aPath, &w, &h, &channels, 4 );
	if( !ptr )
		throw Error( "Unable to load image \"%s\"", aPath );

	return std::make_unique<STBImageRGBA_>(
		std::size_t(w),
		std::size_t(h),
		ptr
	);
}

void blit_masked( Surface& aSurface, ImageRGBA const& aImage, Vec2f aPosition )
{
	
	//min_x and min_y is just aPositino coodinates as we have just checked it is on the screen
		
	float min_x, min_y, max_x, max_y;


	max_x = (aPosition.x + float(aImage.get_width()));
	max_y = (aPosition.y + float(aImage.get_height()));

	
	min_x = max(aPosition.x, 0.f);
	min_y = max(aPosition.y, 0.f);

	max_x = min(max_x, float(aSurface.get_width()));
	max_y = min(max_y, float(aSurface.get_height()));

	ColorU8_sRGB earth;

	for (size_t i = min_x; i < max_x; i++ )
	{
		for (size_t j = min_y; j < max_y; j++ )
		{
			//get the relative pixel value of the image compared to the surface position
			//as everything is relative to aPostion which I have considered to be 0,0 of the image, you can just subtract aPosition from the i and j values.
			ColorU8_sRGB_Alpha blitted_pixel = aImage.get_pixel(i - aPosition.x,j - aPosition.y);

			if (blitted_pixel.a > 128) {
				earth.r = blitted_pixel.r;
				earth.g = blitted_pixel.g;
				earth.b = blitted_pixel.b;

				aSurface.set_pixel_srgb(i,j, earth);
			}
		}
	}
}

namespace
{
	STBImageRGBA_::STBImageRGBA_( std::size_t aWidth, std::size_t aHeight, std::uint8_t* aPtr )
	{
		mWidth = aWidth;
		mHeight = aHeight;
		mData = aPtr;
	}

	STBImageRGBA_::~STBImageRGBA_()
	{
		if( mData )
			stbi_image_free( mData );
	}
}
