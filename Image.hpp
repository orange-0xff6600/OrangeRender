#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <iostream>

class ImageSerializer;

namespace Orange
{
    struct Color
    {
        unsigned char R{0};
        unsigned char G{0};
        unsigned char B{0};
        unsigned char A{0};
    public:
        // Constructors
        Color() = default;
        Color(unsigned char grayscale) : 
            R{grayscale}, G{grayscale}, B{grayscale}, A{255} {}
        Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) :
            R{r}, G{g}, B{b}, A{a} {}
        // Common Colors
        static Color Black() { return Color{0, 0, 0}; }
        static Color White() { return Color{255, 255, 255}; }
 
        static Color Red() { return Color{255, 0, 0}; }
        static Color Green() { return Color{0, 255, 0}; }
        static Color Blue() { return Color{0, 0, 255}; }

        static Color Magenta() { return Color{255, 0, 255}; }
        static Color Yellow() { return Color{255, 255, 0}; }
        static Color Cyan() { return Color{0, 255, 255}; }
    };

    std::ostream& operator<<(std::ostream& os, const Color& color);

    class Image
    {
    public:
        Image(int width, int height, const Color color = Color{});
        Image(int size, const Color color = Color{}) :
            Image(size, size) {}

        Image(const Image& other);

        ~Image();

        Image& operator=(const Image& other);

        int Width() { return width; }
        int Height() { return height; }

        Color& GetColor(int x, int y) { return pixels[y][x]; }
        void SetColor(int x, int y, const Color& c) { pixels[y][x] = c; }

        void Save(const std::string& path);

        friend std::ostream& operator<<(std::ostream& os, const Image& image);
    private:
        int width;
        int height;
        Color** pixels;        
    };
}

#endif