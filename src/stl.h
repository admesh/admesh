/*  ADMesh -- process triangulated solid meshes
 *  Copyright (C) 1995, 1996  Anthony D. Martin <amartin@engr.csulb.edu>
 *  Copyright (C) 2013, 2014  several contributors, see AUTHORS
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *  Questions, comments, suggestions, etc to
 *           https://github.com/admesh/admesh/issues
 */

#ifndef __admesh_stl__
#define __admesh_stl__

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define STL_MAX(A,B) ((A)>(B)? (A):(B))
#define STL_MIN(A,B) ((A)<(B)? (A):(B))
#define ABS(X)  ((X) < 0 ? -(X) : (X))

#define LABEL_SIZE             80
#define NUM_FACET_SIZE         4
#define HEADER_SIZE            84
#define STL_MIN_FILE_SIZE      284
#define ASCII_LINES_PER_FACET  7
#define SIZEOF_EDGE_SORT       24

/** Vertex of a facet, defined by 3D coordinates. */
typedef struct {
  float x;
  float y;
  float z;
} stl_vertex;

/** Normal vector of a facet, defined by 3D coordinates. */
typedef struct {
  float x;
  float y;
  float z;
} stl_normal;

typedef char stl_extra[2];

/** Facet, one triangle of the mesh. */
typedef struct {
  stl_normal normal;    ///< normal vector
  stl_vertex vertex[3]; ///< 3 vertices
  stl_extra  extra;     ///< extra data 
} stl_facet;

#define SIZEOF_STL_FACET       50

/** Type of STL file. */
typedef enum { 
  binary,       ///< Binary STL format.
  ascii,        ///< ASCII STL format.
  inmemory      ///< No associated file, created on-the-fly e.g. via stl_copy.
} stl_type;

/** Edge between two vertices. */
typedef struct {
  stl_vertex p1;           ///< start vertex
  stl_vertex p2;           ///< end vertex
  int        facet_number; ///< id of facet this edge belongs to
} stl_edge;

typedef struct stl_hash_edge {
  unsigned       key[6];
  int            facet_number;
  int            which_edge;
  struct stl_hash_edge  *next;
} stl_hash_edge;

typedef struct {
  int   neighbor[3];
  char  which_vertex_not[3];
} stl_neighbors;

typedef struct {
  int   vertex[3];
} v_indices_struct;

/** Statistics about the STL mesh.
 * Some of them are populated on stl_open() and after some operations,
 * others, such as volume, have to be calculated by appropriate functions. */
typedef struct {
  char          header[81];              /**< header of the STL file */
  stl_type      type;                    /**< type of the STL file */
  int           number_of_facets;        /**< total number of facets */
  stl_vertex    max;                     /**< maximal dimensions of the mesh */
  stl_vertex    min;                     /**< minimal dimensions of the mesh */
  stl_vertex    size;                    /**< size of the bounding box */
  float         bounding_diameter;       /**< diameter of the bounding box */
  float         shortest_edge;           /**< length of the shortest edge */
  float         volume;                  /**< volume of the mesh, has to be calculated by stl_calculate_volume() */
  unsigned      number_of_blocks;        /**< should be number of blocks, but is never set */
  float         surface_area;
  int           connected_edges;         /**< how many edges have been connected by ADMesh */
  int           connected_facets_1_edge; /**< how many facets are connected by at least 1 edge, get's calculated during stl_check_facets_nearby() */
  int           connected_facets_2_edge; /**< how many facets are connected by at least 2 edges, get's calculated during stl_check_facets_nearby() */
  int           connected_facets_3_edge; /**< how many facets are connected by all 3 edges, get's calculated during stl_check_facets_nearby() */
  int           facets_w_1_bad_edge;     /**< how many facets have exactly 1 unconnected edge, get's calculated during stl_repair() */
  int           facets_w_2_bad_edge;     /**< how many facets have exactly 2 unconnected edges, get's calculated during stl_repair() */
  int           facets_w_3_bad_edge;     /**< how many facets have exactly 3 unconnected edges, get's calculated during stl_repair() */
  int           original_num_facets;     /**< original number of facets when the file was loaded */
  int           edges_fixed;             /**< how many edges were fixed by ADMesh */
  int           degenerate_facets;       /**< number of removed degenerate facets */
  int           facets_removed;          /**< number of removed degenerate facets */
  int           facets_added;            /**< number of facets removed by stl_remove_unconnected_facets() */
  int           facets_reversed;         /**< number of facets reversed by stl_fix_normal_directions() */
  int           backwards_edges;         /**< number of edges that are backwards counted during stl_verify_neighbors() */
  int           normals_fixed;           /**< number of normals fixed during stl_fix_normal_values() */
  int           number_of_parts;         /**< number of parts (distinguished shells), calculated during stl_fix_normal_directions() */
  int           malloced;                /**< how many edges have been malloced during stl_check_facets_nearby() */
  int           freed;                   /**< how many edges have been freed during stl_check_facets_nearby() */
  int           facets_malloced;         /**< how many facets have been malloced */
  int           collisions;              /**< internal collision counter for stl_check_facets_nearby() */
  int           shared_vertices;         /**< number of shared vertices, populated by stl_generate_shared_vertices() */
  int           shared_malloced;         /**< how many shared vertices have been malloced by stl_generate_shared_vertices() */
} stl_stats;

