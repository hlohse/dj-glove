/*      BtLatency
 *  This file provides configuration for BtLatency PC project.
 *
 *  NOTE: USE THIS VALUES IN ARDUINO PROJECT, TOO!
 */

#ifndef DJ_GLOVE_PC_BT_LATENCY_H_
#define DJ_GLOVE_PC_BT_LATENCY_H_

namespace BtLatency {

const int  num_delays_ms     = 6;
const int  num_message_sizes = 5;

const int  delays_ms[num_delays_ms]         = {0, 1, 2, 4, 8, 16};
const int  message_sizes[num_message_sizes] = {1, 2, 4, 8, 16};

const int  tries_per_message = 26;
const char message_char      = 'M';
const char start_signal      = '!';

}

#endif // DJ_GLOVE_PC_BT_LATENCY_H_

