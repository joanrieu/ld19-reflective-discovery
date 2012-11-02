#include "GGame.h"

#include <wx/wfstream.h>
#include <wx/txtstrm.h>

#include "GameConstants.h"
#include <deque>

namespace {

    wxTextInputStream& operator>>(wxTextInputStream& stream, GDir& dir) {
        return stream >> *reinterpret_cast<unsigned*>(&dir);
    }

    wxTextInputStream& operator>>(wxTextInputStream& stream, GCol& col) {
        return stream >> *reinterpret_cast<unsigned*>(&col);
    }

}

GLevel* GLevel::NewFromId(const unsigned long id) {

    wxFileInputStream file(GC_LEVEL_FILE(id));

    if (not file.IsOk())
        return 0;

    return new (std::nothrow) GLevel(id, file);

}

GLevel::GLevel(const unsigned long level_id, wxInputStream& stream) : id(level_id) {

    wxTextInputStream text(stream);
    text.SetStringSeparators(" \n");

    // LASER
    GPos& lp(laser.first);
    GLaser& l(laser.second);
    text >> lp.first >> lp.second >> l.dir >> l.col;

    // REFLECTORS
    unsigned reflector_count;
    text >> reflector_count;
    while (reflector_count --> 0) {
        GPos p;
        GReflector r;
        text >> p.first >> p.second >> r.dir >> r.col >> r.col_alt;
        reflectors.insert(std::make_pair(p, r));
    }

    // BLOCKS
    unsigned block_count;
    text >> block_count;
    while (block_count --> 0) {
        GPos p;
        GBlock b;
        text >> p.first >> p.second >> b.col;
        blocks.insert(std::make_pair(p, b));
    }

}

void GGame::Refresh() {

    for (GLevel::GBlockCont::iterator it = level->blocks.begin(); it != level->blocks.end(); ++it)
        it->second.lit_up = false;

    GRay start_ray; // WARNING start_ray NOT DYNAMICALLY ALLOCATED !
    *static_cast<GLaser*>(&start_ray) = level->laser.second;

    GRayP ray(GRayPP(level->laser.first, 0), &start_ray); // WARNING &start_ray stored

    GRPQ queue;
    queue.push_back(ray);

    for (GRPQ::iterator it = queue.begin(); it != queue.end(); ++it)
        TraceRay(it, queue);

    GRPQ lit_up;
    bool light_change = true;
    while (light_change) {

        light_change = false;

        for (GRPQ::iterator it = queue.begin(); it != queue.end(); ++it) {

            if (not it->second->lit_up or not it->first.second)
                continue;

            bool failed = false;
            for (GRPQ::iterator it2 = lit_up.begin(); it2 != lit_up.end(); ++it2) {
                if (it2->second == it->second) {
                    failed = true;
                    break;
                }
            }

            if (failed)
                continue;

            it->first.second->lit_up = light_change = true;
            lit_up.push_back(*it);

        }

    }

    m_won = true;
    for (GLevel::GBlockCont::iterator it = level->blocks.begin(); it != level->blocks.end(); ++it) {
        if (not it->second.lit_up) {
            m_won = false;
            break;
        }
    }

    m_blocks.clear();

    for (GRPQ::const_iterator it = lit_up.begin(); it != lit_up.end(); ++it) {
        for (int dy = -1; dy < 2; ++dy) {
            for (int dx = -1; dx < 2; ++dx) {
                GLevel::GBlockCont::const_iterator block_it = level->blocks.find(GPos(it->first.first.first + dx, it->first.first.second + dy));
                if (block_it != level->blocks.end())
                    m_blocks.insert(*block_it);
            }
        }
    }

    queue.pop_front(); // remove &start_ray
    m_rays.clear();

    for (GRPQ::iterator it = queue.begin(); it != queue.end(); ++it) {
        m_rays.insert(std::make_pair(it->first.first, *it->second));
        delete it->second;
    }

}

void GGame::TraceRay(GRPQ::iterator& it, GRPQ& queue) {

    GRayP next(NextRay(*it));

    if (next.first.first == level->laser.first or next.first.first.first >= GC_LEVEL_WIDTH or next.first.first.second >= GC_LEVEL_HEIGHT)
        return;

    for (GRPQ::const_iterator queue_it = queue.begin(); queue_it != queue.end(); ++queue_it)
        if (queue_it != it and next.first.first == queue_it->first.first and next.second == queue_it->second)
            return;

    GLevel::GBlockCont::iterator block_it = level->blocks.find(next.first.first);
    if (block_it != level->blocks.end()) {

        if (block_it->second.col == next.second->col)
            it->second->lit_up = block_it->second.lit_up = true;

        return;

    }

    GLevel::GReflectorCont::const_iterator refl_it = level->reflectors.find(next.first.first);
    if (refl_it != level->reflectors.end()) {

        const GDir side_real = static_cast<GDir>((next.second->dir + 2) % 4);
        const GDir side_square_up = static_cast<GDir>((side_real - refl_it->second.dir + 4) % 4);

        GDir reflected_dir_square_up;

        if (next.second->col == refl_it->second.col) {

            switch (side_square_up) {
                case GC_DIR_UP:
                    reflected_dir_square_up = GC_DIR_LEFT;
                    break;
                case GC_DIR_LEFT:
                    reflected_dir_square_up = GC_DIR_UP;
                    break;
                default:
                    return;
            }

        } else if (next.second->col == refl_it->second.col_alt) {

            switch (side_square_up) {
                case GC_DIR_RIGHT:
                    reflected_dir_square_up = GC_DIR_DOWN;
                    break;
                case GC_DIR_DOWN:
                    reflected_dir_square_up = GC_DIR_RIGHT;
                    break;
                default:
                    return;
            }

        } else return;

        const GDir reflected_dir_real = static_cast<GDir>((reflected_dir_square_up + refl_it->second.dir) % 4);

        GRayP reflected(NextRay(*it));
        reflected.second->dir = reflected_dir_real;
        queue.push_back(reflected);

        next.second->col = refl_it->second.col_alt;

    }

    queue.push_back(next);

}

GGame::GRayP GGame::NextRay(GRayP& ray) {

    GRayP next(GRayPP(ray.first.first, ray.second), new GRay(*ray.second));

    const GDir& dir(ray.second->dir);
    GPos& pos(next.first.first);
    GCoord& x(pos.first);
    GCoord& y(pos.second);

    switch (dir) {
        case GC_DIR_UP:
            --y;
            break;
        case GC_DIR_RIGHT:
            ++x;
            break;
        case GC_DIR_DOWN:
            ++y;
            break;
        case GC_DIR_LEFT:
            --x;
            break;
    }

    return next;

}
