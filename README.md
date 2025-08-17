

# B-Spline Renderer

Render 3D B-splines in C++ using **Qt**, **OpenGL**, and **Eigen**.

## Overview

B-splines are parametric curves composed of multiple Bézier segments, where each segment's control points satisfy specific continuity conditions. This project uses cubic Bézier curves to interpolate between knots. For each pair of knots, a cubic Bézier curve is generated, and these are joined to form the final B-spline curve.

The core algorithm is based on [this reference](https://www.math.ucla.edu/~baker/149.1.02w/handouts/dd_splines.pdf) (originally for 2D B-splines, but easily adapted to 3D).

## Features

- Interactive 3D B-spline curve rendering
- Modern C++ codebase
- Utilizes Qt for GUI, OpenGL for rendering, and Eigen for linear algebra

## Build Instructions

1. **Install dependencies:**

	- [CMake 3.25.1+](https://cmake.org/download/)
	- [Visual Studio 2022](https://visualstudio.microsoft.com/vs/) with MSVC C++ v143 compiler
	- [Qt 6.7.3 MSVC2022 64bit](https://www.qt.io/download)

2. **Set environment variable:**

	- `Qt6_DIR` to `C:\Qt\6.7.3\msvc2022_64`

3. **Clone the repository:**

	```sh
	git clone https://github.com/berkbavas/BSplineRenderer.git
	```

4. **Create and enter build directory:**

	```sh
	mkdir Build
	cd Build
	```

5. **Generate build files with CMake:**

	```sh
	cmake ..
	```

6. **Open the solution in Visual Studio:**

	- Open `BSplineRenderer.sln` in Visual Studio 2022

7. **Build the project:**

	- Select the `Release` configuration and build

## Demo Video

[Project Demo]([https://github.com/user-attachments/assets/daf205b8-dc9c-4aac-8f7b-df823693db86](https://github.com/user-attachments/assets/5b382d66-f9cf-46d2-999f-00e230bbb8b8))

---

For more details on the mathematics of B-splines, see the [UCLA handout](https://www.math.ucla.edu/~baker/149.1.02w/handouts/dd_splines.pdf).

