#include "Image.hpp"
#include "Model.hpp"
#include <iostream>
#include <array>
#include <vector>
using namespace Orange;
using namespace std;

const int width = 800;
const int height = 800;

int main(int argc, char** argv)
{
    Model model("obj/african_head.obj");
    Image image(width, height, Color::Black());
    std::vector<Vec3f> vertices = model.Vertices();
    for(int i=0; i<vertices.size(); ++i)
    {
        int x = ((vertices[i].X + 1) / 2 ) * (width-1);
        int y = ((vertices[i].Y + 1) / 2 ) * (height-1);
        image.SetColor(x, y, Color::White());
    }
    image.FlipVertical();
    image.Save("test.ppm");    
    return 0;
}