// Task 6 - Generate a Bezier surface of variable density with UV coordinates.
//        - Confine the Bezier data and associated methods into a reusable class.
//        - Define a modelMatrix that uses position, rotation and scale.
//        - Render the generated mesh with texturing applied.
//        - Animate rotation.

#include <iostream>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>


#include <ppgso/ppgso.h>

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include <GLFW/glfw3.h>
using namespace std;
using namespace glm;
using namespace ppgso;

const unsigned int SIZE = 512;

// Object to represent Bezier patch
class BezierPatch {
private:
    // 3D vectors define points/vertices of the shape
    vector<vec3> vertices;

    // Texture coordinates
    vector<vec2> texCoords;

    // Define our face using indexes to 3 vertices
    struct face {
        GLuint v0, v1, v2;
    };

    // Define our mesh as collection of faces
    vector<int> indices;

    // These will hold the data and object buffers
    GLuint vao, vbo, tbo, ibo;
    mat4 modelMatrix{1};

    vec3 lerp3(const vec3& p1, const vec3& p2, const float t) {
      return p1 * (1.f - t) + p2*t;
    }

    vec3 bezierPoint(const vec3 controlPoints[4], float t) {
      vec3 a = lerp3(controlPoints[0], controlPoints[1], t);
      vec3 b = lerp3(controlPoints[1], controlPoints[2], t);
      vec3 c = lerp3(controlPoints[2], controlPoints[3], t);

      vec3 d = lerp3(a, b, t);
      vec3 e = lerp3(b, c, t);

      return lerp3(d, e, t);
    }

    Shader program{texture_vert_glsl, texture_frag_glsl};
    Texture texture{image::loadBMP("lena.bmp")};
public:
    // Public attributes that define position, color ..
    vec3 position{0,0,0};
    vec3 rotation{0,0,0};
    vec3 scale{1,1,1};

    // Initialize object data buffers
    BezierPatch(const vec3 controlPoints[4][4]) {
      // Generate Bezier patch points and incidences
      unsigned int PATCH_SIZE = 10;
      for(unsigned int i = 0; i < PATCH_SIZE ; i++) {
        for (unsigned int j = 0; j < PATCH_SIZE; j++) {
          // TODO: Compute points on the bezier patch
          // HINT: Compute u, v coordinates

          auto u = j * 1.0f / PATCH_SIZE;
          auto v = i * 1.0f / PATCH_SIZE;

          vector<vec3> body;
          for (int k = 0; k < 4; ++k)
            body.push_back(bezierPoint(controlPoints[k], u));
          vertices.push_back(bezierPoint(body.data(), v));

          texCoords.push_back({u, 1-v});
        }
      }
      // Generate indices
      for(unsigned int i = 1; i < PATCH_SIZE; i++) {
        for (unsigned int j = 1; j < PATCH_SIZE; j++) {
          // TODO: Compute indices for triangle 1

          indices.push_back((i-1) + PATCH_SIZE * j);
          indices.push_back((i-1) + PATCH_SIZE * (j-1));
          indices.push_back(i + PATCH_SIZE*(j-1));

          // TODO: Compute indices for triangle 2
          indices.push_back((i-1) + PATCH_SIZE*j);
          indices.push_back(i + PATCH_SIZE*j);
          indices.push_back(i + PATCH_SIZE*(j - 1));
        }
      }

      // Copy data to OpenGL
      glGenVertexArrays(1, &vao);
      glBindVertexArray(vao);

      // Copy positions to gpu
      glGenBuffers(1, &vbo);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), vertices.data(), GL_STATIC_DRAW);

      // Set vertex program inputs
      auto position_attrib = program.getAttribLocation("Position");
      glEnableVertexAttribArray(position_attrib);
      glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

      // Copy texture positions to gpu
      glGenBuffers(1, &tbo);
      glBindBuffer(GL_ARRAY_BUFFER, tbo);
      glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(vec2), texCoords.data(), GL_STATIC_DRAW);

      // Set vertex program inputs
      auto texCoord_attrib = program.getAttribLocation("TexCoord");
      glEnableVertexAttribArray(texCoord_attrib);
      glVertexAttribPointer(texCoord_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

      // Copy indices to gpu
      glGenBuffers(1, &ibo);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

    };
    // Clean up
    ~BezierPatch() {
      // Delete data from OpenGL
      glDeleteBuffers(1, &ibo);
      glDeleteBuffers(1, &tbo);
      glDeleteBuffers(1, &vbo);
      glDeleteVertexArrays(1, &vao);
    }
    // Set the object transformation matrix
    void update() {
      // TODO: Compute transformation by scaling, rotating and then translating the shape
      modelMatrix = glm::mat4{1};

      modelMatrix = glm::scale(modelMatrix, scale);

      modelMatrix = glm::rotate(modelMatrix, rotation.x, {1, 0, 0});
      modelMatrix = glm::rotate(modelMatrix, rotation.y, {0, 1, 0});
      modelMatrix = glm::rotate(modelMatrix, rotation.z, {0, 0, 1});

      modelMatrix = glm::translate(modelMatrix, position);
    }

    // Draw polygons
    void render(int time){
      // Update transformation and color uniforms in the shader
      program.use();

      // Initialize projection
      // Create projection matrix (field of view (radians), aspect ratio, near plane distance, far plane distance)
      // You can think of this as the camera objective settings
      auto projection = perspective( (PI/180.f) * 60.0f, 1.0f, 0.1f, 10.0f);
      program.setUniform("ProjectionMatrix", projection);


      // Create view matrix (translate camera a bit backwards, so we can see the geometry)
      // This can be seen as the camera position/rotation in space
        auto time2 = (float) glfwGetTime();

      auto view = translate(mat4{1}, {-0.5f, 0.5f, -4.0f});
        view = glm::rotate(view, cos(time2), {1,0,0});
        view = translate(view, {0.5f, -0.5f, 1.f});

      program.setUniform("ViewMatrix", view);

      // Set model position
      program.setUniform("ModelMatrix", modelMatrix);

      // Bind texture
      program.setUniform("Texture", texture);

      glBindVertexArray(vao);
      // TODO: Use correct rendering mode to draw the result
      glDrawElements(GL_TRIANGLES, (GLsizei) indices.size() * 3, GL_UNSIGNED_INT , 0);
    };
};

