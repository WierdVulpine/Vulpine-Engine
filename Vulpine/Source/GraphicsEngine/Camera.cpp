#include "Camera.h"
#include <cassert>
#include <cmath>

#define PI 3.14f

bool Camera::Init(float aHorizontalFov, CommonUtilities::Vector2<unsigned int> aResulution, float aNearPlane, float aFarPlane)
{
    assert(aNearPlane < aFarPlane);

    myNearPlane = aNearPlane;
    myFarPlane = aFarPlane;

    const float hFoVRad = aHorizontalFov * (PI / 180);

    const float vFoVRad = 2 * std::atan(hFoVRad / 2) * (static_cast<float>(aResulution.y) / static_cast<float>(aResulution.x));

    const float myXScale = 1 / std::tanf(hFoVRad / 2.0f);
    const float myYScale = 1 / std::tanf(vFoVRad * 0.5f);
    const float Q = myFarPlane / (myFarPlane - myNearPlane);

    myProjection(1, 1) = myXScale;
    myProjection(2, 2) = myYScale;
    myProjection(3, 3) = Q;
    myProjection(3, 4) = 1.0f - Q;
    myProjection(4, 3) = -Q * myNearPlane;
    myProjection(4, 4) = 0.0f;

    return true;
}
