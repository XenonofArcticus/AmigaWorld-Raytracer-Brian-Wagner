/**
 * @file write.h
 * @brief Functions for saving image data to files in RGB or PNG format.
 * 
 * This file contains functions that handle saving the rendered image data 
 * to output files. The image can either be saved as separate RGB files 
 * or as a PNG file if PNG support is enabled. The file uses external 
 * libraries for PNG encoding and supports writing images in RGB format 
 * by interleaving data from separate red, green, and blue buffers.
 */

/**
 * @brief Saves the image to a file, either in PNG or RGB format.
 * 
 * Depending on whether PNG support is enabled (`OUTPUT_PNG`), this function 
 * saves the rendered image data either as a PNG file or as three separate 
 * RGB files. The function acts as a wrapper for the appropriate file-saving 
 * method based on compile-time configuration.
 * 
 * @param name The base name of the output file.
 */
void saveImageToFile(char *name);


/**
 * @brief Saves the image as separate RGB files.
 * 
 * This function writes the rendered image data to three separate files: 
 * one for the red channel, one for the green channel, and one for the blue 
 * channel. The function writes each buffer (red, green, blue) to a file with 
 * the corresponding suffix (`.red`, `.grn`, `.blu`).
 * 
 * @param name The base name of the output file.
 */
void    writeRGB(char *name);

#ifdef OUTPUT_PNG
/**
 * @brief Saves the image to a PNG file.
 * 
 * This function writes the rendered image data to a PNG file. It interleaves 
 * the red, green, and blue buffers into a single array, then uses the SPNG 
 * library to encode the image into PNG format. The image properties are set 
 * according to the current screen dimensions, and the PNG is finalized and 
 * written to disk.
 * 
 * @param name The base name of the output file.
 * @return int Returns 0 on success, or a non-zero value if an error occurs.
 */
int writePNG(char *name);
#endif // OUTPUT_PNG
