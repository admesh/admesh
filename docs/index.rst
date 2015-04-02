.. ADMesh documentation master file, created by
   sphinx-quickstart on Thu Apr  2 19:01:12 2015.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

ADMesh - STL mesh manipulation tool
===================================

Contents
--------

.. toctree::
   :maxdepth: 2

   stl
   cli
   c_api
   python

.. _overview:

Overview
--------

ADMesh is a program for processing triangulated solid meshes. Currently,
ADMesh only reads the STL file format that is used for rapid prototyping
applications, although it can write STL, VRML, OFF, and DXF files.
Additional information regarding the underlying algorithms of ADMesh can be found in
`Anthony Martin's Masters Thesis <http://www.varlog.com/admesh-htm/ADMeshThesis.zip>`_.

ADMesh is written in ANSI C, licensed under GPLv2+. This is documentation for version |release|.

.. _features:

Features
^^^^^^^^

* Read and write binary and ASCII STL files
* Check STL files for flaws (i.e. unconnected facets, bad normals)
* Repair facets by connecting nearby facets that are within a given tolerance
* Fill holes in the mesh by adding facets
* Repair normal directions (i.e. facets should be CCW)
* Repair normal values (i.e. should be perpendicular to facet with length=1)
* Remove degenerate facets (i.e. facets with 2 or more vertices equal)
* Translate in x, y, and z directions
* Rotate about the x, y, and z axes
* Mirror about the xy, yz, and xz planes
* Scale the part by a factor
* Merge 2 STL files into one
* Write an OFF file
* Write a VRML file
* Write a DXF file
* Calculate the volume of a part
* Get other statistics about the mesh
