/**
 * This application simply displays a bunch of artifacts based on different shapes.
 * The difference in this one is that the color of each fragment is based on the normal vector at that fragment.
 *
 * For example, if the normal at a fragment is (0,0,1), so pointing along the z axis, then the color used will be
 * blue.
 * You will see that the faces of the cubes are red or blue or green since the normals to those faces are along the axes.
 *
 *
 *
 * The problem with this application is that the normals are computed before the surface has been changed by the
 * model transformation.
 * For use in shading, the normal should be determined once the artifact has been moved and scaled its
 * position in world coordinates.
 * The face of a cube should be blue just when the normal is along the z-axis in world coordinates.
 * When the cube spins around so that that
 * particular face is normal to the y-axis, then the color should be green (0,1,0).
 *
 * This problem will be fixed in the next example.
 *
 * Most of the code below is the same as previous examples, so only a few places have comments.
 *
 * Be sure to look at the two shader programs for further comments.
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
#include "cs4722/light.h"
#include "cs4722/compile_shaders.h"

static cs4722::view *the_view;
static GLuint program;
static GLuint vao;


static GLuint transform_loc;

static std::vector<cs4722::artifact*> artifact_list;


void init()
{
    the_view = new cs4722::view();


	program = cs4722::compile_shaders("vertex_shader03.glsl",
                                   "fragment_shader03.glsl");
	glUseProgram(program);

    transform_loc = glGetUniformLocation(program, "transform");

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

	/*
	 * Only the positions and the normals are being used for the vertices.
	 */
    vao = cs4722::init_buffers(program, artifact_list, "bPosition", "", "",
                               "bNormal");
}



void display()
{

    glBindVertexArray(vao);
    glUseProgram(program);

    auto view_transform = glm::lookAt(the_view->camera_position,
                                      the_view->camera_position + the_view->camera_forward,
                                      the_view->camera_up);
    auto projection_transform = glm::infinitePerspective(the_view->perspective_fovy,
                                                         the_view->perspective_aspect, the_view->perspective_near);
    auto vp_transform = projection_transform * view_transform;

    static auto last_time = 0.0;
    auto time = glfwGetTime();
	auto delta_time = time - last_time;

	for (auto artf: artifact_list) {

		artf->animate(time, delta_time);
        auto model_transform = artf->animation_transform.matrix() * artf->world_transform.matrix();
        auto transform = vp_transform * model_transform;

        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));

		glDrawArrays(GL_TRIANGLES, artf->the_shape->buffer_start,
			artf->the_shape->buffer_size);
		
	}
}




int
main(int argc, char** argv)
{
	glfwInit();
	auto *window = cs4722::setup_window("Visualize Normals", 0.9);
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
