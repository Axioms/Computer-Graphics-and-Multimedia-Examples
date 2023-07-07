#pragma once


#include <GLM/gtc/type_ptr.hpp>
#include <vector>

#include <glad/gl.h>


#include "cs4722/view.h"
#include "cs4722/artifact.h"
#include "cs4722/buffer_utilities.h"
#include "cs4722/texture_utilities.h"
#include "cs4722/compile_shaders.h"


const auto fb_texture_unit = 61;
const auto frame_buffer_width = 2048;
const auto frame_buffer_height = frame_buffer_width;


const auto rectangle_width = 20.0f;
const auto rectangle_height = 8.0f;



void scene_setup(cs4722::view* view);


void scene_display();

void scene_setup_for_fb();

void scene_setup_for_window(GLFWwindow* window);



void view_in_view_setup(cs4722::view *the_view);

void view_in_view_display();


