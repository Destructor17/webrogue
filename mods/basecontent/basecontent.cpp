#include "../core/include/common/colors.h"
#include "../core/include/common/wr_api.h"
#include "../core/include/core.h"
#include <cstdint>
#include <cstdlib>
#include <string>
#include <type_traits>

#include "../core/include/sqlpp11.hpp"
#include "sqlpp11/all_of.h"
#include "sqlpp_schema.h"

namespace basecontent {

unsigned int field[4][4];

int getFreeCells() {
    int freeCells = 0;
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++) {
            if (field[x][y] <= 0)
                freeCells++;
        }
    return freeCells;
}

void addOne() {
    int selectedFreeCell = rand() % getFreeCells();
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            if (field[x][y] <= 0)
                if (!selectedFreeCell--)
                    field[x][y] = 1;
}

void initField() {
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            field[x][y] = 0;
    addOne();
    addOne();
}

unsigned int &getCell(int x, int y, bool mirror, bool transpone) {
    if (mirror)
        x = 3 - x;
    if (transpone) {
        auto tmp = x;
        x = y;
        y = tmp;
    }
    return field[x][y];
}

void moveField(bool f, bool v) {
    for (int iteration = 0; iteration < 3; iteration++)
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                auto thisCell = getCell(x, y, f, v);
                if (!thisCell)
                    continue;
                int freeCells = 0;
                for (freeCells = 0; x - freeCells - 1 >= 0 &&
                                    !getCell(x - freeCells - 1, y, f, v);
                     freeCells++)
                    ;
                int probableFreeX = x - freeCells;
                bool hasFreeSapce = probableFreeX < x;
                bool isFirst = x == 0;
                int prevX = probableFreeX - 1;
                bool isPrevSame =
                    !isFirst && getCell(prevX, y, f, v) == thisCell;
                if (isPrevSame) {
                    getCell(prevX, y, f, v)++;
                    getCell(x, y, f, v) = 0;
                } else if (hasFreeSapce && probableFreeX != x) {
                    getCell(probableFreeX, y, f, v) = thisCell;
                    getCell(x, y, f, v) = 0;
                }
            }
        }
}

