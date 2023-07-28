#include "Image.hpp"
#include <iostream>
using namespace Orange;
using namespace std;

int main(int argc, char** argv)
{
    Image image(3,2);
    image.SetColor(0, 0, Color::Red());
    image.SetColor(1, 0, Color::Green());
    image.SetColor(2, 0, Color::Blue());
    image.SetColor(0, 1, Color::Yellow());
    image.SetColor(1, 1, Color::White());
    image.SetColor(2, 1, Color::Black());
    cout << image;
    image.Save("test.ppm");
    return 0;
}