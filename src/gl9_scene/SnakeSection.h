//
// Created by Janči on 25/11/2018.
//

#ifndef PPGSO_SNAKESECTION_H
#define PPGSO_SNAKESECTION_H

#include <memory>
#include <ppgso/ppgso.h>

#include <mesh.h>
#include "object.h"
#include "scene.h"

class SnakeSection final: public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    // Delay fire and fire rate
    glm::vec3 color{1,1,1};

    float fireDelay{0.0f};
    float fireRate{0.1f};
    glm::vec3 fireOffset{0.7f,0.0f,0.0f};

public:
    /*!
     * Create a new player
     */
    SnakeSection();

    /*!
     * Update player position considering keyboard inputs
     * @param scene Scene to update
     * @param dt Time delta
     * @return true to delete the object
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render player
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;


    /*!
     * Player click event
     * @param scene
     */
    void onClick(Scene &scene) override ;
};


#endif //PPGSO_SNAKESECTION_H
