#include "BluetoothCommands.h"
using namespace BluetoothCommands;

Command::Command(const String& command, const String& description)
:   command(command), description(description)
{
}

bool BluetoothCommands::IsValidCommand(const String& command)
{
    for (int i = 0; i < num_commands; i++) {
        if (command == commands[i]->command) {
            return true;
        }
    }

    return false;
}

Command* BluetoothCommands::commands[num_commands] = {
    &BluetoothCommands::verify,
    &BluetoothCommands::get_version,
    &BluetoothCommands::set_name,
    &BluetoothCommands::set_pin,
    &BluetoothCommands::set_baud_1200,
    &BluetoothCommands::set_baud_2400,
    &BluetoothCommands::set_baud_4800,
    &BluetoothCommands::set_baud_9600,
    &BluetoothCommands::set_baud_19200,
    &BluetoothCommands::set_baud_38400,
    &BluetoothCommands::set_baud_57600,
    &BluetoothCommands::set_baud_115200
};

Command BluetoothCommands::verify(         "AT",         "Verifies communication");
Command BluetoothCommands::get_version(    "AT+VERSION", "Displays firmware version"); 
Command BluetoothCommands::set_name(       "AT+NAMExyz", "Sets the module name to \"xyz\"");
Command BluetoothCommands::set_pin(        "AT+PIN1234", "Sets the module PIN to 1234");
Command BluetoothCommands::set_baud_1200(  "AT+BAUD1",   "Sets the baud rate to 1200");
Command BluetoothCommands::set_baud_2400(  "AT+BAUD2",   "Sets the baud rate to 2400");
Command BluetoothCommands::set_baud_4800(  "AT+BAUD3",   "Sets the baud rate to 4800");
Command BluetoothCommands::set_baud_9600(  "AT+BAUD4",   "Sets the baud rate to 9600");
Command BluetoothCommands::set_baud_19200( "AT+BAUD5",   "Sets the baud rate to 19200");
Command BluetoothCommands::set_baud_38400( "AT+BAUD6",   "Sets the baud rate to 38400");
Command BluetoothCommands::set_baud_57600( "AT+BAUD7",   "Sets the baud rate to 57600");
Command BluetoothCommands::set_baud_115200("AT+BAUD8",   "Sets the baud rate to 115200");

