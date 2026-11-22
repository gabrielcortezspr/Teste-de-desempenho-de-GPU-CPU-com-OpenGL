# OpenGL GPU/CPU Benchmark

![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus&logoColor=white)
![OpenGL 3.3](https://img.shields.io/badge/OpenGL-3.3%20core-5586A4?logo=opengl&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-build-064F8C?logo=cmake&logoColor=white)
![Python](https://img.shields.io/badge/Python-analysis-3776AB?logo=python&logoColor=white)
![License](https://img.shields.io/badge/license-MIT-green)

A benchmark written in modern OpenGL (C++17, GLSL 3.3 core profile) that measures how
geometry load, lighting and texturing affect frame rate, CPU usage and GPU usage. It renders
an increasing number of rotating triangles, logs the metrics to CSV and turns them into
charts with Python. Built for the Digital Image Processing III course (Computer Engineering).

![Extreme test results](reports/extreme_performance_analysis.png)

## Features

- **Rendering:** rotating colored triangles with GLSL shaders, GLFW window, GLEW loader and
  GLM math.
- **Effects:** omnidirectional point light, spotlight, procedural textures with UV mapping.
- **Hardware monitoring:** GPU identification, CPU usage from `/proc/stat`, GPU usage via
  `nvidia-smi` (NVIDIA) or `radeontop` (AMD).
- **Automated tests:** sweep the triangle count (from 1 up to 200,000), with and without
  effects, and export FPS / CPU / GPU per step to CSV.
- **Interactive demo:** switch between rendering modes and add or remove triangles live while
  watching the FPS.
- **Analysis:** Python scripts (pandas + matplotlib) that generate charts and summary tables.

## Results

Hardware: Intel Core i7-1355U with Intel Iris Xe integrated graphics.

| Mode (100 triangles) | Average FPS | vs. basic |
|---|---|---|
| Basic | ~52 | |
| Lighting | ~45 | −13% |
| Textures | ~38 | −26% |
| Lighting + textures | ~29 | −45% |

In the extreme test the integrated GPU reaches 100% usage at around 30–40k triangles with
all effects on, and the frame rate drops below 10 FPS beyond 100k triangles. Raw data is in
[`data/`](data) and the full analysis in [`docs/final-report.pdf`](docs/final-report.pdf).

## Getting started

Linux (Ubuntu/Debian) with a GPU that supports OpenGL 3.3+.

```bash
./scripts/setup.sh    # installs build-essential, cmake, GLFW, GLEW, GLM and Python deps, then builds
```

Or manually:

```bash
sudo apt install build-essential cmake libglfw3-dev libglew-dev libglm-dev libgl1-mesa-dev
cmake -S . -B build && cmake --build build -j
```

### Executables

| Command | What it does |
|---|---|
| `./scripts/demo.sh` | Interactive demo (`build/DemoInterativo`) |
| `./build/OpenGL_Performance_Test` | Single rotating triangle with an FPS counter |
| `./build/PerformanceTest` | Automated sweep, no effects → `data/performance_data.csv` |
| `./build/AdvancedTest` | Automated sweep with lighting/texture toggles → `data/advanced_performance_data.csv` |
| `./scripts/extreme-test.sh` | Sweep up to 200k triangles, then plots the results |

Interactive demo controls: `1` basic, `2` lighting, `3` textures, `4` combined,
`↑`/`↓` ±100 triangles, `R` reset, `H` help, `Esc` quit.

### Analysis

```bash
pip install pandas matplotlib numpy
python scripts/analyze_data.py data/performance_data.csv
python scripts/analyze_extreme_data.py data/extreme_performance_data.csv
```

Charts are written to `reports/`.

## Project structure

```
├── src/
│   ├── Renderer, MultiTriangleRenderer, AdvancedRenderer   # rendering pipeline
│   ├── Lighting, Texture                                   # point light, spotlight, procedural textures
│   ├── PerformanceMonitor                                  # FPS, CPU and GPU sampling + CSV export
│   └── main / performance_test / advanced_test / extreme_test / demo_interativo .cpp
├── scripts/      # setup, demo and test launchers + Python analysis
├── data/         # collected CSVs
├── reports/      # generated charts
├── docs/         # final report (PDF/Markdown), summary and presentation guide, in Portuguese
└── CMakeLists.txt
```

## Possible improvements

- Instanced rendering and frustum culling to push the triangle ceiling.
- Normal and shadow mapping.
- Real-time charts in the demo with Dear ImGui.

## License

[MIT](LICENSE)
