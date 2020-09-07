#ifndef BITMAP_HPP
#define BITMAP_HPP

#include <string>
#include <vector>

namespace library
{
class Bitmap
{
public:
    typedef unsigned int rgba8_t;

    // supported file-formats
    enum bitmap_type
    {
        BMP,
        PNG
    };

    // constructors
    Bitmap() = default;
    Bitmap(const std::string file, bitmap_type btype);
    Bitmap(int w, int h);
    Bitmap(int w, int h, rgba8_t color);
    Bitmap(int w, int h, const rgba8_t* data, int glformat = 0x80E1);

    // assignment operator (creates new copy)
    Bitmap& operator=(const Bitmap& bmp);
    // copy constructor (creates new copy)
    Bitmap(const Bitmap& bmp) { this->operator=(bmp); }

    // getters
    int getWidth() const { return this->width; }
    int getHeight() const { return this->height; }
    int getFormat() const { return this->format; }

    // get pixel at (x, y)
    rgba8_t getPixel(int x, int y) const { return buffer[y * getWidth() + x]; }
    // set pixel at (x, y)
    void setPixel(int x, int y, rgba8_t color) { buffer[y * getWidth() + x] = color; }

    const rgba8_t* data() const { return this->buffer.data(); }
    rgba8_t* data() { return this->buffer.data(); }

    int getTilesX() const noexcept { return this->tilesX; }
    int getTilesY() const noexcept { return this->tilesY; }
    int getTileCount() const noexcept { return getTilesX() * getTilesY(); }

    // isValid: returns false if the bitmap is currently invalid
    bool isValid() const;

    // operations
    void clear(rgba8_t color);
    void replace(rgba8_t color, rgba8_t replacecolor);
    void blit(Bitmap& dest, int srcX, int srcY, int width, int height, int dstX, int dstY) const;

    // tile operations
    void parse2D(int, int, bool invert_y = false);
    void convert_to_tilesheet(int tile_size, uint32_t tile_zero_color);
    void add_tile(const Bitmap&, int tx, int ty);

    Bitmap rotate90() const;
    Bitmap flipX() const;
    Bitmap flipY() const;

private:
    void loadBMP(const std::string& file);
    void loadPNG(const std::string& file);

    std::vector<rgba8_t> buffer;
    int width = 0, height = 0;
    int format;
    int tilesX = 0;
    int tilesY = 0;
};

} // namespace library

#endif
