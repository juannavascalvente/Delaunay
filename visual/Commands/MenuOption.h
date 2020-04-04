//
// Created by delaunay on 29/3/20.
//

#ifndef DELAUNAY_MENUOPTION_H
#define DELAUNAY_MENUOPTION_H


// Default menu option.
enum MenuOption
{
    defaultOption = 0,
    parameters = 1,
    randomly = 2,
    cluster = 3,
    read_points_flat_file = 4,
    read_points_dcel_file = 5,
    read_delaunay = 7,
    read_voronoi = 8,
    read_gabriel = 9,
    star_triangulation = 10,
    delaunay_triangulation = 11,
    convex_hull = 12,
    circumcentres = 13,
    edge_circles = 14,
    voronoi = 15,
    gabriel = 16,
    closest_point = 17,
    two_closest = 18,
    find_face = 19,
    filter_edges = 20,
    triangulation_path = 21,
    voronoi_path = 22,
    dcel_info = 23,
    voronoi_info = 24,
    clear = 25,
    quit = 26,
    write_points = 27,
    write_dcel = 28,
    write_delaunay = 29,
    write_voronoi = 30,
    write_gabriel = 31
};

#endif //DELAUNAY_MENUOPTION_H
