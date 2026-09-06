#ifndef MESHQUALITY_H
#define MESHQUALITY_H

#include "Mesh.h"

#include <vector>

struct CellQuality
{
    int id;

    double signedArea;

    double minJacobian;

    double aspectRatio;

    bool inverted;

    bool lowQuality;
};

class MeshQuality
{
public:

    //--------------------------------------------------
    // Quality Evaluation
    //--------------------------------------------------

    static std::vector<CellQuality> evaluate
    (
        const Mesh& mesh,
        double aspectRatioLimit
    );

    //--------------------------------------------------
    // Reporting
    //--------------------------------------------------

    static void report(const std::vector<CellQuality>& results);

    static bool hasInvertedCells(const std::vector<CellQuality>& results);

    static bool hasLowQualityCells(const std::vector<CellQuality>& results);

private:

    static double computeSignedArea
    (
        const Point& p0,
        const Point& p1,
        const Point& p2,
        const Point& p3
    );

    static double computeCornerJacobian
    (
        const Point& prev,
        const Point& curr,
        const Point& next
    );

    static double computeAspectRatio
    (
        const Point& p0,
        const Point& p1,
        const Point& p2,
        const Point& p3
    );
};

#endif