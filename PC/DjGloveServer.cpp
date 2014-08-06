#include "LibDjGlove.h"
#include <iostream>
#include <string>
#include <boost/program_options.hpp>
using namespace std;
namespace po = boost::program_options;

int main(int argc, char* argv[])
{
    string arduino_mac;
    string midi_port_name;
    int timeout_s = 0;

    po::options_description options_description("DJ Glove Server Options");
    po::variables_map options;

    options_description.add_options()
        ("help",           "Show options")
        ("arduino-mac",    po::value<std::string>(&arduino_mac),    "Arduino's Bluetooth MAC address")
        ("midi-port-name", po::value<std::string>(&midi_port_name), "Virtual MIDI port's name")
        ("timeout",        po::value<int>(&timeout_s)->required(),  "Bluetooth connection timeout [s]")
    ;

    po::store(po::parse_command_line(argc, argv, options_description), options);
    po::notify(options);

    if (options.count("help")) {
        cout << options_description << endl;
        return 1;
    }
    
    return 0;
}

