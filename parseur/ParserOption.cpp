#include "ParserOption.h"

ParserOption::ParserOption(const std::string &name) {
    this->optionName = name;
    this->charPosition = 0;
    this->state = parseContinue;
}

parseState ParserOption::parseOptionName(char value) {
    if (this->state == parseContinue) {
        if (value == this->optionName[this->charPosition]) {
            this->charPosition++;
            if (this->optionName[this->charPosition] == '\0') {
                this->state = parseOk;
            }
        } else {
            this->state = parseStop;
        }
    }
    return this->state;
}

int ParserOption::parseInt(char *value) {
    int ret = 0;
    int index = 0;

    while (value[index] != '\0') {
        index++;
    }
    int size = index;
    for (int i = 0; i < size; ++i) {
        ret += (value[i] - 48) * std::pow(10,size - i - 1);
    }
    return ret;
}