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
    camera->translateToPoint(player->position() + Vec3D(0, 0.8, 0) * MinecraftConsts::WORLD_SCALE);
    player->attach(camera); //камера теперь компонент плеера, и при перемещении плеера, камера будет перемещаться такими же смещениями и поворотами
    player->translate(Vec3D(0, 1.5, 0)*MinecraftConsts::WORLD_SCALE);//сдвигаем плееера вверх, а то он при старте сразу куда-то проваливается.

    // adding cube in hand
    // TODO: implemented (lesson 2)
    //куб который мы держим в руках
    auto cube_in_hand = world->addBody(std::make_shared<RigidBody>(Mesh::Cube(ObjectNameTag("cube_in_hand"), MinecraftConsts::WORLD_SCALE)));//последний аргумент - размер кубика в руках
    cube_in_hand->setCollider(false);//уберем коллизии, чтобы у нас не выпрыгивал от нас
    cube_in_hand->translateToPoint(player->position() + Vec3D{-1.6, -0.5, 0.7}*MinecraftConsts::WORLD_SCALE);//немного сместим
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

void Minecraft::gui() { //Gui - это часть интерфейса который отображается поверх графики. У нас это прицел.
    // TODO: implemented (lesson 2)
    auto t = ResourceManager::loadTexture(MinecraftConsts::AIM_TEXTURE); //загружаем текустуру прицела.
    if(t != nullptr){
        sf::Sprite sprite; //создаем спрайт
        sprite.setTexture(*t);//задаем спрайту текстуру
        sprite.setPosition(screen->width()/2 - 27, screen->height()/2 - 27); //спрайт распологаем в центре экрана: центр экрана минус половина размера спрайта.
        sprite.setColor(sf::Color(0,0,0,150)); //зададим цвет - черный, и полупрозрачный.
        screen->drawSprite(sprite);//выводим на экран
    }
}

void Minecraft::addCube() {
    // TODO: implemented (lesson 3)
    auto rayCast = world->rayCast(camera->position(), camera->position() + camera->lookAt(), "Player");
    if((rayCast.pointOfIntersection - camera->position()).abs() < MinecraftConsts::REACH_RANGE * MinecraftConsts::WORLD_SCALE){
        Vec3D cubePoint = (rayCast.intersectedTriangle.position() + rayCast.intersectedTriangle.norm() * MinecraftConsts::WORLD_SCALE / 2);

        map->addCube(Vec3D(round(cubePoint.x()/MinecraftConsts::WORLD_SCALE), round(cubePoint.y()/MinecraftConsts::WORLD_SCALE), round(cubePoint.z()/MinecraftConsts::WORLD_SCALE)), player->selectedBlock());
    }
}

void Minecraft::removeCube() {
    // TODO: implemented (lesson 3)
        auto rayCast = world->rayCast(camera->position(), camera->position() + camera->lookAt(), "Player");
    if((rayCast.pointOfIntersection - camera->position()).abs() < MinecraftConsts::REACH_RANGE * MinecraftConsts::WORLD_SCALE){
        Vec3D cubePoint = (rayCast.intersectedTriangle.position() - rayCast.intersectedTriangle.norm() * MinecraftConsts::WORLD_SCALE / 2);

        map->removeCube(Vec3D(round(cubePoint.x()/MinecraftConsts::WORLD_SCALE), round(cubePoint.y()/MinecraftConsts::WORLD_SCALE), round(cubePoint.z()/MinecraftConsts::WORLD_SCALE)));
    }
}
