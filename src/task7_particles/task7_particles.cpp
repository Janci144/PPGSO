#include <iostream>
#include <vector>
#include <map>
#include <list>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#include <ppgso/ppgso.h>

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

const unsigned int SIZE = 512;
vec3 globalRot;
vec3 globalPos;

class Camera {
public:
    // TODO: Add parameters
    vec3 up{0,1,0};
    vec3 position{0,0,0};
    vec3 back{0,0,-1};

    mat4 viewMatrix;
    mat4 projectionMatrix;

    /// Representaiton of
    /// \param fov - Field of view (in degrees)
    /// \param ratio - Viewport ratio (width/height)
    /// \param near - Distance of the near clipping plane
    /// \param far - Distance of the far clipping plane
    Camera(float fov = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f) {
        // TODO: Initialize perspective projection (hint: glm::perspective)
        float fowInRad = (PI/180.0f) * fov;
        projectionMatrix = perspective(fowInRad, ratio, near, far);
    }

    /// Recalculate viewMatrix from position, rotation and scale
    void update() {
        // TODO: Update viewMatrix (hint: glm::lookAt)
        viewMatrix = lookAt(position, back, up);
    }
};

/// Abstract renderable object interface
class Renderable; // Forward declaration for Scene
using Scene = list<unique_ptr<Renderable>>; // Type alias

class Renderable {
public:
    // Virtual destructor is needed for abstract interfaces
    virtual ~Renderable() = default;

    /// Render the object
    /// \param camera - Camera to use for rendering
    virtual void render(const Camera &camera) = 0;

    /// Update the object. Useful for specifing animation and behaviour.
    /// \param dTime - Time delta
    /// \param scene - Scene reference
    /// \return - Return true to keep object in scene
    virtual bool update(float dTime, Scene &scene) = 0;
};

/// Basic particle that will render a sphere
/// TODO: Implement Renderable particle7

class Particle1 final : public Renderable {
    // Static resources shared between all particles
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;

    // TODO: add more parameters as needed
    float age{2.f};
    //vec3 color{(double) rand() / (RAND_MAX),(double) rand() / (RAND_MAX),(double) rand() / (RAND_MAX)};
    vec3 color{0,0,1};
    vec3 speed{0,-10,0};


    vec3 position{0,-10,-10};
    //vec3 position{0,5,5};
    vec3 rotation{0,0,0};
    vec3 scale{0.3,0.3,0.3};
    mat4 modelMatrix{1};
public:
    /// Construct a new Particle
    /// \param p - Initial position
    /// \param s - Initial speed
    /// \param c - Color of particle
    Particle1(vec3 p, vec3 s, vec3 c) {
        // First particle will initialize resources
        if (!shader) shader = make_unique<Shader>(color_vert_glsl, color_frag_glsl);
        if (!mesh) mesh = make_unique<Mesh>("sphere.obj");
        position = p;
        speed = s;
        color = c;
    }

    bool update(float dTime, Scene &scene) override {
        // TODO: Animate position using speed and dTime.
        // - Return true to keep the object alive
        // - Returning false removes the object from the scene
        // - hint: you can add more particles to the scene here also
        age -= dTime;
        if (age < 0)
            return false;

        position += speed * dTime;
        //position.y -= age*.05f;
        speed += vec3{0,-9.8,0}*dTime;
        modelMatrix = glm::translate(mat4(1.0f), position)
                      * glm::orientate4(rotation)
                      * glm::scale(mat4(1.0f), scale);
        return true;
    }

    void render(const Camera &camera) override {
        // TODO: Render the object
        // - Use the shader
        // - Setup all needed shader inputs
        // - hint: use OverallColor in the color_vert_glsl shader for color
        // - Render the mesh
        shader->use();
        shader->setUniform("ProjectionMatrix", camera.projectionMatrix);
        shader->setUniform("ViewMatrix", camera.viewMatrix);
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("OverallColor", color);
        mesh->render();
    }
};

