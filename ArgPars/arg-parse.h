#ifndef ARG_PARSE_H
#define ARG_PARSE_H

/* Copyright (C) 2016, Gepard Graphics
 * Copyright (C) 2016, Szilard Ledan <szledan@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <iostream>
#include <map>
#include <sstream>
#include <string>

namespace argparse {

// ArgOptionMask

enum ArgOptionMask {
    Empty = 0,
    IsArg = 1 << 0,
    IsNeeded = 1 << 1,
    isNeedValue = 1 << 2,
    IsDefaultSet = 1 << 3,

};

// Arg

struct Arg {
    Arg(const Arg& a);
    Arg(const std::string name_ = "",
        const std::string sortFleg_ = "",
        const std::string longFleg_ = "",
        const std::string description_ = "",
        const std::string value_ = "",
        const int defaultMask = Empty);

    std::string name;
    std::string sortFleg;
    std::string longFleg;
    std::string description;
    std::string valueStr;
    int optionMask;
    bool isSet;
};

// ArgPars

class ArgPars {
public:
    ArgPars(int argc, char* argv[]);

    void addArg(Arg arg);

    bool parse();
    const std::string showHelp();

    std::string operator[](const std::string& idx) { return this->_args[idx].valueStr; }
    std::string value(const std::string& idx, const std::string& defVaule = "0")
    {
        std::string& valueStr = _args[idx].valueStr;
        return valueStr.empty() ?  defVaule : valueStr;
    }

    template<typename T>
    bool tryRead(T& value, const std::string& idx)
    {
        std::string& valueStr = _args[idx].valueStr;
        std::stringstream s;
        s << valueStr;
        s >> (value);
        return !s.fail();
    }

private:
    const std::string showError(const std::string& errorArg);

    int _argc;
    char** _argv;
    std::string _programName;
    std::map<std::string, Arg> _args;
};

} // namespace argparse

#endif // ARG_PARSE_H
