//
// Created by Janči on 24/11/2018.
//

#include <glm/gtc/random.hpp>
#include <ppgso/ppgso.h>
#include "CandyGenerator.h"
#include "Candy.h"

using namespace std;
using namespace glm;
using namespace ppgso;

bool CandyGenerator::update(Scene &scene, float dt) {
    // Accumulate time
    time += dt;

    int iterator = 0;

//    auto it = std::begin(scene.objects);
//    if( *it == this)
//    scene.objects.erase(*this);

    // Add object to scene when time reaches certain level
    if (time > .3) {
        //auto obj = make_unique<Asteroid>();
        //obj->position = position;
        //obj->position.x += linearRand(-20.0f, 20.0f);

        auto it = std::begin(scene.objects);

        while (it != std::end(scene.objects)) {
            // Najde seb
            auto obj = it->get();
            if( obj == this)
                //scene.objects.erase(it);

            it++;
        }

        auto obj = make_unique<Candy>();
        obj->position = vec3(5,5,0);
        //obj->position.x += linearRand(-20.0f, 20.0f);

        scene.objects.push_back(move(obj));
        time = 0;
    }

    return true;
}

void CandyGenerator::render(Scene &scene) {
    // Generator will not be rendered
}