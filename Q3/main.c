
/*
 * 
 * Simulation_Run of A Single Server Queueing System
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

/*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "output.h"
#include "simparameters.h"
#include "packet_arrival.h"
#include "cleanup_memory.h"
#include "trace.h"
#include "main.h"

/******************************************************************************/

/*
 * main.c declares and creates a new simulation_run with parameters defined in
 * simparameters.h. The code creates a fifo queue and server for the single
 * server queueuing system. It then loops through the list of random number
 * generator seeds defined in simparameters.h, doing a separate simulation_run
 * run for each. To start a run, it schedules the first packet arrival
 * event. When each run is finished, output is printed on the terminal.
 */

int main(void)
{
  Simulation_Run_Ptr simulation_run;
  Simulation_Run_Data data;

  Simulation_Run_Data for_avg_acc;
  Time sim_time;

  /*
   * Declare and initialize our random number generator seeds defined in
   * simparameters.h
   */

  unsigned RANDOM_SEEDS[] = {RANDOM_SEED_LIST, 0};
  double PACKET_ARRIVAL_RATE_LIST[] = {PACKET_ARRIVAL_RATE};
  //int PACKET_LENGTH_LIST[] = {PACKET_LENGTH};
  int N_BYTE_COUNT_LIST[] = {N_BYTE_COUNT};

  int B_t_LIST[] = {B_T_SIZE};
  int B_d_LIST[] = {MAX_QUEUE_SIZE};
  double CLK_TIC_LIST[] = {CLK_TIC};

  unsigned random_seed;

  int size_rand_seed = (sizeof(RANDOM_SEEDS) / sizeof(unsigned)) - 1;
  printf("size_rand_seed = %d \n", size_rand_seed);

  int j;

#ifndef NO_CSV_OUTPUT
  // create a csv file
  FILE *fp;
  char data_set_name[] = "./Q3.csv";
  //file IO

  fp = fopen(data_set_name, "w");
  //cell/element name/type

  fprintf(fp, ("Random Seed,"));
  fprintf(fp, ("Packet arrival count,"));

  fprintf(fp, ("Transmitted packet count ,"));
  fprintf(fp, ("Service Fraction ,"));
  fprintf(fp, ("Arrival rate,"));
  fprintf(fp, ("Mean Delay (msec),"));
  fprintf(fp, ("B_t,"));
  fprintf(fp, ("B_d,"));
  fprintf(fp, ("block_rate,"));
  fprintf(fp, ("output_rate,"));
  fprintf(fp, ("clk_tic,"));

  fprintf(fp, "\n");
  fclose(fp);
#endif

for (int mm = 0; mm < (sizeof(CLK_TIC_LIST) / sizeof(double)); mm++)
{
    for (int i = 0; i < (sizeof(PACKET_ARRIVAL_RATE_LIST) / sizeof(double)); i++)
    {

    for (int k = 0; k < (sizeof(B_t_LIST) / sizeof(int)); k++)
    {

      j = 0;
      random_seed = RANDOM_SEEDS[j];

      for_avg_acc.packet_arrival_rate = 0;
      for_avg_acc.blip_counter = 0;
      for_avg_acc.arrival_count = 0;
      for_avg_acc.number_of_packets_processed = 0;
      for_avg_acc.num_blocked = 0;
      for_avg_acc.accumulated_delay = 0;
      for_avg_acc.random_seed = 0;
      for_avg_acc.block_rate = 0;
      for_avg_acc.output_rate = 0;

      while (random_seed != 0)
      {

        simulation_run = simulation_run_new(); /* Create a new simulation run. */

        /*
         * Set the simulation_run data pointer to our data object.
         */

        simulation_run_attach_data(simulation_run, (void *)&data);

        /* 
         * Initialize the simulation_run data variables, declared in main.h.
         */

        data.packet_arrival_rate = PACKET_ARRIVAL_RATE_LIST[i];

        /*
        for (int m = 0; m < (sizeof(PACKET_LENGTH_LIST) / sizeof(int)); m++)
        {
          data.packet_length_list[m] = PACKET_LENGTH_LIST[m];
        }

        data.n_byte_count = N_BYTE_COUNT_LIST[i];
        
        data.current_byte_count = data.n_byte_count;
        */

        data.clk_tic = CLK_TIC_LIST[mm];
        data.B_t = B_t_LIST[k];
        data.B_d = B_d_LIST[k];
        data.fixed_packet_len = FIXED_PACKET_LENGTH;

        data.blip_counter = 0;
        data.arrival_count = 0;
        data.number_of_packets_processed = 0;
        data.num_blocked = 0;
        data.accumulated_delay = 0.0;
        data.random_seed = random_seed;

        /* 
         * Create the packet buffer and transmission link, declared in main.h.
         */

        data.buffer = fifoqueue_new();
        data.token_buffer = fifoqueue_new();
        data.link = server_new();

        /* 
         * Set the random number generator seed for this run.
         */

        random_generator_initialize(random_seed);

        /* 
         * Schedule the initial packet arrival for the current clock time (= 0).
         */

        schedule_packet_arrival_event(simulation_run,
                                      simulation_run_get_time(simulation_run));
        schedule_slot_event(simulation_run, simulation_run_get_time(simulation_run));

        /* 
         * Execute events until we are finished. 
         */

        while (data.number_of_packets_processed < RUNLENGTH)
        {
          simulation_run_execute_event(simulation_run);
        }

        sim_time = simulation_run_get_time(simulation_run);
        /*
         * Output results and clean up after ourselves.
         */

        output_results(simulation_run);

        for_avg_acc.packet_arrival_rate += data.packet_arrival_rate;
        for_avg_acc.blip_counter += data.blip_counter;
        for_avg_acc.arrival_count += data.arrival_count;
        for_avg_acc.number_of_packets_processed += data.number_of_packets_processed;
        for_avg_acc.accumulated_delay += data.accumulated_delay;
        for_avg_acc.random_seed += data.random_seed;
        for_avg_acc.block_rate += (double)(data.num_blocked)/(double)(data.arrival_count);
        for_avg_acc.output_rate += (double)(data.number_of_packets_processed) / sim_time * ((double)(data.fixed_packet_len) / LINK_BIT_RATE);

        cleanup_memory(simulation_run);

        j++;
        random_seed = RANDOM_SEEDS[j];
      }

      for_avg_acc.packet_arrival_rate /= size_rand_seed;
      for_avg_acc.blip_counter /= size_rand_seed;
      for_avg_acc.arrival_count /= size_rand_seed;
      for_avg_acc.number_of_packets_processed /= size_rand_seed;
      for_avg_acc.accumulated_delay /= size_rand_seed;
      for_avg_acc.random_seed /= size_rand_seed;
      for_avg_acc.block_rate /= size_rand_seed;
      for_avg_acc.output_rate /= size_rand_seed;

#ifndef NO_CSV_OUTPUT
      fp = fopen(data_set_name, "a");
      //cell/element name/type

      //fprintf(fp, ("Random Seed,"));
      fprintf(fp, "%d,", i);

      //fprintf(fp, ("Packet arrival count,"));
      fprintf(fp, "%ld, ", for_avg_acc.arrival_count);

      //fprintf(fp, ("Transmitted packet count ,"));
      fprintf(fp, "%ld,", for_avg_acc.number_of_packets_processed);

      //fprintf(fp, ("Service Fraction ,"));
      fprintf(fp, "%.5f,", (double)for_avg_acc.number_of_packets_processed / for_avg_acc.arrival_count);

      //fprintf(fp, ("Arrival rate,"));
      fprintf(fp, "%.3f, ", (double)for_avg_acc.packet_arrival_rate);

      //fprintf(fp, ("Mean Delay (msec),"));
      fprintf(fp, "%f, ",
              1e3 * for_avg_acc.accumulated_delay / for_avg_acc.number_of_packets_processed);

      //fprintf(fp, ("B_t,"));
      fprintf(fp, "%d, ", data.B_t);

      //fprintf(fp, ("B_d,"));
      fprintf(fp, "%d, ", data.B_d);

      //fprintf(fp, ("block_rate,"));
      fprintf(fp, "%.3f, ", (double)for_avg_acc.block_rate);

      //fprintf(fp, ("output_rate,"));
      fprintf(fp, "%.3f, ", (double)for_avg_acc.output_rate);

      //fprintf(fp, ("clk_tic,"));
      fprintf(fp, "%f, ", (double)data.clk_tic);

      fprintf(fp, "\n");
      fclose(fp);
#endif

      double xmtted_fraction;
      printf("\n");
      printf("avg Random Seed = %d \n", for_avg_acc.random_seed);
      printf("avg Packet arrival count = %ld \n", for_avg_acc.arrival_count);

      xmtted_fraction = (double)for_avg_acc.number_of_packets_processed /
                        for_avg_acc.arrival_count;

      printf("avg Transmitted packet count  = %ld (Service Fraction = %.5f)\n",
             for_avg_acc.number_of_packets_processed, xmtted_fraction);

      printf("avg Arrival rate = %.3f packets/second \n", (double)for_avg_acc.packet_arrival_rate);

      printf("avg Mean Delay (msec) = %f \n",
             1e3 * for_avg_acc.accumulated_delay / for_avg_acc.number_of_packets_processed);

      printf("B_t : %d, ", data.B_t);

      printf("B_d : %d, ", data.B_d);

      printf("block_rate : %.3f, ", (double)for_avg_acc.block_rate);

      printf("output_rate : %.3f, ", (double)for_avg_acc.output_rate);
      printf("sim_time : %.3f, ", sim_time);
      printf("ratio between a packet time and clk tic: %.3f, ", ((double)(data.fixed_packet_len) / LINK_BIT_RATE)/ CLK_TIC_LIST[mm]);
      printf("\n");
    }
  }
  }

  //getchar();   /* Pause before finishing. */
  return 0;
}
