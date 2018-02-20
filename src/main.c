#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "jpeg.h"
#include "effects.h"

//--------------------------------------------------------------------------//

/**
 * Utility method to print a help message.
 */
void show_help() {
    printf("Usage: main [OPTION]... [FILE]...\n");
    printf("Redecode the given jpeg image file.\n");
	printf("\nOptions:\n");
	printf("  --in=<input file>\t\tspecify a source image file name\n");
	printf("  --out=<output file>\t\tspecify a destination image file name\n");
	printf("  --quality=<quality value>\tspecify a value of output image quality [1:100]\n");
	printf("  --no-color\t\t\tto monochrome image\n");
	printf("  --mirror\t\t\tto mirror image horizontally\n");
	printf("  --scale=<scale value>\t\tto scale image [0.1:10.0]\n");
	printf("  --help\t\t\tdisplay this help and exit\n");
    printf("\nExit status:\n 0\tif OK,\n 1\tin case of problem.\n");

    // TODO oupdate it
}

//--------------------------------------------------------------------------//

typedef bool (*effect)(uint8_t**, uint32_t*, uint32_t*, int*, int*);

//--------------------------------------------------------------------------//

int main(int argv, const char** argc) {
	const char* input_file_name = NULL;
	const char* output_file_name = NULL;

	int image_quality = -1;

	const uint8_t MAX_EFFECTS = 32;
	uint8_t effect_count = 0;
	effect effects[MAX_EFFECTS];
	const char* effect_names[MAX_EFFECTS];
	for (uint8_t i=0; i<MAX_EFFECTS; i++) {
		effects[i] = NULL;
		effect_names[i] = NULL;
	}

	// process options
	for (int i=1; i<argv; i++) {
		const char* arg = argc[i];

		if (strcmp(arg, "--help") == 0) {
			show_help();
			return 0;
		}
		else if (strncmp(arg, "--in=", 5) == 0) {
			input_file_name = arg + 5;
		}
		else if (strncmp(arg, "--out=", 6) == 0) {
			output_file_name = arg + 6;
		}
		else if (strncmp(arg, "--quality=", 10) == 0) {
			const char* quality_str = arg + 10;
			sscanf(quality_str, "%d", &image_quality);

			if (image_quality < 1 || image_quality > 100) {
				printf("Invalid quality value %s. Use default.\n", quality_str);
			}
		}
		else if (strcmp(arg, "--no-color") == 0) {
			if (effect_count < MAX_EFFECTS) {
				effects[effect_count] = &make_monochrome;
				effect_names[effect_count] = arg + 2;
				effect_count++;
			}
			else {
				printf("Unable to apply '--no-color' option. Too much effects.");
			}
		}
		else if (strcmp(arg, "--mirror") == 0) {
			if (effect_count < MAX_EFFECTS) {
				effects[effect_count] = &mirror;
				effect_names[effect_count] = arg + 2;
				effect_count++;
			}
			else {
				printf("Unable to apply '--no-color' option. Too much effects.");
			}
		}
		else if (strncmp(arg, "--scale=", 8) == 0) {
			const char* scale_str = arg + 8;
			float scale = 1.0;
			sscanf(scale_str, "%f", &scale);

			if (scale < 0.1 || scale > 10.0) {
				printf("Invalid scale value %s. Ignore it.\n", scale_str);
			}
			else {
				set_scale(scale);
				if (effect_count < MAX_EFFECTS) {
					effects[effect_count] = &resize;
					effect_names[effect_count] = "scale";
					effect_count++;
				}
				else {
					printf("Unable to apply '--no-color' option. Too much effects.");
				}
			}
		}
	}

	if (input_file_name == NULL) {
		printf("Not enough arguments. Source file is required.\n");
		show_help();
		return 1;
	}

	if (output_file_name == NULL) {
		output_file_name = "out.jpg";
	}

	uint8_t* raw_image_data = NULL;
	uint32_t image_width = 0;
	uint32_t image_height = 0;
	int image_components = 0;
	int image_color_space = 0;

	if (!decode_image_file(input_file_name,
	    &raw_image_data,
	    &image_width,
	    &image_height,
	    &image_components,
	    &image_color_space)) {
		if (raw_image_data != NULL) {
			free(raw_image_data);
		}

		// error message has been printed inside
		return 1;
	}

	// do trasnformations with the image
	// apply effects to the image
	for (uint8_t i=0; i<MAX_EFFECTS; i++) {
		if (effects[i] != NULL) {
			if (!(effects[i])(&raw_image_data,
				&image_width,
				&image_height,
				&image_components,
				&image_color_space)) {
				printf("Failed to apply effect '%s'\n", effect_names[i]);
			}
		}
	}

	if (!encode_image_file(output_file_name,
	    raw_image_data,
	    image_width,
	    image_height,
	    image_components,
	    image_color_space,
	    image_quality)) {
		if (raw_image_data != NULL) {
			free(raw_image_data);
		}

		// error message has been printed inside
		return 1;
	}

	if (raw_image_data != NULL) {
		free(raw_image_data);
	}

	printf("Done. file %s has been created.\n", output_file_name);
	return 0;
}

//--------------------------------------------------------------------------//
