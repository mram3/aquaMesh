#include "Inflation.h"

#include <iostream>
#include <algorithm>
#include <cmath>
#include <stdexcept> // for throwing errors
#include <string>

using namespace std;

Inflation::Inflation()
{
    xmin = 0.0;
    xmax = 0.0;

    ymin = 0.0;
    ymax = 0.0;

    Nx = 0;
    Ny = 0;

    h0 = 0;
    r = 0;
    Nlayer = 0;

    dx = 0.0;
    dy = 0.0;
}

//------------------------------------------------------------//
// Compute Bounding Box
//------------------------------------------------------------//

void Inflation::computeBoundingBox(const Surface& surface)
{
    xmin = surface.boundaries[0]->start->x;
    xmax = xmin;

    ymin = surface.boundaries[0]->start->y;
    ymax = ymin;

    for(auto line : surface.boundaries)
    {
        xmin = min(xmin,line->start->x);
        xmin = min(xmin,line->end->x);

        xmax = max(xmax,line->start->x);
        xmax = max(xmax,line->end->x);

        ymin = min(ymin,line->start->y);
        ymin = min(ymin,line->end->y);

        ymax = max(ymax,line->start->y);
        ymax = max(ymax,line->end->y);
    }

    dx = (xmax-xmin)/Nx;
}

void Inflation::computeCoarseNodes(){
    double L_layer = h0 * (pow(r,Nlayer) - 1)/(r-1);
    double dytop = h0 * pow(r, Nlayer-1);
    Ny = floor((ymax-L_layer) / dytop);
    while(Ny <= 1){
        Nlayer = Nlayer - 1;
        L_layer = h0 * (pow(r,Nlayer) - 1)/(r-1);
        dytop = h0 * pow(r, Nlayer-1);
        Ny = floor((ymax-L_layer) / dytop);
    }
    dy = (ymax-L_layer)/ Ny;
}
//------------------------------------------------------------//
// Generate Inflated Nodes
//------------------------------------------------------------//

void Inflation::writeNodes(){
    int id = 0;

    for(int j = 0;j<=Nlayer+Ny;j++){
        double y;
        if(j<=Nlayer){
            y = ymin + h0 * (pow(r,j)-1)/(r-1);
        }
        else{
            y = h0 * (pow(r,Nlayer)-1)/(r-1) + (j-Nlayer)*dy;
        }

        for(int i = 0;i<=Nx;i++){

            double x = xmin + i*dx;

            nodes.emplace_back(id, x, y);

            id++;
        }
    }
}

void Inflation::generateInflatedNodes(){

    if(r > 1){
        nodes.clear();
        computeCoarseNodes();
        nodes.reserve((Nlayer+Ny+1)*(Nx+1));
        writeNodes();
    }

    else if(r == 1){
        nodes.clear();
        Ny = Nlayer;
        Nlayer = 0;
        dy = ymax/Ny;
        nodes.reserve((Nx+1)*(Ny+1));
        
        int id = 0;

        for(int j=0;j<=Ny;j++)
        {
            double y = ymin + j*dy;
            
            for(int i=0;i<=Nx;i++)
            {
                double x = xmin + i*dx;
                nodes.emplace_back(id,x,y);
                id++;
            }
        }
    }

    else{
        throw invalid_argument("Error: r must be 1.0 or greater. You provided: " + to_string(r));
    }

}

//------------------------------------------------------------//
// Generate Inflated Cells
//------------------------------------------------------------//

void Inflation::generateInflatedCells()
{
    cells.clear();

    cells.reserve(Nx*(Nlayer+Ny));

    int id = 0;

    for(int j=0;j<Nlayer+Ny;j++)
    {
        for(int i=0;i<Nx;i++)
        {
            int n0 = j*(Nx+1)+i;

            int n1 = n0+1;

            int n2 = n1+(Nx+1);

            int n3 = n0+(Nx+1);

            cells.emplace_back
            (
                id,
                n0,
                n1,
                n2,
                n3
            );

            id++;
        }
    }
}

//------------------------------------------------------------//
// Generate Inflated Cartesian Mesh
//------------------------------------------------------------//

void Inflation::generateInflation
(
    const Surface& surface,
    double h0_,
    double r_,
    int Nlayer_,
    int Nx_
)
{
    h0 = h0_;
    r = r_;
    Nlayer = Nlayer_;
    Nx = Nx_;

    computeBoundingBox(surface);

    generateInflatedNodes();

    generateInflatedCells();

    cout << "\n";
    cout << "Inflated Mesh Generated\n";
    cout << "------------------------\n";

    cout << "Nodes : "
         << nodes.size()
         << endl;

    cout << "Cells : "
         << cells.size()
         << endl;
    
    cout << "Inflation Layers : "
         << Nlayer
         << endl;
}

//------------------------------------------------------------//
// Number of Nodes
//------------------------------------------------------------//

int Inflation::getNumberOfNodes() const
{
    return static_cast<int>(nodes.size());
}


//------------------------------------------------------------//
// Number of Cells
//------------------------------------------------------------//

int Inflation::getNumberOfCells() const
{
    return static_cast<int>(cells.size());
}