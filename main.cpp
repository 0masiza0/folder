#include <algorithm>
#include <iostream>
#include "CImg.h"          //that library provides toolkit for image processing
#include <map>
#include <unordered_map>
#include <vector>

using namespace cimg_library;

// A nickname for std::vector<unsigned char>
using chars = std::vector<unsigned char>;

//  Function ShrinkBlock shrinks a block of pixels to one pixel
//  (which colour is the most "popular" in the block).

//  The block belongs to image im,
//  has size (size_of_block x size_of_block) pixels
//  and (x_start, y_start) coordinates of upper-left corner.
chars ShrinkBlock(
        const CImg<unsigned char>& im,
        size_t x_start,
        size_t y_start,
        size_t size_of_block) {

    //  The block has (x_border, y_border) coordinates of lower-right corner.
    //  Assign x_border as im.width() in case (x_start + size_of_block) value is out of range.
    //  The same for y_border
    size_t x_border = std::min<size_t>(x_start + size_of_block, im.width());
    size_t y_border = std::min<size_t>(y_start + size_of_block, im.height());

    //  Define map of colour frequencies
    std::map<chars, size_t> freqs_of_block;

    //  Define variables for RGB values
    unsigned char red, green, blue;

    //  Search for the most common colour,
    //  Put the frequency in max_freq,
    //  Put the colour in max_freq_in_chars
    size_t max_freq = 0;
    chars max_freq_in_chars;
    for (size_t i = x_start; i < x_border; ++i) {
        for (size_t j = y_start; j < y_border; ++j) {
            //  Retrieve colour components at pixel (i, j)
            std::tie(red, green, blue) =
                    std::tie(im(i, j, 0, 0),
                             im(i, j, 0, 1),
                             im(i, j, 0, 2));
            ++freqs_of_block[{red, green, blue}];
            // Check if freqs_of_block[...] could be the max frequency
            if (freqs_of_block[{red, green, blue}] > max_freq) {
                max_freq_in_chars = {red, green, blue};
                max_freq = freqs_of_block[{red, green, blue}];
            }
        }
    }
    return max_freq_in_chars;
}

int main() {
    //  Define input file path
    const char* input_path = "input.bmp";

    //  q is a degree of image compression
    size_t q = 600;

    //  image is an input file,
    // display is an output with size (x = image.width() / q, y = image.height() / q, z = 1)
    // and dimension of colours = 3 (RGB)
    CImg<unsigned char> image(input_path);
    CImg<unsigned char> display(image.width() / q, image.height() / q, 1, 3);

    //  Define std::vector<std::vector<chars>> and resize it to size of display
    std::vector<std::vector<chars>> freqs_of_blocks;
    freqs_of_blocks.resize(display.height());
    for (auto& freq_of_block : freqs_of_blocks) {
        freq_of_block.resize(display.width());
    }

    //  Fill freqs_of_blocks whith frequencies
    size_t x = 0, y = 0;
    for (size_t i = 0; i + q - 1 < image.width(); i += q) {
        y = 0;
        for (size_t j = 0; j + q - 1 < image.height(); j += q) {
            freqs_of_blocks[y++][x] = ShrinkBlock(image, i, j, q);
        }
        ++x;
        std::cout << x << "\n";
    }

    //  Fill the display
    display.fill(0);
    for (size_t i = 0; i < display.width(); ++i) {
        for (size_t j = 0; j < display.height(); ++j) {
            unsigned char col[] = {freqs_of_blocks[j][i][0],
                                   freqs_of_blocks[j][i][1],
                                   freqs_of_blocks[j][i][2]};
            display.draw_rectangle(i, j, i + 1, y + 1, col);
        }
    }

    //  Show the display
    display.display("Shrank Image");
    return 0;
}