class Particle final : public Renderable {
    // Static resources shared between all particles
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;

    // TODO: add more parameters as needed
    float age{0.0f};
    bool be_weird = false;
    vec3 color{1,0,1};
    //vec3 color{(double) rand() / (RAND_MAX),(double) rand() / (RAND_MAX),(double) rand() / (RAND_MAX)};

    //  vec3 speed{0,40,0};
    vec3 speed{-20,0,0};

    vec3 rot = globalRot;

    // vec3 position{linearRand(-20.0f,20.0f),30,-10};
    vec3 position{15, globalPos.y,-10};

    vec3 rotation{0,0,0};
    vec3 scale{1,1,1};
    mat4 modelMatrix{1};

    int v1 = rand() % 2;
public:
    /// Construct a new Particle
    /// \param p - Initial position
    /// \param s - Initial speed
    /// \param c - Color of particle
    Particle() {
        // First particle will initialize resources
        if(v1 == 0)
            be_weird = true;
        if (!shader) shader = make_unique<Shader>(color_vert_glsl, color_frag_glsl);
        if (!mesh) mesh = make_unique<Mesh>("sphere.obj");
    }

    bool update(float dTime, Scene &scene) override {
        // TODO: Animate position using speed and dTime.
        // - Return true to keep the object alive
        // - Returning false removes the object from the scene
        // - hint: you can add more particles to the scene here also
        age += dTime;
        //age > 1.0f ||
        if (position.x < -10) {
           // for(int i=0; i<100; i++)
             //   scene.push_back(make_unique<Particle1>(position, vec3{linearRand(-10.0f, 10.0f), linearRand(-2.0f, 10.0f), linearRand(-2.0f, 2.0f)}, vec3{linearRand(0.0f, 1.0f),linearRand(0.0f, 1.0f),linearRand(0.0f, 1.0f)}));
            return false;
        }

        double ran = (double) rand() / (RAND_MAX);

        int v1 = rand() % 2;
        if(v1 == 0)
            ran = -ran;

        std::cout << rot.y << '\n';
        //float a = 3;
        speed += vec3{0,-4,0}*dTime;
        position += vec3{0,-rot.y /10, 0} ;

        if(be_weird)
        {
            position += vec3{0,sin(ran)/10,0};
        }

        position += speed * dTime;
        //position.y -= age*.005f;
        speed -= vec3{0,1,0}*dTime;
        modelMatrix = glm::translate(mat4(1.0f), position)
                      //* glm::orientate4(rotation)
                      * glm::scale(mat4(1.0f), scale);
        return true;
    }

    void render(const Camera &camera) override {
        // TODO: Render the object
        // - Use the shader
        // - Setup all needed shader inputs
        // - hint: use OverallColor in the color_vert_glsl shader for color
        // - Render the mesh
        shader->use();
        shader->setUniform("ProjectionMatrix", camera.projectionMatrix);
        shader->setUniform("ViewMatrix", camera.viewMatrix);
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("OverallColor", color);
        mesh->render();
    }
};

// Static resources need to be instantiated outside of the class as they are globals
std::unique_ptr<ppgso::Mesh> Particle::mesh;
std::unique_ptr<ppgso::Shader> Particle::shader;
std::unique_ptr<ppgso::Mesh> Particle1::mesh;
std::unique_ptr<ppgso::Shader> Particle1::shader;


class Hocus final : public Renderable {
    // Static resources shared between all particles
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;

    // TODO: add more parameters as needed
    float age{2.f};
    double rot = -0.0005;
    //vec3 color{(double) rand() / (RAND_MAX),(double) rand() / (RAND_MAX),(double) rand() / (RAND_MAX)};
    vec3 color{1,0,0};
    vec3 speed{0,5,0};

    vec3 position{18,0,-10};
    //vec3 position{0,5,5};
    vec3 rotation{0,0,0};
    vec3 scale{4,0.5,0};
    mat4 modelMatrix{1};
public:
    /// Construct a new Particle
    /// \param p - Initial position
    /// \param s - Initial speed
    /// \param c - Color of particle

