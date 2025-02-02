**Lab 2 Submission - Virtual City Rendering with OpenGL**

## **Features Implemented**
- ✅ **Procedural City Generation**: The project generates multiple buildings dynamically using a 9x9 grid.
- ✅ **Camera Transformations**: Implemented `glm::lookAt()` for view transformation and `glm::perspective()` for projection.
- ✅ **Texture Mapping**: Each building is assigned one of four random textures from `facade1.jpg` to `facade4.jpg`.
- ✅ **Vertex and Fragment Shaders**:
  - The vertex shader processes UV coordinates and passes them to the fragment shader.
  - The fragment shader applies textures to the buildings instead of vertex colors.
- ✅ **Key Controls for Camera Movement**:
  - `W` - Move Up
  - `S` - Move Down
  - `A` - Move Left
  - `D` - Move Right
  - `Q` - Move Forward
  - `E` - Move Backward
- ✅ **Fixed Object Placement and Transformation**: Buildings are positioned correctly with proper model scaling and translation.
- ✅ **Optimized Rendering Pipeline**: Buffers for vertices, UVs, and shaders are properly allocated and bound.

## **Issues Faced & Solutions**
1. **Limited Movement in Upper and Lower Bounds**
   - Issue: Camera movement was restricted due to incorrect `zNear` and `zFar` values.
   - Fix: Adjusted `projectionMatrix` to increase `zFar` and lower `zNear`.

2. **Texture Mapping Not Appearing**
   - Issue: UV coordinates were not passed correctly to the fragment shader.
   - Fix: Enabled `vertexUV` in the vertex shader and used `texture(textureSampler, uv)` in the fragment shader.

3. **Buildings Floating Above Ground**
   - Issue: Incorrect model matrix transformations.
   - Fix: Adjusted `modelMatrix` to correctly scale and translate buildings.

4. **Compilation Warnings on macOS (GL Deprecation)**
   - Issue: OpenGL API deprecated in macOS.
   - Fix: Used `-DGL_SILENCE_DEPRECATION=ON` in `cmake` command.

## **How to Compile and Run the Project**

### **Step 1: Navigate to the Build Directory**
```sh
cd build
```

### **Step 2: Compile the Project**
```sh
cmake .. -DGL_SILENCE_DEPRECATION=ON
make -j$(sysctl -n hw.ncpu)
```

### **Step 3: Run the Executable**
```sh
./lab2
```

### **Step 4: Capture Video for Submission**
- Record the rendered scene using a screen recording tool.
- Save it as `lab2_demo.mp4`.
- Zip all required files into `lab2_results.zip` and submit.

🚀 **Submission Complete!**

