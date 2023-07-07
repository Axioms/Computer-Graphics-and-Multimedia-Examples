


#include <GLM/gtc/type_ptr.hpp>


#include <iostream>


#include <glad/gl.h>

#include <GLFW/glfw3.h>


#include "cs4722/artifact.h"
#include "cs4722/buffer_utilities.h"
#include "cs4722/window.h"
#include "cs4722/callbacks.h"
#include "cs4722/light.h"
#include "cs4722/compile_shaders.h"

static cs4722::view *the_view;
static GLuint program;
static GLuint vao;

/**
 *  We need the location of the ambient light variable in the fragment shader.
 */
static GLuint transform_loc, ambient_color_loc, ambient_light_loc;

static std::vector<cs4722::artifact*> artifact_list;

/*
 * Declare a light object.
 * The default ambient color of a light object is Gray50.
 */
static cs4722::light a_light;

void init()
{
    the_view = new cs4722::view();

	program = cs4722::compile_shaders("vertex_shader02.glsl",
                                   "fragment_shader02.glsl");
	glUseProgram(program);


    transform_loc = glGetUniformLocation(program, "transform");
    ambient_color_loc = glGetUniformLocation(program, "ambient_color");
    ambient_light_loc = glGetUniformLocation(program, "ambient_light");

    auto tr = transform_loc;
    auto amcol = ambient_color_loc;
    auto amlgt = ambient_light_loc;


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
                artf->animation_transform.rotation_center =
                        artf->world_transform.matrix() * glm::vec4(0,3,0,1);
                artf->rotation_rate = ((x + y + z) % 12 * M_PI / 24);
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

    glBindVertexArray(vao);
    glUseProgram(program);

    auto view_transform = glm::lookAt(the_view->camera_position,
                                      the_view->camera_position + the_view->camera_forward,
                                      the_view->camera_up);
    auto projection_transform = glm::infinitePerspective(the_view->perspective_fovy,
                                                         the_view->perspective_aspect, the_view->perspective_near);
    auto vp_transform = projection_transform * view_transform;

    /*
     * The light is uniform for the whole drawing operation.
     * The light belongs to the scene not to any particular artifact.
     * So, the ambient light value is sent to the fragment shader outside of the artifact loop.
     */
    glUniform4fv(ambient_light_loc, 1, a_light.ambient_light.as_float());

    auto time = glfwGetTime();
	auto delta_time = time - last_time;

	for (auto artf: artifact_list) {

		artf->animate(time, delta_time);
        auto model_transform = artf->animation_transform.matrix() * artf->world_transform.matrix();
        auto transform = vp_transform * model_transform;

        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));
		glUniform4fv(ambient_color_loc, 1, artf->surface_material.ambient_color.as_float());

		glDrawArrays(GL_TRIANGLES, artf->the_shape->buffer_start,
			artf->the_shape->buffer_size);
		
	}
}




int
main(int argc, char** argv)
{
	glfwInit();
	auto *window = cs4722::setup_window("Ambient Lighting", 0.9);
    gladLoadGL(glfwGetProcAddress);
	cs4722::setup_debug_callbacks();

	init();

	glfwSetWindowUserPointer(window, the_view);
	cs4722::setup_user_callbacks(window);

	while (!glfwWindowShouldClose(window))
	{
        glClearBufferfv(GL_COLOR, 0, cs4722::x11::gray25.as_float());
        glClear(GL_DEPTH_BUFFER_BIT);

        display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
}
