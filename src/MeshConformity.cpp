#include "MeshConformity.h"
#include <algorithm>

std::vector<const Point*> MeshConformity::getBoundary
(
    const Mesh& mesh, 
    Boundary b
) 
{
    std::vector<const Point*> edgeNodes;
    int nodesX = mesh.Nx + 1;
    int nodesY = mesh.Ny + 1;

    switch (b) {
        case Boundary::Bottom: 
            for(int i = 0; i < nodesX; i++) edgeNodes.push_back(&mesh.nodes[i]);
            break;
        case Boundary::Top:    
            for(int i = 0; i < nodesX; i++) edgeNodes.push_back(&mesh.nodes[mesh.Ny * nodesX + i]);
            break;
        case Boundary::Left:   
            for(int j = 0; j < nodesY; j++) edgeNodes.push_back(&mesh.nodes[j * nodesX]);
            break;
        case Boundary::Right:  
            for(int j = 0; j < nodesY; j++) edgeNodes.push_back(&mesh.nodes[j * nodesX + mesh.Nx]);
            break;
    }
    return edgeNodes;
}

bool MeshConformity::checkInterface
(
    const Mesh& mesh1,
    Boundary b1,
    const Mesh& mesh2,
    Boundary b2
)
{
    auto edge1 = getBoundary(mesh1, b1);
    auto edge2 = getBoundary(mesh2, b2);

    if (edge1.size() != edge2.size()) return false;

    for(const Point* p1 : edge1){

        auto isMatch = [p1](const Point* p2) {
            return std::abs(p1->x - p2->x) < 1e-6 && std::abs(p1->y - p2->y) < 1e-6;
        };

        if(std::find_if(edge2.begin(), edge2.end(), isMatch) == edge2.end()){
            return false;
        }
    }

    return true;
}

bool MeshConformity::checkConformity
(
    const Mesh& mesh1,
    const Mesh& mesh2
)
{
    std::vector<Boundary> all_boundaries = {
            Boundary::Top, Boundary::Bottom, Boundary::Left, Boundary::Right
        };

        for (Boundary b1 : all_boundaries) 
        {
            for (Boundary b2 : all_boundaries) 
            {
                if (checkInterface(mesh1, b1, mesh2, b2)) 
                {
                    return true;
                }
            }
        }
        
        return false;
}