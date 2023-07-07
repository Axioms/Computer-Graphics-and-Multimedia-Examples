


#include <GLM/vec4.hpp>
#include <GLM/vec2.hpp>
#include <GLM/vec3.hpp>
#include <glm/gtc/noise.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include "cs4722/x11.h"
#include "cs4722/compile_shaders.h"
#include "cs4722/window.h"
#include "cs4722/callbacks.h"


#include "FastNoiseLite.h"


const GLuint  NumVertices = 6;
GLuint program;

void init(void)
{


    program = cs4722::compile_shaders("vertex_shader04.glsl","fragment_shader04.glsl" );
    glUseProgram(program);

    glEnable(GL_PROGRAM_POINT_SIZE);
}

void init_buffers() {


    GLfloat a = .7f;
    GLfloat b = .7f;
    glm::vec4 ul = glm::vec4(-b, a, 0, 1);
    glm::vec4 ll = glm::vec4(-b, -b, 0, 1);
    glm::vec4 lr = glm::vec4(a, -b, 0, 1);
    glm::vec4 ur = glm::vec4(a, a, 0, 1);

    glm::vec4 position_list[NumVertices] = { ll, ur, ul, ur, ll, lr };


    // generate the texture coordinate data

	auto upper_texture = 1.0f;
	auto lower_texture = 0.0f;
	auto ult = glm::vec2(lower_texture, upper_texture);
	auto llt = glm::vec2(lower_texture, lower_texture);
	auto lrt = glm::vec2(upper_texture, lower_texture);
	auto urt = glm::vec2(upper_texture, upper_texture);
	glm::vec2 texture_coordinate_list[NumVertices] = { llt, urt, ult, urt, llt, lrt };



    const auto number_of_buffers = 2;
    GLuint buffers[number_of_buffers];

    glCreateBuffers(number_of_buffers, buffers);
    auto bPosition = glGetAttribLocation(program, "bPosition");
    auto bTextureCoord = glGetAttribLocation(program, "bTextureCoord");
   
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferStorage(GL_ARRAY_BUFFER, 4 * 4 * NumVertices, position_list, 0);
    glVertexAttribPointer(bPosition, 4, GL_FLOAT,
        GL_FALSE, 0, 0);
    glEnableVertexAttribArray(bPosition);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferStorage(GL_ARRAY_BUFFER, 4 * 2 * NumVertices,
        texture_coordinate_list, 0);
    glVertexAttribPointer(bTextureCoord, 2, GL_FLOAT,
        GL_FALSE, 0, 0);
    glEnableVertexAttribArray(bTextureCoord);

   

}


void init_texture3D() {


    const auto texture_size = 8;
    auto *texture_data = new GLubyte[4*texture_size*texture_size*texture_size];

    auto const number_of_octaves = 4;

	auto  frequency = 4.0f;
    auto amp = 0.5f;

	
	for(auto f = 0; f < number_of_octaves; ++f)
	{
        auto inc = 1.0f / (texture_size / frequency);

        auto* ptr = texture_data;
		
        for (auto i = 0; i < texture_size; ++i)
        {
	        for (auto j = 0; j < texture_size; ++j)
	        {
		        for (auto k = 0; k < texture_size; ++k)
		        {

                    *(ptr + f) = static_cast<GLubyte>((glm::simplex(glm::vec3(i, j,k) * inc) + 1.0f) * amp * 128.0f);
                    ptr += 4;
		        }
	        }
        }

		
        frequency *= 2;
        ++inc;
        amp *= 0.5;
	}


	

    // initialize texture and sampler

    GLuint texture;
    glCreateTextures(GL_TEXTURE_3D, 1, &texture);

    auto number_of_levels = 1;
    auto internal_format = GL_RGBA8;
    auto external_format = GL_RGBA;

   
    glTextureStorage3D(texture, number_of_levels, internal_format, texture_size, texture_size, texture_size);
    glTextureSubImage3D(texture, 0, 0, 0, 0, texture_size, texture_size, texture_size, 
       external_format, GL_UNSIGNED_BYTE, texture_data);
    delete texture_data;
    glBindTextureUnit(3, texture);

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

    glUniform1i(sampler_loc, 3);
    
}



void init_texture3D_F() {


    const auto texture_size = 128;
    auto* texture_data = new GLubyte[4 * texture_size * texture_size * texture_size];


    auto const number_of_octaves = 1;


    auto  frequency = 4.0f;
    auto amp = 0.5f;
    //int inc = 0;

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetFrequency(frequency);

    for (auto f = 0; f < number_of_octaves; ++f)
    {
        // auto ni = glm::vec3(0.0);
        auto inc = 1.0f / (texture_size / frequency);

        auto* ptr = texture_data;

        for (auto i = 0; i < texture_size; ++i)
        {
            for (auto j = 0; j < texture_size; ++j)
            {
                for (auto k = 0; k < texture_size; ++k)
                {
                    //(GLubyte)(((noise3(ni)+1.0) * amp) * 128.0);

                    auto sample = noise.GetNoise(i * inc, j * inc, k * inc);
                	
                    *(ptr + f) = static_cast<GLubyte>((sample + 1.0f) * amp * 128.0f);
                    // *(ptr + f) = static_cast<GLubyte>(glm::perlin(ni / frequency) * amp * 128.0f);
                    // ni.z += inc;
                    ptr += 4;
                }
                // ni.y += inc;
            }
            // ni.x += inc;
        }


        frequency *= 2;
        ++inc;
        amp *= 0.5;
    }




    // initialize texture and sampler

    GLuint texture;
    glCreateTextures(GL_TEXTURE_3D, 1, &texture);

    auto number_of_levels = 1;
    auto internal_format = GL_RGBA8;
    auto external_format = GL_RGBA;


    glTextureStorage3D(texture, number_of_levels, internal_format, texture_size, texture_size, texture_size);
    glTextureSubImage3D(texture, 0, 0, 0, 0, texture_size, texture_size, texture_size,
        external_format, GL_UNSIGNED_BYTE, texture_data);
    delete texture_data;
    glBindTextureUnit(4, texture);

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

    glUniform1i(sampler_loc, 4);

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
                *(ptr + f) = static_cast<GLubyte>((glm::simplex(glm::vec2(i,j) * inc) + 1.0f) * amp * 128.0f);
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


    auto sampler_loc = glGetUniformLocation(program, "sampler2");

    glUniform1i(sampler_loc, 2);

}




//----------------------------------------------------------------------------
//
// display
//

void
display(void)
{
    glClearBufferfv(GL_COLOR, 0, cs4722::x11::gray50.as_float());
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

int
main(int argc, char** argv)
{
    glfwInit();

    auto *window = cs4722::setup_window("Noise on Square", .9);

    gladLoadGL(glfwGetProcAddress);
    cs4722::setup_debug_callbacks();


    init();
    init_buffers();
//     init_texture3D();
    // init_texture2D();
    init_texture3D_F();
	
//    std::cout << "OpenGL version " << glGetString(GL_VERSION) << std::endl;
//    std::cout << "OpenGL renderer " << glGetString(GL_RENDERER) << std::endl;
//    GLint max_texture_units;
//    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_units);
//    std::cout << "max texture units " << max_texture_units << std::endl;
//    GLint max_texture_size;
//    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &max_texture_size);
//    std::cout << "max texture size " << max_texture_size << std::endl;
	

    while (!glfwWindowShouldClose(window))
    {
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
}
