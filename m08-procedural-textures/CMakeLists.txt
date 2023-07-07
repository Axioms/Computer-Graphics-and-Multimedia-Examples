cmake_minimum_required(VERSION 3.17)
project(m08_procedural_textures)


link_libraries(cs4722  glfw3 opengl32 glu32)

set(CMAKE_CXX_STANDARD 20)

include_directories(../lib-common  lib)
link_directories(../lib-common lib)

file(GLOB glsls */*.glsl)
#file(COPY ${glsls} DESTINATION .)
foreach(shader ${glsls})
    configure_file(${shader} .)
endforeach()


add_executable(01-stripes 01-stripes/stripes.cpp)
#configure_file(01-stripes/fragment_shader01.glsl .)
#configure_file(01-stripes/vertex_shader01.glsl .)




add_executable(02-fractal 02-fractal/fractal.cpp)
#configure_file(02-fractal/fragment_shader02.glsl .)
#configure_file(02-fractal/vertex_shader02.glsl .)


add_executable(03-bump-map 03-bump-map/bump_map.cpp)
add_executable(04-noise-on-square 04-noise-on-square/noise-on-square.cpp)
add_executable(05-compare-noise 05-compare-noise/compare_noise.cpp)
add_executable(06-clouds 06-clouds/clouds.cpp)
add_executable(07-clouds-glsl 07-clouds-glsl/clouds_glsl.cpp)