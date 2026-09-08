#ifndef COORDINATEMAPPING_H
#define COORDINATEMAPPING_H

#include "Mesh.h"

class CoordinateMapping
{
public:

    //------------------------------------------------------------
    // Identity Mapping
    //------------------------------------------------------------

    static void identity(Mesh& mesh);

    //------------------------------------------------------------
    // Trapezoidal Mapping
    //------------------------------------------------------------

    static void trapezoidal
    (
        Mesh& mesh,
        double topScale
    );

    //------------------------------------------------------------
    // Sinusoidal Mapping
    //------------------------------------------------------------

    static void sinusoidal
    (
        Mesh& mesh,
        double amplitude
    );

    //------------------------------------------------------------
    // Polar Mapping
    //------------------------------------------------------------

    static void polar
    (
        Mesh& mesh,
        double rInner,
        double rOuter
    );

    //------------------------------------------------------------
    // PipeBend Mapping
    //------------------------------------------------------------
    static void PipeBend
    (
        Mesh& mesh,
        double angle,
        double rInner,
        double rOuter
    );

    static void inlet
    (
        Mesh& mesh,
        double length,
        double rInner,
        double rOuter
    );

    static void outlet(
        Mesh& mesh, 
        double length,
        double angle,
        double rInner,
        double rOuter
    );
};

#endif