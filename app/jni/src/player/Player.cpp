#include "Player.h"
#include "EnumsAndVariables.h"

namespace AngryBomb3D
{
    Player::Player(std::shared_ptr<Beryll::SimpleCollidingObject> so, float health)
    : m_obj(std::move(so)), m_maxHP(health), m_currentHP(health)
    {
        BR_INFO("%s", "Player::Player(std::shared_ptr<Beryll::SimpleCollidingObject> so, float health)");

        m_obj->enableCollisionMesh();
        m_obj->enableUpdate();
        m_obj->enableDraw();
    }

    Player::~Player()
    {

    }

    void Player::update()
    {
        if(m_state == PlayerState::MOVE)
        {
            const glm::vec3 move = glm::normalize(m_nextPoint - m_currentPoint) * 300.0f * Beryll::TimeStep::getTimeStepSec();
            if(glm::distance(m_obj->getOrigin(), m_nextPoint) < glm::length(move))
            {
                m_obj->setOrigin(m_nextPoint);
                m_state = PlayerState::NONE;
            }
            else
            {
                m_obj->setOrigin(m_obj->getOrigin() + move);
            }
        }
    }

    void Player::setPlayerPoints(std::vector<glm::vec3> points)
    {
        BR_ASSERT((points.empty() == false), "%s", "points.empty().");

        if(points.size() > 1)
            EnumsAndVars::showPlayerMoveArrows = true;

        m_availablePositions.clear();
        m_availablePositions.reserve(points.size());

        float biggestX = -999999.0f;
        int biggestXIndex = 0;
        for(int i = 0; i < points.size(); ++i)
        {
            if(points[i].x > biggestX)
            {
                biggestX = points[i].x;
                biggestXIndex = i;
            }

            if(glm::length(points[i]) > m_farthestPointDistance)
                m_farthestPointDistance = glm::length(points[i]);
        }

        m_availablePositions.push_back(points[biggestXIndex]);

        glm::vec3 currentDirToSearchLeftPoint = m_availablePositions.back();
        currentDirToSearchLeftPoint.y = 0.0f;
        currentDirToSearchLeftPoint = glm::normalize(currentDirToSearchLeftPoint);

        while(m_availablePositions.size() < points.size())
        {
            currentDirToSearchLeftPoint = glm::rotate(currentDirToSearchLeftPoint, glm::radians(5.0f), BeryllConstants::worldUp);
            currentDirToSearchLeftPoint = glm::normalize(currentDirToSearchLeftPoint);

            float smallestAngle = 999999.0f;
            int smallestAngleIndex = 0;
            for(int i = 0; i < points.size(); ++i)
            {
                if(BeryllUtils::Common::getVectorSide(currentDirToSearchLeftPoint, glm::normalize(points[i])) == BeryllUtils::Common::VectorSide::ON_LEFT_SIDE)
                {
                    const float angle = BeryllUtils::Common::getAngleInRadians(currentDirToSearchLeftPoint, glm::normalize(points[i]));
                    if(angle < smallestAngle)
                    {
                        smallestAngle = angle;
                        smallestAngleIndex = i;
                    }
                }
            }

            if(std::find(m_availablePositions.begin(), m_availablePositions.end(), points[smallestAngleIndex]) == m_availablePositions.end())
                m_availablePositions.push_back(points[smallestAngleIndex]);
        }

        BR_ASSERT((points.size() == m_availablePositions.size()), "%s", "points.size() must == m_availablePositions.size()");

        for(glm::vec3& point : m_availablePositions)
            point.y += m_obj->getFromOriginToBottom();

        m_obj->setOrigin(m_availablePositions[m_currentPositionIndex]);
    }

    void Player::moveLeft()
    {
        if(m_state == PlayerState::MOVE)
            return;

        m_state = PlayerState::MOVE;

        m_currentPoint = m_availablePositions[m_currentPositionIndex];

        --m_currentPositionIndex;
        if(m_currentPositionIndex <  0)
            m_currentPositionIndex = m_availablePositions.size() - 1;

        m_nextPoint = m_availablePositions[m_currentPositionIndex];
    }

    void Player::moveRight()
    {
        if(m_state == PlayerState::MOVE)
            return;

        m_state = PlayerState::MOVE;

        m_currentPoint = m_availablePositions[m_currentPositionIndex];

        ++m_currentPositionIndex;
        if(m_currentPositionIndex >=  m_availablePositions.size())
            m_currentPositionIndex = 0;

        m_nextPoint = m_availablePositions[m_currentPositionIndex];
    }
}
