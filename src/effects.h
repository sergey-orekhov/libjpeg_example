#ifndef _EFFECTS_H_
#define _EFFECTS_H_

#include "stdbool.h"
#include "stdint.h"

//--------------------------------------------------------------------------//

/**
 * makes monochrome picture from RGB
 *
 * raw_data    - pointer to pointer to buffer, that will keep raw image data in case of success
 * width       - pointer to variable that stores the value of picture width
 * height      - pointer to variable that stores the value of picture height
 * components  - pointer to variable that stores the number of color components and will be filled by new value in case of success
 * color_space - pointer to variable that stores the value of color space enumeration and will be fiiled by new value in case of success
 *
 * returns true in case of success, false otherwise
 */
bool make_monochrome(uint8_t** raw_data,
                     uint32_t* width,
                     uint32_t* height,
                     int* components,
                     int* color_space);

//--------------------------------------------------------------------------//

/**
 * mirrors the image horizontelly
 *
 * raw_data    - pointer to pointer to buffer, that will keep raw image data and will be filled by new values in case of success
 * width       - pointer to variable that stores the value of picture width
 * height      - pointer to variable that stores the value of picture height
 * components  - pointer to variable that stores the number of color components
 * color_space - pointer to variable that stores the value of color space enumerations
 *
 * returns true in case of success, false otherwise
 */
bool mirror(uint8_t** raw_data,
            uint32_t* width,
            uint32_t* height,
            int* components,
            int* color_space);

//--------------------------------------------------------------------------//

/**
 * Sets a scale value that will be used by resize function.
 *
 * scale       - scale factor
 */
void set_scale(float scale);

//--------------------------------------------------------------------------//

/**
 * mirrors the image horizontelly
 *
 * raw_data    - pointer to pointer to buffer, that will keep raw image data and will be filled by new values in case of success
 * width       - pointer to variable that stores the value of picture width and will be filled by new values in case of success
 * height      - pointer to variable that stores the value of picture height and will be filled by new values in case of success
 * components  - pointer to variable that stores the number of color components
 * color_space - pointer to variable that stores the value of color space enumerations
 *
 * returns true in case of success, false otherwise
 */
bool resize(uint8_t** raw_data,
            uint32_t* width,
            uint32_t* height,
            int* components,
            int* color_space);

//--------------------------------------------------------------------------//

#endif // _EFFECTS_H_