    Hocus() {
        // First particle will initialize resources
        if (!shader) shader = make_unique<Shader>(color_vert_glsl, color_frag_glsl);
        if (!mesh) mesh = make_unique<Mesh>("cube.obj");
    }

//    Hocus(vec3 p, vec3 s, vec3 c) {
//        // First particle will initialize resources
//        if (!shader) shader = make_unique<Shader>(color_vert_glsl, color_frag_glsl);
//        if (!mesh) mesh = make_unique<Mesh>("sphere.obj");
//        position = p;
//        speed = s;
//        color = c;
//    }

    bool update(float dTime, Scene &scene) override {
        // TODO: Animate position using speed and dTime.
        // - Return true to keep the object alive
        // - Returning false removes the object from the scene
        // - hint: you can add more particles to the scene here also
       // age -= dTime;
//        if (age < 0)
//            return false;

        if(rotation.y > 0.3)
            rot += -0.0005;
        if(rotation.y < -0.3)
            rot += 0.0005;

        if(position.y > 18)
        {

            speed = {0,-5,0};
        }

        if(position.y < -18)
        {
            speed = {0,5,0};
            rot = -0.0005;
        }

        globalRot = rotation;
        globalPos = position;
        rotation += vec3{0,rot,0};
        position += speed * dTime;
        //position.y -= age*.05f;
        //speed += vec3{0,,0}*dTime;
        modelMatrix = glm::translate(mat4(1.0f), position)
                      * glm::orientate4(rotation)
                      * glm::scale(mat4(1.0f), scale);
        return true;
    }

    void render(const Camera &camera) override {
        // TODO: Render the object
        // - Use the shader
        // - Setup all needed shader inputs
        // - hint: use OverallColor in the color_vert_glsl shader for color
        // - Render the mesh
        shader->use();
        shader->setUniform("ProjectionMatrix", camera.projectionMatrix);
        shader->setUniform("ViewMatrix", camera.viewMatrix);
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("OverallColor", color);
        mesh->render();
    }
};

std::unique_ptr<ppgso::Mesh> Hocus::mesh;
std::unique_ptr<ppgso::Shader> Hocus::shader;

class ParticleWindow : public Window {
private:
    // Scene of objects
    Scene scene;

    // Create camera
    Camera camera = {130.0f, (float) width / (float) height, 1.0f, 400.0f};


    // Store keyboard state
    map<int, int> keys;
public:
    ParticleWindow() : Window{"task7_particles", SIZE, SIZE} {
        // Initialize OpenGL state
        // Enable Z-buffer
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        scene.push_back(make_unique<Hocus>());
    }

    void onKey(int key, int scanCode, int action, int mods) override {
        // Collect key state in a map
        keys[key] = action;
        if (keys[GLFW_KEY_SPACE]) {
            // TODO: Add renderable object to the scene
//            auto i = std::begin(scene);
//                auto obj = i->get();
            scene.push_back(make_unique<Particle>());
        }
    }

    void onIdle() override {
        // Track time
        static auto time = (float) glfwGetTime();
        // Compute time delta
        float dTime = (float) glfwGetTime() - time;
        time = (float) glfwGetTime();

        // Set gray background
        glClearColor(.1f, .1f, .1f, 1.0f);

        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update all objects in scene
        // Because we need to delete while iterating this is implemented using c++ iterators
        // In most languages mutating the container during iteration is undefined behaviour
        auto i = std::begin(scene);
        while (i != std::end(scene)) {
            // Update object and remove from list if needed
            auto obj = i->get();
            if (!obj->update(dTime, scene))
                i = scene.erase(i);
            else
                ++i;
        }

        // Render every object in scene
        for (auto &object : scene) {
            object->render(camera);
        }
    }
};

int main() {
    // Create new window
    auto window = ParticleWindow{};

    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}