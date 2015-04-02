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

/** Vertex of a facet, defined by 3D coordinates */
typedef struct {
  float x;
  float y;
  float z;
} stl_vertex;

/** Normal vector of a facet, defined by 3D coordinates */
typedef struct {
  float x;
  float y;
  float z;
} stl_normal;

typedef char stl_extra[2];

/** Facet, one triangle of the mesh */
typedef struct {
  stl_normal normal;    /**< normal vector */
  stl_vertex vertex[3]; /**< 3 vertices */
  stl_extra  extra;     /**< extra data */
} stl_facet;

#define SIZEOF_STL_FACET       50

/** Type of STL file */
typedef enum {binary, ascii, inmemory} stl_type;

/** Edge between two vertices */
typedef struct {
  stl_vertex p1;           /**< start vertex */
  stl_vertex p2;           /**< end vertex */
  int        facet_number; /**< id of facet this edge belongs to */
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

extern void stl_open(stl_file *stl, char *file);
extern void stl_close(stl_file *stl);
extern void stl_stats_out(stl_file *stl, FILE *file, char *input_file);
extern void stl_print_edges(stl_file *stl, FILE *file);
extern void stl_print_neighbors(stl_file *stl, char *file);
extern void stl_put_little_int(FILE *fp, int value_in);
extern void stl_put_little_float(FILE *fp, float value_in);
extern void stl_write_ascii(stl_file *stl, const char *file, const char *label);
extern void stl_write_binary(stl_file *stl, const char *file, const char *label);
extern void stl_write_binary_block(stl_file *stl, FILE *fp);
extern void stl_check_facets_exact(stl_file *stl);
extern void stl_check_facets_nearby(stl_file *stl, float tolerance);
extern void stl_remove_unconnected_facets(stl_file *stl);
extern void stl_write_vertex(stl_file *stl, int facet, int vertex);
extern void stl_write_facet(stl_file *stl, char *label, int facet);
extern void stl_write_edge(stl_file *stl, char *label, stl_hash_edge edge);
extern void stl_write_neighbor(stl_file *stl, int facet);
extern void stl_write_quad_object(stl_file *stl, char *file);
extern void stl_verify_neighbors(stl_file *stl);
extern void stl_fill_holes(stl_file *stl);
extern void stl_fix_normal_directions(stl_file *stl);
extern void stl_fix_normal_values(stl_file *stl);
extern void stl_reverse_all_facets(stl_file *stl);
extern void stl_translate(stl_file *stl, float x, float y, float z);
extern void stl_translate_relative(stl_file *stl, float x, float y, float z);
extern void stl_scale_versor(stl_file *stl, float versor[3]);
extern void stl_scale(stl_file *stl, float factor);
extern void stl_rotate_x(stl_file *stl, float angle);
extern void stl_rotate_y(stl_file *stl, float angle);
extern void stl_rotate_z(stl_file *stl, float angle);
extern void stl_mirror_xy(stl_file *stl);
extern void stl_mirror_yz(stl_file *stl);
extern void stl_mirror_xz(stl_file *stl);
extern void stl_open_merge(stl_file *stl, char *file);
extern void stl_invalidate_shared_vertices(stl_file *stl);
extern void stl_generate_shared_vertices(stl_file *stl);
extern void stl_write_obj(stl_file *stl, char *file);
extern void stl_write_off(stl_file *stl, char *file);
extern void stl_write_dxf(stl_file *stl, char *file, char *label);
extern void stl_write_vrml(stl_file *stl, char *file);
extern void stl_calculate_normal(float normal[], stl_facet *facet);
extern void stl_normalize_vector(float v[]);
extern void stl_calculate_volume(stl_file *stl);

extern void stl_repair(stl_file *stl, int fixall_flag, int exact_flag, int tolerance_flag, float tolerance, int increment_flag, float increment, int nearby_flag, int iterations, int remove_unconnected_flag, int fill_holes_flag, int normal_directions_flag, int normal_values_flag, int reverse_all_flag, int verbose_flag);

extern void stl_initialize(stl_file *stl);
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
