#include <mutex>
struct Vector2;

class BulletManager
{
public:
    void Update(float time);
    void Fire(const Vector2& pos, const Vector2& dir, float speed, float time, float lifeTime);

private:
    std::mutex mutex;
};