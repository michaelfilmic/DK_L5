
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

#include <stdio.h>
#include "trace.h"
#include "main.h"
#include "output.h"
#include "packet_transmission.h"

/******************************************************************************/

/*
 * This function will schedule the end of a packet transmission at a time given
 * by event_time. At that time the function "end_packet_transmission" (defined
 * in packet_transmissionl.c) is executed. A packet object is attached to the
 * event and is recovered in end_packet_transmission.c.
 */

long schedule_end_packet_transmission_event(Simulation_Run_Ptr simulation_run,
                                            double event_time,
                                            Server_Ptr link)
{
  Event event;

  event.description = "Packet Xmt End";
  event.function = end_packet_transmission_event;
  event.attachment = (void *)link;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

/******************************************************************************/

/*
 * This is the event function which is executed when the end of a packet
 * transmission event occurs. It updates its collected data then checks to see
 * if there are other packets waiting in the fifo queue. If that is the case it
 * starts the transmission of the next packet.
 */

void end_packet_transmission_event(Simulation_Run_Ptr simulation_run, void *link)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr this_packet, next_packet;

  TRACE(printf("End Of Packet.\n"););

  data = (Simulation_Run_Data_Ptr)simulation_run_data(simulation_run);

  /* 
   * Packet transmission is finished. Take the packet off the data link.
   */

  this_packet = (Packet_Ptr)server_get(link);

  data->end_time = simulation_run_get_time(simulation_run);

  /* Collect statistics. */
  data->number_of_packets_processed++;
  data->accumulated_delay += simulation_run_get_time(simulation_run) -
                             this_packet->arrive_time;
  int index = 0;
  for(int i = 0; i<sizeof(data->packet_length_list)/sizeof(int); i++){
    if (this_packet->packet_size == data->packet_length_list[i]){
      index = i;
      //printf("index %d\n", index);
      break;
    }
  }
  data->packet_count[index]++;

  /* Output activity blip every so often. */
  //output_progress_msg_to_screen(simulation_run);

  /* This packet is done ... give the memory back. */
  xfree((void *)this_packet);

  /* 
   * See if there is are packets waiting in the buffer. If so, take the next one
   * out and transmit it immediately.
  */
  //next_packet = (Packet_Ptr) fifoqueue_see_front(data->buffer);
  if (fifoqueue_size(data->buffer) > 0)
  {
    next_packet = (Packet_Ptr)fifoqueue_see_front(data->buffer);

    if (next_packet->packet_size <= data->current_byte_count)
    {
      next_packet = (Packet_Ptr)fifoqueue_get(data->buffer);
      data->current_byte_count -= next_packet->packet_size;

      start_transmission_on_link(simulation_run, next_packet, data->link);
    }
  }
}

/*
 * This function ititiates the transmission of the packet passed to the
 * function. This is done by placing the packet in the server. The packet
 * transmission end event for this packet is then scheduled.
 */

void start_transmission_on_link(Simulation_Run_Ptr simulation_run,
                                Packet_Ptr this_packet,
                                Server_Ptr link)
{
  TRACE(printf("Start Of Packet.\n");)
  
  server_put(link, (void *)this_packet);
  this_packet->status = XMTTING;

  /* Schedule the end of packet transmission event. */
  schedule_end_packet_transmission_event(simulation_run,
                                         simulation_run_get_time(simulation_run) + this_packet->service_time,
                                         (void *)link);
}
