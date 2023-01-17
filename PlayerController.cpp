//
// Created by Иван Ильин on 19.09.2021.
//

#include "PlayerController.h"
#include "engine/utils/Log.h"
#include "engine/animation/Animations.h"
#include "engine/io/SoundController.h"

PlayerController::PlayerController(std::shared_ptr<Player> player,
                                   std::shared_ptr<Keyboard> keyboard,
                                   std::shared_ptr<Mouse> mouse) : _player(player), _keyboard(keyboard), _mouse(mouse) {
}

void PlayerController::update() {
    auto camera = _player->attached(ObjectNameTag("Camera"));

    // Left and right movement
    // TODO: implemented (lesson 2)
    if(Keyboard::isKeyPressed(sf::Keyboard::A)){
        _player->translate(_player->left()*Time::deltaTime()*MinecraftConsts::WALK_SPEED*MinecraftConsts::WORLD_SCALE);
    }
    if(Keyboard::isKeyPressed(sf::Keyboard::D)){
        _player->translate(-_player->left()*Time::deltaTime()*MinecraftConsts::WALK_SPEED*MinecraftConsts::WORLD_SCALE);
    }

    // Forward and backward movement
    // TODO: implemented (lesson 2)
    if(Keyboard::isKeyPressed(sf::Keyboard::W)){
        _player->translate(_player->lookAt()*Time::deltaTime()*MinecraftConsts::WALK_SPEED*MinecraftConsts::WORLD_SCALE);
    }
    if(Keyboard::isKeyPressed(sf::Keyboard::S)){
        _player->translate(-_player->lookAt()*Time::deltaTime()*MinecraftConsts::WALK_SPEED*MinecraftConsts::WORLD_SCALE);
    }

    // Jump
    // TODO: implemented (lesson 2)
    if(Keyboard::isKeyPressed(sf::Keyboard::Space) && _player->inCollision()){
            _player->setVelocity(Vec3D(0, sqrt(2 * MinecraftConsts::GRAVITY * MinecraftConsts::JUMP_HEIGHT), 0)*MinecraftConsts::WORLD_SCALE);
        }

    // Horizontal player rotation
    // TODO: implemented (lesson 2)
    //влево-вправо поворачиваем человечка (камера поворачивается вместе с человечком, т.к. она прикреплена к нему)
    Vec2D disp = _mouse->getMouseDisplacement();
    _player->rotate(Vec3D(0, -disp.x() * MinecraftConsts::MOUSE_SENSITIVITY, 0));

    // Vertical player rotation
    // TODO: implemented (lesson 2)
    //вверх-вниз - человечка не трогаем, поворачиваем только камеру.
    double rotationLeft = disp.y() * MinecraftConsts::MOUSE_SENSITIVITY;

    //ограничим возможность поворачивать голову вверх и вних на величину 90 градусов.
    double headAngle = camera->angleLeftUpLookAt().x();
    if(headAngle + rotationLeft > M_PI/2){ //если поворачиваем вниз, то не больше чем на 90 градусов.
        rotationLeft = M_PI/2 - headAngle;
    }
    if(headAngle + rotationLeft < -M_PI/2){ //если поворачиваем вверх, то не больше чем на 90 градусов.
        rotationLeft = -M_PI/2 - headAngle;
    }

    camera->rotateLeft(rotationLeft);

    // Change selected block
    // TODO: implemented (lesson 2)
    if(_keyboard->isKeyTapped(sf::Keyboard::Right)){
            _player->nextBlock();
            _updateCubeInHandCallBack();
        }
    if(_keyboard->isKeyTapped(sf::Keyboard::Left)){
            _player->previousBlock();
            _updateCubeInHandCallBack();
        }
    // Add or remove block
    // TODO: implemented (lesson 3)
    if(_mouse->isButtonTapped(sf::Mouse::Right)){
        _addCubeCallBack();
    }
    if(_mouse->isButtonTapped(sf::Mouse::Left)){
        _removeCubeCallBack();
    }

    animateCameraMotion();
}

void PlayerController::animateCameraMotion() {
    bool inRunning_old = _inRunning;//было ли движение на предыдущем шаге
    _inRunning = Keyboard::isKeyPressed(sf::Keyboard::A) || Keyboard::isKeyPressed(sf::Keyboard::D) || Keyboard::isKeyPressed(sf::Keyboard::W) || Keyboard::isKeyPressed(sf::Keyboard::S);
    auto camera = _player->attached(ObjectNameTag("Camera"));

    // Camera movement during run
    // TODO: implemented (lesson 3)
    if(_inRunning){
        //анимация влево на 0.3 сек, график скорости по косинусу
        Timeline::addAnimation<ATranslate>(AnimationListTag("h"), camera, -camera->left()*MinecraftConsts::WORLD_SCALE/3, 0.3, Animation::LoopOut::None, Animation::InterpolationType::Cos);
        Timeline::addAnimation<AWait>(AnimationListTag("h"), 0);
        //анимация вправо на 0.3 сек, график скорости по косинусу (возвращаем в начальное положение)
        Timeline::addAnimation<ATranslate>(AnimationListTag("h"), camera, camera->left()*MinecraftConsts::WORLD_SCALE/3, 0.3, Animation::LoopOut::None, Animation::InterpolationType::Cos);
        Timeline::addAnimation<AWait>(AnimationListTag("h"), 0);
    } else if(! _inRunning && inRunning_old){//если перестали двигаться то
        Timeline::deleteAnimationList(AnimationListTag("h")); //удаляем анимацию кочания головой
        //анимация на 0.15 сек возвращаем голову на место (туда куда была прикреплена камера когда мы её создавали)
        Timeline::addAnimation<ATranslateToPoint>(camera, (_player->position() + Vec3D(0, 0.8, 0) * MinecraftConsts::WORLD_SCALE), 0.15);
    }
}

// setting callbacks
void PlayerController::setAddCubeCallBack(std::function<void()> addCube) {
    _addCubeCallBack = std::move(addCube);
}
void PlayerController::setRemoveCubeCallBack(std::function<void()> removeCube) {
    _removeCubeCallBack = std::move(removeCube);
}
void PlayerController::setUpdateCubeInHandCallBack(std::function<void()> updateCube) {
    _updateCubeInHandCallBack = std::move(updateCube);
}
