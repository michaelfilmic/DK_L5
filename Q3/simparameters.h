
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

#define Q3B

#define TOKEN_RESOL 1000.0
//#define D_D_system
//#define exp_slot_time
//#define FAST_RUN
/******************************************************************************/

#ifdef FAST_RUN

#define N_BYTE_COUNT 3000
#define FIXED_PACKET_LENGTH 1E3 /* bits */
#define LINK_BIT_RATE 1E6 /* bits per second */
#define RUNLENGTH 1E1 /* packets */
#define PACKET_ARRIVAL_RATE 300/* packets per second */

#define B_T_SIZE 15 
#define MAX_QUEUE_SIZE 5 
#define CLK_TIC  0.002

/* Comma separated list of random seeds to run. */
#define RANDOM_SEED_LIST 400050636

#else

#define N_BYTE_COUNT 3000
#define FIXED_PACKET_LENGTH 1E3 /* bits */
#define LINK_BIT_RATE 1E6 /* bits per second */
#define RUNLENGTH 1E4 /* packets */
#define PACKET_ARRIVAL_RATE 300, 500, 1000, 1500/* packets per second */

#define B_T_SIZE 15 , 15 , 15 , 15 , 15 
#define MAX_QUEUE_SIZE 5, 10 , 15 , 20, 25 
#define CLK_TIC  0.002, 0.0005

/* Comma separated list of random seeds to run. */
#define RANDOM_SEED_LIST 400050636, 400099173, 225

#endif

#define BLIPRATE (RUNLENGTH/1000)

/******************************************************************************/

#endif /* simparameters.h */
