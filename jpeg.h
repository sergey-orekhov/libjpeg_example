#ifndef _JPEG_H_
#define _JPEG_H_

#include "stdbool.h"
#include "stdint.h"

//--------------------------------------------------------------------------//

/**
 * Decodes jpeg image from file using libjpeg library.
 *
 * file_name   - image file name
 * raw_data    - pointer to pointer to buffer, that will keep raw image data in case of success
 * width       - pointer to variable that will store the value of picture width in case of success
 * height      - pointer to variable that will store the value of picture height in case of success
 * components  - pointer to variable that will store the number of color components in case of success
 * color_space - pointer to variable that will store the value of color space enumeration in case of success
 *
 * returns true in case of success, false otherwise
 */
bool decode_image_file(const char* file_name,
                       uint8_t** raw_data,
                       uint32_t* width,
                       uint32_t* height,
                       int* components,
                       int* color_space);

//--------------------------------------------------------------------------//

/**
 * Encodes raw image data to jpeg file libjpeg library.
 *
 * file_name   - image file name, in case if not exists will be created
 * raw_data    - pointer to buffer, that keeps raw image data
 * width       - the value of picture width
 * height      - the value of picture height
 * components  - the number of color components
 * color_space - the value of color space enumeration
 *
 * returns true in case of success, false otherwise
 */
bool encode_image_file(const char* file_name,
                       uint8_t* raw_data,
                       uint32_t width,
                       uint32_t height,
                       int components,
                       int color_space,
                       int quality);

//--------------------------------------------------------------------------//

#endif // _JPEG_H_
