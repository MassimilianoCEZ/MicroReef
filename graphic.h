/*
 * graphic.h
 * Ferrulli Massimiliano
 * Waldorff Carl Johan Traeholt
 * version 18
 */

#ifndef GTKMM_GRAPHIC_H
#define GTKMM_GRAPHIC_H

enum Color {RED, GREEN, BLUE, GREY, BLACK};

void graphic_draw_segment(const double& baseX, const double& baseY, const double& endX,
                           const double& endY, const size_t& colorIndex);
void graphicDrawCircle(const double& baseX, const double& baseY, const double& radius,
                        const size_t& colorIndex);
void graphicDrawRect(const double& centerX, const double& centerY, const double& width,
                        const double& height, const size_t& colorIndex);

#endif // GTKMM_GRAPHIC_H   
