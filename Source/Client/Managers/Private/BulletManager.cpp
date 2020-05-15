#include "Managers/BulletManager.hpp"
#include "GameScene.hpp"
#include "Core/Renderer.hpp"
#include <assert.h>
#include "Utils/Intersection.hpp"
#include "Math/MathHelpers.hpp"
#include "Math/AABBox2.hpp"

BulletManager::BulletManager(GameScene& aGameScene)
    : gameScene(aGameScene)
{

}

void BulletManager::Update([[ maybe_unused ]] float time)
{
    UpdateBulletPositions(time);
    ClipOutOfBordersBullets();
    DrawBullets();
}

void BulletManager::Fire(
    [[ maybe_unused ]] const Vector2& pos,
    [[ maybe_unused ]] const Vector2& dir, 
    [[ maybe_unused ]] float speed, 
    [[ maybe_unused ]] float time, 
    [[ maybe_unused ]] float lifeTime)
{
    std::lock_guard lock(mutex);

    Bullet bullet = {};
    bullet.dir = dir;
    bullet.pos = pos;
    bullet.speed = speed;
    bullet.remainingLifeTime = lifeTime;

    firedBullets.push_back(std::move(bullet));
}

void BulletManager::SetRenderer(const Renderer* aRenderer)
{
    renderer = aRenderer;
}

void BulletManager::DrawBullets()
{
    assert(renderer);

    for (const Bullet& bullet : firedBullets)
    {
        renderer->DrawPoint(bullet.pos, 0.02f);
    }
}

void BulletManager::UpdateBulletPositions(float deltaTime)
{
    for (Bullet& bullet : firedBullets)
    {
        Vector2 deltaVelocityVector = bullet.dir * bullet.speed * deltaTime;
        Vector2 newBulletPos = bullet.pos + deltaVelocityVector;

        // TODO: optimize
        for (const Line& wall : gameScene.walls)
        {
            const Vector2& wallStart = std::get<0>(wall);
            const Vector2& wallEnd = std::get<1>(wall);
            std::optional<Vector2> collidedPoint = Intersection::SegmentSegmentIntersection(
                wallStart, wallEnd, bullet.pos, newBulletPos);

            if (collidedPoint)
            {
                Vector2 wallVector = wallEnd - wallStart;
                Vector2 wallNormal = MathHelpers::GetPerpendicular(wallVector);
               
                // The idea is to get our normal always pointing towards our bullet hit
                MathHelpers::eOrientation orientation = MathHelpers::GetOrientation(wallStart, wallEnd, bullet.pos);
                wallNormal = orientation == MathHelpers::eOrientation::kCounterclockwise ? wallNormal : -wallNormal;

                Vector2 newDirection = MathHelpers::Reflect(-bullet.dir, wallNormal);
                bullet.dir = newDirection.Normalized();
                newBulletPos = collidedPoint.value() + newDirection * bullet.speed * deltaTime;
                break;
            }
        }

        bullet.pos = newBulletPos;
    }
}

void BulletManager::ClipOutOfBordersBullets()
{
    constexpr AABBox2 worldBordersBBox = { {-1.0f, -1.0f}, {1.0f, 1.0f} };
    auto clipBulletConditionsCheck = [&worldBordersBBox](const Bullet& bullet) {
        return !Intersection::PointBoxIntersection(bullet.pos, worldBordersBBox);
    };

    firedBullets.erase(std::remove_if(firedBullets.begin(), firedBullets.end(), clipBulletConditionsCheck), firedBullets.end());
}
