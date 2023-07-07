

/**
 *
 *  There are few changes in this code from previous examples. Here is what to look for:
 * 1.  Creates artifacts and sets the ambient color of the material for each artifact
 * 2.  Passes the ambient color to the fragment shader
 *
 * Coloring with a solid color gives little or no feeling of depth.
 * Ambient color and light (introduced in the next example) are not used alone but are used
 *      to fill in where diffuse light does not reach.
 *
 *
 */


#include <GLM/gtc/type_ptr.hpp>


#include <iostream>



#include <glad/gl.h>

#include <GLFW/glfw3.h>


#include "cs4722/view.h"
#include "cs4722/x11.h"
#include "cs4722/shape.h"
#include "cs4722/artifact.h"
#include "cs4722/buffer_utilities.h"
#include "cs4722/window.h"
#include "cs4722/callbacks.h"
#include "cs4722/compile_shaders.h"

static cs4722::view *the_view;
static GLint program;
static GLint vao;


/*
 * There is a uniform variable for ambient color in the fragment shader.
 */
static GLint transform_loc, ambient_color_loc;

static std::vector<cs4722::artifact*> artifact_list;

void init()
{
    the_view = new cs4722::view();


	program = cs4722::compile_shaders("vertex_shader01.glsl",
                                "fragment_shader01.glsl");
	glUseProgram(program);

    transform_loc = glGetUniformLocation(program, "transform");
    ambient_color_loc = glGetUniformLocation(program, "ambient_color");



    glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);


	// create a list of shapes that will be shared by the objects we have

	const auto color_set = std::vector <cs4722::color>({
			cs4722::x11::white, cs4722::x11::grey50, cs4722::x11::grey75,
		});
	
	auto* shape_list = new std::vector<cs4722::shape*>();
	shape_list->push_back(new cs4722::sphere());
	shape_list->push_back(new cs4722::block());
	shape_list->push_back(new cs4722::torus());
	shape_list->push_back(new cs4722::cylinder());
	auto numshp = shape_list->size();



	auto number = 4;
	auto d = 20.0f / (2 * number + 1);
	auto radius = d / 4;
	auto base = -number * d / 2 + radius;

	for (auto x = 0; x < number; ++x)
	{
		for (auto y = 0; y < number; ++y)
		{
			for (auto z = 0; z < number; ++z)
			{
				auto* artf = new cs4722::artifact_rotating();
				artf->the_shape = (shape_list->at((x + y + z) % numshp));
				artf->world_transform.translate = (glm::vec3(base + x * d, base + y * d, base + z * d));
				artf->world_transform.scale = (glm::vec3(radius, radius, radius));
				artf->animation_transform.rotation_axis = (glm::vec3(x + 1, y + 1, z + 1));
                artf->animation_transform.rotation_center = artf->world_transform.translate;
				artf->rotation_rate = ((x + y + z) % 12 * M_PI / 24);
                /**
                 * Set the ambient color of the surface material of the artifact.
                 * The colors are chosen so that red varies in intensity along the x direction,
                 * green varies in the y direction and blue varies in the z direction.
                 */
				artf->surface_material.ambient_color = (cs4722::color(
					x * 255 / (number-1), y * 255 / (number - 1), 
					z * 255 / (number - 1), 255));
				artifact_list.push_back(artf);
			}
		}
	}

    vao = cs4722::init_buffers(program, artifact_list, "bPosition");
}



void display()
{
    static auto last_time = 0.0;

    static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	glClearBufferfv(GL_COLOR, 0, cs4722::x11::gray50.as_float());
	glClear(GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao);
    glUseProgram(program);

    auto view_transform = glm::lookAt(the_view->camera_position,
                                      the_view->camera_position + the_view->camera_forward,
                                      the_view->camera_up);
    auto projection_transform = glm::infinitePerspective(the_view->perspective_fovy,
                                                         the_view->perspective_aspect, the_view->perspective_near);
    auto vp_transform = projection_transform * view_transform;


    auto time = glfwGetTime();
	auto delta_time = time - last_time;

	for (auto artf: artifact_list) {

		artf->animate(time, delta_time);
        auto model_transform = artf->animation_transform.matrix() * artf->world_transform.matrix();
        auto transform = vp_transform * model_transform;

        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));
        /**
         * Send the ambient color to the fragment shader.
         */
		glUniform4fv(ambient_color_loc, 1, artf->surface_material.ambient_color.as_float());

		
		glDrawArrays(GL_TRIANGLES, artf->the_shape->buffer_start,
			artf->the_shape->buffer_size);
		
	}
}




int
main(int argc, char** argv)
{
	glfwInit();

	auto *window = cs4722::setup_window("Ambient Color", 0.9);
    gladLoadGL(glfwGetProcAddress);
	cs4722::setup_debug_callbacks();

	init();

	glfwSetWindowUserPointer(window, the_view);
	cs4722::setup_user_callbacks(window);


	while (!glfwWindowShouldClose(window))
	{
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
}
