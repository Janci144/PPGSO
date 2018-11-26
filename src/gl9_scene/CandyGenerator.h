//
// Created by Janči on 24/11/2018.
//

#ifndef PPGSO_CANDYGENERATOR_H
#define PPGSO_CANDYGENERATOR_H


#include "object.h"
#include "scene.h"

class CandyGenerator final : public Object {
public:
/*!
 * Generate new asteroids over time
 * @param scene Scene to update
 * @param dt Time delta
 * @return true to delete the object
 */
bool update(Scene &scene, float dt) override;

/*!
 * Render placeholder for generator
 * @param scene Scene to render in
 */
void render(Scene &scene) override;

float time = 0.0f;
};


#endif //PPGSO_CANDYGENERATOR_H
