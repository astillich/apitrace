/**************************************************************************
 *
 * Copyright 2008-2010 VMware, Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 **************************************************************************/

/*
 * Image I/O.
 */

#ifndef _IMAGE_HPP_
#define _IMAGE_HPP_


#include <iostream>


namespace image {


enum ChannelType {
    TYPE_UNORM8 = 0,
    TYPE_FLOAT
};


class Image {
public:
    unsigned width;
    unsigned height;
    unsigned channels;
    ChannelType channelType;
    unsigned bytesPerPixel;

    // Flipped vertically or not
    bool flipped;

    // Pixels in RGBA format
    unsigned char *pixels;

    inline Image(unsigned w, unsigned h, unsigned c = 4, bool f = false, ChannelType t = TYPE_UNORM8) :
        width(w),
        height(h),
        channels(c),
        channelType(t),
        bytesPerPixel(channels * (t == TYPE_FLOAT ? 4 : 1)),
        flipped(f),
        pixels(new unsigned char[h*w*bytesPerPixel])
    {}

    inline ~Image() {
        delete [] pixels;
    }

    // Absolute stride
    inline unsigned
    _stride() const {
        return width*bytesPerPixel;
    }

    inline unsigned char *start(void) {
        return flipped ? pixels + (height - 1)*_stride() : pixels;
    }

    inline const unsigned char *start(void) const {
        return flipped ? pixels + (height - 1)*_stride() : pixels;
    }

    inline unsigned char *end(void) {
        return flipped ? pixels - _stride() : pixels + height*_stride();
    }

    inline const unsigned char *end(void) const {
        return flipped ? pixels - _stride() : pixels + height*_stride();
    }

    inline signed stride(void) const {
        return flipped ? -(signed)_stride() : _stride();
    }

    bool
    writeBMP(const char *filename) const;

    void
    writePNM(std::ostream &os, const char *comment = NULL) const;

    bool
    writePNM(const char *filename, const char *comment = NULL) const;

    bool
    writePNG(std::ostream &os) const;

    bool
    writePNG(const char *filename) const;

    void
    writeRAW(std::ostream &os) const;

    bool
    writeRAW(const char *filename) const;
};


Image *
readPNG(std::istream &is);

Image *
readPNG(const char *filename);

const char *
readPNMHeader(const char *buffer, size_t size, unsigned *channels, unsigned *width, unsigned *height);


} /* namespace image */


#endif /* _IMAGE_HPP_ */
