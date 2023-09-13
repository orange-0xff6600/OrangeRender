#ifndef MODEL_HPP
#define MODEL_HPP

#include<vector>
#include<string>
#include"Math.hpp"

namespace Orange
{

class Model
{
public:
    Model() {}
    Model(const char *filename);
    Model(const std::string &filename) : Model(filename.c_str()) {}

    std::vector<Vec3f> Vertices() const;
    std::vector<std::vector<int>> Faces() const;
private:
    std::vector<Vec3f> vertices; 
    std::vector<std::vector<int>> faces;
};

} 

#endif