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
#include <vector>

namespace argparse {

// ArgOptionMask

enum ArgOptionMask {
    Empty = 0,
    IsArgNeeded = 1 << 0,
    HasValueAfterFleg = 1 << 1,
    HasNeededValueAfterFleg = 1 << 2,

};

// Arg

struct Arg {
    Arg(const Arg& a);
    Arg(const std::string name_ = "",
        const std::string description_ = "",
        const std::string value_ = "",
        const int defaultMask = IsArgNeeded);

    std::string name;
    std::string description;
    std::string value;
    int optionMask;
    bool isSet;
};

// Fleg

struct Fleg {
    Fleg(const Fleg& a);
    Fleg(const std::string longFleg_ = "",
         const std::string sortFleg_ = "",
         const std::string description_ = "",
         const Arg arg_ = Arg(),
         const int defaultMask = Empty);

    std::string longFleg;
    std::string sortFleg;
    std::string description;
    Arg arg;
    int optionMask;
    bool isSet;
};

// ArgPars

class ArgParse {
public:
    ArgParse(int argc, char* argv[]);

    void add(Arg arg);
    void add(Fleg fleg);

    bool parse();
    const std::string showHelp();

    std::string operator[](const std::size_t& idx) { return _args[idx].value; }
    const bool checkFleg(const std::string& longFleg)
    {
        return _flegs[longFleg].isSet;
    }

    template<typename T>
    const bool checkAndReadFleg(const std::string& longFleg, T* value)
    {
        if (!checkFleg(longFleg))
            return false;

        std::string& valueStr = _flegs[longFleg].arg.value;
        std::stringstream s;
        s << valueStr;
        s >> (*value);
        return !s.fail();
    }

private:
    const std::string showError(const std::string& errorArg);

    int _argc;
    char** _argv;
    std::string _programName;
    std::vector<Arg> _args;
    std::map<std::string, Fleg> _flegs;
};

} // namespace argparse

#endif // ARG_PARSE_H
