//
// Created by secondaire on 25/07/2024.
//

#ifndef WLEDCONTROLLERCPP_PARSEROPTION_H
#define WLEDCONTROLLERCPP_PARSEROPTION_H

#include <iostream>
#include <math.h>

#endif //WLEDCONTROLLERCPP_PARSEROPTION_H

enum parseState {
    parseStop,
    parseContinue,
    parseOk
};

class ParserOption {
private:
    std::string optionName;
    int charPosition;
    parseState state;

public:
    static int parseInt(char* value);

    ParserOption(const std::string &name);
    parseState parseOptionName(char value);
};