/** STL file.
 * The main structure representing the mesh.
 * All functions take reference to this as a first argument. */
typedef struct {
  FILE          *fp;              /**< pointer to associated file */
  stl_facet     *facet_start;     /**< array of facets */
  stl_edge      *edge_start;      /**< array of edges (never populated) */
  stl_hash_edge **heads;          /**< head of linked list of edges, used internally by some repairs */
  stl_hash_edge *tail;            /**< tail of linked list of edges, used internally by some repairs */
  int           M;                /**< magic variable, used internally by some repairs */
  stl_neighbors *neighbors_start; /**< array of neighbors populated by various repairs  */
  v_indices_struct *v_indices;    /**< internal array used by stl_generate_shared_vertices() */
  stl_vertex    *v_shared;        /**< vertices array used by stl_generate_shared_vertices() */
  stl_stats     stats;            /**< statistics about the mesh */
  char          error;            /**< error flag, when something went wrong, this is not 0 */
} stl_file;

/** Open an STL file and load it's contents.
 * \warning As IO operation, this could result in an error,
 * always check the error flag with stl_get_error() or use stl_exit_on_error() after using stl_open()
 * \param stl The struct to load the file data to
 * \param file Path to the STL file */
extern void stl_open(stl_file *stl, char *file);

/** Perform cleanup on \ref stl_file.
 * This function frees memory, always use it, when you no longer needs the stl_file instance
 * \param stl What to close */
extern void stl_close(stl_file *stl);

/** Print statistics in human readable form to some file.
 * \param stl Where to obtain the statisctics
 * \param file Where to print the statisctics to (can be stdout)
 * \param input_file What filename to use in the human readable output */
extern void stl_stats_out(stl_file *stl, FILE *file, char *input_file);

/** Print edges to some file.
 * \warning This prints from edge_start array, that is never populated and thus this will never actually work
 * \param stl From what data
 * \param file Where to print the edges to (can be stdout) */
extern void stl_print_edges(stl_file *stl, FILE *file);

/**
 * @brief Print neighbor list to the specified file.
 * 
 * @param stl Source STL.
 * @param file Target file to print to.
 */
extern void stl_print_neighbors(stl_file *stl, char *file);

/**
 * @brief Store a integer value to a specified file in a little-endian format.
 * 
 * @param fp Target file.
 * @param value_in A value to write.
 */
extern void stl_put_little_int(FILE *fp, int value_in);

/**
 * @brief Store a float value to a specified file in a little-endian format.
 * 
 * @param fp Target file.
 * @param value_in A value to write.
 */
extern void stl_put_little_float(FILE *fp, float value_in);

/**
 * @brief Write STL data to the STL file in the ASCII format.
 * 
 * @param stl Data to write to a file.
 * @param file Target file location.
 * @param label A label to use in the file. (Name of the solid.)
 */
