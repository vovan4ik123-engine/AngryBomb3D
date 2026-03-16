#include "BaseEnemy.h"

namespace AngryBomb3D
{
    int BaseEnemy::m_activeCount = 0;
    int BaseEnemy::m_allCount = 0;

    BaseEnemy::BaseEnemy(std::shared_ptr<Beryll::SimpleCollidingObject> obj)
    {
        m_obj = std::move(obj);
        m_startPosition = m_obj->getOrigin();

        ++m_activeCount;
        ++m_allCount;
    }

    BaseEnemy::~BaseEnemy()
    {

    }

    void BaseEnemy::update()
    {
        if(Beryll::Physics::getAnyCollisionForID(m_obj->getID()) == 0)
            return;

        if(glm::distance(m_startPosition, m_obj->getOrigin()) > 10.0f ||
           Beryll::Physics::getIsCollisionWithGroup(m_obj->getID(), Beryll::CollisionGroups::PLAYER_BULLET))
        {
            die();
        }
        else
        {
            const auto points = Beryll::Physics::getAllCollisionPoints(m_obj->getID(), Beryll::Physics::getAllCollisionsForID(m_obj->getID()));

            const float enemyOriginY = m_obj->getOrigin().y;
            for(const auto& point : points)
            {
                if(point.first.y > enemyOriginY)
                {
                    die();
                    break;
                }
            }
        }
    }

    void BaseEnemy::die()
    {
        m_obj->disableForEver();
        --m_activeCount;
    }
}
