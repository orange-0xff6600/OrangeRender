#include "Model.hpp"
#include <fstream>
#include <sstream>

namespace Orange
{

Model::Model(const char *filename)
{
    std::ifstream ifs;
    ifs.open(filename, std::ifstream::in);
    if(ifs.fail()) return;
    std::string line;
    while (!ifs.eof())          
    {           
        std::getline(ifs, line);
        std::istringstream iss(line.c_str());
        char trash;
        if(!line.compare(0, 2, "v "))
        {
            iss >> trash;
            Vec3f v;
            for(int i=0; i<3; ++i)
                iss >> v.Raw[i];
            vertices.push_back(v);
        }
    }
    ifs.close();
}

std::vector<Vec3f> Model::Vertices() const
{
    return this->vertices;
}

size_t Model::VertNum() const
{
    return vertices.size();
}

}