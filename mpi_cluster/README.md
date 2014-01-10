cirrus
======

These programs were designed to run on a cluster, I believe you can run them locally by using openMPI. These come with no warranty, use as is. I won't provide support.

Some people may find the MPI code interesting, shows how to use Makefiles to compile C and use Cunit as unit testing. I personally liked this course quite a bit.

In order to setup Ubuntu, see this: http://jetcracker.wordpress.com/2012/03/01/how-to-install-mpi-in-ubuntu/
Use mpirun instead of bsub to run these on a local machine without cluster.

a1: Implements a dart algorithm that uses a brute force approach to calculate pi.
    This isn't an efficient way to calc pi, just busy work for first project.
    Works as expected.

a2: Implements a hypercube quicksort algorithm, where we speed up slightly over quicksort
    by distributing work across n nodes.
    Works fine.

a3: An attempted implementation of floyd's all pairs, didn't finish it though.
    The serial version works, but the cluster one doesn't and needs work.
    I started a little too late on this.

Other folders are largely irrelevant, they were just scratch code for the main work.
