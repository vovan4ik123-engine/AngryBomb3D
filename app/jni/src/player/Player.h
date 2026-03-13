#pragma once

#include "EngineHeaders.h"

namespace AngryBomb3D
{
    enum class PlayerState
    {
        NONE, MOVE
    };

    class Player
    {
    public:
        Player() = delete;
        Player(std::shared_ptr<Beryll::SimpleCollidingObject> so, float health);
        ~Player();

        void update();
        const std::shared_ptr<Beryll::SimpleCollidingObject>& getObj() { return m_obj; }
        void setPlayerPoints(std::vector<glm::vec3> points);
        float getFarthestPointDistance() { return m_farthestPointDistance; }
        void moveLeft();
        void moveRight();
        bool getIsMoving() { return m_state == PlayerState::MOVE; }

    private:
        std::shared_ptr<Beryll::SimpleCollidingObject> m_obj;

        // HP.
        float m_maxHP = 0.0f;
        float m_currentHP = 0.0f;

        PlayerState m_state = PlayerState::NONE;
        std::vector<glm::vec3> m_availablePositions;
        int m_currentPositionIndex = 0;
        glm::vec3 m_currentPoint{0.0f};
        glm::vec3 m_nextPoint{0.0f};
        float m_farthestPointDistance = 0.0f;
    };
}
