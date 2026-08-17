#ifndef MESHSTATISTICS_H
#define MESHSTATISTICS_H

#include "Mesh.h"
#include "Inflation.h"
#include "Bump.h"
class MeshStatistics
{

public:

    static void print(const Mesh& mesh);
    static void print(const Inflation& inflation);
    static void print(const Bump& bump);
};

#endif