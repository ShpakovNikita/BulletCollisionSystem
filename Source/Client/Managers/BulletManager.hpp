#include <mutex>
#include <vector>
#include "Math\Vector2.hpp"
#include "Math\Line.hpp"

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
        float speed;
        float remainingLifeTime;
    };

    void DrawBullets();
    void UpdateBulletPositions(float time);
    void ClipOutOfBordersBullets();

    mutable std::mutex mutex;

    std::vector<Bullet> firedBullets;
    std::vector<Bullet> bulletsQueue;
    GameScene& gameScene;

    const AppContext& appContext;
};