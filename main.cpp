#include "Image.hpp"
#include "Model.hpp"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <array>
#include <vector>

const int width = 200;
const int height = 200;

char *modelPath{nullptr};
char *outputPath{nullptr};

enum class RenderMode
{
    PointCloud,
    WireFrame,
};

RenderMode renderMode{RenderMode::WireFrame};

void line(int x0, int y0, int x1, int y1, 
    Orange::Image &image, Orange::Color color = Orange::Color::White())
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

void line(Orange::Vec2i v0, Orange::Vec2i v1, 
    Orange::Image &image, Orange::Color color = Orange::Color::White())
{
    line(v0.X, v0.Y, v1.X, v1.Y, image, color);
}

void triangle(Orange::Vec2i v0, Orange::Vec2i v1,  Orange::Vec2i v2,
    Orange::Image &image, Orange::Color color = Orange::Color::White())
{
    if(v0.Y==v1.Y && v0.Y==v2.Y) return;
    if(v0.Y>v1.Y) std::swap(v0, v1);
    if(v0.Y>v2.Y) std::swap(v0, v2);
    if(v1.Y>v2.Y) std::swap(v1, v2);
    int total_height = v2.Y - v0.Y;
    for(int i=0; i<total_height; ++i)
    {
        bool second_half = i>(v1.Y-v0.Y) || v1.Y==v0.Y;
        int segment_height = second_half ? v2.Y-v1.Y : v1.Y-v0.Y;
        float alpha = (float)i/total_height;
        float beta = (float)(i-(second_half?v1.Y-v0.Y:0))/segment_height;
        int x1 = v0.X + (v2.X-v0.X)*alpha;
        int x2 = second_half?v1.X+(v2.X-v1.X)*beta:v0.X+(v1.X-v0.X)*beta;
        if(x1>x2) std::swap(x1, x2);
        for(int j=x1; j<x2; ++j)
            image.SetColor(j, i+v0.Y, color);
    }
}


// argv:
// 0: app_name; 1: model_path; 2: output_path
// -m : model_path
// -o : output_path
// -p --point: point_cloud
// -w --wire : wireframe
// -h --help : help
int argv_process(int argc, char** argv)
{
    for(int i=1; i<argc; ++i)
    {
        if(!strcmp(argv[i], "-m"))
        {
            modelPath = argv[i+1];
            ++i;
        }
        else if(!strcmp(argv[i], "-o"))
        {
            outputPath = argv[i+1];
            ++i;
        }
        else if(!strcmp(argv[i], "-p"))
        {
            renderMode = RenderMode::PointCloud;
        }
        else if(!strcmp(argv[i], "-w"))
        {
            renderMode = RenderMode::WireFrame;
        }
        else if(!strcmp(argv[i], "-h"))
        {
            puts("Usage: render -m [model_path] -o [output_path] <other_args>...");
            puts("Options:");
            puts("\t-m\tModel path");
            puts("\t-o\tOutput path");
            puts("\t-h\tShow help");
            puts("\tRender Modes:");
            puts("\t\t-p\tPoint cloud");
            puts("\t\t-w\tWireframe");
            exit(0);
        }
    }
    if(modelPath==nullptr || outputPath==nullptr)
    {
        puts("Usage: render -m [model_path] -o [output_path] <other_args>...");
        puts("More Info: render -h");
        exit(1);
    }
}

int main(int argc, char** argv)
{
    // argv_process(argc, argv);
    
    // Orange::Model model(modelPath);
    // std::vector<std::vector<int>> faces = model.Faces();
    // std::vector<Orange::Vec3f> vertices = model.Vertices();
    
    Orange::Image image(width, height, Orange::Color::Black());

    // switch (renderMode)
    // {   
    // case RenderMode::PointCloud:
    //     for(int i=0; i<vertices.size(); ++i)
    //     {
    //         int x = ((vertices[i].X + 1) / 2 ) * (width-1);
    //         int y = ((vertices[i].Y + 1) / 2 ) * (height-1);
    //         image.SetColor(x, y, Orange::Color::White());
    //     }
    //     break;
    // case RenderMode::WireFrame:
    //     for(int i=0; i<faces.size(); ++i)
    //     {
    //         std::vector<int> face = faces[i];
    //         for(int j=0; j<3; ++j)
    //         {
    //             Orange::Vec3f v0 = vertices[face[j]];
    //             Orange::Vec3f v1 = vertices[face[(j+1)%3]];
    //             int x0 = ((v0.X+1)/2)*(width-1);
    //             int y0 = ((v0.Y+1)/2)*(height-1);
    //             int x1 = ((v1.X+1)/2)*(width-1);
    //             int y1 = ((v1.Y+1)/2)*(height-1);
    //             line(x0, y0, x1, y1, image, Orange::Color::White());
    //         }
    //     }
    //     break;
    // default:
    //     break;
    // }  

    Orange::Vec2i t0[3] = {Orange::Vec2i(10, 70), Orange::Vec2i(50, 160), Orange::Vec2i(70, 80)}; 
    Orange::Vec2i t1[3] = {Orange::Vec2i(180, 50), Orange::Vec2i(150, 1), Orange::Vec2i(70, 180)}; 
    Orange::Vec2i t2[3] = {Orange::Vec2i(180, 150), Orange::Vec2i(120, 160), Orange::Vec2i(130, 180)};

    triangle(t0[0], t0[1], t0[2], image, Orange::Color::Red());
    triangle(t1[0], t1[1], t1[2], image, Orange::Color::White());
    triangle(t2[0], t2[1], t2[2], image, Orange::Color::Green());
    
    image.FlipVertical();
    //image.Save(outputPath);
    image.Save("triangle.ppm");
    return 0;
}