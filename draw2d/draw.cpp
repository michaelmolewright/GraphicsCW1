#include "draw.hpp"

#include <algorithm>

#include <cmath>

#include "surface.hpp"


float cross_product(Vec2f a, Vec2f b)
{
	return (a.x * b.y) - (a.y * b.x); 
}

void draw_line_solid( Surface& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor )
{
	// start by making sure aBegin has the samller value of x
	if (aBegin.x > aEnd.x)
	{
		std::swap(aBegin, aEnd); //less computationally expensive than using a temp variable
	}

	float dx, dy, gradient;

	dx = aEnd.x - aBegin.x;
	dy = aEnd.y - aBegin.y;

	//call them once so its more efficient than each iteration
	float width = (float) aSurface.get_width();
	float height = (float) aSurface.get_height();

	if(dy == 0.f){ // edge case for a horizontal line
		for (size_t i = aBegin.x; i <= aEnd.x; i++){
			if ( i < width && i >= 0 && aBegin.y < height && aBegin.y >= 0 ){
				aSurface.set_pixel_srgb( round(i), round(aBegin.y), aColor );
			}
		}
		return;
	}

	if(dx == 0.f){ //edge case for a vertical line
		//have to check which point has a greater y value and adjust the for loops acordingly
		if(aBegin.y < aEnd.y){
			for (size_t j = aBegin.y; j <= aEnd.y; j++){
				if ( aBegin.x < width && aBegin.x >= 0 && j < height && j >= 0 ){
					aSurface.set_pixel_srgb( round(aBegin.x), round(j), aColor );
				}
			}
		}
		else{
			for (size_t j = aEnd.y; j <= aBegin.y; j++){
				if ( aBegin.x < width && aBegin.x >= 0 && j < height && j >= 0 ){
					aSurface.set_pixel_srgb( round(aBegin.x), round(j), aColor );
				}
			}
		}
		
		return;
	}
	
	gradient = dy/dx;

	//DDA algorithm
	if( gradient > 1) {
		float x = aBegin.x;
		for (size_t j = aBegin.y; j <= aEnd.y; j++){
			if ( x < width && x >= 0 && j < height && j >= 0 ){
				aSurface.set_pixel_srgb( round(x), round(j), aColor );
			}
			x += 1/gradient;
		}
	}
	else if ( gradient < -1){ 
		float x = aBegin.x;
		for (size_t j = aBegin.y; j >= aEnd.y; j--){
			if ( x < width && x >= 0 && j < height && j >= 0 ){
				aSurface.set_pixel_srgb( round(x), round(j), aColor );
			}
			x += 1/ ( -1 * gradient);
		}
	}
	else{
		float y = aBegin.y;
		for (size_t i = aBegin.x; i <= aEnd.x; i++){
			if ( i < width && i >= 0 && y < height && y >= 0 ){
				aSurface.set_pixel_srgb( round(i), round(y), aColor );
			}
			y += gradient;
		}
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

void draw_triangle_solid( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor )
{
	float r1,r2,r3;
	bool all_negative, all_positive;
	Vec2f point;

	//find the min and max x and y values to shorten the enumerations for checking pixels
	//this way you are only checking the pixels in the immediate rectangle around the triangle

	float min_x, max_x, min_y, max_y;

	min_x = std::max(std::min(aP0.x, std::min(aP1.x, aP2.x)), 0.f);
	min_y = std::max(std::min(aP0.y, std::min(aP1.y, aP2.y)), 0.f);

	max_x = std::max(aP0.x + 1.f, std::max(aP1.x + 1.f, aP2.x + 1.f));
	max_y = std::max(aP0.y + 1.f, std::max(aP1.y + 1.f, aP2.y + 1.f));

	max_x = std::min(max_x, float(aSurface.get_width()));
	max_y = std::min(max_y, float(aSurface.get_height()));
	
	for ( size_t i = min_x; i < max_x; i++ )
	{
		for ( size_t j = min_y; j < max_y; j++ )
		{
			point = {i + 0.5f, j + 0.5f};

			//using the cross_product() function to evaluate the sign of the different normals of the edge vectors and the point vector
			r1 = cross_product( {point.x - aP0.x, point.y - aP0.y}, {aP1.x - aP0.x, aP1.y - aP0.y} );
			r2 = cross_product( {point.x - aP1.x, point.y - aP1.y}, {aP2.x - aP1.x, aP2.y - aP1.y} );
			r3 = cross_product( {point.x - aP2.x, point.y - aP2.y}, {aP0.x - aP2.x, aP0.y - aP2.y} );

			all_negative = (r1 < 0 && r2 < 0 && r3 < 0 );
			all_positive = (r1 > 0 && r2 > 0 && r3 > 0 );

			//if either all negative or all positive, print the pixel else skip to next pixel
			if (all_negative || all_positive) {
				aSurface.set_pixel_srgb( i, j, aColor );
			}
		}
	}	
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
	//compute the area of the triangle outside the for loop to be more effcient
	//using cross product as its the same but without dividing by 2 which cancels out later
	float area_main = cross_product({aP2.x - aP0.x, aP2.y - aP0.y}, {aP2.x - aP1.x, aP2.y - aP1.y});

	//alpha, beta, gamma for the baycentric coodinates
	float a, b, g;

	Vec2f point;
	ColorF interp_Col;

	//find the min and max x and y values to shorten the enumerations for checking pixels
	//this way you are only checking the pixels in the immediate rectangle around the triangle

	float min_x, max_x, min_y, max_y;

	min_x = std::max(std::min(aP0.x, std::min(aP1.x, aP2.x)), 0.f);
	min_y = std::max(std::min(aP0.y, std::min(aP1.y, aP2.y)), 0.f);

	max_x = std::max(aP0.x, std::max(aP1.x, aP2.x));
	max_y = std::max(aP0.y, std::max(aP1.y, aP2.y));

	max_x = std::min(max_x, float(aSurface.get_width()));
	max_y = std::min(max_y, float(aSurface.get_height()));


	//size_t for i and j but float for min and max values, makes computation way faster
	//We dont have to cast float values to i and j for each iteration which costs a ot computationally
	//can be a size_t as the pixels will never be negative, sue to the min and maxing in the previous step
	for (size_t i = min_x; i < max_x; i++ )
	{
		for (size_t j = min_y; j < max_y; j++ )
		{
			//create the point vector, and add 0.5 to improve rasterisation (treat pixel as if it was centered)
			point = { i + 0.5f, j + 0.5f};

			//compute the baycentric coodinate values by coimputing the area of the mini triangles divided by the area of the whole triangle
			a = cross_product({point.x - aP0.x, point.y - aP0.y}, {point.x - aP1.x, point.y - aP1.y}) / area_main;
			b = cross_product({point.x - aP1.x, point.y - aP1.y}, {point.x - aP2.x, point.y - aP2.y}) / area_main;

			//optimised by reducing division and function call
			g = 1 - a - b;
			
			if (a >= 0 && a <= 1 && b >= 0 && b <= 1 && g >= 0 && g <= 1){
				
				interp_Col = times_col(aC0, aC1, aC2, a, b, g);
				aSurface.set_pixel_srgb( i, j, linear_to_srgb(interp_Col) );
			}
		}
	}
	//draw_triangle_solid(aSurface, aP0, aP1, aP2, linear_to_srgb(aC0));

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
