#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <rendering/shader.hpp>

static const GLuint WIDTH = 800;
static const GLuint HEIGHT = 600;

/* ourColor is passed on to the fragment shader. */
#define CIRCLE_SIDES 100
#define CIRCLE_VERTS (CIRCLE_SIDES + 2)
#define COLOR_LEN 3

class Circle {
public:

  Circle(float x, float y) : _x(x), _y(y), _radius(1) {
    update_verts();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STREAM_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
  }

  float verts[3 * CIRCLE_VERTS];
  float color[COLOR_LEN] = {0.5, 0.5, 0.5};
  unsigned int vao; // vertex attribute object
  unsigned int vbo; // vertex buffer object (gpu memory)

  float x() const {
    return _x;
  }

  float y() const {
    return _y;
  }

  void set_location(float x, float y) {
    _x = x;
    _y = y;
  }

  void set_radius(float radius) {
    _radius = radius;
  }

  void draw(Shader &shader) {
    shader.setVec3("color", color[0], color[1], color[2]);

    auto world_position = glm::vec3(_x, _y, 0);

    glm::mat4 position_transform(1);
    glm::mat4 scale_transform(1);
    position_transform = glm::translate(position_transform, world_position);
    scale_transform = glm::scale(scale_transform, glm::vec3(_radius, _radius, 0));

    glm::mat4 model_matrix = position_transform * scale_transform;

    GLint model_loc = glGetUniformLocation(shader.program, "model_transform");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, &model_matrix[0][0]);

    // draw them!
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_VERTS);
    glBindVertexArray(0);
  }

private:
  float _x;
  float _y;
  float _radius;

  void update_verts() {
    verts[0] = 0;
    verts[1] = 0;
    verts[2] = 0;
    for (int i = 1; i < CIRCLE_VERTS; i++) {
      verts[i * 3] = static_cast<float> (0 + (1 * cos(i *  2 * M_PI / CIRCLE_SIDES)));
      verts[i * 3 + 1] = static_cast<float> (0 + (1 * sin(i * 2 * M_PI / CIRCLE_SIDES)));
      verts[i * 3 + 2] = 0;
    }
  }
};

void set_view_projection(Shader &shader, float x, float y) {
  glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f);
  GLint proj_location = glGetUniformLocation(shader.program, "projection_transform");
  glUniformMatrix4fv(proj_location, 1, GL_FALSE, &Projection[0][0]);

  glm::mat4 View = glm::lookAt(
    glm::vec3(x, y, 3), // Camera location in World Space
    glm::vec3(x, y, 0), // camera "looks at" location
    glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
  );
  GLint view_location = glGetUniformLocation(shader.program, "view_transform");
  glUniformMatrix4fv(view_location, 1, GL_FALSE, &View[0][0]);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    double xpos, ypos;
    //getting cursor position
    glfwGetCursorPos(window, &xpos, &ypos);
    std::cout << "Cursor Position at (" << xpos << " : " << ypos << std::endl;
  }
}

int main(int argc, char *argv[]) {

  GLuint vbo;
  GLuint vao;
  GLFWwindow* window;
  double time;

  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

  window = glfwCreateWindow(WIDTH, HEIGHT, __FILE__, nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glewExperimental = GL_TRUE;
  glewInit();

  glfwSetMouseButtonCallback(window, mouse_button_callback);

//  glMatrixMode(GL_PROJECTION);
//  glLoadIdentity();
//  glOrtho(0.0f, WIDTH, HEIGHT, 0.0f, 0.0f, 1.0f);

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glViewport(0, 0, WIDTH, HEIGHT);

  Shader shader("../rendering/vertex.glsl", "../rendering/fragment.glsl");

  Circle circle(0, 0);

  Circle c(0, 0);

  static GLfloat vertices[] = {
    /*   Positions          Colors */
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
  };

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  /* Position attribute */
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
  glEnableVertexAttribArray(0);

  /* Color attribute */
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    double xpos, ypos;
    //getting cursor position
    glfwGetCursorPos(window, &xpos, &ypos);

    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();

    time = glfwGetTime();

    set_view_projection(shader, circle.x(), circle.y());

    circle.set_location(15 - time / 3, 14);
    circle.set_radius(0.1 * (1 + 0.5 * sin(time)));
    circle.draw(shader);

    c.set_location(13, 13);
    circle.set_radius(0.1 * (1 + 0.5 * cos(time)));
    c.draw(shader);

//    transform_location = glGetUniformLocation(shader.program, "transform");
//    GLfloat transform[] = {
//      0.0f, 0.0f, 0.0f, 0.0f,
//      0.0f, 0.0f, 0.0f, 0.0f,
//      0.0f, 0.0f, 1.0f, 0.0f,
//      0.0f, 0.0f, 0.0f, 1.0f,
//    };

//    transform[0] = 1.0f * sin(3.14 * time);
//    transform[5] = 1.0f * cos(time);
//    glUniformMatrix4fv(transform_location, 1, GL_FALSE, transform);

//    glBindVertexArray(vao);
//    glDrawArrays(GL_TRIANGLES, 0, 3);
//    glBindVertexArray(0);

    glfwSwapBuffers(window);
  }
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glfwTerminate();
  return EXIT_SUCCESS;
}