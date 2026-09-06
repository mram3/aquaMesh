//------------------------------------------------------------//
// File: MeshQuality.cpp
//
// Description:
//
// Evaluates geometric quality of every cell in a Mesh.
//
// Checks performed:
//   1. Signed area of the cell
//   2. Scaled Jacobian at each of the 4 corners
//   3. Detects inverted cells
//   4. Detects cells exceeding a specified aspect ratio limit
//   5. Reports the results
//
//------------------------------------------------------------//

#include "MeshQuality.h"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>

using namespace std;


//------------------------------------------------------------//
// Signed Area
//------------------------------------------------------------//

double MeshQuality::computeSignedArea
(
    const Point& p0,
    const Point& p1,
    const Point& p2,
    const Point& p3
)
{
    double area = 0.0;

    area += p0.x*p1.y - p1.x*p0.y;
    area += p1.x*p2.y - p2.x*p1.y;
    area += p2.x*p3.y - p3.x*p2.y;
    area += p3.x*p0.y - p0.x*p3.y;

    return 0.5*area;
}


//------------------------------------------------------------//
// Corner Jacobian
//------------------------------------------------------------//

double MeshQuality::computeCornerJacobian
(
    const Point& prev,
    const Point& curr,
    const Point& next
)
{
    double ax = curr.x-prev.x;
    double ay = curr.y-prev.y;

    double bx = next.x-curr.x;
    double by = next.y-curr.y;

    double cross = ax*by - ay*bx;

    double lenA = sqrt(ax*ax+ay*ay);
    double lenB = sqrt(bx*bx+by*by);

    double denom = lenA*lenB;

    if(denom < numeric_limits<double>::epsilon())
    {
        return 0.0;
    }

    return cross/denom;
}


//------------------------------------------------------------//
// Aspect Ratio
//------------------------------------------------------------//

double MeshQuality::computeAspectRatio
(
    const Point& p0,
    const Point& p1,
    const Point& p2,
    const Point& p3
)
{
    double e01 = sqrt((p1.x-p0.x)*(p1.x-p0.x)+(p1.y-p0.y)*(p1.y-p0.y));
    double e12 = sqrt((p2.x-p1.x)*(p2.x-p1.x)+(p2.y-p1.y)*(p2.y-p1.y));
    double e23 = sqrt((p3.x-p2.x)*(p3.x-p2.x)+(p3.y-p2.y)*(p3.y-p2.y));
    double e30 = sqrt((p0.x-p3.x)*(p0.x-p3.x)+(p0.y-p3.y)*(p0.y-p3.y));

    double width = 0.5*(e01+e23);
    double height = 0.5*(e12+e30);

    double longer = max(width,height);
    double shorter = min(width,height);

    if(shorter < numeric_limits<double>::epsilon())
    {
        return numeric_limits<double>::max();
    }

    return longer/shorter;
}


//------------------------------------------------------------//
// Evaluate
//------------------------------------------------------------//

vector<CellQuality> MeshQuality::evaluate
(
    const Mesh& mesh,
    double aspectRatioLimit
)
{
    vector<CellQuality> results;

    results.reserve(mesh.cells.size());

    for(const auto& cell : mesh.cells)
    {
        const Point& p0 = mesh.nodes[cell.nodeIDs[0]];
        const Point& p1 = mesh.nodes[cell.nodeIDs[1]];
        const Point& p2 = mesh.nodes[cell.nodeIDs[2]];
        const Point& p3 = mesh.nodes[cell.nodeIDs[3]];

        double signedArea = computeSignedArea(p0,p1,p2,p3);

        double j0 = computeCornerJacobian(p3,p0,p1);
        double j1 = computeCornerJacobian(p0,p1,p2);
        double j2 = computeCornerJacobian(p1,p2,p3);
        double j3 = computeCornerJacobian(p2,p3,p0);

        double minJacobian = min(min(j0,j1),min(j2,j3));

        double aspectRatio = computeAspectRatio(p0,p1,p2,p3);

        CellQuality quality;

        quality.id = cell.id;
        quality.signedArea = signedArea;
        quality.minJacobian = minJacobian;
        quality.aspectRatio = aspectRatio;
        quality.inverted = (minJacobian <= 0.0);
        quality.lowQuality = (!quality.inverted && aspectRatio > aspectRatioLimit);

        results.push_back(quality);
    }

    return results;
}


//------------------------------------------------------------//
// Has Inverted Cells
//------------------------------------------------------------//

bool MeshQuality::hasInvertedCells(const vector<CellQuality>& results)
{
    for(const auto& quality : results)
    {
        if(quality.inverted)
        {
            return true;
        }
    }

    return false;
}


//------------------------------------------------------------//
// Has Low Quality Cells
//------------------------------------------------------------//

bool MeshQuality::hasLowQualityCells(const vector<CellQuality>& results)
{
    for(const auto& quality : results)
    {
        if(quality.lowQuality)
        {
            return true;
        }
    }

    return false;
}


//------------------------------------------------------------//
// Report
//------------------------------------------------------------//

void MeshQuality::report(const vector<CellQuality>& results)
{
    int invertedCount = 0;
    int lowQualityCount = 0;

    double worstJacobian = numeric_limits<double>::max();
    double worstAspectRatio = 0.0;

    for(const auto& quality : results)
    {
        if(quality.inverted) invertedCount++;
        if(quality.lowQuality) lowQualityCount++;

        worstJacobian = min(worstJacobian,quality.minJacobian);
        worstAspectRatio = max(worstAspectRatio,quality.aspectRatio);
    }

    cout << "\n";
    cout << "=====================================\n";
    cout << "          Mesh Quality Report\n";
    cout << "=====================================\n\n";

    cout << left << setw(25)
         << "Cells Checked"
         << " : "
         << results.size()
         << endl;

    cout << left << setw(25)
         << "Inverted Cells"
         << " : "
         << invertedCount
         << endl;

    cout << left << setw(25)
         << "Low Quality Cells"
         << " : "
         << lowQualityCount
         << endl;

    if(!results.empty())
    {
        cout << left << setw(25)
             << "Worst Scaled Jacobian"
             << " : "
             << worstJacobian
             << endl;

        cout << left << setw(25)
             << "Worst Aspect Ratio"
             << " : "
             << worstAspectRatio
             << endl;
    }

    if(invertedCount > 0)
    {
        cout << "\nInverted Cell IDs\n";
        cout << "------------------\n";

        for(const auto& quality : results)
        {
            if(quality.inverted)
            {
                cout << "  Cell " << quality.id
                     << " (min Jacobian = "
                     << quality.minJacobian
                     << ", signed area = "
                     << quality.signedArea
                     << ")\n";
            }
        }
    }

    if(lowQualityCount > 0)
    {
        cout << "\nLow Quality Cell IDs\n";
        cout << "---------------------\n";

        for(const auto& quality : results)
        {
            if(quality.lowQuality)
            {
                cout << "  Cell " << quality.id
                     << " (aspect ratio = "
                     << quality.aspectRatio
                     << ")\n";
            }
        }
    }

    cout << "\n=====================================\n";
}