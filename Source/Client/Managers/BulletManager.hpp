#include <mutex>
#include <vector>
#include "Math\Vector2.hpp"
#include "Math\Vector3.hpp"

class GameScene;
class AppContext;

class BulletManager
{
public:
    BulletManager(GameScene& gameScene, const AppContext& appContext);
    ~BulletManager();

    void Update(float time);
    void Fire(const Vector2& pos, const Vector2& dir, float speed, float time, float lifeTime);

    size_t GetBulletsOnSceneCount() const;
    size_t GetBulletsInQueueCount() const;

private:
    struct Bullet
    {
        Vector2 pos;
        Vector2 dir;
        float fireTime;
        float speed;
        float lifeTime;

        float previousUpdateTime;
    };

    void DrawBullets(float time);
    void UpdateBulletPositions(float time);
    void UpdateBulletPosition(Bullet& bullet, float deltaTime);
    void ClipOutOfBordersBullets();
    void RemoveDeadBullets(float time);

    mutable std::mutex mutex;

    std::vector<Bullet> bulletsPool;
    uint32_t bulletsInQueueCount = 0;

    GameScene& gameScene;

    const AppContext& appContext;
};