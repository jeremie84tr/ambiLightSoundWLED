#include "ambiLight/Ambilight.h"
#include "soundLight/SoundLight.h"
#include "parseur/ParserOption.h"
#include <iostream>

int main(int argc, char** argv) {
    bool light = true;
    bool fourrier = false;
    int stripSize = 458;
    int stripIp = 5;
    bool stripReverse = false;
    int stripAmortissement = 5;

    for (int argIndex = 0; argIndex < argc; ++argIndex) {
        char character = argv[argIndex][0];
        int charIndice = 0;
        while (character != '\0') {
            if (character == '-') {
                std::cout << "option" << std::endl;
                character = argv[argIndex][++charIndice];
                if (character == 's') {
                    std::cout << "sound" << std::endl;
                    light = false;
                } else if (character == 'l') {
                    std::cout << "light" << std::endl;
                    light = true;
                } else if (character == 'f') {
                    std::cout << "fourrier" << std::endl;
                    fourrier = true;
                } else if (character == '-') {
                    ParserOption ip = ParserOption("ip");
                    ParserOption size = ParserOption("size");
                    ParserOption reverse = ParserOption("reverse");
                    ParserOption amortissement = ParserOption("amortissement");
                    bool end = false;
                    while (!end) {
                        character = argv[argIndex][++charIndice];
                        end = true;
                        switch (size.parseOptionName(character)) {
                            case parseOk:
                                argIndex++;
                                stripSize = ParserOption::parseInt(argv[argIndex]);
                            character = '\0';
                            break;
                            case parseStop:
                                break;
                            default:
                                end = false;
                            break;
                        }
                        switch (ip.parseOptionName(character)) {
                            case parseOk:
                                argIndex++;
                                stripIp = ParserOption::parseInt(argv[argIndex]);
                            character = '\0';
                            break;
                            case parseStop:
                                break;
                            default:
                                end = false;
                            break;
                        }
                        switch (reverse.parseOptionName(character)) {
                            case parseOk:
                                stripReverse = true;
                                character = '\0';
                                break;
                            case parseStop:
                                break;
                            default:
                                end = false;
                                break;
                        }
                        switch (amortissement.parseOptionName(character)) {
                            case parseOk:
                                argIndex++;
                                stripAmortissement = ParserOption::parseInt(argv[argIndex]);
                                character = '\0';
                                break;
                            case parseStop:
                                break;
                            default:
                                end = false;
                                break;
                        }
                    }
                } else {
                    std::cout << "DK" << std::endl;
                }
            } else {
                character = argv[argIndex][++charIndice];
            }

        }
    }

    // Initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    if (light) {
        Ambilight ambilight;
        ambilight.start();
    } else {
        std::cout << "ambilight with stripSize = " << stripSize << " and ip = 192.168.1." << stripIp << std::endl;
        SoundLight soundLight = SoundLight(stripSize, stripIp, stripReverse, stripAmortissement);
        soundLight.Start(fourrier);
    }


    WSACleanup();  // Nettoyez Winsock avant de quitter
    return 0;
}