extern void stl_write_ascii(stl_file *stl, const char *file, const char *label);

/**
 * @brief Write STL data to the STL file in the binary format.
 * 
 * @param stl Data to write to a file.
 * @param file Target file location.
 * @param label A label to use in the file. (Name of the solid.)
 */
extern void stl_write_binary(stl_file *stl, const char *file, const char *label);

/**
 * @brief Write STL facets to a file. Used internally by @ref stl_write_binary.
 * 
 * @param stl Data from which to extract facets.
 * @param fp Target file.
 */
extern void stl_write_binary_block(stl_file *stl, FILE *fp);

/**
 * @brief Build a neighbor list.
 * 
 * This function builds the neighbors list.  No modifications are made
 * to any of the facets.  The edges are said to match only if all six
 * floats of the first edge matches all six floats of the second edge.
 * 
 * @param stl 
 */
extern void stl_check_facets_exact(stl_file *stl);

/**
 * @brief Check nearby facets available for connection within a distance specified by tolerance.
 * 
 * @param stl Data to work with.
 * @param tolerance Distance that is searched for the neighboring facet.
 */
extern void stl_check_facets_nearby(stl_file *stl, float tolerance);

/**
 * @brief Remove unconnected and degenerate facets.
 * 
 * A couple of things need to be done here. One is to remove any
 * completely unconnected facets (0 edges connected) since these are
 * useless and could be completely wrong. The second thing that needs to
 * be done is to remove any degenerate facets that were created during
 * stl_check_facets_nearby().
 * 
 * @param stl Data to work with.
 */
extern void stl_remove_unconnected_facets(stl_file *stl);

/**
 * @brief Print a specified vertex of a selected facet to stdout.
 * 
 * @param stl Data to get a facet from.
 * @param facet Selected facet index.
 * @param vertex Selected vertex index.
 */
extern void stl_write_vertex(stl_file *stl, int facet, int vertex);

/**
 * @brief Print a selected facet with all three vertices to stdout.
 * 
 * @param stl Data to get a facet from.
 * @param label A text to print after "facet".
 * @param facet Selected facet index.
 */
extern void stl_write_facet(stl_file *stl, char *label, int facet);

/**
 * @brief Print a selected facet's edge to the stdout.
 * 
 * @param stl Data to get an edge from.
 * @param label A text to print after "edge".
 * @param edge A selected edge to print.
 */
extern void stl_write_edge(stl_file *stl, char *label, stl_hash_edge edge);

/**
 * @brief Print a facet's neighbors to the stdout.
 * 
 * @param stl Data to get a facet from.
 * @param facet Selected facet index.
 */
extern void stl_write_neighbor(stl_file *stl, int facet);

// TODO
extern void stl_write_quad_object(stl_file *stl, char *file);

/**
 * @brief Check each facet for its neighbors. There should be 3 for every facet.
 * 
 * @param stl Data to check facets in.
 */
extern void stl_verify_neighbors(stl_file *stl);

/**
 * @brief Try to fill all holes in the mesh by adding facets.
 * @warning The result may not be what do you expect - admesh doesn't know what the result should look like.
 * @param stl Mesh to fill holes in.
 */
extern void stl_fill_holes(stl_file *stl);

/**
 * @brief Fix inverted normals - normal should point outwards from solid.
 * @param stl Mesh in which to fix normals.
 */
extern void stl_fix_normal_directions(stl_file *stl);

/**
 * @brief Verify normals values and directions by calculating the value from coordinates using the right-hand rule.
 * 
 * @param stl Mesh to fix normals in.
 */
extern void stl_fix_normal_values(stl_file *stl);

/**
 * @brief Reverse all facets - coords, neighbors and normals appropriately.
 * 
 * @param stl Mesh to reverse facets in.
 */
extern void stl_reverse_all_facets(stl_file *stl);

/**
 * @brief Translate the mesh absolutely to the specified coordinates - modifies all facets and max and min values.
 * 
 * @param stl Mesh to translate.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param z Z coordinate.
 */
