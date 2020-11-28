
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

/******************************************************************************/

#include <assert.h>
#include "trace.h"
#include <math.h>
#include <stdio.h>
#include "main.h"
#include "packet_transmission.h"
#include "packet_arrival.h"

/******************************************************************************/

/*
 * This function will schedule a packet arrival at a time given by
 * event_time. At that time the function "packet_arrival" (located in
 * packet_arrival.c) is executed. An object can be attached to the event and
 * can be recovered in packet_arrival.c.
 */

long int
schedule_packet_arrival_event(Simulation_Run_Ptr simulation_run,
			      double event_time)
{
  Event event;

  event.description = "Packet Arrival";
  event.function = packet_arrival_event;
  event.attachment = (void *) NULL;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

/*******************************************************************************/

long int
schedule_slot_event(Simulation_Run_Ptr simulation_run,
			      Time event_time)
{
  Event event;

  event.description = "Slot";
  event.function = slot_event;
  event.attachment = NULL;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

/******************************************************************************/

/*
 * This is the event function which is executed when a packet arrival event
 * occurs. It creates a new packet object and places it in either the fifo
 * queue if the server is busy. Otherwise it starts the transmission of the
 * packet. It then schedules the next packet arrival event.
 */

void
packet_arrival_event(Simulation_Run_Ptr simulation_run, void * ptr)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr new_packet;

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);
  data->arrival_count++;

  new_packet = (Packet_Ptr) xmalloc(sizeof(Packet));
  new_packet->arrive_time = simulation_run_get_time(simulation_run);

  /*
  int index = (int) uniform_generator() * 5;
  int packet_length = data->packet_length_list[index];
  */
  double transmission_time = (double) data->fixed_paket_len/LINK_BIT_RATE;
 
  new_packet->service_time = transmission_time;
  new_packet->status = WAITING;

  /* 
   * Start transmission if the data link is free. Otherwise put the packet into
   * the buffer.
   */
#ifndef ASSERT_OFF
  assert(fifoqueue_size(data->token_buffer) >= 0);
#endif

  if(server_state(data->link) == BUSY) {
      if (fifoqueue_size(data->buffer) < data->B_d)
      {
        fifoqueue_put(data->buffer, (void*) new_packet);
      }
      else
      {
        TRACE(printf("data fifo is full size %d \n",fifoqueue_size(data->buffer)););
      }
  } 
  else if (fifoqueue_size(data->token_buffer) > 0 ){

    TRACE(printf("in arrival token fifo size %d \n",fifoqueue_size(data->token_buffer)););
    
    fifoqueue_get(data->token_buffer);
    
    start_transmission_on_link(simulation_run, new_packet, data->link);
  }
  else
  {
      if (fifoqueue_size(data->buffer) < data->B_d)
      {
        fifoqueue_put(data->buffer, (void*) new_packet);
      }
      else
      {
        TRACE(printf("data fifo is full size %d \n",fifoqueue_size(data->buffer)););
      }
  }

  /* 
   * Schedule the next packet arrival. Independent, exponentially distributed
   * interarrival times gives us Poisson process arrivals.
   */

#ifdef D_D_system
  schedule_packet_arrival_event(simulation_run,
			simulation_run_get_time(simulation_run) +
			(double) 1/data->packet_arrival_rate);
#else
  schedule_packet_arrival_event(simulation_run,
			simulation_run_get_time(simulation_run) +
			exponential_generator((double) 1/data->packet_arrival_rate));
#endif
}

/******************************************************************************/
void
slot_event(Simulation_Run_Ptr simulation_run, void* dummy_ptr) 
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr token;
  Packet_Ptr data_packet;
  Time now;

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

  token = (Packet_Ptr) xmalloc(sizeof(Packet));
  token->arrive_time = -1;

  token->service_time = -1;
  token->status = WAITING;

  now = simulation_run_get_time(simulation_run);

  if (fifoqueue_size(data->token_buffer) < data->B_t)
  {
      fifoqueue_put(data->token_buffer, (void*) token);
  }
  else
  {
    TRACE(printf("token fifo is full with size %d \n",fifoqueue_size(data->token_buffer)););

  }
  
  if(server_state(data->link) == FREE) {
      if (fifoqueue_size(data->buffer) > 0 ){

        data_packet = (Packet_Ptr) fifoqueue_get(data->buffer);

        TRACE(printf("not enough token case, data fifo size %d \n",fifoqueue_size(data->token_buffer)););
    
        fifoqueue_get(data->token_buffer);
    
        start_transmission_on_link(simulation_run, data_packet, data->link);
      }
  }
 

  /*
  //update n
  data->current_byte_count = data->n_byte_count;

  next_packet = (Packet_Ptr) fifoqueue_see_front(data->buffer);
  if (next_packet->packet_size <= data->current_byte_count){
    next_packet = (Packet_Ptr) fifoqueue_get(data->buffer);
    start_transmission_on_link(simulation_run, next_packet, data->link);
  }
  */

  schedule_slot_event(simulation_run, data->clk_tic + now);
}



