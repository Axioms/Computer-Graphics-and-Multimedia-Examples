


#include <GLM/vec4.hpp>
#include <GLM/vec2.hpp>
#include <glm/gtc/noise.hpp>
#include <GLM/gtc/type_ptr.hpp>


#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include "cs4722/callbacks.h"
#include "cs4722/compile_shaders.h"
#include "cs4722/artifact.h"
#include "cs4722/buffer_utilities.h"
#include "cs4722/window.h"

#include "FastNoiseLite.h"

static GLuint program;
static cs4722::view *the_view;
static std::vector<cs4722::artifact*> parts_list;

void init()
{

    the_view = new cs4722::view();

	program = cs4722::compile_shaders("vertex_shader05.glsl","fragment_shader05.glsl");
	glUseProgram(program);

	glEnable(GL_DEPTH_TEST);


	auto* rectangle_shape = new cs4722::rectangle(1, 1);

	auto scale = 0.45f;
	
	auto part = new cs4722::artifact();
    part->the_shape = rectangle_shape;
    part->world_transform.translate = glm::vec3(-0.5, -0.5, 0.0);
    part->world_transform.scale = glm::vec3(scale, scale, scale );
    part->texture_unit = 3;
	parts_list.push_back(part);


    part = new cs4722::artifact();
    part->the_shape = rectangle_shape;
    part->world_transform.translate = glm::vec3(0.5, -0.5, 0.0);
    part->world_transform.scale = glm::vec3(scale, scale, scale);
    part->texture_unit = 2;
	parts_list.push_back(part);

    part = new cs4722::artifact();
    part->the_shape = rectangle_shape;
    part->world_transform.translate = glm::vec3(0.5, 0.5, 0.0);
    part->world_transform.scale = glm::vec3(scale, scale, scale);
    part->texture_unit = 1;
	parts_list.push_back(part);

    part = new cs4722::artifact();
    part->the_shape = rectangle_shape;
    part->world_transform.translate = glm::vec3(-0.5, 0.5, 0.0);
    part->world_transform.scale = glm::vec3(scale, scale, scale);
    part->texture_unit = 0;
	parts_list.push_back(part);

    part = new cs4722::artifact();
    part->the_shape = rectangle_shape;
    part->world_transform.translate = glm::vec3(0, 0, 0.0);
    part->world_transform.scale = glm::vec3(scale, scale, scale);
    part->texture_unit = 4;
	parts_list.push_back(part);

	cs4722::init_buffers(program, parts_list, "bPosition", "", "bTextureCoord");
}



void init_texture2D() {


	const auto texture_size = 512;
	auto* texture_data = new GLubyte[4 * texture_size * texture_size];


	auto const number_of_octaves = 4;


	auto  frequency = 4.0f;
	auto amp = 0.5f;
	//int inc = 0;

	// auto* noise_gen = new SimplexNoise();


	for (auto f = 0; f < number_of_octaves; ++f)
	{
		auto ni = glm::vec2(0.0);
		auto inc = 1.0f / (texture_size / frequency);

		auto* ptr = texture_data;

		for (auto i = 0; i < texture_size; ++i)
		{
			for (auto j = 0; j < texture_size; ++j)
			{
				// *(ptr + f) = static_cast<GLubyte>(glm::perlin(ni) * amp * 128.0f);
				// *(ptr + f) = static_cast<GLubyte>((glm::simplex(ni) + 1.0f) * amp * 128.0f);
				// The following seems to be what was intended by the code in the book
				//      ni.y is incremented too much in the text code, if I'm reading it correctly
				// *(ptr + f) = static_cast<GLubyte>((glm::simplex(glm::vec2(i, j) * inc) + 1.0f) * amp * 128.0f);
				// don't use amplitude here
				*(ptr + f) = static_cast<GLubyte>((glm::simplex(glm::vec2(i, j) * inc) + 1.0f) * 128.0f);
				//  next makes all octaves the same amplitude, for easier comparison
				//*(ptr + f) = static_cast<GLubyte>((glm::simplex(glm::vec2(i, j) * inc) + 1.0f) * 128.0f);
				ptr += 4;
				ni.y += inc;
			}
			ni.x += inc;
		}


		frequency *= 2;
		++inc;
		amp *= 0.5;
	}




	// initialize texture and sampler

	GLuint texture;
	glCreateTextures(GL_TEXTURE_2D, 1, &texture);

	auto number_of_levels = 1;
	auto internal_format = GL_RGBA8;
	auto external_format = GL_RGBA;


	glTextureStorage2D(texture, number_of_levels, internal_format, texture_size, texture_size);
	glTextureSubImage2D(texture, 0, 0, 0, texture_size, texture_size,
		external_format, GL_UNSIGNED_BYTE, texture_data);
	delete texture_data;
	glBindTextureUnit(2, texture);

	glTextureParameterfv(texture, GL_TEXTURE_BORDER_COLOR, cs4722::x11::aquamarine.as_float());
	glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// auto mag_filter = GL_NEAREST;
	auto mag_filter = GL_LINEAR;
	glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, mag_filter);
	// auto wrap_type = GL_CLAMP_TO_EDGE;
	// auto wrap_type = GL_MIRRORED_REPEAT;
	auto wrap_type = GL_REPEAT;
	// auto wrap_type = GL_CLAMP_TO_BORDER;
	glTextureParameteri(texture, GL_TEXTURE_WRAP_S, wrap_type);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_T, wrap_type);


	auto sampler_loc = glGetUniformLocation(program, "sampler");

	glUniform1i(sampler_loc, 2);

}




