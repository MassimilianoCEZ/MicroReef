/*
 * graphic.cc
 * Ferrulli Massimiliano
 * Waldorff Carl Johan Traeholt
 * version 18
 * 50% Massimiliano 50% Carl
 */
#include "graphic_gui.h"

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr); 

static const std::vector<std::array<double, 3>> colors = {
    {1.0, 0.0, 0.0},     // RED
    {0.32, 0.74, 0.1},   // GREEN
    {0.0, 0.60, 1.0},    // BLUE
    {0.79, 0.79, 0.79},  // GREY
    {0.8, 0.8, 0.8}      // BLACK

};

void set_color(const std::array<double, 3>& rgb)
{
    (*ptcr)->set_source_rgb(rgb[0], rgb[1], rgb[2]);
}

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr)
{
	ptcr = &cr;
}

void graphic_draw_segment(const double& baseX, const double& baseY, const double& endX, 
                          const double& endY, const size_t& colorIndex)
{
  set_color(colors[colorIndex]);
  (*ptcr)->set_line_width(1.0);
  (*ptcr)->move_to(baseX, baseY);
  (*ptcr)->line_to(endX, endY);
  (*ptcr)->stroke();
}


void graphicDrawCircle(const double& baseX, const double& baseY, const double& radius, 
                       const size_t& colorIndex) {
    set_color(colors[colorIndex]);
    (*ptcr)->set_line_width(1.0);
    (*ptcr)->arc(baseX, baseY, radius, 0.0, 2 * M_PI);
    (*ptcr)->stroke();
}

void graphicDrawRect(const double& centerX, const double& centerY, const double& width, 
                      const double& height, const size_t& colorIndex) {
    set_color(colors[colorIndex]);
    (*ptcr)->set_line_width(1.0);
    (*ptcr)->rectangle(centerX - width/2, centerY - height/2, width, height);
    (*ptcr)->stroke();
}






