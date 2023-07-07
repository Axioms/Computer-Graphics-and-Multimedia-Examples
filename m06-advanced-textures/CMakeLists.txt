cmake_minimum_required(VERSION 3.17)
project(m06_advanced_textures)



link_libraries(cs4722 glfw3 opengl32 glu32)

set(CMAKE_CXX_STANDARD 20)

include_directories(lib ../lib-common)
link_directories(lib ../lib-common)

add_executable(01-cube-map 01-cube-map/cube-map.cpp)
configure_file(01-cube-map/vertex_shader01.glsl .)
configure_file(01-cube-map/fragment_shader01.glsl .)

add_executable(02-sky-box 02-sky-box/skybox.cpp)
configure_file(02-sky-box/vertex_shader02.glsl .)
configure_file(02-sky-box/fragment_shader02.glsl .)

#add_executable(21-sky-box-orientation 21-sky-box-orientation/skybox-orientation.cpp)
#configure_file(21-sky-box-orientation/fragment_shader02-orientation.glsl .)
#configure_file(21-sky-box-orientation/vertex_shader02-orientation.glsl .)

add_executable(03-reflecting-object 03-reflecting-object/reflecting-object.cpp)
configure_file(03-reflecting-object/fragment_shader03x.glsl .)
configure_file(03-reflecting-object/vertex_shader03x.glsl .)

#add_executable(22-reflecting-object-orientation 22-reflecting-object-orientation/reflecting-object-orientation.cpp)
#configure_file(22-reflecting-object-orientation/vertex_shader07-orientation.glsl .)
#configure_file(22-reflecting-object-orientation/fragment_shader07-orientation.glsl .)

add_executable(04-render-to-texture 04-render-to-texture/render-to-texture.cpp 04-render-to-texture/setup_scene.cpp
        04-render-to-texture/setup_view_in_view.cpp)
configure_file(04-render-to-texture/view_in_view_vertex_shader04.glsl .)
configure_file(04-render-to-texture/view_in_view_fragment_shader04.glsl .)
configure_file(04-render-to-texture/scene_fragment_shader04.glsl .)
configure_file(04-render-to-texture/scene_vertex_shader04.glsl .)


add_executable(05B-image-processing 05B-image-processing/image-processing.cpp
        05B-image-processing/setup_scene.cpp 05B-image-processing/setup_view_in_view.cpp)
configure_file(05B-image-processing/image_processing_vertex_shader.glsl .)
configure_file(05B-image-processing/image_processing_fragment_shader.glsl .)
configure_file(05B-image-processing/scene_fragment_shader05B.glsl .)
configure_file(05B-image-processing/scene_vertex_shader05B.glsl .)



add_executable(06-point-sprites 06-point-sprites/point_sprites.cpp 06-point-sprites/point-shape.h)
configure_file(06-point-sprites/vertex_shader06.glsl .)
configure_file(06-point-sprites/fragment_shader06.glsl .)

add_executable(07-point-sprites-2 07-point-sprites-2/point_sprites.cpp)
configure_file(07-point-sprites-2/vertex_shader07.glsl .)
configure_file(07-point-sprites-2/fragment_shader07.glsl .)

add_executable(05A-pixel-filters 05A-pixel-filters/pixel_filters.cpp)
configure_file(05A-pixel-filters/vertex_shader05A.glsl .)
configure_file(05A-pixel-filters/fragment_shader05A.glsl .)
