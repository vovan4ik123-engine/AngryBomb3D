#pragma once

#include "EngineHeaders.h"

namespace AngryBomb3D
{
    class BaseEnemy
    {
    public:
        BaseEnemy(std::shared_ptr<Beryll::SimpleCollidingObject> obj);
        ~BaseEnemy();

        static void resetCounts()
        {
            m_activeCount = 0;
            m_allCount = 0;
        }
        static int getActiveCount() { return m_activeCount; }
        static int getAllCount() { return m_allCount; }

        void update();
        std::shared_ptr<Beryll::SimpleCollidingObject> getObj() const { return m_obj; }

    private:
        std::shared_ptr<Beryll::SimpleCollidingObject> m_obj;
        glm::vec3 m_startPosition{0.0f};
        static int m_activeCount;
        static int m_allCount;

        void die();
    };
}
