/*  ADMesh -- process triangulated solid meshes
 *  Copyright (C) 1995, 1996  Anthony D. Martin
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *  
 *  Questions, comments, suggestions, etc to <amartin@engr.csulb.edu>
 */

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>


#include "stl.h"

static void usage(int status, char *program_name);

void
main(int argc, char **argv)
{
  stl_file stl_in;
  int      i;
  int      last_edges_fixed = 0;
  float    tolerance = 0;
  float    increment = 0;
  float    x_trans;
  float    y_trans;
  float    z_trans;
  float    scale_factor = 0;
  float    rotate_x_angle = 0;
  float    rotate_y_angle = 0;
  float    rotate_z_angle = 0;
  int      c;
  char     *program_name;
  char     *binary_name = NULL;
  char     *ascii_name = NULL;
  char     *merge_name = NULL;
  char     *off_name = NULL;
  char     *dxf_name = NULL;
  char     *vrml_name = NULL;
  int      fixall_flag = 1;	       /* Default behavior is to fix all. */
  int      exact_flag = 0;	       /* All checks turned off by default. */
  int      tolerance_flag = 0;	       /* Is tolerance specified on cmdline */
  int      nearby_flag = 0;
  int      remove_unconnected_flag = 0;
  int      fill_holes_flag = 0;
  int      normal_directions_flag = 0;
  int      normal_values_flag = 0;
  int      reverse_all_flag = 0;
  int      write_binary_stl_flag = 0;
  int      write_ascii_stl_flag = 0;
  int      generate_shared_vertices_flag = 0;
  int      write_off_flag = 0;
  int      write_dxf_flag = 0;
  int      write_vrml_flag = 0;
  int      translate_flag = 0;
  int      scale_flag = 0;
  int      rotate_x_flag = 0;
  int      rotate_y_flag = 0;
  int      rotate_z_flag = 0;
  int      mirror_xy_flag = 0;
  int      mirror_yz_flag = 0;
  int      mirror_xz_flag = 0;
  int      merge_flag = 0;
  int      help_flag = 0;
  int      version_flag = 0;
  
  int      iterations = 2;	       /* Default number of iterations. */
  int      increment_flag = 0;
  char     *input_file = NULL;
  
  enum {rotate_x = 1000, rotate_y, rotate_z, merge, help, version,
      mirror_xy, mirror_yz, mirror_xz, scale, translate, reverse_all,
      off_file, dxf_file, vrml_file};
  
  struct option long_options[] =
    {
	{"exact",              no_argument,       NULL, 'e'},
	{"nearby",             no_argument,       NULL, 'n'},
	{"tolerance",          required_argument, NULL, 't'},
	{"iterations",         required_argument, NULL, 'i'},
	{"increment",          required_argument, NULL, 'm'},
	{"remove-unconnected", no_argument,       NULL, 'u'},
	{"fill-holes",         no_argument,       NULL, 'f'},
	{"normal-directions",  no_argument,       NULL, 'd'},
	{"normal-values",      no_argument,       NULL, 'v'},
	{"no-check",           no_argument,       NULL, 'c'},
	{"reverse-all",        no_argument,       NULL, reverse_all},
	{"write-binary-stl",   required_argument, NULL, 'b'},
	{"write-ascii-stl",    required_argument, NULL, 'a'},
	{"write-off",          required_argument, NULL, off_file},
	{"write-dxf",          required_argument, NULL, dxf_file},
	{"write-vrml",         required_argument, NULL, vrml_file},
	{"translate",          required_argument, NULL, translate},
	{"scale",              required_argument, NULL, scale},
	{"x-rotate",           required_argument, NULL, rotate_x},
	{"y-rotate",           required_argument, NULL, rotate_y},
	{"z-rotate",           required_argument, NULL, rotate_z},
	{"xy-mirror",          no_argument,       NULL, mirror_xy},
	{"yz-mirror",          no_argument,       NULL, mirror_yz},
	{"xz-mirror",          no_argument,       NULL, mirror_xz},
	{"merge",              required_argument, NULL, merge},
	{"help",               no_argument,       NULL, help},
	{"version",            no_argument,       NULL, version},
	{NULL, 0, NULL, 0}
    };

  program_name = argv[0];
  while((c = getopt_long(argc, argv, "et:i:m:nufdcvb:a:",
			 long_options, (int *) 0)) != EOF)
    {
      switch(c)
	{
	 case 0:		       /* If *flag is not null */
	  break;
	 case 'e':
	  exact_flag = 1;
	  fixall_flag = 0;
	  break;
	 case 'n':
	  nearby_flag = 1;
	  fixall_flag = 0;
	  break;
	 case 't':
	  tolerance_flag = 1;
	  tolerance = atof(optarg);
	  break;
	 case 'i':
	  iterations = atoi(optarg);
	  break;
	 case 'm':
	  increment_flag = 1;
	  increment = atof(optarg);
	  break;
	 case 'u':
	  remove_unconnected_flag = 1;
	  fixall_flag = 0;
	  break;
	 case 'f':
	  fill_holes_flag = 1;
	  fixall_flag = 0;
	  break;
	 case 'd':
	  normal_directions_flag = 1;
	  fixall_flag = 0;
	  break;
	 case 'v':
	  normal_values_flag = 1;
	  fixall_flag = 0;
	  break;
	 case 'c':
	  fixall_flag = 0;
	  break;
	 case reverse_all:
	  reverse_all_flag = 1;
	  fixall_flag = 0;
	  break;
	 case 'b':
	  write_binary_stl_flag = 1;
	  binary_name = optarg;	       /* I'm not sure if this is safe. */
	  break;
	 case 'a':
	  write_ascii_stl_flag = 1;
	  ascii_name = optarg;	       /* I'm not sure if this is safe. */
	  break;
	 case off_file:
	  generate_shared_vertices_flag = 1;
	  write_off_flag = 1;
	  off_name = optarg;
	  break;
	 case vrml_file:
	  generate_shared_vertices_flag = 1;
	  write_vrml_flag = 1;
	  vrml_name = optarg;
	  break;
	 case dxf_file:
	  write_dxf_flag = 1;
	  dxf_name = optarg;
	  break;
	 case translate:
	  translate_flag = 1;
	  sscanf(optarg, "%f,%f,%f", &x_trans, &y_trans, &z_trans);
	  break;
	 case scale:
	  scale_flag = 1;
	  scale_factor = atof(optarg);
	  break;
	 case rotate_x:
	  rotate_x_flag = 1;
	  rotate_x_angle = atof(optarg);
	  break;
	 case rotate_y:
	  rotate_y_flag = 1;
	  rotate_y_angle = atof(optarg);
	  break;
	 case rotate_z:
	  rotate_z_flag = 1;
	  rotate_z_angle = atof(optarg);
	  break;
	 case mirror_xy:
	  mirror_xy_flag = 1;
	  break;
	 case mirror_yz:
	  mirror_yz_flag = 1;
	  break;
	 case mirror_xz:
	  mirror_xz_flag = 1;
	  break;
	 case merge:
	  merge_flag = 1;
	  merge_name = optarg;
	  break;
	 case help:
	  help_flag = 1;
	  break;
	 case version:
	  version_flag = 1;
	  break;
    	 default:
	  usage(1, program_name);
	  break;
	}
    }
  
  if(help_flag)
    {
      usage(0, program_name);
    }
  if(version_flag)
    {
      printf("ADMesh - version 0.95\n");
      exit(0);
    }
  
  if(optind == argc)
    {
      printf("No input file name given.\n");
      usage(1, program_name);
    }
  else
    {
      input_file = argv[optind];
    }

  printf("\
ADMesh version 0.95, Copyright (C) 1995, 1996 Anthony D. Martin\n\
ADMesh comes with NO WARRANTY.  This is free software, and you are welcome to\n\
redistribute it under certain conditions.  See the file COPYING for details.\n");

  
  printf("Opening %s\n", input_file);
  stl_open(&stl_in, input_file);
  
  if(rotate_x_flag)
    {
      printf("Rotating about the x axis by %f degrees...\n", rotate_x_angle);
      stl_rotate_x(&stl_in, rotate_x_angle);
    }
  if(rotate_y_flag)
    {
      printf("Rotating about the y axis by %f degrees...\n", rotate_y_angle);
      stl_rotate_y(&stl_in, rotate_y_angle);
    }
  if(rotate_z_flag)
    {
      printf("Rotating about the z axis by %f degrees...\n", rotate_z_angle);
      stl_rotate_z(&stl_in, rotate_z_angle);
    }
  if(mirror_xy_flag)
    {
      printf("Mirroring about the xy plane...\n");
      stl_mirror_xy(&stl_in);
    }
  if(mirror_yz_flag)
    {
      printf("Mirroring about the yz plane...\n");
      stl_mirror_yz(&stl_in);
    }
  if(mirror_xz_flag)
    {
      printf("Mirroring about the xz plane...\n");
      stl_mirror_xz(&stl_in);
    }
  
  if(scale_flag)
    {
      printf("Scaling by factor %f...\n", scale_factor);
      stl_scale(&stl_in, scale_factor);
    }  
  if(translate_flag)
    {
      printf("Translating to %f, %f, %f ...\n", x_trans, y_trans, z_trans);
      stl_translate(&stl_in, x_trans, y_trans, z_trans);
    }
  if(merge_flag)
    {
      printf("Merging %s with %s\n", input_file, merge_name);
      stl_open_merge(&stl_in, merge_name);
    }
  
  if(exact_flag || fixall_flag || nearby_flag || remove_unconnected_flag
     || fill_holes_flag || normal_directions_flag)
    {
      printf("Checking exact...\n");
      exact_flag = 1;
      stl_check_facets_exact(&stl_in);
      stl_in.stats.facets_w_1_bad_edge = 
	(stl_in.stats.connected_facets_2_edge -
	 stl_in.stats.connected_facets_3_edge);
      stl_in.stats.facets_w_2_bad_edge = 
	(stl_in.stats.connected_facets_1_edge -
	 stl_in.stats.connected_facets_2_edge);
      stl_in.stats.facets_w_3_bad_edge = 
	(stl_in.stats.number_of_facets -
	 stl_in.stats.connected_facets_1_edge);
    }  
  
  if(nearby_flag || fixall_flag)
    {
      if(!tolerance_flag)
	{
	  tolerance = stl_in.stats.shortest_edge;
	}
      if(!increment_flag)
	{
	  increment = stl_in.stats.bounding_diameter / 10000.0;
	}
     
      if(stl_in.stats.connected_facets_3_edge < stl_in.stats.number_of_facets)
	{
	  for(i = 0; i < iterations; i++)
	    {
	      if(stl_in.stats.connected_facets_3_edge < 
		 stl_in.stats.number_of_facets)
		{
		  printf("\
Checking nearby. Tolerance= %f Iteration=%d of %d...",
			 tolerance, i + 1, iterations);
		  stl_check_facets_nearby(&stl_in, tolerance);
		  printf("  Fixed %d edges.\n",
			 stl_in.stats.edges_fixed - last_edges_fixed);
		  last_edges_fixed = stl_in.stats.edges_fixed;
		  tolerance += increment;
		}
	      else
		{
		  printf("\
All facets connected.  No further nearby check necessary.\n");
		  break;
		}
	    }
	}
      else
	{
	  printf("All facets connected.  No nearby check necessary.\n");
	}
    }
  
  if(remove_unconnected_flag || fixall_flag || fill_holes_flag)
    {
      if(stl_in.stats.connected_facets_3_edge <  stl_in.stats.number_of_facets)
	{
	  printf("Removing unconnected facets...\n");
	  stl_remove_unconnected_facets(&stl_in);
	}
      else
	printf("No unconnected need to be removed.\n");
    }
  
  if(fill_holes_flag || fixall_flag)
    {
      if(stl_in.stats.connected_facets_3_edge <  stl_in.stats.number_of_facets)
	{
	  printf("Filling holes...\n");
	  stl_fill_holes(&stl_in);
	}
      else
	printf("No holes need to be filled.\n");
    }

  if(reverse_all_flag)
    {
      printf("Reversing all facets...\n");
      stl_reverse_all_facets(&stl_in);
    }
  
  if(normal_directions_flag || fixall_flag)
    {
      printf("Checking normal directions...\n");
      stl_fix_normal_directions(&stl_in);
    }
  
  if(normal_values_flag || fixall_flag)
    {
      printf("Checking normal values...\n");
      stl_fix_normal_values(&stl_in);
    }

  /* Always calculate the volume.  It shouldn't take too long */
  printf("Calculating volume...\n");
  stl_calculate_volume(&stl_in);
	
  if(exact_flag)
    {
      printf("Verifying neighbors...\n");
      stl_verify_neighbors(&stl_in);
    }
  
  if(generate_shared_vertices_flag)
    {
      printf("Generating shared vertices...\n");
      stl_generate_shared_vertices(&stl_in);
    }
  
  if(write_off_flag)
    {
      printf("Writing OFF file %s\n", off_name);
      stl_write_off(&stl_in, off_name);
    }

  if(write_dxf_flag)
    {
      printf("Writing DXF file %s\n", dxf_name);
      stl_write_dxf(&stl_in, dxf_name, "Created by ADMesh version 0.95");
    }

  if(write_vrml_flag)
    {
      printf("Writing VRML file %s\n", vrml_name);
      stl_write_vrml(&stl_in, vrml_name);
    }

  if(write_ascii_stl_flag)
    {
      printf("Writing ascii file %s\n", ascii_name);
      stl_write_ascii(&stl_in, ascii_name, 
		      "Processed by ADMesh version 0.95");
    }
  
  if(write_binary_stl_flag)
    {
      printf("Writing binary file %s\n", binary_name);
      stl_write_binary(&stl_in, binary_name,
		       "Processed by ADMesh version 0.95");
    }
  
  if(exact_flag)
    {
      stl_stats_out(&stl_in, stdout, input_file);
    }
  
  stl_close(&stl_in);

  exit(0);
}

