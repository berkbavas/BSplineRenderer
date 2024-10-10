# B-Spline Renderer 
Rendering 3D B-splines in ``C++`` using ``Qt``, ``OpenGL`` and ``Eigen``.

B-splines are parametric curves that consist of several Bézier curves whose control points satisfy specific conditions.
I used cubic Bézier curves for the interpolation of knots. Given a set of knots, 
a cubic Bézier curve is generated between each pair of knots. 
Then, these Bézier curves are glued together to form the final curve, the B-spline.
The algorithm for the generation of the curves can be found [here](https://www.math.ucla.edu/~baker/149.1.02w/handouts/dd_splines.pdf). Although it is about 2D B-splines, interpolating 3D B-splines is not so different.

## Build
1) Install `CMake 3.25.1` or above.
2) Install `Visual Studio 2022` and `MSVC C++ v143 Compiler`.
3) Install `Qt 6.7.3 MSVC2022 64bit` kit.
4) Set environment variable `Qt6_DIR` as `C:\Qt\6.7.3\msvc2022_64`.
5) Clone the repo `git clone https://github.com/berkbavas/BSplineRenderer.git`.
6) Create a folder `mkdir Build`.
7) Enter the folder `cd Build`.
8) Run CMake `cmake ..`.
9) Open `BSplineRenderer.sln` with `Visual Studio 2022`.
10) Build `Release` configuration.

## Video
https://github.com/user-attachments/assets/daf205b8-dc9c-4aac-8f7b-df823693db86

