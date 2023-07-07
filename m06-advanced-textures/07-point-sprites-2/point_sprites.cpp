
/**
 * This example is from the OpenGL programming guide.
 * The setup is the same as the first point-sprite example.
 * The main change is that the pattern on the point is computed in the fragment shader.
 *      So, no textures are created.
 *
 *  This example also demonstrates blending colors.  This makes the sprites semi-transparent.
 *  See the comments near the beginning of the init() function for a discussion.  Also check the fragment shader.
 */

#include <GLM/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <glad/gl.h>


#include <GLFW/glfw3.h>



#include "cs4722/buffer_utilities.h"
#include "cs4722/window.h"
#include "cs4722/callbacks.h"
#include "cs4722/texture_utilities.h"
#include "cs4722/compile_shaders.h"
#include "point-shape.h"

static GLuint program;
static auto* the_view = new cs4722::view();
static std::vector<cs4722::artifact*> parts_list;
GLuint  vao;

static GLuint transform_loc;



void init()
{

	program = cs4722::compile_shaders("vertex_shader07.glsl",
                                      "fragment_shader07.glsl");
	glUseProgram(program);

    transform_loc = glGetUniformLocation(program, "transform");

    glPointSize(80);
    // the following is needed to enable point sprites.
    glEnable(GL_POINT_SPRITE_ARB);


    // the texture used has transparency so sprites can be partially visible even if behind other sprites.
	glEnable(GL_BLEND);
	// Blending can be done in many ways.
	// This way uses the alpha channel of the value assigned to fColor
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*
	 * Blending can be complex if mixed with depth testing.
	 * A fragment that is behind another fragment but is computed later will be ignored under
	 * depth testing, even if it should be partially visible.
	 *
	 * You will notice some anomalies, shapes 'popping', when these conflicts arise dynamically.
	 *
	 * One solution is, even using depth testing, to render primitives from back to front
	 */
//	glEnable(GL_DEPTH_TEST);


	the_view->set_flup(glm::vec3(-0.352275, -7.45058e-09, -0.935897),
		glm::vec3(-0.935897, 0, 0.352274),
		glm::vec3(5.85944e-08, 1, 1.20934e-08),
		glm::vec3(1.34801, 0.785008, 3.00568));

	auto num_groups = 15;
	auto num_points_per_group = 3;
	auto min_c = -4.0;
	auto max_c = 4.0;

	const auto z_base = -5.0;
	const auto z_delta = .1;
    auto z = z_base;

	for (int group = 0; group < num_groups; group++)
	{

		auto* b = new points_shape();

		for (int i = 0; i < num_points_per_group; i++)
		{
			auto x = glm::linearRand(min_c, max_c);
			auto y = glm::linearRand(min_c, max_c);
//			auto z = glm::linearRand(min_c, max_c);
			b->position_list->push_back(glm::vec4(x, y, z, 1));
		}

		// auto* obj = new object_random_walk();
		auto* obj = new cs4722::artifact_rotating();

		
		auto cx = glm::linearRand(min_c, max_c);
		auto cy = glm::linearRand(min_c, max_c);
//		auto cz = glm::linearRand(min_c, max_c);
		auto cz = z;
		obj->animation_transform.rotation_center = glm::vec3(cx, cy, cz);

		obj->the_shape = b;
		obj->animation_transform.rotation_axis = (glm::vec3(1, 1, 0));
		obj->rotation_rate = M_PI / 3;
		obj->texture_unit = group % 3;
		parts_list.push_back(obj);

		z -= z_delta;
	}

//	auto pl = parts_list;   // used to make parts_list visible during debugging

	vao = cs4722::init_buffers(program, parts_list, "bPosition");

}



void display()
{

    glBindVertexArray(vao);
    glUseProgram(program);

    static auto last_time = 0.0;
//    auto *tv = the_view;  /// used to make the_view visible during debugging

    auto view_transform = glm::lookAt(the_view->camera_position,
                                      the_view->camera_position + the_view->camera_forward,
                                      the_view->camera_up);
    auto projection_transform = glm::infinitePerspective(the_view->perspective_fovy,
                                                         the_view->perspective_aspect, the_view->perspective_near);
    auto vp_transform = projection_transform * view_transform;


    auto time = glfwGetTime();
    auto delta_time = time - last_time;
    last_time = time;


    for (auto obj : parts_list) {
         obj->animate(time, delta_time);

        auto model_transform = obj->animation_transform.matrix() * obj->world_transform.matrix();
        auto transform = vp_transform * model_transform;
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));

        glDrawArrays(obj->the_shape->drawing_mode, obj->the_shape->buffer_start, obj->the_shape->buffer_size);
    }
}


//int main(int argc, char** argv)
int main()
{
	glfwInit();


    auto *window = cs4722::setup_window("Point Sprites", .9);

    gladLoadGL(glfwGetProcAddress);
	cs4722::setup_debug_callbacks();

	the_view->set_camera_position(glm::vec3(0, 0, 1));
    the_view->enable_logging = false;
	
	init();

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
