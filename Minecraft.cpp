//
// Created by Иван Ильин on 03.10.2021.
//

#include "Minecraft.h"
#include "engine/utils/ResourceManager.h"

using namespace std;

void Minecraft::start() {
    // This code executed once in the beginning:

    screen->setMouseCursorVisible(false);
    setGlEnable(true);

    // loading map
    // TODO: implemented (lesson 1)
    map->loadMap(MinecraftConsts::MAP_NAME);//загрузка карты

    // setting callbacks for playerController
    playerController->setAddCubeCallBack([this](){ addCube(); });
    playerController->setRemoveCubeCallBack([this](){ return removeCube(); });
    playerController->setUpdateCubeInHandCallBack([this](){ updateCubeInHandColor(); });
    world->addBody(player);

    // orientating and attaching camera
    // TODO: implemented (lesson 2)
    //камера на том же месте что и плеер, только выше
    //camera->translateToPoint(player->position() + Vec3D(0, 2.9, 0));
    camera->translateToPoint(player->position() + Vec3D(0, 4, -4));
    player->attach(camera); //камера теперь компонент плеера, и при перемещении плеера, камера будет перемещаться такими же смещениями и поворотами
    player->translate(Vec3D(0, 6, 0));

    // adding cube in hand
    // TODO: implemented (lesson 2)
    //куб который мы держим в руках
    auto cube_in_hand = world->addBody(std::make_shared<RigidBody>(Mesh::Cube(ObjectNameTag("cube_in_hand"))));
    cube_in_hand->setCollider(false);//уберем коллизии, чтобы у нас не выпрыгивал от нас
    cube_in_hand->translateToPoint(player->position() + Vec3D{-3, -1, 1.4});//немного сместим
    cube_in_hand->rotate(Vec3D(0, M_PI/10, 0));//немного повернем
    camera->attach(cube_in_hand);//прикрепим к камере, чтобы он вращался вместе с камерой.
    updateCubeInHandColor();

    world->addBody(player);//в мир добавляем игрока
}

void Minecraft::update() {
    // This code executed every time step:

    // exit the game
    if(keyboard->isKeyTapped(sf::Keyboard::Escape)) {
        exit();
    }

    // switch  OpenGL on/off
    if (keyboard->isKeyTapped(sf::Keyboard::O)) {
        setGlEnable(!glEnable());
    }

    // display debug info
    if (keyboard->isKeyTapped(sf::Keyboard::Tab)) {
        setDebugInfo(!showDebugInfo());
    }

    // save map
    if (keyboard->isKeyTapped(sf::Keyboard::R)) {
        map->saveMap(MinecraftConsts::MAP_NAME);
    }

    playerController->update();
}

void Minecraft::updateCubeInHandColor() {
    // TODO: implemented (lesson 2)
    world->body(ObjectNameTag("cube_in_hand"))->setColor(Cube::cubeColor(player->selectedBlock()));
}

void Minecraft::gui() {
    // TODO: implement (lesson 2)
}

void Minecraft::addCube() {
    // TODO: implement (lesson 3)
}

void Minecraft::removeCube() {
    // TODO: implement (lesson 3)
}
