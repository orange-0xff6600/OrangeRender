#include "Image.hpp"
#include "Model.hpp"
#include <iostream>
#include <array>
#include <vector>
using namespace Orange;
using namespace std;

const int width = 800;
const int height = 800;

void line(int x0, int y0, int x1, int y1, Image &image, Color color = Color::White())
{
    bool steep = false; 
    if (std::abs(x0-x1)<std::abs(y0-y1)) { 
        std::swap(x0, y0); 
        std::swap(x1, y1); 
        steep = true; 
    } 
    if (x0>x1) { 
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    } 
    int dx = x1-x0; 
    int dy = y1-y0; 
    int derror2 = std::abs(dy)*2; 
    int error2 = 0; 
    int y = y0; 
    for (int x=x0; x<=x1; x++) { 
        if (steep) { 
            image.SetColor(y, x, color); 
        } else { 
            image.SetColor(x, y, color); 
        } 
        error2 += derror2; 
        if (error2 > dx) { 
            y += (y1>y0?1:-1); 
            error2 -= dx*2; 
        } 
    }  
} 

int main(int argc, char** argv)
{
    Model model("obj/african_head.obj");
    Image image(width, height, Color::Black());
    // std::vector<Vec3f> vertices = model.Vertices();
    // for(int i=0; i<vertices.size(); ++i)
    // {
    //     int x = ((vertices[i].X + 1) / 2 ) * (width-1);
    //     int y = ((vertices[i].Y + 1) / 2 ) * (height-1);
    //     image.SetColor(x, y, Color::White());
    // }
    // image.FlipVertical();
    //  image.Save("line.ppm");
    std::vector<std::vector<int>> faces = model.Faces();
    std::vector<Vec3f> vertices = model.Vertices();
    for(int i=0; i<faces.size(); ++i)
    {
        std::vector<int> face = faces[i];
        for(int j=0; j<3; ++j)
        {
            Vec3f v0 = vertices[face[j]];
            Vec3f v1 = vertices[face[(j+1)%3]];
            int x0 = ((v0.X+1)/2)*(width-1);
            int y0 = ((v0.Y+1)/2)*(height-1);
            int x1 = ((v1.X+1)/2)*(width-1);
            int y1 = ((v1.Y+1)/2)*(height-1);
            line(x0, y0, x1, y1, image, Color::White());
        }
    }
    image.FlipVertical();
    image.Save("frame.ppm");
    return 0;
}