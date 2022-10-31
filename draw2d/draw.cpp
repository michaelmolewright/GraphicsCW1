#include "draw.hpp"

#include <algorithm>

#include <cmath>

#include "surface.hpp"

void draw_line_solid( Surface& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor )
{
	Vec2f temp;

	if (aBegin.x > aEnd.x)
	{
		temp = aBegin;
		aBegin = aEnd;
		aEnd = temp;
	}

	float dx, dy, y_inc, x_inc;

	dx = aEnd.x - aBegin.x;

	dy = aEnd.y - aBegin.y;

	float step;

	if ( fabsf( dx ) > fabsf( dy ))
	{
		step = fabsf( dx );
	}
	else
	{
		step = fabsf( dy );
	}

	x_inc = dx / step;
	y_inc = dy / step;

	float x = aBegin.x;
	float y = aBegin.y;

	float width = (float) aSurface.get_width();
	float height = (float) aSurface.get_height();

	for( size_t i = 0; i < (size_t) step; i++ )
	{

		if ( x < width && x >= 0 && y < height && y >= 0 )
		{
			aSurface.set_pixel_srgb( x, y, aColor );
		}

		x += x_inc;
		y += y_inc;

	}
}

void draw_triangle_wireframe( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation

	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aP0;   // is properly implemented.
	(void)aP1;
	(void)aP2;
	(void)aColor;
}

float equation( Vec2f aP0, Vec2f aP1, size_t aX, size_t aY )
{
	float dy, dx, c;

	dy = aP0.y - aP1.y;
	dx = aP0.x - aP1.x;
	
	c = (aP0.y * dx) + (-dy * aP0.x);

	return ( (float) aY * dx ) + ( (float) aX * -dy ) - c;
}

void draw_triangle_solid( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor )
{
	float r1,r2,r3;
	bool neg, pos; 
	for ( size_t i = 0; i < aSurface.get_width(); i++ )
	{
		for ( size_t j = 0; j < aSurface.get_height(); j++ )
		{
			r1 = equation( aP0, aP1, i, j );
			r2 = equation( aP1, aP2, i, j );
			r3 = equation( aP2, aP0, i, j );


			neg = (r1 < 0 || r2 < 0 || r3 < 0 );
			pos = (r1 > 0 || r2 > 0 || r3 > 0 );

			if (!(neg&&pos)) {
				aSurface.set_pixel_srgb( i, j, aColor );
			}
			
		}
	}

	
}

float get_area(Vec2f aP0, Vec2f aP1, Vec2f aP2)
{
	//dont have to divide by 2, because it will cancel out in later operation -- more efficient
	return ((aP2.x - aP0.x) * (aP1.y - aP0.y)) - ((aP2.y - aP0.y) * (aP1.x - aP0.x)); 
}

ColorF times_col(ColorF c1, ColorF c2, ColorF c3, float f1, float f2, float f3){
	return {
		(c1.r * f1) + (c2.r * f2) + (c3.r * f3),
		(c1.g * f1) + (c2.g * f2) + (c3.g * f3),
		(c1.b * f1) + (c2.b * f2) + (c3.b * f3)
	};
}

void draw_triangle_interp( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorF aC0, ColorF aC1, ColorF aC2 )
{
	float area = get_area(aP0, aP1, aP2);

	float n1, n2, n3;

	Vec2f point;
	ColorF interp_Col;
	
	for (size_t i = 0; i < aSurface.get_width(); i++ )
	{
		for (size_t j = 0; j < aSurface.get_height(); j++ )
		{

			point = {(float) i, (float) j};

			n1 = get_area(aP0, aP1, point)/area;
			n2 = get_area(aP1, aP2, point)/area;
			n3 = get_area(aP0, point, aP2)/area;
			

			if (n1 >= 0 && n1 <= 1 && n2 >= 0 && n2 <= 1 && n3 >= 0 && n3 <= 1){
				
				interp_Col = times_col(aC0, aC1, aC2, n1, n2, n3);
				aSurface.set_pixel_srgb( i, j, linear_to_srgb(interp_Col) );
			}
		}
	}
}


void draw_rectangle_solid( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aMinCorner;   // is properly implemented.
	(void)aMaxCorner;
	(void)aColor;
}

void draw_rectangle_outline( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments
	(void)aMinCorner;
	(void)aMaxCorner;
	(void)aColor;
}
