#include "CollisionManager.h"
#include "Cube.h"
#include <cmath>

void CollisionManager::Register(Cube* cube) {
    for (Cube* existing : cubes_) {
        pairs_.push_back({ existing, cube, false });
    }
    cubes_.push_back(cube);
}

void CollisionManager::Update() {
    for (auto& pair : pairs_) {
        bool isColliding = CheckAABB(*pair.a, *pair.b);

        if (isColliding && !pair.wasColliding) {
            if (pair.a->onCollisionEnter) pair.a->onCollisionEnter(pair.b->gameObject);
            if (pair.b->onCollisionEnter) pair.b->onCollisionEnter(pair.a->gameObject);
        } else if (isColliding && pair.wasColliding) {
            if (pair.a->onCollisionStay) pair.a->onCollisionStay(pair.b->gameObject);
            if (pair.b->onCollisionStay) pair.b->onCollisionStay(pair.a->gameObject);
        } else if (!isColliding && pair.wasColliding) {
            if (pair.a->onCollisionExit) pair.a->onCollisionExit(pair.b->gameObject);
            if (pair.b->onCollisionExit) pair.b->onCollisionExit(pair.a->gameObject);
        }

        pair.wasColliding = isColliding;
    }
}

bool CollisionManager::CheckAABB(const Cube& a, const Cube& b) {
    float halfX = a.transform.localScale.x * 0.5f;
    float halfY = a.transform.localScale.y * 0.5f;
    float halfZ = a.transform.localScale.z * 0.5f;
    float otherHalfX = b.transform.localScale.x * 0.5f;
    float otherHalfY = b.transform.localScale.y * 0.5f;
    float otherHalfZ = b.transform.localScale.z * 0.5f;

    return (fabsf(a.transform.position.x - b.transform.position.x) <= halfX + otherHalfX) &&
           (fabsf(a.transform.position.y - b.transform.position.y) <= halfY + otherHalfY) &&
           (fabsf(a.transform.position.z - b.transform.position.z) <= halfZ + otherHalfZ);
}