static void 
usage(int status, char *program_name)
{
  if(status != 0)
    {
      fprintf(stderr, "Try '%s --help' for more information.\n", program_name);
    }
  else
    {
      printf("\n\
ADMesh version 0.95\n\
Copyright (C) 1995, 1996  Anthony D. Martin\n\
Usage: %s [OPTION]... file\n", program_name);
      printf("\n\
     --x-rotate=angle     Rotate CCW about x-axis by angle degrees\n\
     --y-rotate=angle     Rotate CCW about y-axis by angle degrees\n\
     --z-rotate=angle     Rotate CCW about z-axis by angle degrees\n\
     --xy-mirror          Mirror about the xy plane\n\
     --yz-mirror          Mirror about the yz plane\n\
     --xz-mirror          Mirror about the xz plane\n\
     --scale=factor       Scale the file by factor (multiply by factor)\n\
     --translate=x,y,z    Translate the file to x, y, and z\n\
     --merge=name         Merge file called name with input file\n\
 -e, --exact              Only check for perfectly matched edges\n\
 -n, --nearby             Find and connect nearby facets. Correct bad facets\n\
 -t, --tolerance=tol      Initial tolerance to use for nearby check = tol\n\
 -i, --iterations=i       Number of iterations for nearby check = i\n\
 -m, --increment=inc      Amount to increment tolerance after iteration=inc\n\
 -u, --remove-unconnected Remove facets that have 0 neighbors\n\
 -f, --fill-holes         Add facets to fill holes\n\
 -d, --normal-directions  Check and fix direction of normals(ie cw, ccw)\n\
     --reverse-all        Reverse the directions of all facets and normals\n\
 -v, --normal-values      Check and fix normal values\n\
 -c, --no-check           Don't do any check on input file\n\
 -b, --write-binary-stl=name   Output a binary STL file called name\n\
 -a, --write-ascii-stl=name    Output an ascii STL file called name\n\
     --write-off=name     Output a Geomview OFF format file called name\n\
     --write-dxf=name     Output a DXF format file called name\n\
     --write-vrml=name    Output a VRML format file called name\n\
     --help               Display this help and exit\n\
     --version            Output version information and exit\n\
\n\
The functions are executed in the same order as the options shown here.\n\
So check here to find what happens if, for example, --translate and --merge\n\
options are specified together.  The order of the options specified on the\n\
command line is not important.\n");
    }
  exit(status);
}  
