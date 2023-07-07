


#include "sharing.h"

static cs4722::view *the_view;
static GLuint program;
static std::vector<cs4722::artifact*> part_list;

static GLuint transform_loc;
static GLuint sampler_loc;


static GLuint vao;

//static GLuint frame_buffer_height;
//static GLuint frame_buffer_width;
static GLuint frame_buffer;
//static GLuint fb_texture_unit = 61;

static std::vector<GLuint> texture_unit_list;
#define tlget(i)  texture_unit_list[i % texture_unit_list.size()]

static GLuint setup_frame_buffer()
{
    glActiveTexture(GL_TEXTURE0 + fb_texture_unit);
    GLuint texture;
    // glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // glTextureStorage2D(texture, 1, GL_RGBA8, fb_width, fb_height);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, frame_buffer_width, frame_buffer_height);

    GLuint fb;
    glGenFramebuffers(1, &fb);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb);
    glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

    GLuint depth_buffer;
    glGenRenderbuffers(1, &depth_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, frame_buffer_width, frame_buffer_height);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, depth_buffer);

    //  http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/

    // GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    // glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    std::cout << "frame buffer complete? " <<
              glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) <<
              "(should be " << GL_FRAMEBUFFER_COMPLETE << ")" <<
              std::endl;

    return fb;
}





void parts_setup(cs4722::view* view)
{

    the_view = view;

    program = cs4722::compile_shaders("scene_vertex_shader05B.glsl",
                                      "scene_fragment_shader05B.glsl");
    glUseProgram(program);

    transform_loc = glGetUniformLocation(program, "transform");
    sampler_loc = glGetUniformLocation(program, "sampler");

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);


    cs4722::init_texture_from_file("../media/square-2703542_512x512.jpg", 0);
    cs4722::init_texture_from_file("../media/tulips-bed-2048x2048.png", 2);
    texture_unit_list.push_back(2);
    cs4722::init_texture_computed(1, 8);
    texture_unit_list.push_back(1);

    frame_buffer = setup_frame_buffer();




    cs4722::shape* b = new cs4722::sphere(15, 50);
    auto* obj = new cs4722::artifact_rotating();
    obj->the_shape = (b);
    obj->world_transform.translate = (glm::vec3(.5, -.5, 0));
    obj->rotation_rate = (M_PI / 3);
    obj->world_transform.scale = (glm::vec3(.4f, .4f, .4f));
//    obj->the_color = (cs4722::x11::gold);
    obj->animation_transform.rotation_axis = (glm::vec3(1, 1, 0));
    obj->animation_transform.rotation_center =
            obj->world_transform.matrix() * glm::vec4(0,0,0,1);
    obj->texture_unit = (0);

    part_list.push_back(obj);


    cs4722::shape* b1 = new cs4722::block();
    auto* obj1 = new cs4722::artifact_rotating();
    obj1->the_shape = (b1);
    obj1->world_transform.translate = (glm::vec3(-.5, 0, 0));
    obj1->rotation_rate = (M_PI / 3);
//    obj1->animation_transform.rotation_center = (glm::vec3(0, .3f, 0));
    obj1->world_transform.scale = (glm::vec3(.2f, .6f, .2f));
    obj1->animation_transform.rotation_axis = (glm::vec3(1, 0, 0));
    obj1->animation_transform.rotation_center =
            obj1->world_transform.matrix() * glm::vec4(0,.3,0,1);
//    obj1->the_color = (cs4722::x11::magenta);
    obj1->texture_unit = (1);

    part_list.push_back(obj1);


    cs4722::shape* b2 = new cs4722::torus(.5f, 17, 20);
    auto* obj2 = new cs4722::artifact_rotating();
    obj2->the_shape = (b2);
    obj2->world_transform.translate = (glm::vec3(0, 0, 0));
    obj2->animation_transform.rotation_axis = (glm::vec3(1, 0, 0));
    obj2->rotation_rate = (M_PI / 3);
    //obj2->rotation_center(glm::vec3(0, .3f, 0));
    obj2->world_transform.scale = (glm::vec3(.2f, .2f, .2f));
    obj2->animation_transform.rotation_center =
            obj2->world_transform.matrix() * glm::vec4(0,0,0,1);
    obj2->texture_unit = (2);
//    obj2->the_color = (cs4722::x11::chartreuse);

    part_list.push_back(obj2);

    vao = cs4722::init_buffers(program, part_list, "bPosition", "", "bTextureCoord");

}



//----------------------------------------------------------------------------
//
// display
//


void parts_display()
{

    glBindVertexArray(vao);
    glUseProgram(program);

    static auto last_time = 0.0;

    auto view_transform = glm::lookAt(the_view->camera_position,
                                      the_view->camera_position + the_view->camera_forward,
                                      the_view->camera_up);
    auto projection_transform = glm::infinitePerspective(the_view->perspective_fovy,
                                                         the_view->perspective_aspect, the_view->perspective_near);
    auto vp_transform = projection_transform * view_transform;


    auto time = glfwGetTime();
    auto delta_time = time - last_time;
    last_time = time;


    for (auto obj : part_list) {

        obj->animate(time, delta_time);

        auto model_transform = obj->animation_transform.matrix() * obj->world_transform.matrix();
        auto transform = vp_transform * model_transform;
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));
        glUniform1i(sampler_loc, obj->texture_unit);

        glDrawArrays(GL_TRIANGLES, obj->the_shape->buffer_start, obj->the_shape->buffer_size);
    }
}


void parts_setup_for_fb() {
//    the_view->reverse_camera();
    the_view->camera_forward = -the_view->camera_forward;
    the_view->camera_left = -the_view->camera_left;
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame_buffer);
    glViewport(0, 0, frame_buffer_width, frame_buffer_height);
    parts_display();
    the_view->camera_forward = -the_view->camera_forward;
    the_view->camera_left = -the_view->camera_left;
}

void parts_setup_for_window(GLFWwindow* window) {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    int w_width, w_height;
    glfwGetFramebufferSize(window, &w_width, &w_height);
    glViewport(0, 0, w_width, w_height);

}
