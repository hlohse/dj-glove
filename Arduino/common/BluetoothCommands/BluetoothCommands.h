/*      BluetoothCommands
 *  This file provides several variables describing what commands can be send
 *  to the Bluetooth module and what they do.
 *  The commands array provides a set of all commands for iteration or similar.
 *  IsValidCommand checks whether a given string corresponds to a command.
 */

#include <Arduino.h>

namespace BluetoothCommands {

typedef struct Command {
    String command;
    String description;

    Command(const String& command, const String& description);
} Command;

bool IsValidCommand(const String& command);

const int num_commands = 12;
extern Command* commands[num_commands];

extern Command verify;
extern Command get_version;
extern Command set_name;
extern Command set_pin;
extern Command set_baud_1200;
extern Command set_baud_2400;
extern Command set_baud_4800;
extern Command set_baud_9600;
extern Command set_baud_19200;
extern Command set_baud_38400;
extern Command set_baud_57600;
extern Command set_baud_115200;

}

