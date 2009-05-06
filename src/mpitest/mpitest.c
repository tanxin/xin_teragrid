/**
 * MPI test learning codes
 *
 * Copyright 2005-2009 Shin Tan <tanxincn@gmail.com>
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <stdio.h>
#include <mpi.h>

#define n 10

int main(int argc, char **argv){
	double a[n],sum[n];
	int  myid, numprocs, i=0, j;
	int  namelen;
	char processor_name[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	MPI_Get_processor_name(processor_name,&namelen);
	fprintf(stderr,"Process %d on %s\n", myid, processor_name);


	for(i=0;i<=n;i=i+1)
	{
		sum[i]=0;
	};	
	for(i=0;i<n;i=i+1)
	{
		a[i]=i+myid*1.0/10+0.00001;
	};
	i=2;j=3;
	MPI_Alltoall(&a[0],i,MPI_DOUBLE,&sum[0],j,
			MPI_DOUBLE,MPI_COMM_WORLD);
	for(i=0;i<n;i=i+1)
		printf("myid=%d a[%d]=%f sum[%d]=%f\n ",myid,i,a[i],i,sum[i]);

	MPI_Finalize();
	return 0;
}

