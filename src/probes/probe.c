/**
 * Probe for teragrid
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

#include <sys/sysinfo.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <stdint.h>


#define CPUFILE "/proc/cpuinfo"
#define BUFFERLEN 1024

void show_speed(){
	FILE *cpuinfo;
	uint64_t procspeed = 1;
	char buf[BUFFERLEN];
	float st;
	int found = 0;
	int index = 0;

	if ((cpuinfo = fopen("/proc/cpuinfo","r")) == NULL)
		perror ("get_procspeed: fopen");

	while (!(found || feof (cpuinfo))) {
		fgets (buf,BUFFERLEN-1,cpuinfo);
		if (strstr(buf,"cpu MHz") != NULL) {
			while (!isdigit(buf[index]))
				index++;
			sscanf (&buf[index],"%f\n",&st);
			procspeed = (int) st;
			found = 1;
		} else if (strstr(buf,"clock") != NULL) {
			while (!isdigit(buf[index]))
				index++;
			sscanf (&buf[index],"%fMHz\n",&st);
			procspeed = (int) st;
			found = 1;
		} else if (strstr(buf,"ClkTck") != NULL) {
			uint64_t clockspeed = 0;

			while (!isdigit(buf[index]))
				index++;
			index++;

			while (!isdigit(buf[index]))
				index++;
			clockspeed = strtoll (&buf[index],NULL,16);
			procspeed = (uint32_t)(clockspeed / 1e6);
			found = 1;
		}
	}

	printf("speed %lld\n", (unsigned long long) procspeed);
}

void show_sysinfo(){
	struct sysinfo info;
	int errno;

	// check memory
	errno = sysinfo(&info);

	printf("error: %d\n", errno);
	printf("totalram: %luM\n", info.totalram >> 20);
	printf("freeram: %luM\n", info.freeram >> 20);
	printf("totalswap: %luM\n", info.totalswap >> 20);
	printf("freeswap: %luM\n", info.freeswap >> 20);

}


void show_numproc (void) {
	FILE *cpuinfo;
	int numproc = 0;
	char buf[BUFFERLEN];
	int index = 0;

	if ((cpuinfo = fopen("/proc/cpuinfo","r")) == NULL)
		perror ("get_numproc: fopen");

	while (!feof (cpuinfo)) {
		fgets (buf,BUFFERLEN-1,cpuinfo);
		if (strstr(buf,"ncpus active") != NULL) {
			while (!isdigit(buf[index]))
				index++;
			sscanf (&buf[index],"%i\n",&numproc);
			break;
		} else if (strcasestr(buf,"BogoMIPS") != NULL) {
			numproc++;
		}
	}

	fclose (cpuinfo);

	printf("num: %d\n", numproc);
}


int main(int argc, char **argv){

	show_sysinfo();
	show_speed();
	show_numproc();
	return 0;
}

