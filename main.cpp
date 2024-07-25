#include "ambiLight/Ambilight.h"
#include "soundLight/SoundLight.h"
#include "parseur/ParserOption.h"
#include <iostream>

std::string PROGNAME = "AmbilightSoundWLED";
std::string FILE_NAME = __FILE__;
std::string RELEASE = "Revision 1.0 | Last update 25 july 2024";
std::string AUTHOR = "\033[1mJérémie Lohyer\033[0m";
std::string COPYRIGHT = "(c) 2024 " + AUTHOR;

auto print_release = []
{
    std::cout << RELEASE << '\n'
              << COPYRIGHT << '\n';
};

auto failure = [](std::string_view message)
{
    std::cerr << "❌ Error: " << message << " ❌\n";
};

auto print_usage = []()
{
    std::cout << std::endl
              << PROGNAME << " by " << AUTHOR << std::endl
              << "\033[1mUsage: \033[0m" << PROGNAME << " | [-h | --help] | [-v | --version] | [ -s | --sound]" << std::endl
              << "          -h            help" << std::endl
              << "          -v            Version" << std::endl
              << "          -s | --sound           Ambilight using the sound as input" << std::endl
              << "          -l | --light           Ambilight using the screen as input (by default)" << std::endl
              << "          -f | --fourrier        Display Fourrier decomposition values" << std::endl
              << "          -L | --length          Define the length of your LED strip" << std::endl
              << "          -r | --reverse         For sound, going from the edges to the centers" << std::endl
              << "          -S | --smooth          Smooth update of the LED (default 5)" << std::endl
              << "          -i | --ip              Define x in 192.168.1.x" << std::endl; // TODO: later support full customization
};

int main(int argc, char **argv)
{
    bool light = true;
    bool fourrier = false;
    int stripSize = 458;
    int stripIp = 5;
    bool stripReverse = false;
    int stripAmortissement = 5;

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
        {
            print_usage();
            exit(0);
        }
        else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version"))
        {
            print_release();
            exit(0);
        }
        else if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--sound"))
        {
            light = false;
        }
        else if (!strcmp(argv[i], "-l") || !strcmp(argv[i], "--light"))
        {
            light = true;
        }
        else if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "--fourrier"))
        {
            fourrier = true;
        }
        else if (!strcmp(argv[i], "-L") || !strcmp(argv[i], "--length"))
        {
            std::string value = agrv[++i];
            stripSize = stoi(value);
        }
        else if (!strcmp(argv[i], "-r") || !strcmp(argv[i], "--reverse"))
        {
            stripReverse = true;
        }
        else if (!strcmp(argv[i], "-S") || !strcmp(argv[i], "--smooth"))
        {
            std::string value = agrv[++i];
            stripAmortissement = stoi(value);
        }
        else if (!strcmp(argv[i], "-i") || !strcmp(argv[i], "--ip"))
        {
            std::string value = agrv[++i];
            stripIp = stoi(value);
        }
    }

    // Initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    if (light)
    {
        Ambilight ambilight;
        ambilight.start();
    }
    else
    {
        std::cout << "ambilight with stripSize = " << stripSize << " and ip = 192.168.1." << stripIp << std::endl;
        SoundLight soundLight = SoundLight(stripSize, stripIp, stripReverse, stripAmortissement);
        soundLight.Start(fourrier);
    }

    WSACleanup(); // Nettoyez Winsock avant de quitter
    return 0;
}