extern void stl_translate(stl_file *stl, float x, float y, float z);

/**
 * @brief Translate a mesh relatively.x_off
 * Adds specified XYZ to the vertices and max/min values.
 * @param stl Mesh to translate.
 * @param x X offset.
 * @param y Y offset.
 * @param z Z offset.
 */
extern void stl_translate_relative(stl_file *stl, float x, float y, float z);

/**
 * @brief Stretch the STL, i.e. move a point by a relative XYZ offset if it fits within a given bounding box.
 * 
 * @param stl   Mesh to stretch.
 * @param x_min Bounding box first X coord.
 * @param x_max Bounding box second X coord.
 * @param x_off Relative X offset.
 * @param y_min Bounding box first Y coord.
 * @param y_max Bounding box second Y coord.
 * @param y_off Relative Y offset.
 * @param z_min Bounding box first Z coord.
 * @param z_max Bounding box second Z coord.
 * @param z_off Relative Z offset.
 */
extern void stl_stretch(stl_file *stl, float x_min, float x_max, float x_off, float y_min, float y_max, float y_off, float z_min, float z_max, float z_off);

/**
 * @brief Scale the mesh using a versor.
 * 
 * @param stl     Mesh to scale.
 * @param versor  Scaling versor.
 */
extern void stl_scale_versor(stl_file *stl, float versor[3]);

/**
 * @brief Scale the mesh using a single factor.
 * Converts the factor to a versor and applies stl_scale_versor.
 * @param stl     Mesh to scale.
 * @param factor  Scaling factor.
 */
extern void stl_scale(stl_file *stl, float factor);

/**
 * @brief Rotate the mesh around the X axis by a specified angle in degrees.
 * 
 * @param stl   Mesh to rotate.
 * @param angle Angle in degrees.
 */
extern void stl_rotate_x(stl_file *stl, float angle);

/**
 * @brief Rotate the mesh around the Y axis by a specified angle in degrees.
 * 
 * @param stl   Mesh to rotate.
 * @param angle Angle in degrees.
 */
extern void stl_rotate_y(stl_file *stl, float angle);

/**
 * @brief Rotate the mesh around the Z axis by a specified angle in degrees.
 * 
 * @param stl   Mesh to rotate.
 * @param angle Angle in degrees.
 */
extern void stl_rotate_z(stl_file *stl, float angle);

/**
 * @brief Mirror the mesh about the XY plane.
 * The signs of all of the Z coordinates in the mesh are reversed.
 * @param stl Mesh to mirror.
 */
extern void stl_mirror_xy(stl_file *stl);

/**
 * @brief Mirror the mesh about the YZ plane.
 * The signs of all of the X coordinates in the mesh are reversed.
 * @param stl Mesh to mirror.
 */
extern void stl_mirror_yz(stl_file *stl);

/**
 * @brief Mirror the mesh about the XZ plane.
 * The signs of all of the Y coordinates in the mesh are reversed.
 * @param stl Mesh to mirror.
 */
extern void stl_mirror_xz(stl_file *stl);

/**
 * @brief Merge the mesh with a file at the location specified.
 * No translation is done, so if, for example, a file was merged with itself,
 * the resulting file would end up with two meshes exactly the same,
 * occupying exactly the same space. So generally, translations need to be done to the files 
 * to be merged so that when the two meshes are merged into one,
 * the two resulting parts are properly spaced.
 * If you know the nature of the parts to be merged, it is possible to “nest” one part inside the other.
 * Note, however, that no warnings will be given if one part intersects with the other.
 * @param stl  Mesh to which merge the file.
 * @param file A location of the file to merge.
 */
extern void stl_open_merge(stl_file *stl, char *file);

/**
 * @brief Free v_indices and v_shared members of the STL struct.
 * 
 * @param stl STL to invalidate vertices in.
 */
extern void stl_invalidate_shared_vertices(stl_file *stl);

/**
 * @brief Find shared vertices and populate v_indices and v_shared accordingly.
 * 
 * @param stl STL to generate shared vertices in.
 */
