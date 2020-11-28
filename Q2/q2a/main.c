
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

  /*
   * Declare and initialize our random number generator seeds defined in
   * simparameters.h
   */

  unsigned RANDOM_SEEDS[] = {RANDOM_SEED_LIST, 0};
  int PACKET_LENGTH_LIST[] = {PACKET_LENGTH};
  double CLK_TIC_LIST[] = {CLK_TIC};
  int N_BYTE_COUNT_LIST[] = {N_BYTE_COUNT};
  unsigned random_seed;

  int size_rand_seed = (sizeof(RANDOM_SEEDS) / sizeof(unsigned)) - 1;

  // create a csv file
  FILE *fp;
  char data_set_name[] = "./Q2b.csv";
  //file IO

  fp = fopen(data_set_name, "w");
  //cell/element name/type
  fprintf(fp, ("n,"));
  fprintf(fp, ("Packet arrival count,"));
  fprintf(fp, ("Blocking rate,"));
  fprintf(fp, ("Total bits (Mbits),"));
  fprintf(fp, ("Output rate (Mbps),"));
  fprintf(fp, ("Transmitted packet count ,"));
  fprintf(fp, ("Service Fraction ,"));
  fprintf(fp, ("Arrival rate,"));
  fprintf(fp, ("Mean Delay (msec),"));

  fprintf(fp, "\n");
  fclose(fp);

  for (int k = 0; k < (sizeof(CLK_TIC_LIST) / sizeof(double)); k++)
  {

    for (int i = 0; i < (sizeof(N_BYTE_COUNT_LIST) / sizeof(int)); i++)
    {

      int j = 0;
      random_seed = RANDOM_SEEDS[j];

      for_avg_acc.packet_arrival_rate = 0;
      for_avg_acc.blip_counter = 0;
      for_avg_acc.arrival_count = 0;
      for_avg_acc.number_of_packets_processed = 0;
      for_avg_acc.block_count = 0;
      for_avg_acc.total_bits = 0.0;
      for_avg_acc.output_rate = 0.0;
      for_avg_acc.accumulated_delay = 0;
      for_avg_acc.random_seed = 0;

      while ((random_seed = RANDOM_SEEDS[j++]) != 0)
      {
        simulation_run = simulation_run_new(); /* Create a new simulation run. */

        /*
         * Set the simulation_run data pointer to our data object.
         */

        simulation_run_attach_data(simulation_run, (void *)&data);

        /* 
         * Initialize the simulation_run data variables, declared in main.h.
         */

        data.packet_arrival_rate = PACKET_ARRIVAL_RATE;
        for (int m = 0; m < (sizeof(PACKET_LENGTH_LIST) / sizeof(int)); m++)
        {
          data.packet_length_list[m] = PACKET_LENGTH_LIST[m];
          data.packet_count[m] = 0;
        }
        data.n_byte_count = N_BYTE_COUNT_LIST[i];
        data.current_byte_count = data.n_byte_count;
        data.clk_tic = CLK_TIC_LIST[k];
        data.current_slot_end_time = 0;
        data.block_count = 0;
        data.blip_counter = 0;
        data.arrival_count = 0;
        data.number_of_packets_processed = 0;
        data.accumulated_delay = 0.0;
        data.output_rate = 0.0;
        data.end_time = 0.0;
        data.random_seed = random_seed;

        /* 
         * Create the packet buffer and transmission link, declared in main.h.
         */

        data.buffer = fifoqueue_new();
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
        /*
         * Output results and clean up after ourselves.
         */

        output_results(simulation_run);

        double output_rate = 0.0;
        double total_bits = 0.0;
        for(int n = 0; n<sizeof(data.packet_count)/sizeof(int); n++){
          printf("packet %d, count %d \n", n, data.packet_count[n]);
          total_bits += (data.packet_count[n] / 1e6) * PACKET_LENGTH_LIST[n]; //Mbits
        }
        output_rate = total_bits / data.end_time;
        printf("simtime = %f \n", data.end_time);
        for_avg_acc.packet_arrival_rate += data.packet_arrival_rate;
        for_avg_acc.blip_counter += data.blip_counter;
        for_avg_acc.block_count += data.block_count;
        for_avg_acc.total_bits += total_bits;
        for_avg_acc.output_rate += output_rate;
        for_avg_acc.arrival_count += data.arrival_count;
        for_avg_acc.number_of_packets_processed += data.number_of_packets_processed;
        for_avg_acc.accumulated_delay += data.accumulated_delay;
        for_avg_acc.random_seed += data.random_seed;

        cleanup_memory(simulation_run);

        random_seed = RANDOM_SEEDS[j];
      }

      for_avg_acc.packet_arrival_rate /= size_rand_seed;
      for_avg_acc.blip_counter /= size_rand_seed;
      for_avg_acc.arrival_count /= size_rand_seed;
      for_avg_acc.block_count /= size_rand_seed;
      for_avg_acc.total_bits /= size_rand_seed;
      for_avg_acc.output_rate /= size_rand_seed;
      for_avg_acc.number_of_packets_processed /= size_rand_seed;
      for_avg_acc.accumulated_delay /= size_rand_seed;
      for_avg_acc.random_seed /= size_rand_seed;

      fp = fopen(data_set_name, "a");
      //cell/element name/type

      //fprintf(fp, ("n,"));
      fprintf(fp, "%d,", N_BYTE_COUNT_LIST[i]);

      //fprintf(fp, ("Packet arrival count,"));
      fprintf(fp, "%ld, ", for_avg_acc.arrival_count);

      //fprintf(fp, ("Blocking rate,"));
      fprintf(fp, "%f, ", (double)for_avg_acc.block_count / for_avg_acc.arrival_count);

      //fprintf(fp, ("Total bits (Mbits),"));
      fprintf(fp, "%f, ", for_avg_acc.total_bits);

      //fprintf(fp, ("Output rate (Mbps),"));
      fprintf(fp, "%f, ", for_avg_acc.output_rate);

      //fprintf(fp, ("Transmitted packet count ,"));
      fprintf(fp, "%ld,", for_avg_acc.number_of_packets_processed);

      //fprintf(fp, ("Service Fraction ,"));
      fprintf(fp, "%.5f,", (double)for_avg_acc.number_of_packets_processed / for_avg_acc.arrival_count);

      //fprintf(fp, ("Arrival rate,"));
      fprintf(fp, "%.3f, ", (double)for_avg_acc.packet_arrival_rate);

      //fprintf(fp, ("Mean Delay (msec),"));
      fprintf(fp, "%f, ",
              1e3 * for_avg_acc.accumulated_delay / for_avg_acc.number_of_packets_processed);

      fprintf(fp, "\n");
      fclose(fp);

      double xmtted_fraction;
      printf("\n");
      printf("avg Random Seed = %d \n", for_avg_acc.random_seed);
      printf("avg Packet arrival count = %ld \n", for_avg_acc.arrival_count);
      printf("block rate = %f \n", (double)for_avg_acc.block_count / for_avg_acc.arrival_count);
      xmtted_fraction = (double)for_avg_acc.number_of_packets_processed /
                        for_avg_acc.arrival_count;

      printf("total bits (Mbits) = %f \n", for_avg_acc.total_bits);

      printf("output rate (Mbps) = %f\n", for_avg_acc.output_rate);

      printf("avg Transmitted packet count  = %ld (Service Fraction = %.5f)\n",
             for_avg_acc.number_of_packets_processed, xmtted_fraction);

      printf("avg Arrival rate = %.3f packets/second \n", (double)for_avg_acc.packet_arrival_rate);

      printf("avg Mean Delay (msec) = %f \n",
             1e3 * for_avg_acc.accumulated_delay / for_avg_acc.number_of_packets_processed);

      printf("\n");

    }
  }

  return 0;
}
