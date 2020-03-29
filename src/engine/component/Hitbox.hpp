/*
** EPITECH PROJECT, 2020
** OOP_arcade_2019
** File description:
** Hitbox.hpp
*/

#ifndef OOP_ARCADE_2019_COMPONENT_HITBOX_HPP
#define OOP_ARCADE_2019_COMPONENT_HITBOX_HPP

#include "../ecs/AComponent.hpp"
#include "../ecs/Entity.hpp"

namespace engine {

namespace component {

class Hitbox : public ecs::AComponent {
  public:
    int width;
    int height;

  public:
    Hitbox(engine::ecs::Entity& entity, int width, int height);
    ~Hitbox() override = 0;
};

} // namespace component

} // namespace engine

#endif // OOP_ARCADE_2019_COMPONENT_HITBOX_HPP
