#ifndef GGAME_H_
#define GGAME_H_

#include <map>
#include <queue>
#include <algorithm>
#include <wx/string.h>
#include <wx/stream.h>

typedef unsigned GCoord;
typedef std::pair<GCoord, GCoord> GPos;

enum GDir {
    GC_DIR_UP = 0,
    GC_DIR_RIGHT,
    GC_DIR_DOWN,
    GC_DIR_LEFT
};

enum GCol {
    GC_COL_RED = 1,
    GC_COL_GREEN = 2,
    GC_COL_BLUE = 4,
    GC_COL_WHITE = GC_COL_RED | GC_COL_GREEN | GC_COL_BLUE, // 7
    GC_COL_CYAN = GC_COL_WHITE & ~GC_COL_RED, // 6
    GC_COL_MAGENTA = GC_COL_WHITE & ~GC_COL_GREEN, // 5
    GC_COL_YELLOW = GC_COL_WHITE & ~GC_COL_BLUE // 3
};

struct GLaser {
    GDir dir;
    GCol col;
};

struct GReflector {
    GDir dir;
    GCol col, col_alt;
};

struct GBlock {
    GBlock() : lit_up(false) {}
    GCol col;
    bool lit_up;
};

struct GLevel {

    static GLevel* NewFromId(const unsigned long id);
    GLevel(const unsigned long level_id, wxInputStream& stream);

    const unsigned long id;

    std::pair<GPos, GLaser> laser;

    typedef std::map<GPos, GReflector> GReflectorCont;
    GReflectorCont reflectors;

    typedef std::map<GPos, GBlock> GBlockCont;
    GBlockCont blocks;

};

struct GRay : public GLaser {
    GRay() : lit_up(false) {}
    bool lit_up;
};

class GGame {

public:

    GGame(GLevel* const lvl) : level(lvl), rays(m_rays), blocks(m_blocks), won(m_won), m_won(false) { Refresh(); }
    ~GGame() { delete level; }

    void Refresh();

    GLevel* const level;

    typedef std::multimap<GPos, GRay> GRayCont;
    const GRayCont& rays;

    const GLevel::GBlockCont& blocks;

    const bool& won;

private:

    GRayCont m_rays;

    typedef std::pair<GPos, GRay*> GRayPP;
    typedef std::pair<GRayPP, GRay*> GRayP;
    typedef std::deque<GRayP> GRPQ;
    void TraceRay(GRPQ::iterator& it, GRPQ& queue);
    GRayP NextRay(GRayP& ray);

    GLevel::GBlockCont m_blocks;

    bool m_won;

};

#endif
