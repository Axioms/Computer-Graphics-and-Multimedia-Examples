


#include <GLM/vec4.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_inverse.hpp>


#include <iostream>

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include "cs4722/view.h"
#include "cs4722/artifact.h"
#include "cs4722/compile_shaders.h"
#include "cs4722/buffer_utilities.h"
#include "cs4722/light.h"
#include "cs4722/window.h"
#include "cs4722/callbacks.h"

static GLuint program;
static cs4722::view* the_view;
static std::vector<cs4722::artifact*> artifact_list;
static cs4722::light the_light;

void init()
{
    program = cs4722::compile_shaders("vertex_shader07.glsl","fragment_shader07.glsl");
    glUseProgram(program);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);



    auto* shape_list = new std::vector<cs4722::shape*>();
    auto pos = 0;
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
                artf->rotation_rate = ((x + y + z) % 12 * M_PI / 24);
                artf->surface_material.diffuse_color = (cs4722::color(
                        x * 255 / (number - 1), y * 255 / (number - 1),
                        z * 255 / (number - 1), 255));
                artf->surface_material.ambient_color = cs4722::color(255 - artf->surface_material.diffuse_color.r,
                                                255 - artf->surface_material.diffuse_color.g,
                                                255 - artf->surface_material.diffuse_color.b, 255);
                artifact_list.push_back(artf);
            }
        }
    }

    cs4722::init_buffers(program, artifact_list, "MCvertex","","","MCnormal");


}



void
display()
{


    // vertex shader
    const auto mv_transform_loc = glGetUniformLocation(program, "MVMatrix");
    //const auto mvp_transform = glGetUniformLocation(the_scene->program, " MVPMatrix");
    const auto n_transform = glGetUniformLocation(program, "NormalMatrix");
    const auto light_position = glGetUniformLocation(program, "LightPos");
    const auto cloud_scale = glGetUniformLocation(program, "Scale");

    const auto SkyColor = glGetUniformLocation(program, "SkyColor");
    const auto CloudColor = glGetUniformLocation(program, "CloudColor");

    glUniform4fv(light_position, 1, glm::value_ptr(the_light.light_direction_position));


    auto view_transform = glm::lookAt(the_view->camera_position,
                                      the_view->camera_position + the_view->camera_forward,
                                      the_view->camera_up);
    auto projection_transform = glm::infinitePerspective(the_view->perspective_fovy,
                                                         the_view->perspective_aspect, the_view->perspective_near);
//    auto vp_transform = projection_transform * view_transform;


//    const auto view_transform = the_view->look_at();
//    const auto projection_transform = the_view->projection();
    auto p_transform = glGetUniformLocation(program, "p_transform");
    glUniformMatrix4fv(p_transform, 1, GL_FALSE,
                       glm::value_ptr(projection_transform));

    static auto last_time = 0.0;
    auto time = glfwGetTime();
    auto delta_time = time - last_time;

    for (auto *artf: artifact_list) {

        artf->animate(time, delta_time);

        auto model_transform = artf->animation_transform.matrix() * artf->world_transform.matrix();
        auto mv_transform = view_transform * model_transform;
        glUniformMatrix4fv(mv_transform_loc, 1, GL_FALSE, glm::value_ptr(mv_transform));
        glUniform4fv(SkyColor, 1, artf->surface_material.diffuse_color.as_float());
        glUniform4fv(CloudColor, 1, artf->surface_material.ambient_color.as_float());
        glUniformMatrix4fv(n_transform, 1, GL_FALSE,
                           glm::value_ptr(glm::inverseTranspose(mv_transform)));

        glDrawArrays(GL_TRIANGLES, artf->the_shape->buffer_start,
                     artf->the_shape->buffer_size);

    }
}


int
main(int argc, char** argv)
{

    the_view = new cs4722::view();
    the_view->set_camera_position(glm::vec3(0, 0, 1));

	glfwInit();
    auto *window = cs4722::setup_window("Clouds", .9);

    gladLoadGL(glfwGetProcAddress);
    cs4722::setup_debug_callbacks();

	the_light.light_direction_position = glm::vec4(0, -1, 0, 1);

	init();
	const auto cloud_scale = glGetUniformLocation(program, "Scale");
	glUniform1f(cloud_scale, 0.2f);
	const auto SkyColor = glGetUniformLocation(program, "SkyColor");
	const auto CloudColor = glGetUniformLocation(program, "CloudColor");
	glUniform4fv(SkyColor, 1, glm::value_ptr(glm::vec4(0.0, 0.0, 0.8, 1.0)));
	glUniform4fv(CloudColor, 1, glm::value_ptr(glm::vec4(0.8, 0.8, 0.8, 1.0)));

	glfwSetWindowUserPointer(window, the_view);
	cs4722::setup_user_callbacks(window);



	while (!glfwWindowShouldClose(window))
	{
        glClearBufferfv(GL_COLOR, 0, cs4722::x11::gray50.as_float());
        glClear(GL_DEPTH_BUFFER_BIT);
        display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
}
