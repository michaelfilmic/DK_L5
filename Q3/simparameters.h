
/*
 * 
 * Simulation of A Single Server Queueing System
 * 
 * Copyright (C) 2014 Terence D. Todd Hamilton, Ontario, CANADA,
 * todd@mcmaster.ca
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/******************************************************************************/

#ifndef _SIMPARAMETERS_H_
#define _SIMPARAMETERS_H_

//#define D_D_system
//#define FAST_RUN
/******************************************************************************/

#ifdef FAST_RUN

#define N_BYTE_COUNT 3000
#define FIXED_PACKET_LENGTH 1E3 /* bits */
#define LINK_BIT_RATE 1E6 /* bits per second */
#define RUNLENGTH 2 /* packets */
#define PACKET_ARRIVAL_RATE 120 /* packets per second */

#define B_T_SIZE 10, 20
#define MAX_QUEUE_SIZE 20, 30 //B_d
#define CLK_TIC 0.01

/* Comma separated list of random seeds to run. */
//#define RANDOM_SEED_LIST 400050636, 400099173, 225
#define RANDOM_SEED_LIST 400050636

#else

#define N_BYTE_COUNT 3000
#define FIXED_PACKET_LENGTH 1E3 /* bits */
#define LINK_BIT_RATE 1E6 /* bits per second */
#define RUNLENGTH 1E3 /* packets */
#define PACKET_ARRIVAL_RATE 1, 5, 10, 20, 50, 70, 90, 120 /* packets per second */

#define B_T_SIZE 5*1 , 5*2 , 5*3 , 5*4 
#define MAX_QUEUE_SIZE 5*1 , 5*1 , 5*1 , 5*1  //B_d
#define CLK_TIC 0.01, 0.01, 0.01, 0.01

/* Comma separated list of random seeds to run. */
#define RANDOM_SEED_LIST 400050636, 400099173, 225
//#define RANDOM_SEED_LIST 400050636

#endif

#define PACKET_XMT_TIME ((double) PACKET_LENGTH/LINK_BIT_RATE)
#define BLIPRATE (RUNLENGTH/1000)

/******************************************************************************/

#endif /* simparameters.h */
