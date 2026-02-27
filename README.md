# raytracing

A C++ ray tracer built from scratch, following the [_Ray Tracing in One Weekend_](https://raytracing.github.io/) book series. This project implements a PBR capable of producing photorealistic images through recursive ray-sphere intersection, realistic lighting, and multiple material types.

## Features

- **Ray-sphere intersection** with accurate surface normal computation
- **Diffuse (Lambertian) materials** with realistic light scattering
- **Reflective metal materials** with configurable fuzziness
- **Dielectric (glass) materials** with refraction and Schlick approximation
- **Defocus blur** (depth of field) via a configurable aperture camera
- **Anti-aliasing** through multi-sample averaging per pixel
- **Configurable camera** with adjustable field of view, position, and orientation
- **PPM image output**

## Requirements

- C++17 or later
- CMake 3.10+
- A C++ compiler

## Building

```bash
# Clone the repository
git clone https://github.com/TideSinger/raytracing.git
cd raytracing

# Configure and build with CMake
cmake -B build
cmake --build build

# Run the renderer and output a PPM image
./build/raytracing > output.ppm
```
## How It Works

The renderer shoots rays from a virtual camera through each pixel of the image. When a ray hits an object in the scene, it determines the material and recursively bounces the ray according to the material's scattering rules — diffuse for matte surfaces, specular for metals, and Snell's law for glass. Multiple samples per pixel are averaged together for anti-aliasing, and color values are gamma-corrected before writing to the output.

## References

- [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html) — Peter Shirley
