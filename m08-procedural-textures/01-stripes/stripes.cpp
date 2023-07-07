


#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_inverse.hpp>

//#include <iostream>


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
static GLuint program;
static GLuint vao;

static GLint diffuse_color_loc;
static GLint width_loc;
static GLint fuzz_loc;
static GLint scale_loc;


static GLint transform_loc;



static std::vector<cs4722::artifact*> artifact_list;

//static cs4722::light a_light;

static float width = .6f;
static float fuzz = .05f;
static float scale = 5;


void init()
{
    the_view = new cs4722::view();
    the_view->enable_logging = false;

	program = cs4722::compile_shaders("vertex_shader01.glsl",
                                   "fragment_shader01.glsl");
	glUseProgram(program);



    // fragment shader uniformas
    diffuse_color_loc = glGetUniformLocation(program, "diffuse_color");
    width_loc = glGetUniformLocation(program, "width");
    fuzz_loc = glGetUniformLocation(program, "fuzz");
    scale_loc = glGetUniformLocation(program, "scale");
    // vertex shader uniforms
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
                        artf->world_transform.matrix() * glm::vec4(0,0,0,1);
                artf->rotation_rate = ((x + y + z) % 12 * M_PI / 24);
				artf->surface_material.ambient_color = (cs4722::color(
					x * 255 / (number-1), y * 255 / (number - 1), 
					z * 255 / (number - 1), 255));
				artf->surface_material.specular_color = cs4722::x11::white;
				artf->surface_material.diffuse_color = artf->surface_material.ambient_color;
				artf->surface_material.specular_strength = 30.0;
				artf->surface_material.shininess = 15.0;
				artifact_list.push_back(artf);
			}
		}
	}
	
    vao = cs4722::init_buffers(program, artifact_list, "bPosition","",
                               "bTextureCoord");
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
                                                         the_view->perspective_aspect,
                                                         the_view->perspective_near);

    glUniform1f(scale_loc, scale);
    glUniform1f(fuzz_loc, fuzz);
    glUniform1f(width_loc, width);

    auto time = glfwGetTime();
	auto delta_time = time - last_time;

	for (auto artf: artifact_list) {

		artf->animate(time, delta_time);
        auto transform = projection_transform * view_transform *
                artf->animation_transform.matrix() * artf->world_transform.matrix();

        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(  transform));

        glUniform4fv(diffuse_color_loc, 1, artf->surface_material.diffuse_color.as_float());

        glDrawArrays(GL_TRIANGLES, artf->the_shape->buffer_start,
			artf->the_shape->buffer_size);
		
	}
}




int
main(int argc, char** argv)
{
	glfwInit();
	auto *window = cs4722::setup_window("Stripes", 0.9);
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