void init_texture2D_alt() {


	const auto texture_size = 512;
	auto* texture_data = new GLubyte[4 * texture_size * texture_size];


	auto const number_of_octaves = 4;


	auto  frequency = 4.0f;
	auto amp = 1.0f;// 0.5f;
	//int inc = 0;

	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);


	for (auto f = 0; f < number_of_octaves; ++f)
	{
		noise.SetFrequency(frequency);

		// auto inc = 1.0f / (texture_size / frequency);
		auto inc = 1.0f / texture_size;

		auto* ptr = texture_data;

		for (auto i = 0; i < texture_size; ++i)
		{
			for (auto j = 0; j < texture_size; ++j)
			{
				auto sample = noise.GetNoise(i * inc, j * inc);
				*(ptr + f) = static_cast<GLubyte>((sample + 1.0f) * amp * 128.0f);

				ptr += 4;
			}
		}


		frequency *= 2;
		amp *= 0.5f;
	}




	// initialize texture and sampler

	GLuint texture;
	glCreateTextures(GL_TEXTURE_2D, 1, &texture);

	auto number_of_levels = 1;
	auto internal_format = GL_RGBA8;
	auto external_format = GL_RGBA;


	glTextureStorage2D(texture, number_of_levels, internal_format, texture_size, texture_size);
	glTextureSubImage2D(texture, 0, 0, 0, texture_size, texture_size,
		external_format, GL_UNSIGNED_BYTE, texture_data);
	delete texture_data;
	glBindTextureUnit(2, texture);

	glTextureParameterfv(texture, GL_TEXTURE_BORDER_COLOR, cs4722::x11::aquamarine.as_float());
	glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// auto mag_filter = GL_NEAREST;
	auto mag_filter = GL_LINEAR;
	glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, mag_filter);
	// auto wrap_type = GL_CLAMP_TO_EDGE;
	// auto wrap_type = GL_MIRRORED_REPEAT;
	auto wrap_type = GL_REPEAT;
	// auto wrap_type = GL_CLAMP_TO_BORDER;
	glTextureParameteri(texture, GL_TEXTURE_WRAP_S, wrap_type);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_T, wrap_type);


	auto sampler_loc = glGetUniformLocation(program, "sampler");

	glUniform1i(sampler_loc, 2);

}


void display()
{
    static auto last_time = 0.0;
	auto time = glfwGetTime();
	auto delta_time = time - last_time;
	last_time = time;

    auto view_transform = glm::lookAt(the_view->camera_position,
                                      the_view->camera_position + the_view->camera_forward,
                                      the_view->camera_up);
    auto projection_transform = glm::infinitePerspective(the_view->perspective_fovy,
                                                         the_view->perspective_aspect, the_view->perspective_near);
    auto vp_transform = projection_transform * view_transform;



	auto uTransform = glGetUniformLocation(program, "transform");
	auto uTextureUnit = glGetUniformLocation(program, "texture_unit");

	for (auto* obj : parts_list) {
		obj->animate(time, delta_time);
        auto model_transform = obj->animation_transform.matrix() * obj->world_transform.matrix();
		auto transform = vp_transform * model_transform;
		glUniformMatrix4fv(uTransform, 1, GL_FALSE, glm::value_ptr(transform));
		glUniform1i(uTextureUnit, obj->texture_unit);
		glDrawArrays(GL_TRIANGLES, obj->the_shape->buffer_start,
			obj->the_shape->buffer_size);
	}
}


int
main(int argc, char** argv)
{
	glfwInit();
    auto *window = cs4722::setup_window("Compare Noise", .9);
    gladLoadGL(glfwGetProcAddress);
    cs4722::setup_debug_callbacks();
    init();
	the_view->set_flup(glm::vec3(0, 0, -1), glm::vec3(-1, 0, 0),
		glm::vec3(0, 1, 0), glm::vec3(0.3, 0.3, 0.9));
	// init_texture2D();
	init_texture2D_alt();
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