namespace sql = sqlpp::sqlite3;
void basecontentInitializationStep() {
    sql::connection_config config;
    config.path_to_database = ":memory:";
    config.flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    config.debug = true;
    sql::connection db(config);
    const auto basecontentScores = webrogue::basecontent::BasecontentScores{};
    db.execute(webrogue::basecontent::BasecontentScores::ddl);

    std::srand(13);
    bool hasStoredField = true;
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++) {
            bool foundRow = false;
            for (const auto &value :
                 db(sqlpp::select(sqlpp::all_of(basecontentScores))
                        .from(basecontentScores)
                        .where(basecontentScores.x.in(x) and
                               basecontentScores.y.in(y)))) {
                field[x][y] = value.score;
                foundRow = true;
            }
            hasStoredField &= foundRow;
        }
    if (!hasStoredField)
        initField();
    nr_start_color();
    nr_set_color(8, 0, 0, 0);
    nr_set_color(9, 1000, 1000, 1000);
    nr_set_color_pair(1, 8, 9);

    while (true) {
        size_t eventCount;
        bool hasArrowAction = false;
        size_t width, height;
        nr_glyph *drawingArea = webrogue_core_get_drawing_area(&width, &height);
        size_t cellSizeX = 7;
        size_t cellSizeY = 4;
        size_t cellPadX = 2;
        size_t cellPadY = 1;
        size_t xPad = (width - 4 * cellSizeX - cellPadX) / 2;
        size_t yPad = (height - 4 * cellSizeY - cellPadY) / 2;
        webrogue_arrow_direction arrowDirection;
        webrogue_event *events = webrogue_core_get_events(&eventCount);
        for (size_t i = 0; i < eventCount; i++) {
            webrogue_event event = events[i];
            switch (event.type) {
            case webrogue_event_type::Click: {
                int dx = event.data1 - (xPad + cellSizeX * 2);
                int dy = 2 * (event.data2 - (yPad + cellSizeY * 2));
                if (abs(dx) > abs(dy)) {
                    arrowDirection = dx < 0 ? webrogue_arrow_direction::left
                                            : webrogue_arrow_direction::right;
                } else {
                    arrowDirection = dy < 0 ? webrogue_arrow_direction::up
                                            : webrogue_arrow_direction::down;
                }
                hasArrowAction = true;
            } break;
            case webrogue_event_type::Arrow:
                arrowDirection = (webrogue_arrow_direction)event.data1;
                hasArrowAction = true;
                break;
            case webrogue_event_type::Close:
                return;
            default:
                break;
            }
        }
        if (!getFreeCells())
            initField();
        if (hasArrowAction)
            switch (arrowDirection) {
            case webrogue_arrow_direction::left:
                moveField(false, false);
                break;
            case webrogue_arrow_direction::right:
                moveField(true, false);
                break;
            case webrogue_arrow_direction::up:
                moveField(false, true);
                break;
            case webrogue_arrow_direction::down:
                moveField(true, true);
                break;
            default:
                break;
            }
        if (hasArrowAction)
            addOne();
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
                drawingArea[x + width * y] = {U' ', 0};
        for (int y = 0; y < 4; y++)
            for (int x = 0; x < 4; x++) {
                size_t cellX = xPad + x * cellSizeX;
                size_t cellY = yPad + y * cellSizeY;

                for (int cy = 0; cy < cellSizeY - cellPadY; cy++)
                    for (int cx = 0; cx < cellSizeX - cellPadX; cx++) {
                        drawingArea[(cellX + cx) + width * (cellY + cy)] = {
                            U' ', 1};
                    }
                if (field[x][y]) {
                    std::string str = std::to_string(1 << field[x][y]);
                    int dx = (cellSizeX - cellPadX - str.size() + 1) / 2;
                    int dy = (cellSizeY - cellPadY - 1) / 2;
                    for (int i = 0; i < str.size(); i++) {
                        drawingArea[(cellX + dx + i) + width * (cellY + dy)] = {
                            (uint32_t)str[i], 1};
                    }
                }
            }
        int totalScore = 0;
        for (int y = 0; y < 4; y++)
            for (int x = 0; x < 4; x++)
                if (field[x][y])
                    totalScore += 1 << (field[x][y] - 1);
        std::string totalScoreStr = std::to_string(totalScore);
        for (int i = 0; i < totalScoreStr.size(); i++) {
            drawingArea[(xPad + cellSizeX * 2 - totalScoreStr.size() + i) +
                        width * (yPad + cellSizeY * 4 + 1 - cellPadY)] = {
                (uint32_t)totalScoreStr[i], 0};
        }
        for (int y = 0; y < 4; y++)
            for (int x = 0; x < 4; x++) {
                bool hasValue = false;
                for (const auto &value :
                     db(sqlpp::select(sqlpp::all_of(basecontentScores))
                            .from(basecontentScores)
                            .where(basecontentScores.x.in(x) and
                                   basecontentScores.y.in(y)))) {
                    hasValue = true;
                    db(sqlpp::update(basecontentScores)
                           .set(basecontentScores.score = field[x][y])
                           .where(basecontentScores.id.in(value.id)));
                }
                if (!hasValue) {
                    db(sqlpp::insert_into(basecontentScores)
                           .set(basecontentScores.score = totalScore,
                                basecontentScores.x = x,
                                basecontentScores.y = y));
                }
            }
        webrogue_core_interrupt();
    }
}

extern "C" NR_EXPORTED(void, init_mod_basecontent)() {
    webrogue_core_add_initialization_step(&basecontentInitializationStep);
}
} // namespace basecontent