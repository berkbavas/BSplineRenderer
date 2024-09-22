# BSpline Curves 3D
Rendering 3D B-splines using `OpenGL`, `Qt`, `C++` and `Eigen`.

B-splines are parameterics curves which consists of several Bezier curves whose control points satisfy some special conditions.
I used cubic Bezier curves for the interpolation of knots.
Given a set of knots, a cubic Bezier is generated between each knot.
Then these Bezier curves are glued together and forming the final curve, B-spline.
The algorithm for the generation of the curves can be found [here](https://www.math.ucla.edu/~baker/149.1.02w/handouts/dd_splines.pdf). Although it is about 2D B-splines, interpolating 3D B-splines is not so different.
I implemented the algorithm in `CreateCoefficientMatrix`, `GetSplineControlPoints` and `Update` methods of `Spline` class.

For the rendering algorithm, [this](https://www.songho.ca/opengl/gl_cylinder.html) resource helped me a lot.
The vertex generation algorithm can be found in `PipeDumb.geom` shader or in `GenerateVertices` method of `Bezier` class.

## Build
1) Install `CMake 3.25.1` or above.
2) Install `Visual Studio 2022` and `MSVC C++ v143 Compiler`.
3) Install `Qt 6.x.y MSVC2019 64bit` kit.
4) Set environment variable `Qt6_DIR` as `C:\Qt\6.x.y\msvc2019_64`.
5) Clone the repo `git clone https://github.com/berkbavas/BSplineCurves3D.git`.
6) Create a folder `mkdir Build`.
7) Enter the folder `cd Build`.
8) Run CMake `cmake ..`.
9) Open `BSplineCurves3D.sln` with `Visual Studio 2022`.
10) Build & Run with `Release` configuration.

## Screenshots
![1](https://github.com/berkbavas/BSplineCurves3D/assets/53399385/6235fa70-f048-4890-b0e0-d44e470c30bd)

![2](https://github.com/berkbavas/BSplineCurves3D/assets/53399385/61454042-3d5e-4b35-8330-d4c486bc9390)

## UML Diagram
![UML](https://github.com/berkbavas/BSplineCurves3D/assets/53399385/c2c29dcc-d797-4f91-967c-afb11702af70)

## Keywords
`C++`,
`Qt 6`,
`OpenGL`,
`GLSL`,
`Curves`,
`Bezier Curves`,
`3D B-Spline`,
`Eigen`,
`Curve rendering`
