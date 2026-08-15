#ifndef MESHSTATISTICS_H
#define MESHSTATISTICS_H

#include "Mesh.h"
#include "Inflation.h"

class MeshStatistics
{

public:

    static void print(const Mesh& mesh);
    static void print(const Inflation& inflation);

};

#endif