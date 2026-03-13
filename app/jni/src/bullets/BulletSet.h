#pragma once

#include "EngineHeaders.h"

namespace AngryBomb3D
{
    enum class BulletType
    {
        NONE = 0,
        STANDARD_1_KG = 1
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
            BR_ASSERT((m_bullets.size() - m_currentBulletIndex >= 0), "%s", " Available bullet count < 0.");
            return m_bullets.size() - m_currentBulletIndex;
        }
        bool getIsAnyBulletActive() const { return m_isAnyBulletActive; }
        bool getIsAnyBulletHasCollision() const { return m_anyBulletHasCollision; }
        std::shared_ptr<Beryll::SimpleCollidingObject> getCurrentBullet() { return m_bullets[m_currentBulletIndex]; }

    private:
        BulletType m_bulletType = BulletType::NONE;
        std::vector<std::shared_ptr<Beryll::SimpleCollidingObject>> m_bullets;
        int m_currentBulletIndex = 0;
        bool m_isAnyBulletActive = false;
        float m_disableTimeAfterCollision = 0.0f;
        float m_collisionTime = 0.0f;
        bool m_firstCollisionHappened = false;
        bool m_anyBulletHasCollision = false;
    };
}
