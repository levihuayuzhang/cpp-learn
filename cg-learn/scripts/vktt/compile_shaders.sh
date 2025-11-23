mkdir -p build/vktt/shaders
glslc vktt/src/shader.vert -o build/vktt/shaders/vert.spv
glslc vktt/src/shader.frag -o build/vktt/shaders/frag.spv