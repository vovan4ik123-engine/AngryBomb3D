#include "BaseEnemy.h"
#include "EnumsAndVariables.h"

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

        if(m_isDead)
            return;

        if(glm::distance(m_startPosition, m_obj->getOrigin()) > 10.0f ||
           Beryll::Physics::getIsCollisionWithGroup(m_obj->getID(), EnumsAndVars::CollGr_PLAYER_BULLET))
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
        Beryll::ParticleSystem::EmitCubesFromCenter(4, 0.8f, 0.8f, 0.8f, // Black.
                                                    glm::vec4{0.078f, 0.078f, 0.078f, 1.0f}, glm::vec4{0.078f, 0.078f, 0.078f, 1.0f},
                                                    m_obj->getOrigin(),
                                                    glm::vec3{Beryll::RandomGenerator::getFloat() * 8.0f - 4.0f,
                                                              Beryll::RandomGenerator::getFloat() * 8.0f - 4.0f,
                                                              Beryll::RandomGenerator::getFloat() * 8.0f - 4.0f},
                                                    8.0f);
        Beryll::ParticleSystem::EmitCubesFromCenter(3, 0.8f, 0.6f, 0.6f, // Red.
                                                    glm::vec4{0.8476f, 0.0f, 0.0f, 1.0f}, glm::vec4{0.8476f, 0.0f, 0.0f, 1.0f},
                                                    m_obj->getOrigin(),
                                                    glm::vec3{Beryll::RandomGenerator::getFloat() * 8.0f - 4.0f,
                                                              Beryll::RandomGenerator::getFloat() * 8.0f - 4.0f,
                                                              Beryll::RandomGenerator::getFloat() * 8.0f - 4.0f},
                                                    8.0f);
        Beryll::ParticleSystem::EmitCubesFromCenter(7, 0.8f, 1.0f, 1.0f, // Dark green.
                                                    glm::vec4{0.1445f, 0.3789f, 0.0f, 1.0f}, glm::vec4{0.1445f, 0.3789f, 0.0f, 1.0f},
                                                    m_obj->getOrigin(),
                                                    glm::vec3{Beryll::RandomGenerator::getFloat() * 8.0f - 4.0f,
                                                              Beryll::RandomGenerator::getFloat() * 8.0f - 4.0f,
                                                              Beryll::RandomGenerator::getFloat() * 8.0f - 4.0f},
                                                    10.0f);

        float numberHeight = 2.5f + (glm::distance(Beryll::Camera::getCameraPos(), m_obj->getOrigin()) * 0.02f);
        Beryll::TextOnScene::addNumbersToShow(500, numberHeight, 1.5f,
                                              m_obj->getOrigin() + glm::vec3{0.0f, 2.5f, 0.0f},
                                              glm::vec3{0.0f, 1.0f, 0.0f},
                                              Beryll::RandomGenerator::getFloat() * 2.5f + 2.5f);

        m_obj->disableForEver();
        --m_activeCount;
        m_isDead = true;
    }
}
