#include "Managers/BulletManager.hpp"
#include "GameScene.hpp"
#include "Core/Renderer.hpp"
#include <assert.h>
#include "Utils/Intersection.hpp"
#include "Math/MathHelpers.hpp"
#include "Math/AABBox2.hpp"
#include "Core/AppContext.hpp"

namespace SBulletManager
{
    constexpr Vector3 kBulletStartColor = { 1.0f, 1.0f, 0.0f };
    constexpr Vector3 kBulletEndColor = { 0.45f, 0.55f, 0.60f };
}

BulletManager::BulletManager(GameScene& aGameScene, const AppContext& aAppContext)
    : gameScene(aGameScene)
    , appContext(aAppContext)
{}

BulletManager::~BulletManager()
{
    bulletsPool.clear();
}

void BulletManager::Update(float time)
{
    std::lock_guard lock(mutex);
    RemoveDeadBullets(time);
    UpdateBulletPositions(time);
    ClipOutOfBordersBullets();
    DrawBullets(time);
}

void BulletManager::Fire(
    const Vector2& pos,
    const Vector2& dir, 
    float speed, 
    float time, 
    float lifeTime)
{
    assert(dir != Vector2::kZero && speed != 0.0f && lifeTime != 0.0f);

    std::lock_guard lock(mutex);

    Bullet bullet = {};
    bullet.dir = dir.Normalized();
    bullet.pos = pos;
    bullet.fireTime = time;
    bullet.speed = speed;
    bullet.lifeTime = lifeTime;
    bullet.previousUpdateTime = time;

    ++bulletsInQueueCount;

    bulletsPool.push_back(std::move(bullet));
}

size_t BulletManager::GetBulletsOnSceneCount() const
{
    std::lock_guard lock(mutex);
    return bulletsPool.size() - bulletsInQueueCount;
}

size_t BulletManager::GetBulletsInQueueCount() const
{
    std::lock_guard lock(mutex);
    return bulletsInQueueCount;
}

void BulletManager::DrawBullets(float time)
{
    for (const Bullet& bullet : bulletsPool)
    {
        if (time >= bullet.fireTime)
        {
            float lerpParam = (time - bullet.fireTime) / bullet.lifeTime;
            appContext.renderer->DrawPoint(bullet.pos, 0.02f,
                MathHelpers::Lerp(SBulletManager::kBulletStartColor, SBulletManager::kBulletEndColor, lerpParam));
        }
    }
}

void BulletManager::UpdateBulletPositions(float time)
{
    for (Bullet& bullet : bulletsPool)
    {
        float deltaTime = time - bullet.previousUpdateTime;

        if (deltaTime > 0.0f)
        {
            if (bullet.previousUpdateTime == bullet.fireTime)
            {
                --bulletsInQueueCount;
            }

            UpdateBulletPosition(bullet, deltaTime);
            bullet.previousUpdateTime = time;
        }
    }
}

void BulletManager::UpdateBulletPosition(Bullet& bullet, float deltaTime)
{
    struct BulletCollisionData
    {
        float distanceToCollisionPoint;
        Vector2 collisionPoint;
        Vector2 wallStart;
        Vector2 wallEnd;
    };

    float bulletRemainingTracerLength = bullet.speed * deltaTime;

    bool collided = true;
    while (collided)
    {
        collided = false;

        Vector2 bulletNewPossiblePosition = bullet.pos + bullet.dir * bulletRemainingTracerLength;
        std::vector<Line> bboxCollidedWalls = gameScene.GetBBoxCollidedWalls({ bullet.pos, bulletNewPossiblePosition });

        // We may have a case, where two walls close to each other, and we intersect both of them in the same tick.
        // To provide correctness, we have to collide with the closest wall.
        std::vector<BulletCollisionData> collisionPointsData;
        collisionPointsData.reserve(bboxCollidedWalls.size());

        for (const Line& wall : bboxCollidedWalls)
        {
            const Vector2& wallStart = std::get<0>(wall);
            const Vector2& wallEnd = std::get<1>(wall);
            std::optional<Vector2> collisionPoint = Intersection::SegmentSegmentIntersection(
                wallStart, wallEnd, bullet.pos, bulletNewPossiblePosition);

            if (collisionPoint)
            {
                BulletCollisionData collisionData;
                collisionData.collisionPoint = collisionPoint.value();
                collisionData.distanceToCollisionPoint = (collisionPoint.value() - bullet.pos).Length();
                collisionData.wallStart = wallStart;
                collisionData.wallEnd = wallEnd;
                collisionPointsData.push_back(std::move(collisionData));
            }
        }

        if (!collisionPointsData.empty())
        {
            const auto minDistancePredicate = [](const BulletCollisionData& d1, const BulletCollisionData& d2) 
            { 
                return d1.distanceToCollisionPoint < d2.distanceToCollisionPoint;
            };

            const BulletCollisionData collisionData = *std::min_element(
                collisionPointsData.begin(), collisionPointsData.end(), minDistancePredicate);

            bulletRemainingTracerLength -= collisionData.distanceToCollisionPoint;

            Vector2 wallVector = collisionData.wallEnd - collisionData.wallStart;
            Vector2 wallNormal = MathHelpers::GetPerpendicular(wallVector).Normalized();

            // The main idea behind this is to get our normal always pointing towards our bullet hit
            MathHelpers::eOrientation orientation = MathHelpers::GetOrientation(
                collisionData.wallStart, collisionData.wallEnd, bullet.pos);
            wallNormal = orientation == MathHelpers::eOrientation::kCounterclockwise ? wallNormal : -wallNormal;

            Vector2 newDirection = MathHelpers::Reflect(-bullet.dir, wallNormal);

            bullet.dir = newDirection.Normalized();
            bullet.pos = collisionData.collisionPoint;

            gameScene.RemoveWall({ collisionData.wallStart, collisionData.wallEnd });

            collided = true;
            break;
        }
    }

    bullet.pos = bullet.pos + bullet.dir * bulletRemainingTracerLength;
}

void BulletManager::ClipOutOfBordersBullets()
{
    constexpr AABBox2 worldBordersBBox = { {-1.0f, -1.0f}, {1.0f, 1.0f} };
    auto removeBulletConditionsCheck = [&worldBordersBBox](const Bullet& bullet) {
        return !Intersection::PointBoxIntersection(bullet.pos, worldBordersBBox);
    };

    bulletsPool.erase(std::remove_if(bulletsPool.begin(), bulletsPool.end(), removeBulletConditionsCheck), bulletsPool.end());
}

void BulletManager::RemoveDeadBullets(float time)
{
    auto removeBulletConditionsCheck = [time](const Bullet& bullet) {
        return bullet.fireTime + bullet.lifeTime < time;
    };

    bulletsPool.erase(std::remove_if(bulletsPool.begin(), bulletsPool.end(), removeBulletConditionsCheck), bulletsPool.end());
}
