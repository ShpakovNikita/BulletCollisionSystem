#include "Managers/BulletManager.hpp"
#include "GameScene.hpp"
#include "Core/Renderer.hpp"
#include <assert.h>
#include "Utils/Intersection.hpp"
#include "Math/MathHelpers.hpp"
#include "Math/AABBox2.hpp"
#include "Core/AppContext.hpp"

BulletManager::BulletManager(GameScene& aGameScene, const AppContext& aAppContext)
    : gameScene(aGameScene)
    , appContext(aAppContext)
{

}

BulletManager::~BulletManager()
{
    std::lock_guard lock(mutex);
    firedBullets.clear();
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
    assert(dir != Vector2::kZero && speed != 0.0f && lifeTime != 0.0f);

    std::lock_guard lock(mutex);

    Bullet bullet = {};
    bullet.dir = dir.Normalized();
    bullet.pos = pos;
    bullet.speed = speed;
    bullet.remainingLifeTime = lifeTime;

    firedBullets.push_back(std::move(bullet));
}

void BulletManager::DrawBullets()
{
    for (const Bullet& bullet : firedBullets)
    {
        appContext.renderer->DrawPoint(bullet.pos, 0.02f);
    }
}

void BulletManager::UpdateBulletPositions(float deltaTime)
{
    for (Bullet& bullet : firedBullets)
    {
        Vector2 deltaVelocityVector = bullet.dir * bullet.speed * deltaTime;
        Vector2 newBulletPos = bullet.pos + deltaVelocityVector;

        std::vector<const Line*> bboxCollidedWalls = gameScene.quadtree.GetCollidedObjects({ bullet.pos, newBulletPos });

        for (const Line* wallPtr : bboxCollidedWalls)
        {
            const Line& wall = *wallPtr;

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
