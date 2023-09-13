#include "Image.hpp"
#include <fstream>

namespace Orange
{
    std::ostream& operator<<(std::ostream& os, const Color& color)
    {
        std::cout << "Color{" << static_cast<int>(color.R)<< 
            ", " << static_cast<int>(color.G) <<
            ", " << static_cast<int>(color.B) <<
            ", " << static_cast<int>(color.A) <<
            "}";
        return os;
    }

    Image::Image(int width, int height, const Color color) :
        width{width}, height{height}, pixels{nullptr}
    {
        pixels = new Color*[height];
        for(int i=0; i<height; i++)
        {
            Color* row = new Color[width];
            for(int j=0; j<width; j++)
            {
                row[j] = color;
            }
            pixels[i] = row;
        }
    }

    Image::Image(const Image& other) :
        width{other.width}, height{other.height}, pixels{nullptr}
    {
        pixels = new Color*[height];
        for(int i=0; i<height; i++)
        {
            Color* row = new Color[width];
            for(int j=0; j<width; j++)
            {
                row[j] = other.pixels[height][j];
            }
            pixels[i] = row;
        }
    }

    Image::~Image()
    {
        for(int i=0; i<height; i++)
        {
            delete [] pixels[i];
        }
    }

    Image& Image::operator=(const Image& other)
    {
        if(this != &other)
        {
            width = other.width;
            height = other.height;
            pixels = new Color*[height];
            for(int i=0; i<height; i++)
            {
                Color* row = new Color[width];
                for(int j=0; j<width; j++)
                {
                    row[j] = other.pixels[height][j];
                }
                pixels[i] = row;
            }           
        }
        return *this;
    }

    void Image::FlipVertical()
    {
        std::size_t BytesPerLine = width * sizeof(Color);
        Color *line = new Color[width];
        int half = height / 2;
        for(int i=0; i<half; ++i)
        {
            memmove(line, pixels[i], BytesPerLine);
            memmove(pixels[i], pixels[height-1-i], BytesPerLine);
            memmove(pixels[height-1-i], line, BytesPerLine);
        }
    }

    void Image::Save(const std::string& path)
    {
        std::ofstream file;
        file.open(path, std::ios::out);
        file << "P3" << "\n";
        file << width << " " << height << "\n";
        file << "255" << "\n";
        for(int i=0; i<height; i++)
        {
            for(int j=0; j<width; j++)
            {
                file << static_cast<int>(pixels[i][j].R) << " " <<
                        static_cast<int>(pixels[i][j].G) << " " <<
                        static_cast<int>(pixels[i][j].B) << "\n";
            }
        }
        file.close();
    }

    std::ostream& operator<<(std::ostream& os, const Image& image)
    {
        std::cout << "Image" << image.width << "x" << image.height << ":\n";
        for(int i=0; i<image.height; i++)
        {
            for(int j=0; j<image.width-1; j++)
            {
                std::cout << image.pixels[i][j] << " ";
            }
            std::cout << image.pixels[i][image.width-1] << "\n";
        }
        return os;
    }
}