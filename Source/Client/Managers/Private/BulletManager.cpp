#include "Managers/BulletManager.hpp"

void BulletManager::Update([[ maybe_unused ]] float time)
{

}

void BulletManager::Fire(
    [[ maybe_unused ]] const Vector2& pos,
    [[ maybe_unused ]] const Vector2& dir, 
    [[ maybe_unused ]] float speed, 
    [[ maybe_unused ]] float time, 
    [[ maybe_unused ]] float lifeTime)
{
    std::lock_guard lock(mutex);
}
