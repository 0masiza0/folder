#include <iostream>
#include "CImg.h"           //that library provides toolkit for image processing
#include <map>
#include <unordered_map>
#include <vector>

using namespace cimg_library;

// A nickname for std::vector<unsigned char>
using chars = std::vector<unsigned char>;

int main() {
    //  Define input file path
    const char* input_path = "C:\\Users\\Ian\\CLionProjects\\colours\\masiza.bmp";

    // "image" is an input file,
    // "most_common_colour" is an output with size (x = 10, y = 10, z = 1)
    // and dimension of colours = 3 (RGB)
    CImg<unsigned char> image(input_path);
    CImg<unsigned char> most_common_colour(10, 10, 1, 3);

    //  Define mp of colour frequencies
    std::map<chars, size_t> freqs;

    //  Define variables for RGB values
    unsigned char red, green, blue;

    //  In the loop:
    //  Search for the most common colour,
    //  Put the frequency in max_freq,
    //  Put the colour in max_freq_in_chars
    size_t max_freq = 0;
    chars max_freq_in_chars;
    for (size_t i = 0; i < image.width(); ++i) {
        for (size_t j = 0; j < image.height(); ++j) {
            //  Retrieve colour components at pixel (i, j)
            std::tie(red, green, blue) = std::tie(image(i, j, 0, 0),
                                                  image(i, j, 0, 1),
                                                  image(i, j, 0, 2));
            ++freqs[{red, green, blue}];
            // Check if freqs_of_block[...] could be the max frequency
            if (freqs[{red, green, blue}] > max_freq) {
                max_freq_in_chars = {red, green, blue};
                max_freq = freqs[{red, green, blue}];
            }
        }
    }

    //  Fill the output display with black colour
    most_common_colour.fill(0);

    //  Gather colour components to one variable
    unsigned char col[] = {max_freq_in_chars[0], max_freq_in_chars[1], max_freq_in_chars[2]};

    //  Draw a rectangle with (0, 0) coordinates of upper-left corner,
    //  (5, 5) coordinates of lower-right corner,
    //  and colour col
    //most_common_colour.draw_rectangle(0, 0, 5, 5, col);

    /*
     *  I have no idea why, but when I use line 66 instead of line 58,
     *  the printed colour looks very different, though both colours have the same RGB code.
     *  Maybe the problem is only in my display. (ine 66 just fills the region with colour col)
     */

    most_common_colour.draw_fill(0, 0, col);

    //  Show the answer
    most_common_colour.display("Most Common Colour");

    return 0;
}
