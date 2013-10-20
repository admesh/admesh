                     ADMesh version 0.95
                    ********************

ADMesh is a program for processing triangulated solid meshes. Currently,
ADMesh only reads the STL file format that is used for rapid prototyping
applications, although it can write STL, VRML, OFF, and DXF files.

Features:
---------

 * Read and write binary and ASCII STL files
 * Check STL files for flaws (i.e. unconnected facets, bad normals)
 * Repair facets by connecting nearby facets that are within a given tolerance
 * Fill holes in the mesh by adding facets.
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

 ** ADMesh outputs the following statistics after processing:

================= Results produced by ADMesh version 0.95 =================
Input file         : sphere.stl
File type          : Binary STL file
Header             : Processed by ADMesh version 0.95
============== Size ==============
Min X = -1.334557, Max X = 1.370952
Min Y = -1.377953, Max Y = 1.377230
Min Z = -1.373225, Max Z = 1.242838
========= Facet Status ========== Original ============ Final ====
Number of facets                 :  3656                3656
Facets with 1 disconnected edge  :    18                   0
Facets with 2 disconnected edges :     3                   0
Facets with 3 disconnected edges :     0                   0
Total disconnected facets        :    21                   0
=== Processing Statistics ===     ===== Other Statistics =====
Number of parts       :     1        Volume   :  10.889216
Degenerate facets     :     0
Edges fixed           :    24
Facets removed        :     0
Facets added          :     0
Facets reversed       :     0
Backwards edges       :     0
Normals fixed         :     0

There are two different algorithms used for fixing unconnected facets.  The
first algorithm finds an unconnected edge, and then checks nearby within a
given tolerance for another unconnected edge.  It then fixes edges within
tolerance.  Some meshes can be completely fixed just using this method.  If
there are still unconnected facets after this "nearby check" has been done,
then a second algorithm is used.  This algorithm just fills any holes in the
mesh by adding facets until all of the holes are filled.  Using these two
algorithms, almost any imperfect STL file can be "fixed" 100% so that there
are 0 unconnected facets.  Whether the resulting mesh is what you really
want is another question since there is no way for ADMesh to add information
that isn't there.

At this point ADMesh is only command-line driven and has no windowing
capabilities.  This should make it extremely easy to port to any UNIX-like
system, and it shouldn't have any problems compiling on Windows NT, and some
people have had success compiling it under DOS or Windows using DJGPP.

ADMesh was developed on a 486/66 with 16Mb running the Linux operating system.
It has also been compiled and run on the following systems:
SunOS 4.1.3
IRIX  5.2
Please let me know about successes or failures with other systems.

On my Linux system with 16Mb of memory, I can easily process files that have
up to about 200,000 facets.  Files larger than this can be processed, but
the system begins to slow down significantly due to swapping.  A system with
more memory will be able to process greater numbers of facets more easily.
Following are some indications of process times:
40,000  facets: 10 seconds
80,000  facets: 20 seconds
160,000 facets: 50 seconds
320,000 facets: 13 minutes (heavy swapping occurred)

There are a few features that are "missing" from ADMesh but that might be
added in the future.  These include:
 * Display capability
 * Slicing
 * Splitting a file into smaller files

ADMesh is free but Copyrighted software.  It is distributed under the terms
of the GNU General Public License (GPL).  It is NOT in the public domain and
its source code cannot be used in commercial software.  Details of the GPL
are in the file COPYING that comes with the ADMesh software package.

ADMesh can be obtained via http from:
  http://www.engr.csulb.edu/~amartin/admesh/admesh-0.95.tar.gz
or visit my home page at:
  http://www.engr.csulb.edu/~amartin

Send questions, comments, suggestions, etc. to me at the following address.
<amartin@engr.csulb.edu>
Anthony D. Martin
