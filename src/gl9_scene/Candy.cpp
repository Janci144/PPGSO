#pragma once
//
// Created by Janči on 24/11/2018.
//

#include <glm/gtc/random.hpp>
#include "Candy.h"
#include "SnakeSection.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
unique_ptr<Mesh> Candy::mesh;
unique_ptr<Texture> Candy::texture;
unique_ptr<Shader> Candy::shader;

Candy::Candy() {
    // Set random scale speed and rotation
    //scale *= linearRand(1.0f, 3.0f);
    //speed = {linearRand(-2.0f, 2.0f), linearRand(-5.0f, -10.0f), 0.0f};
    //rotation = ballRand(PI);
    //rotMomentum = ballRand(PI);

    // Initialize static resources if needed
    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = make_unique<Texture>(image::loadBMP("asteroid.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("asteroid.obj");
}

bool Candy::update(Scene &scene, float dt) {
    if(is_destroyed)
        return false;

    // Count time alive
    age += dt;

    if (age > 10.0f) return false;

//    //kontrolovanie kolizie
//    for (auto &obj : scene.objects) {
//        // Ignore self in scene
//        if (obj.get() == this) continue;
//
//        // We only need to collide with asteroids and projectiles, ignore other objects
//        auto section = dynamic_cast<SnakeSection*>(obj.get()); // dynamic_pointer_cast<Asteroid>(obj);
//        //auto candy = dynamic_cast<Candy*>(obj.get()); // dynamic_pointer_cast<Asteroid>(obj);
//
//        if (!section) {
//            //std::cout << "Nie je to SnakeSection ani sladkost nechapem" << endl;
//            continue;
//        }
//
//
//        // When colliding with other asteroids make sure the object is older than .5s
//        // This prevents excessive collisions when asteroids explode.
//        //if (age < 0.5f) continue;
//
//        // Compare distance to approximate size of the asteroid estimated from scale.
//        if(section) {
//            if (distance(position, obj->position) < (obj->scale.y + scale.y) * 0.7f) {
//
//                if (section) {
//                    std::cout << "Koniec hry!!!!!!!!!!!!!!!!!!1" << endl;
//                    return false;
//                }
//
//                break;
////            int pieces = 3;
////
////            // Too small to split into pieces
////            if (scale.y < 0.5) pieces = 0;
//
//                // The projectile will be destroyed
//                //if () ->destroy();
//                //section->destroy();
//
//                // Generate smaller asteroids
//                //explode(scene, (obj->position + position) / 2.0f, (obj->scale + scale) / 2.0f, pieces);
//
//                // Destroy self
//            }
//        }
//    }

    // Animate position according to time
    //position += speed * dt;

    // Rotate the object
    //rotation += rotMomentum * dt;

    // Delete when alive longer than 10s or out of visibilit

    // Collide with scene
//    for (auto &obj : scene.objects) {
//        // Ignore self in scene
//        if (obj.get() == this) continue;
//
//        // We only need to collide with asteroids and projectiles, ignore other objects
//        auto section = dynamic_cast<SnakeSection*>(obj.get()); // dynamic_pointer_cast<Asteroid>(obj);
//        if (!section) continue;
//
//        // When colliding with other asteroids make sure the object is older than .5s
//        // This prevents excessive collisions when asteroids explode.
//        //if (age < 0.5f) continue;
//
//        // Compare distance to approximate size of the asteroid estimated from scale.
//        if (distance(position, obj->position) < (obj->scale.y + scale.y) * 0.7f) {
//            int pieces = 3;
//
//            // Too small to split into pieces
//            if (scale.y < 0.5) pieces = 0;
//
//            // The projectile will be destroyed
//            //if () ->destroy();
//            //section->destroy();
//
//            // Generate smaller asteroids
//            //explode(scene, (obj->position + position) / 2.0f, (obj->scale + scale) / 2.0f, pieces);
//
//            // Destroy self
//            return false;
//        }
//    }

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}

void Candy::explode(Scene &scene, vec3 explosionPosition, vec3 explosionScale, int pieces) {
    // Generate explosion
//    auto explosion = make_unique<Explosion>();
//    explosion->position = explosionPosition;
//    explosion->scale = explosionScale;
//    explosion->speed = speed / 2.0f;
//    scene.objects.push_back(move(explosion));
//
//    // Generate smaller asteroids
//    for (int i = 0; i < pieces; i++) {
//        auto asteroid = make_unique<Asteroid>();
//        asteroid->speed = speed + vec3(linearRand(-3.0f, 3.0f), linearRand(0.0f, -5.0f), 0.0f);;
//        asteroid->position = position;
//        asteroid->rotMomentum = rotMomentum;
//        float factor = (float) pieces / 2.0f;
//        asteroid->scale = scale / factor;
//        scene.objects.push_back(move(asteroid));
//    }
}

void Candy::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}

void Candy::onClick(Scene &scene) {
    cout << "Asteroid clicked!" << endl;
    explode(scene, position, {10.0f, 10.0f, 10.0f}, 0 );
    age = 10000;
}

void Candy::destroy() {
    is_destroyed = true;
}

