#ifndef BUMP_H
#define BUMP_H

#include <vector>

#include "Point.h"
#include "Cell.h"
#include "Surface.h"

class Bump{

public:
    //--------------------------------------------------
    // Data
    //--------------------------------------------------

    std::vector<Point> nodes;
    std::vector<Cell> cells;

    double xmin;
    double xmax;

    double ymin;
    double ymax;

    int Nx;
    int Ny;

    double h0;
    double r;
    int Nlayer;

    double dx;
    double dy;

    //--------------------------------------------------
    // Constructor
    //--------------------------------------------------

    Bump();

    //--------------------------------------------------
    // Mesh Generation
    //--------------------------------------------------

    void generateInflation
    (
        const Surface& surface,
        double h0_,
        double r_,
        int Nlayer_,
        int Nx_
    );

    //--------------------------------------------------
    // Utility Functions
    //--------------------------------------------------

    int getNumberOfNodes() const;

    int getNumberOfCells() const;

protected:

    void computeBoundingBox(const Surface& surface);
    //void computeBoundingBox();

private:

    void generateInflatedNodes();
    void generateInflatedCells();
    void computeCoarseNodes();
    void writeNodes();
};

#endif