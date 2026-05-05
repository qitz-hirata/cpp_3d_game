#pragma once
#include <vector>

class Cube;

class CollisionManager {
public:
    // Cube を1つ登録する。既存の全 Cube とのペアが自動生成される
    void Register(Cube* cube);
    void Update();

private:
    struct Pair {
        Cube* a;
        Cube* b;
        bool wasColliding = false;
    };
    std::vector<Cube*> cubes_;
    std::vector<Pair>  pairs_;

    static bool CheckAABB(const Cube& a, const Cube& b);
};
