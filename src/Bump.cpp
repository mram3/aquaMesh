#include "Bump.h"

#include <iostream>
#include <algorithm>
#include <cmath>
#include <stdexcept> // for throwing errors
#include <string>

using namespace std;

Bump::Bump()
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

void Bump::computeBoundingBox(const Surface& surface)
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

void Bump::computeCoarseNodes(){
    double L_layer = h0 * (pow(r,Nlayer) - 1)/(r-1);
    double dytop = h0 * pow(r, Nlayer-1);
    Ny = floor(((ymax/2)-L_layer) / dytop);
    while(Ny <= 1){
        Nlayer = Nlayer - 1;
        L_layer = h0 * (pow(r,Nlayer) - 1)/(r-1);
        dytop = h0 * pow(r, Nlayer-1);
        Ny = floor(((ymax/2)-L_layer) / dytop);
    }
    dy = ((ymax/2)-L_layer)/ Ny;
}

//------------------------------------------------------------//
// Generate Inflated Nodes
//------------------------------------------------------------//

void Bump::writeNodes(){
    int id = 0;
    double y, iy = 2*dy;

    for(int j = 0;j<=Nlayer+Ny;j++){
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

    for(int j = 1; j<=Nlayer+Ny; j++){
        if(j <= Ny){
            y = (ymax/2) + j*dy;
        }
        else{
            iy = iy + h0 * pow(r, Nlayer+Ny-j);
            y = (ymax/2) + iy;
        }

        for(int i = 0;i<=Nx;i++){

            double x = xmin + i*dx;

            nodes.emplace_back(id, x, y);

            id++;
        }
    }
}

void Bump::generateInflatedNodes(){

    if(r > 1){
        nodes.clear();
        computeCoarseNodes();
        nodes.reserve((2*(Nlayer+Ny)+1)*(Nx+1));
        writeNodes();
    }

    else if(r == 1){
        nodes.clear();
        Ny = Nlayer;
        Nlayer = 0;
        dy = ymax/(2*Ny);
        nodes.reserve((Nx+1)*(2*Ny+1));
        
        int id = 0;

        for(int j=0;j<=2*Ny;j++)
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

void Bump::generateInflatedCells()
{
    cells.clear();

    cells.reserve(Nx*2*(Nlayer+Ny));

    int id = 0;

    for(int j=0;j<2*(Nlayer+Ny);j++)
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

void Bump::generateInflation
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

int Bump::getNumberOfNodes() const
{
    return static_cast<int>(nodes.size());
}


//------------------------------------------------------------//
// Number of Cells
//------------------------------------------------------------//

int Bump::getNumberOfCells() const
{
    return static_cast<int>(cells.size());
}