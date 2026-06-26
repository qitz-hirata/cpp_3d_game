#include "EnemyManager.h"
#include "BulletManager.h"
#include "DxLibCommon.h"
#include <algorithm>
#include <unordered_map>
#include <cstring>
#include <cstdlib>

#pragma warning(push)
#pragma warning(disable: 4996)
#include "csv.h"
#pragma warning(pop)

static unsigned int ColorNameToRGB(DxLibCommon* dxLib, const char* name) {
    if (strcmp(name, "RED")    == 0) return dxLib->MakeColor(220, 60,  60);
    if (strcmp(name, "BLUE")   == 0) return dxLib->MakeColor(60,  100, 220);
    if (strcmp(name, "YELLOW") == 0) return dxLib->MakeColor(220, 200, 60);
    return dxLib->MakeColor(200, 200, 200);
}

void EnemyManager::Add(DxLibCommon* dxLib, Vector3 position, int hp, unsigned int color, float speed) {
    Enemy enemy;
    enemy.Initialize(dxLib, position, hp, color, speed);
    enemies_.push_back(std::move(enemy));
}

void EnemyManager::LoadFromCSV(DxLibCommon* dxLib) {
    // --- マスターデータ読込 (ID,COLOR,HP,SPEED) ---
    std::unordered_map<int, EnemyMasterData> masterMap;
    {
        io::LineReader reader("Resources/enemy_master.csv");
        reader.next_line(); // ヘッダ行をスキップ
        while (char* line = reader.next_line()) {
            char idBuf[8]    = {};
            char colorBuf[16] = {};
            char hpBuf[8]    = {};
            char speedBuf[8] = {};
            char* p = line;
            auto readCell = [&](char* buf, int maxLen) {
                int i = 0;
                while (*p && *p != ',' && i < maxLen - 1) buf[i++] = *p++;
                if (*p == ',') ++p;
            };
            readCell(idBuf,    sizeof(idBuf));
            readCell(colorBuf, sizeof(colorBuf));
            readCell(hpBuf,    sizeof(hpBuf));
            readCell(speedBuf, sizeof(speedBuf));

            EnemyMasterData data;
            data.id    = atoi(idBuf);
            data.color = ColorNameToRGB(dxLib, colorBuf);
            data.hp    = atoi(hpBuf);
            data.speed = static_cast<float>(atof(speedBuf));
            if (data.id > 0) masterMap[data.id] = data;
        }
    }

    // --- 配置データ読込 (行=Z方向, 列=X方向, 値=敵ID) ---
    static const float kCellWidth = 5.0f;
    static const float kCellDepth = 5.0f;
    static const int   kColCenter = 4;
    static const int   kTotalRows = 27;

    {
        io::LineReader reader("Resources/enemy_placement.csv");
        int row = 0;
        while (char* line = reader.next_line()) {
            int  col     = 0;
            char cell[8] = {};
            int  ci      = 0;
            for (char* p = line; ; ++p) {
                if (*p == ',' || *p == '\0') {
                    cell[ci] = '\0';
                    if (ci > 0) {
                        int id = atoi(cell);
                        auto it = masterMap.find(id);
                        if (id > 0 && it != masterMap.end()) {
                            const EnemyMasterData& m = it->second;
                            float x = static_cast<float>(col - kColCenter) * kCellWidth;
                            float z = -static_cast<float>(kTotalRows - row) * kCellDepth;
                            Add(dxLib, { x, 0.0f, z }, m.hp, m.color, m.speed);
                        }
                    }
                    ci = 0;
                    ++col;
                    if (*p == '\0') break;
                } else {
                    if (ci < 7) cell[ci++] = *p;
                }
            }
            ++row;
        }
    }
}

void EnemyManager::Update(float deltaTime, BulletManager& bulletManager) {
    for (auto& enemy : enemies_) {
        if (!enemy.IsAlive()) continue;

        enemy.Update(deltaTime);

        if (bulletManager.CheckAndKillAllyHit(enemy.cube)) {
            enemy.TakeDamage(1);
        }
    }

    enemies_.erase(
        std::remove_if(enemies_.begin(), enemies_.end(),
            [](const Enemy& e) { return !e.IsAlive(); }),
        enemies_.end()
    );
}

void EnemyManager::Draw(DxLibCommon* dxLib) const {
    for (const auto& enemy : enemies_) {
        enemy.Draw(dxLib);
    }
}
