#include "Model.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace Orange
{

Model::Model(const char *filename)
{
    std::ifstream ifs;
    ifs.open(filename, std::ifstream::in);
    if(ifs.fail()) throw std::runtime_error("File not found!");
    std::string line;
    while (!ifs.eof())          
    {           
        std::getline(ifs, line);
        std::istringstream iss(line);
        char trash;
        if(!line.compare(0, 2, "v "))
        {
            iss >> trash;
            Vec3f v;
            for(int i=0; i<3; ++i)
                iss >> v.Raw[i];
            vertices.push_back(v);
        }
        else if(!line.compare(0, 2, "f "))
        {
            std::vector<int> f;
            int itrash, idx;
            iss >> trash;
            while (iss >> idx >> trash >> itrash >> trash >> itrash)
            {
                idx--; // (1,n) => (0, n-1)
                f.push_back(idx);
            }
            faces.push_back(f);
        }
    }
    ifs.close();
}

std::vector<Vec3f> Model::Vertices() const
{
    return this->vertices;
}

std::vector<std::vector<int>> Model::Faces() const
{
    return this->faces;
}


}