#pragma once
#include <game_math.h>
#include "btBulletDynamicsCommon.h"

namespace nConvert
{
    inline btVector3 toBullet(const glm::vec3 &v)
    {
        return btVector3(v.x, v.y, v.z);
    }
    inline glm::vec3 toBullet(const btVector3 &v)
    {
        return glm::vec3(v.x(), v.y(), v.z());
    }

    inline void toGLM(const btTransform &transformIn, glm::mat4 &transformOut)
    {
        transformIn.getOpenGLMatrix(&transformOut[0][0]);
    }
}
