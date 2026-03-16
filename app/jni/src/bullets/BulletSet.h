#pragma once

#include "EngineHeaders.h"

namespace AngryBomb3D
{
    enum class BulletType
    {
        NONE = 0,
        STANDARD_1_KG = 1
    };

    struct BulletCollisionInfo
    {
        bool firstCollisionHappened = false;
        float collisionTime = 0.0f;
    };

    class BulletSet
    {
    public:
        BulletSet(BulletType type,
                  std::vector<std::shared_ptr<Beryll::SimpleCollidingObject>> bullets,
                  float disableTimeAfterCollision);
        ~BulletSet();

        bool shoot(const glm::vec3& startPos, const glm::vec3& impulse);
        void update();

        BulletType getType() const { return m_bulletType; }
        int getAvailableCount() const
        {
            BR_ASSERT((m_bullets.size() - m_currentBulletIndexToShoot >= 0), "%s", " Available bullet count < 0.");
            return m_bullets.size() - m_currentBulletIndexToShoot;
        }
        bool getIsAnyBulletHasCollision() const { return m_anyBulletHasCollision; }

    private:
        BulletType m_bulletType = BulletType::NONE;
        std::vector<std::shared_ptr<Beryll::SimpleCollidingObject>> m_bullets;
        std::vector<BulletCollisionInfo> m_bulletsCollisionInfo;
        int m_currentBulletIndexToShoot = 0;
        float m_disableTimeAfterCollision = 0.0f;
        bool m_anyBulletHasCollision = false;
    };
}
