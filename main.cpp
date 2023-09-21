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
    Fill,
    Flat,
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

Orange::Vec3f barycentric(Orange::Vec2i v0, Orange::Vec2i v1, Orange::Vec2i v2, Orange::Vec2i p)
{
    Orange::Vec3f uv1 = Cross(Orange::Vec3f(v2.X - v0.X, v1.X - v0.X, v0.X - p.X),
        Orange::Vec3f(v2.Y - v0.Y, v1.Y - v0.Y, v0.Y - p.Y));
    if(std::abs(uv1.Z < 1)) return Orange::Vec3f(-1, 1, 1);
    return Orange::Vec3f(1.0f - (uv1.X + uv1.Y) / uv1.Z, uv1.Y / uv1.Z, uv1.X / uv1.Z);
}

void triangle(Orange::Vec2i v0, Orange::Vec2i v1, Orange::Vec2i v2,
    Orange::Image& image, Orange::Color color = Orange::Color::White())
{
    Orange::Vec2i BBoxMin(image.Width() - 1, image.Height() - 1);
    Orange::Vec2i BBoxMax(0, 0);

    BBoxMin.X = std::min(v0.X, v1.X);
    BBoxMin.X = std::min(BBoxMin.X, v2.X);
    BBoxMin.X = std::max(0, BBoxMin.X);
	
    BBoxMin.Y = std::min(v0.Y, v1.Y);
	BBoxMin.Y = std::min(BBoxMin.Y, v2.Y);
	BBoxMin.Y = std::max(0, BBoxMin.Y);

    BBoxMax.X = std::max(v0.X, v1.X);
    BBoxMax.X = std::max(BBoxMax.X, v2.X);
    BBoxMax.X = std::min(image.Width() - 1, BBoxMax.X);
    
	BBoxMax.Y = std::max(v0.Y, v1.Y);
	BBoxMax.Y = std::max(BBoxMax.Y, v2.Y);
	BBoxMax.Y = std::min(image.Height() - 1, BBoxMax.Y);

    Orange::Vec2i P;
    for (P.X = BBoxMin.X; P.X <= BBoxMax.X; P.X++)
    {
        for (P.Y = BBoxMin.Y; P.Y <= BBoxMax.Y; P.Y++)
        {
            Orange::Vec3f bc = barycentric(v0, v1, v2, P);
            if(bc.X<0 || bc.Y<0 || bc.Z<0)
                continue;
            image.SetColor(P.X, P.Y, color);
        }
    }
}

//void triangle(Orange::Vec2i v0, Orange::Vec2i v1,  Orange::Vec2i v2,
//    Orange::Image &image, Orange::Color color = Orange::Color::White())
//{
//    if(v0.Y==v1.Y && v0.Y==v2.Y) return;
//    if(v0.Y>v1.Y) std::swap(v0, v1);
//    if(v0.Y>v2.Y) std::swap(v0, v2);
//    if(v1.Y>v2.Y) std::swap(v1, v2);
//    int total_height = v2.Y - v0.Y;
//    for(int i=0; i<total_height; ++i)
//    {
//        bool second_half = i>(v1.Y-v0.Y) || v1.Y==v0.Y;
//        int segment_height = second_half ? v2.Y-v1.Y : v1.Y-v0.Y;
//        float alpha = (float)i/total_height;
//        float beta = (float)(i-(second_half?v1.Y-v0.Y:0))/segment_height;
//        int x1 = v0.X + (v2.X-v0.X)*alpha;
//        int x2 = second_half?v1.X+(v2.X-v1.X)*beta:v0.X+(v1.X-v0.X)*beta;
//        if(x1>x2) std::swap(x1, x2);
//        for(int j=x1; j<x2; ++j)
//            image.SetColor(j, i+v0.Y, color);
//    }
//}


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
        else if(!strcmp(argv[i], "-f"))
        {
            renderMode = RenderMode::Fill;
        }
        else if(!strcmp(argv[i], "-F"))
        {
            renderMode = RenderMode::Flat;
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
            puts("\t\t-f\tfill");
            puts("\t\t-F\tFlat Shade");
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
    
    Orange::Image image(width, height, Orange::Color::Black());

    Orange::Model model(modelPath);
    std::vector<std::vector<int>> faces = model.Faces();
    std::vector<Orange::Vec3f> vertices = model.Vertices();
    
    Orange::Vec3f light(0, 0, -1);

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
    case RenderMode::Fill:
        for(int i=0; i<faces.size(); ++i)
        {
            std::vector<int> face = faces[i];
            Orange::Vec2i screenCoords[3];
            for (int j = 0; j < 3; j++)
            {
                Orange::Vec3f worldCoords = vertices[face[j]];
                screenCoords[j] = {(int)((worldCoords.X+1)/2)*width,
                    (int)((worldCoords.Y+1)/2)*height};
            }
            triangle(screenCoords[0], screenCoords[1], screenCoords[2],
                image, Orange::Color::White());
        }
        break;
    case RenderMode::Flat:
        for(int i=0;i<faces.size(); ++i)
        {
            std::vector<int> face = faces[i];
            Orange::Vec2i screenCoords[3];
            Orange::Vec3f worldCoords[3];
            for(int j=0; j<3; ++j)
            {
                Orange::Vec3f v = vertices[face[j]];
                screenCoords[j] = {static_cast<int>(((v.X+1)/2)*width), 
                    static_cast<int>(((v.Y+1)/2)*height)};
                worldCoords[j] = v;
            }
            Orange::Vec3f normal = Orange::Cross(worldCoords[2]-worldCoords[0], worldCoords[1]-worldCoords[0]);
            normal.Norm();
            float intensity = Orange::Dot(normal, light);
            if (intensity>0)
                printf("face[%d]: %f\n", i, intensity);
                triangle(screenCoords[0], screenCoords[1], screenCoords[2], image, 
                    Orange::Color(intensity*255, intensity * 255, intensity * 255, 255));
        }       
        break;
    default:
        break;
    }

    image.FlipVertical();
    image.Save(outputPath);
    return 0;
}