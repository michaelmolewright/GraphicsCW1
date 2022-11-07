
inline
ColorU8_sRGB_Alpha ImageRGBA::get_pixel( std::size_t aX, std::size_t aY ) const
{
	assert( aX < mWidth && aY < mHeight ); // Leave this at the top of the function.

	std::size_t lin_index = get_linear_index(aX, aY);
	
	return { mData[lin_index], mData[lin_index+1], mData[lin_index+2], mData[lin_index+3] };
}

inline
std::size_t ImageRGBA::get_width() const noexcept
{
	return mWidth;
}
inline
std::size_t ImageRGBA::get_height() const noexcept
{
	return mHeight;
}

inline
std::uint8_t* ImageRGBA::get_image_ptr() noexcept
{
	return mData;
}
inline
std::uint8_t const* ImageRGBA::get_image_ptr() const noexcept
{
	return mData;
}

inline
std::size_t ImageRGBA::get_linear_index( std::size_t aX, std::size_t aY ) const noexcept
{
	std::size_t lin_index = 4*(aY*mWidth + aX);
	return lin_index; 
}