
# nodeflow

Flexible C++ node-based dataflow framework, similiar to Unreal Engine's Blueprint system.

![preview](https://github.com/Ruwen14/nodeflow/assets/73891246/d3cedb5a-6a04-4521-82f4-83ce0404c90a)


Still in _very_ early development as it's a hobby project. Not ready to use yet.

# Working Feature's
- [x] Node-based processing
- [x] Data propagation of any data type without limitations
- [x] Zero overhead compile time reflection system via nf::type_id<>
- [x] Automatic node generation for existing C++ code (still in dev)

# Planned Feature's
- [ ] Qt6 Frontend (currently prototyped in python)
- [ ] ImGui Frontend with custom render backend(OpenGL, Vulkan, DirectX)
- [ ] Dynamic Input and Output Pins
- [ ] Static computation-graph-analysis for parallelization
- [ ] Code generation of graphs back to c++

# Requirements (planned included)
- C++20
- Qt6 or Dear ImGui (https://github.com/ocornut/imgui)
- nlohmann json library (https://github.com/nlohmann/json)


# Installation & Build
- CMake yet to be setup
