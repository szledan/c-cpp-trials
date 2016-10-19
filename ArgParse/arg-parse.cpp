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

#include "arg-parse.h"

#include <assert.h>
#include <iostream>
#include <sstream>
#include <string>

namespace argparse {

// Arg

Arg::Arg(const Arg& a)
    : name(a.name)
    , description(a.description)
    , value(a.value)
    , optionMask(a.optionMask)
    , isSet(a.isSet)
{
}

Arg::Arg(const std::string name_,
         const std::string description_,
         const std::string value_,
         const int defaultMask)
    : name(name_)
    , description(description_)
    , value(value_)
    , optionMask(defaultMask)
    , isSet(!value_.empty())
{
}

// Fleg

Fleg::Fleg(const Fleg& a)
    : longFleg(a.longFleg)
    , sortFleg(a.sortFleg)
    , description(a.description)
    , arg(a.arg)
    , optionMask(a.optionMask)
    , isSet(a.isSet)
{
}

Fleg::Fleg(const std::string longFleg_,
           const std::string sortFleg_,
           const std::string description_,
           const Arg arg_,
           const int defaultMask)
    : longFleg(longFleg_)
    , sortFleg(sortFleg_)
    , description(description_)
    , arg(arg_)
    , optionMask(defaultMask)
    , isSet(!arg_.value.empty())
{
}

// ArgPars

ArgParse::ArgParse(int argc, char* argv[])
    : _argc(argc)
    , _argv(argv)
    , _programName(argv[0])
{
}

void ArgParse::add(Arg arg)
{
    _args.push_back(arg);
}

void ArgParse::add(Fleg fleg)
{
    assert(!fleg.longFleg.empty());
    _flegs[fleg.longFleg] = fleg;
}

bool ArgParse::parse()
{
//    for (int adv = 1; adv < _argc; ++adv) {
//        bool isFleg = false;
//        bool hasFlegValue = false;
//        std::string value = "";
//        std::string arg(_argv[adv]);

//        std::cout << arg << std::endl;

//        // Valid flags:
//        //  -abc
//        //  -abc valueForC
//        //  -x
//        //  -x valueForX
//        //  --flag
//        //  --flag valueForFlag
//        // Arguments:
//        //  argument
//        //  -
//        //  --

//        assert(arg.size());
//        if (arg[0] == '-')                  // '-___
//        {
//            isFleg = true;
//            if (arg.size() == 1)            // '-'
//            {
//                isFleg = false;
//                value = arg;
//            }
//            else if (arg[1] == '-')         // '--___
//            {
//                if (arg.size() == 2)        // '--'
//                {
//                    isFleg = false;
//                    value = arg;
//                }
//                else                        // '--flag' valid long flag
//                {
//                    // TODO: check the fleg type: simple or need value
//                }
//            }
//            else if (arg.size() == 2)       // '-_' valid sort flag
//            {
//                // TODO: check the fleg type: simple or need value
//            }
//            else                            // '-___' valid sort flags
//            {
//                // TODO: check the fleg type: simple or need value
//            }
//        }
//        else                                // '___' argument
//        {
//            isFleg = false;
//            value = arg;
//        }

//        if (isFleg) {

//        } else {
//            std::string name = "unknow";
//            _args[name].valueStr = value;
//        }
//    }

    return true;
}

const std::string ArgParse::showHelp()
{
    std::stringstream help;

    // Print program name.
    help << "usage: " << _programName;
    // Print arguments after programname.
    for (auto const& it : _args) {
        const Arg& arg = it;
        if ((!arg.name.empty()) && (arg.optionMask & IsArgNeeded))
            help << " <" << arg.name << "> ";
    }

    // Print option flags.
    help << std::endl << std::endl << "Option flags:" << std::endl;

    for (auto const& it : _flegs) {
        const Fleg& fleg = it.second;
        help << "\t";
        if (!fleg.sortFleg.empty()) {
            help << fleg.sortFleg;
            if (fleg.optionMask & HasValueAfterFleg && !fleg.arg.name.empty())
                help << " [<" << fleg.arg.name << ">]";
            else if (fleg.optionMask & HasNeededValueAfterFleg && !fleg.arg.name.empty())
                help << " <" << fleg.arg.name << ">";
            help << ", ";
        }
        help << fleg.longFleg;
        if (fleg.optionMask & HasValueAfterFleg && !fleg.arg.name.empty())
            help << "=[<" << fleg.arg.name << ">]";
        else if (fleg.optionMask & HasNeededValueAfterFleg && !fleg.arg.name.empty())
            help << "=<" << fleg.arg.name << ">";
        help << "\t" << fleg.description << std::endl;
    }

    return help.str();
}

const std::string ArgParse::showError(const std::string& errorArg)
{
    std::stringstream error;
    error << "error: the '" << errorArg << "' is not a valid argument or flag.";
    return error.str();
}

} // namespace argparse
