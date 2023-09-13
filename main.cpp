#include "Image.hpp"
#include "Model.hpp"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <array>
#include <vector>

const int width = 800;
const int height = 800;

char *modelPath{nullptr};
char *outputPath{nullptr};

enum class RenderMode
{
    PointCloud,
    WireFrame,
};

RenderMode renderMode{RenderMode::WireFrame};

void line(int x0, int y0, int x1, int y1, Orange::Image &image, Orange::Color color = Orange::Color::White())
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
    argv_process(argc, argv);
    
    Orange::Model model(modelPath);
    std::vector<std::vector<int>> faces = model.Faces();
    std::vector<Orange::Vec3f> vertices = model.Vertices();
    
    Orange::Image image(width, height, Orange::Color::Black());

    switch (renderMode)
    {   
    case RenderMode::PointCloud:
        for(int i=0; i<vertices.size(); ++i)
        {
            int x = ((vertices[i].X + 1) / 2 ) * (width-1);
            int y = ((vertices[i].Y + 1) / 2 ) * (height-1);
            image.SetColor(x, y, Orange::Color::White());
        }
        break;
    case RenderMode::WireFrame:
        for(int i=0; i<faces.size(); ++i)
        {
            std::vector<int> face = faces[i];
            for(int j=0; j<3; ++j)
            {
                Orange::Vec3f v0 = vertices[face[j]];
                Orange::Vec3f v1 = vertices[face[(j+1)%3]];
                int x0 = ((v0.X+1)/2)*(width-1);
                int y0 = ((v0.Y+1)/2)*(height-1);
                int x1 = ((v1.X+1)/2)*(width-1);
                int y1 = ((v1.Y+1)/2)*(height-1);
                line(x0, y0, x1, y1, image, Orange::Color::White());
            }
        }
        break;
    default:
        break;
    }   
    image.FlipVertical();
    image.Save(outputPath);
    return 0;
}