class BezierSurfaceWindow : public Window {
private:
    // Define 16 control points
    vec3 controlPoints[4][4]{
            { {-1,1,0}, {-0.5,1,0}, {.5,1,0}, {1,1,0}, },
            { {-1,.5,0}, {-0.5,.5,0}, {.5,.5,0}, {1,.5,0}, },
            { {-1,-.5,0}, {-0.5,-.5,0}, {.5,-.5,0}, {1,-.5,0}, },
            { {-1,-1,0}, {-0.5,-1,0}, {.5,-1,0}, {1,-1,0}, },
    };

    BezierPatch bezier{controlPoints};
public:
    BezierSurfaceWindow() : Window{"task6_bezier_surface", SIZE, SIZE} {
      // Initialize OpenGL state
      // Enable Z-buffer
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);
      //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    }


    void onIdle() final {
      // Set gray background
      glClearColor(.1f,.1f,.1f,1.0f);


      // Clear depth and color buffers
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


      // Move and Render shape
      auto time = (float) glfwGetTime();
      //controlPoints[0][0].x =  cos(time);
      //controlPoints[0][0].y =  sin(time);


      controlPoints[0][0].z = 0;
      controlPoints[1][0].z = 0;
      controlPoints[2][0].z = 0;
      controlPoints[3][0].z = 0;
      controlPoints[0][1].z = 0;
      controlPoints[1][1].z = sin(time) * 2;
      controlPoints[2][1].z = sin(time) * 2;
      controlPoints[3][1].z = 0;
      controlPoints[0][2].z = 0;
      controlPoints[1][2].z = sin(time) * 2;
      controlPoints[2][2].z = sin(time) * 2;
      controlPoints[3][2].z = 0;
      controlPoints[0][3].z = 0;
      controlPoints[1][3].z = 0;
      controlPoints[2][3].z = 0;
      controlPoints[3][3].z = 0;


//        controlPoints[0][3].z = cos(time+3)/2;
//        controlPoints[1][3].z = cos(time+3)/2;
//        controlPoints[2][3].z = cos(time+3)/2;
//        controlPoints[3][3].z = cos(time+3)/2;

      //controlPoints[3][0].x =  cos(time);
      //controlPoints[3][0].y =  sin(time);
      //controlPoints[3][0].z = cos(time);

      BezierPatch bezier(controlPoints);

      bezier.rotation = {0, 0, cos(time)};
      //bezier.rotation = {0, 0, time};
      //bezier.scale = {cos(time), 1, 1};
      bezier.update();
      bezier.render(time);
    }
};

int main() {
  // Create new window
  auto window = BezierSurfaceWindow{};

  // Main execution loop
  while (window.pollEvents()) {}

  return EXIT_SUCCESS;
}