#include "BulletSet.h"
#include "EnumsAndVariables.h"

namespace AngryBomb3D
{
    BulletSet::BulletSet(BulletType type,
                         std::vector<std::shared_ptr<Beryll::SimpleCollidingObject>> bullets,
                         float disableTimeAfterCollision)
    {
        BR_ASSERT((bullets.size() > 0), "%s", "bullets can not be empty.");

        m_bulletType = type;
        m_bullets = std::move(bullets);
        m_bulletsCollisionInfo.resize(m_bullets.size());
        m_disableTimeAfterCollision = disableTimeAfterCollision;

        for(const auto& bullet : m_bullets)
        {
            bullet->disableDraw();
            bullet->disableUpdate();
            bullet->disableCollisionMesh();
        }

        const float screenAR = Beryll::Window::getInstance()->getScreenAspectRatio();
        if(type == BulletType::STANDARD_1_KG)
        {
            buttonBullet = std::make_shared<Beryll::ButtonWithTexture>("GUI/playState/bullets/Standard_1kg.png", "",
                                                                       glm::vec3{0.0f, 0.0f, 0.5f}, glm::vec2{11.0f / screenAR, 11.0f});
        }
        else
        {
            BR_ASSERT(false, "%s", "Unknown bullet type.");
        }

        textBulletsCount = Beryll::Renderer::createGUIText("", glm::vec3{0.06f, 0.06f, 0.06f}, glm::vec3{0.0f, 0.0f, 0.5f}, 5.0f);
    }

    BulletSet::~BulletSet()
    {

    }

    bool BulletSet::shoot(const glm::vec3& startPos, const glm::vec3& impulse)
    {
        BR_ASSERT((m_currentBulletIndexToShoot < m_bullets.size()), "%s m_bullets.size() %d", "m_currentBulletIndexToShoot must be < m_bullets.size()", m_bullets.size());

        if(m_currentBulletIndexToShoot >= m_bullets.size())
            return false;

        BR_INFO("%s", "Shoot.");
        m_bullets[m_currentBulletIndexToShoot]->enableDraw();
        m_bullets[m_currentBulletIndexToShoot]->enableUpdate();
        m_bullets[m_currentBulletIndexToShoot]->enableCollisionMesh();
        m_bullets[m_currentBulletIndexToShoot]->setGravity(EnumsAndVars::bulletGravity, false, false);
        m_bullets[m_currentBulletIndexToShoot]->setOrigin(startPos, true);
        m_bullets[m_currentBulletIndexToShoot]->applyCentralImpulse(impulse);

        ++m_currentBulletIndexToShoot;
        m_anyBulletIsActive = true;

        return true;
    }

    void BulletSet::update()
    {
        m_anyBulletHasCollision = false;
        m_anyBulletIsActive = false;
        textBulletsCount->text = std::to_string(m_bullets.size() - m_currentBulletIndexToShoot);

        for(int i = 0; i < m_bullets.size(); ++i)
        {
            if(!m_bullets[i]->getIsEnabledUpdate())
                continue;

            m_anyBulletIsActive = true;

            if(!m_bulletsCollisionInfo[i].firstCollisionHappened &&
               Beryll::Physics::getAnyCollisionForID(m_bullets[i]->getID()) != 0)
            {
                m_bulletsCollisionInfo[i].firstCollisionHappened = true;
                m_bulletsCollisionInfo[i].collisionTime = EnumsAndVars::mapPlayTimeSec;
                m_anyBulletHasCollision = true;
            }

            if(m_bulletsCollisionInfo[i].firstCollisionHappened &&
               m_bulletsCollisionInfo[i].collisionTime + m_disableTimeAfterCollision < EnumsAndVars::mapPlayTimeSec)
            {
                m_bullets[i]->disableForEver();
            }
        }
    }
}
