#include "effects.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "jpeglib.h"

//--------------------------------------------------------------------------//

bool make_monochrome(uint8_t** raw_data,
                     uint32_t* width,
                     uint32_t* height,
                     int* components,
                     int* color_space) {
	if ((raw_data == NULL)
		|| (*raw_data == NULL)
		|| (width == NULL)
		|| (height == NULL)
		|| (components == NULL)
		|| (color_space == NULL)) {
		return false;
	}

	J_COLOR_SPACE e_color_space = (J_COLOR_SPACE)*color_space;
	// allow only RGB for this example
	if (e_color_space != JCS_RGB) {
		// not supported
		return false;
	}

	// already monochome
	if (e_color_space == JCS_GRAYSCALE) {
		return true;
	}

	uint8_t* in_data = *raw_data;
	uint32_t out_size = *width * *height;
	uint8_t* out_raw_data = (uint8_t*)malloc(out_size);

	// calculate luma for each single rgb point and put it into new buffer
	for (uint32_t i=0; i<out_size; i++) {
		// get colors
		uint8_t r = *(in_data + i * 3);
		uint8_t g = *(in_data + i * 3 + 1);
		uint8_t b = *(in_data + i * 3 + 2);

		// calculate
		float luma = 0.2126*r + 0.7152*g + 0.0722*b;
		*(out_raw_data + i) = (uint8_t)luma;
	}

	// now we have only one component
	*components = 1;

	// color space has changed
	*color_space = (int)JCS_GRAYSCALE;

	// return new buffer
	*raw_data = out_raw_data;

	// release old buffer
	free(in_data);

	return true;
}

//--------------------------------------------------------------------------//

bool mirror(uint8_t** raw_data,
            uint32_t* width,
            uint32_t* height,
            int* components,
            int* color_space) {
	if ((raw_data == NULL)
		|| (*raw_data == NULL)
		|| (width == NULL)
		|| (height == NULL)
		|| (components == NULL)) {
		return false;
	}

	uint8_t* in_data = *raw_data;
	uint32_t row_size = *width * *components;

	// go through the rows
	for (uint32_t i=0; i<*height; i++) {
		uint8_t* row = in_data + i * row_size;

		for (uint32_t j=0; j<*width/2; j++) {
			uint8_t tmp_buf[3];

			uint8_t* pix1 = row + j * *components;
			uint8_t* pix2 = row + row_size - (j + 1) * *components;

			// swap them
			memmove(&tmp_buf, pix1, *components);
			memmove(pix1, pix2, *components);
			memmove(pix2, &tmp_buf, *components);
		}
	}

	return true;
}

//--------------------------------------------------------------------------//

float scale_value = 1.0;

void set_scale(float scale) {
	scale_value = scale;
}

//--------------------------------------------------------------------------//

bool resize(uint8_t** raw_data,
            uint32_t* width,
            uint32_t* height,
            int* components,
            int* color_space) {
	if ((raw_data == NULL)
		|| (*raw_data == NULL)
		|| (width == NULL)
		|| (height == NULL)
		|| (components == NULL)) {
		return false;
	}

	if (scale_value == 1.0) {
		return true;
	}

	if (scale_value < 0.1 || scale_value > 10.0) {
		return false;
	}

	uint32_t out_width = *width * scale_value;
	uint32_t out_height = *height * scale_value;

	uint8_t* in_data = *raw_data;
	uint32_t in_row_size = *width * *components;

	uint32_t out_size = out_width * out_height * *components;
	uint8_t* out_raw_data = (uint8_t*)malloc(out_size);
	uint32_t out_row_size = out_width * *components;

	uint8_t* out_row_tmp = malloc(out_row_size);

	// go through the rows
	for (uint32_t i=0; i<*height; i++) {
		uint8_t* in_row = in_data + i * in_row_size;

		for (uint32_t j=0; j<*width; j++) {
			// get original pixel
			uint8_t* in_pix = in_row + j * *components;

			// copy original pixel as many times as we need to scale, put it into temporary buffer
			for (uint32_t out_j = (uint32_t)(j * scale_value); (out_j < (uint32_t)((j + 1) * scale_value)) && (out_j < out_width); out_j++) {
				uint8_t* out_pix = out_row_tmp + out_j * *components;
				memcpy(out_pix, in_pix, *components);
			}
		}

		// copy create row as many time as we need to scale
		for (uint32_t out_i = (uint32_t)(i * scale_value); (out_i < (uint32_t)((i + 1) * scale_value)) && (out_i < out_height); out_i++) {
			uint8_t* out_row = out_raw_data + out_i * out_row_size;
			memcpy(out_row, out_row_tmp, out_row_size);
		}
	}

	// release tmp buffer
	free(out_row_tmp);

	// return new values
	*width = out_width;
	*height = out_height;

	// return new buffer
	*raw_data = out_raw_data;

	// release old buffer
	free(in_data);

	return true;
}


//--------------------------------------------------------------------------//
