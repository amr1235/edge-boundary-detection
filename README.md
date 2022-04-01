# edge and boundary detection
For any given image we can detect edges using Canny edge detector, detect lines, circles, ellipsed located in the image (if any). also, we implemented the the Active Contour Model (snake) using the greedy algorithm.


### Prerequsites
1. Visual Studio C++ Workload
2. OpenCV
3. CMake

<br>

### Build
1. Configure project.

*N.b* use only after cloning or changing *CMakeLists.txt*.
```sh
$ cmake . -B ./build
```
2. Build the executable
```sh
$ cmake --build ./build/
```
3. The executable will be in the *build/Debug/* folder
```sh
$ build/Debug/cv-project-1.exe
```
4. Clean the build folder (Optional)
```sh
$ cmake --build ./build/ --target clean
```