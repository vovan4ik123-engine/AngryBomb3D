#include "BulletSet.h"
#include "EnumsAndVariables.h"

namespace AngryBomb3D
{
    BulletSet::BulletSet(BulletType type,
                         std::vector<std::shared_ptr<Beryll::SimpleCollidingObject>> bullets,
                         float disableTimeAfterCollision)
    {
        m_bulletType = type;
        m_bullets = std::move(bullets);
        m_disableTimeAfterCollision = disableTimeAfterCollision;

        for(const auto& bullet : m_bullets)
        {
            bullet->disableDraw();
            bullet->disableUpdate();
            bullet->disableCollisionMesh();
        }

    }

    BulletSet::~BulletSet()
    {

    }

    bool BulletSet::shoot(const glm::vec3& startPos, const glm::vec3& impulse)
    {
        BR_ASSERT((m_currentBulletIndex < m_bullets.size()), "%s m_bullets.size() %d", "m_currentBulletIndex must be < m_bullets.size()", m_bullets.size());

        if(m_currentBulletIndex >= m_bullets.size())
            return false;

        BR_INFO("%s", "Shoot.");
        m_bullets[m_currentBulletIndex]->enableDraw();
        m_bullets[m_currentBulletIndex]->enableUpdate();
        m_bullets[m_currentBulletIndex]->enableCollisionMesh();
        m_bullets[m_currentBulletIndex]->setGravity(EnumsAndVars::bulletGravity, false, false);
        m_bullets[m_currentBulletIndex]->setOrigin(startPos, true);
        m_bullets[m_currentBulletIndex]->applyCentralImpulse(impulse);

        m_isAnyBulletActive = true;

        return true;
    }

    void BulletSet::update()
    {
        BR_ASSERT((m_currentBulletIndex < m_bullets.size()), "%s", "m_currentBulletIndex must be < m_bullets.size()");

        m_anyBulletHasCollision = false;

        if(m_currentBulletIndex >= m_bullets.size())
            return;

        if(!m_firstCollisionHappened && Beryll::Physics::getAnyCollisionForID(m_bullets[m_currentBulletIndex]->getID()) != 0)
        {
            m_firstCollisionHappened = true;
            m_anyBulletHasCollision = true;
            m_collisionTime = EnumsAndVars::mapPlayTimeSec;
        }

        if(m_firstCollisionHappened && m_collisionTime + m_disableTimeAfterCollision < EnumsAndVars::mapPlayTimeSec)
        {
            m_bullets[m_currentBulletIndex]->disableForEver();

            ++m_currentBulletIndex;
            m_isAnyBulletActive = false;
            m_firstCollisionHappened = false;
        }
    }
}
