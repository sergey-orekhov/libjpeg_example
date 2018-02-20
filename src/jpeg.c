#include "jpeg.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "jpeglib.h"

//--------------------------------------------------------------------------//

bool decode_image_file(const char* file_name,
                       uint8_t** raw_data,
                       uint32_t* width,
                       uint32_t* height,
                       int* components,
                       int* color_space) {
	FILE* file;
	if ((file = fopen(file_name, "rb")) == NULL) {
		printf("Unable to open file %s\n", file_name);
		return false;
	}

	// init decoder
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, file);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	int row_size = cinfo.output_width * cinfo.output_components;

	JSAMPARRAY row_data = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_size, 1);

	uint8_t* out_buf = (uint8_t*)malloc(row_size * cinfo.output_height);
	*raw_data = out_buf;

	// read row by row
	while (cinfo.output_scanline < cinfo.output_height) {
		//read a single row
		jpeg_read_scanlines(&cinfo, row_data, 1);

		//prepare destination pointer
		void* dst = out_buf + row_size * (cinfo.output_scanline - 1); // do -1 as it has already bben increased by readind method

		// move the row data into output buffer
		memmove(dst, row_data[0], row_size);
	}

	// return some data
	*width = cinfo.image_width;
	*height = cinfo.image_height;
	*components = cinfo.output_components;
	*color_space = (int)cinfo.out_color_space;

	// clean up
	(*cinfo.mem->free_pool)((j_common_ptr) &cinfo, JPOOL_IMAGE);


	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	fclose(file);

	return true;
}

//--------------------------------------------------------------------------//

bool encode_image_file(const char* file_name,
                       uint8_t* raw_data,
                       uint32_t width,
                       uint32_t height,
                       int components,
                       int color_space,
                       int quality) {
	FILE* file;
	if ((file = fopen(file_name, "wb")) == NULL) {
		printf("Unable to open file %s\n", file_name);
		return false;
	}

	// init encoder
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, file);
	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = components;
	cinfo.in_color_space = (J_COLOR_SPACE)color_space;

	jpeg_set_defaults(&cinfo);

	if (quality > 0 && quality <= 100) {
		jpeg_set_quality(&cinfo, quality, TRUE);
	}

	jpeg_start_compress(&cinfo, TRUE);

	int row_size = width * cinfo.input_components;

	JSAMPARRAY row_data = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_size, 1);

	// write raw by row
	while (cinfo.next_scanline < cinfo.image_height) {
		// prepare a source pointer
		void* src = raw_data + row_size * cinfo.next_scanline;

		// move data from input buffer into single row buffer
		memmove(row_data[0], src, row_size);

		// write it
		jpeg_write_scanlines(&cinfo, row_data, 1);
	}

	// clean up
	(*cinfo.mem->free_pool)((j_common_ptr) &cinfo, JPOOL_IMAGE);

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);


	fclose(file);

	return true;
}

//--------------------------------------------------------------------------//
