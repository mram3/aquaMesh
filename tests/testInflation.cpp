#include "Test.h"

#include "Point.h"
#include "Line.h"
#include "Surface.h"
#include "Inflation.h"

#include <cmath>

int main()
{
    Test test;

    // --------------------------------------------------
    // 1. Create geometry
    // --------------------------------------------------

    Point p0(0, 0.0, 0.0);
    Point p1(1, 4.0, 0.0);
    Point p2(2, 4.0, 6.0);
    Point p3(3, 0.0, 6.0);

    Line l0(0, &p0, &p1);
    Line l1(1, &p1, &p2);
    Line l2(2, &p2, &p3);
    Line l3(3, &p3, &p0);

    Surface rectangle(0);

    rectangle.addBoundary(&l0);
    rectangle.addBoundary(&l1);
    rectangle.addBoundary(&l2);
    rectangle.addBoundary(&l3);

    // --------------------------------------------------
    // 2. Generate inflation mesh
    // --------------------------------------------------

    Inflation mesh;

    mesh.generateInflation(
        rectangle,
        0.12,   // h0
        1.3,    // growth ratio
        15,     // requested inflation layers
        20      // Nx
    );

    // --------------------------------------------------
    // 3. Basic mesh checks
    // --------------------------------------------------

    test.expectTrue(
        mesh.getNumberOfNodes() > 0,
        "Mesh contains nodes"
    );

    test.expectTrue(
        mesh.getNumberOfCells() > 0,
        "Mesh contains cells"
    );

    // --------------------------------------------------
    // 4. Check mesh parameters
    // --------------------------------------------------

    test.expectEqual(
        mesh.Nx,
        20,
        1e-12,
        "Nx = 20"
    );

    test.expectEqual(
        mesh.Nlayer,
        9,
        1e-12,
        "Final inflation layers = 9"
    );

    test.expectEqual(
        mesh.Ny,
        2,
        1e-12,
        "Coarse layers = 2"
    );

    test.expectEqual(
        mesh.dx,
        0.2,
        1e-12,
        "dx = 0.2"
    );

    // --------------------------------------------------
    // 5. Check number of nodes and cells
    // --------------------------------------------------

    test.expectEqual(
        mesh.getNumberOfNodes(),
        252,
        1e-12,
        "Number of nodes = 252"
    );

    test.expectEqual(
        mesh.getNumberOfCells(),
        220,
        1e-12,
        "Number of cells = 220"
    );

    // --------------------------------------------------
    // 6. Check X coordinates
    // --------------------------------------------------

    for(int i = 0; i <= mesh.Nx; i++)
    {
        double expectedX = 0.0 + i * mesh.dx;

        test.expectEqual(
            mesh.nodes[i].x,
            expectedX,
            1e-12,
            "X coordinate of first row node"
        );
    }

    // --------------------------------------------------
    // 7. Check inflation Y coordinates
    // --------------------------------------------------

    for(int j = 0; j <= mesh.Nlayer; j++)
    {
        double expectedY =
            0.0
            + 0.12 * (std::pow(1.3, j) - 1.0)
            / (1.3 - 1.0);

        int nodeIndex = j * (mesh.Nx + 1);

        test.expectEqual(
            mesh.nodes[nodeIndex].y,
            expectedY,
            1e-10,
            "Inflation layer Y coordinate"
        );
    }

    // --------------------------------------------------
    // 8. Check last node reaches xmax and ymax
    // --------------------------------------------------

    int lastNode = mesh.getNumberOfNodes() - 1;

    test.expectEqual(
        mesh.nodes[lastNode].x,
        4.0,
        1e-12,
        "Last node X = xmax"
    );

    test.expectEqual(
        mesh.nodes[lastNode].y,
        6.0,
        1e-12,
        "Last node Y = ymax"
    );

    // --------------------------------------------------
    // 9. Check cell connectivity
    // --------------------------------------------------

    bool validCells = true;

    for(const auto& cell : mesh.cells)
    {
        for(int nodeID : cell.nodeIDs)
        {
            if(nodeID < 0 ||
               nodeID >= mesh.getNumberOfNodes())
            {
                validCells = false;
            }
        }
    }

    test.expectTrue(
        validCells,
        "All cell node indices are valid"
    );

    // --------------------------------------------------
    // 10. Print summary
    // --------------------------------------------------

    test.summary();

    return test.success() ? 0 : 1;
}