extern void stl_generate_shared_vertices(stl_file *stl);

/**
 * @brief Output mesh data to a file in the OBJ format.
 * 
 * @param stl   Data to write.
 * @param file  Location of the output file including filename.
 */
extern void stl_write_obj(stl_file *stl, char *file);

/**
 * @brief Output mesh data to a file in the Geomview OFF format.
 * 
 * @param stl  Data to write.
 * @param file Location of the output file including filename.
 */
extern void stl_write_off(stl_file *stl, char *file);

/**
 * @brief Output mesh data to a file in the DXF format.
 * 
 * @param stl   Data to write.
 * @param file  Location of the output file including filename.
 * @param label Label to write to the file.
 */
extern void stl_write_dxf(stl_file *stl, char *file, char *label);

/**
 * @brief Output mesh data to a file in the VRML format.
 * 
 * @param stl   Data to write.
 * @param file  Location of the output file including filename.
 */
extern void stl_write_vrml(stl_file *stl, char *file);

/**
 * @brief Calculate a normal vector from the facet vertices.
 * 
 * @param[out]  normal Output vector.
 * @param       facet  Facet to calculate the normal from.
 */
extern void stl_calculate_normal(float normal[], stl_facet *facet);

/**
 * @brief Normalize a specified vector.
 * 
 * @param[inout] v Vector to normalize.
 */
extern void stl_normalize_vector(float v[]);

/**
 * @brief Calculate a volume of the mesh using get_volume() and save it to the stats.volume.
 * 
 * @param stl Mesh to calculate a volume of.
 */
extern void stl_calculate_volume(stl_file *stl);

/**
 * @brief Calculate a surface area of the mesh using get_surface_area() and save it to the stats.surface_area.
 * 
 * @param stl Mesh to calculate a surface area of.
 */
extern void stl_calculate_surface_area(stl_file *stl);

/**
 * @brief Perform a series of repairs to attempt to repair the mesh.
 * 
 * @param stl                       Mesh to repair.
 * @param fixall_flag               Perform all fixes.
 * @param exact_flag                Check each facet of the mesh for its 3 neighbors and remove degenerates.
 * @param tolerance_flag            Tolerance is explicitly specified.
 * @param tolerance                 Tolerance value.
 * @param increment_flag            Increment is explicitly specified.
 * @param increment                 The amount that the tolerance is increased after each iteration.
 * @param nearby_flag               Find and connect nearby facets and correct bad facets.
 * @param iterations                The number of times that facets are checked for nearby facets.
 * @param remove_unconnected_flag   Remove unconnected facets.
 * @param fill_holes_flag           Fill holes in the mesh.
 * @param normal_directions_flag    Check and fix direction of normals.
 * @param normal_values_flag        Check and fix normal values.
 * @param reverse_all_flag          Reverse all facets.
 * @param verbose_flag              Provide detailed information during the process.
 */
extern void stl_repair(stl_file *stl, int fixall_flag, int exact_flag, int tolerance_flag, float tolerance, int increment_flag, float increment, int nearby_flag, int iterations, int remove_unconnected_flag, int fill_holes_flag, int normal_directions_flag, int normal_values_flag, int reverse_all_flag, int verbose_flag);

/**
 * @brief Initialize the STL struct to default values.
 * 
 * @param stl Struct to init.
 */
extern void stl_initialize(stl_file *stl);

/**
 * @brief Count facets in the mesh.
 * 
 * @param stl 
 * @param file 
 */
extern void stl_count_facets(stl_file *stl, char *file);
extern void stl_allocate(stl_file *stl);
extern void stl_read(stl_file *stl, int first_facet, int first);
extern void stl_facet_stats(stl_file *stl, stl_facet facet, int first);
extern void stl_reallocate(stl_file *stl);
extern void stl_add_facet(stl_file *stl, stl_facet *new_facet);
extern void stl_get_size(stl_file *stl);

extern void stl_clear_error(stl_file *stl);
extern int stl_get_error(stl_file *stl);
extern void stl_exit_on_error(stl_file *stl);

#ifdef __cplusplus
}
#endif

#endif
