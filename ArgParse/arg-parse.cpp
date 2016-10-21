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
#include <map>
#include <sstream>
#include <string>

namespace argparse {

// Value

Value::Value(const Value& v)
    : _name(v._name)
    , _description(v._description)
    , _str(v._str)
    , _chooseList(v._chooseList)
    , _isValueNeeded(v._isValueNeeded)
{
}

Value::Value(const std::string& defaultValue, const std::string& name, const std::string& description)
    : _str(defaultValue)
    , _name(name)
    , _description(description)
    , _isValueNeeded(defaultValue.empty())
{
}

Value::Value(const std::string& defaultValue, const ChooseList& chooseList, const std::string& name, const std::string& description)
    : Value(defaultValue, name, description)
{
    for(auto choose : chooseList)
        _chooseList.push_back(choose);
}

// Arg

Arg::Arg(const Arg& a)
    : Value((Value)a)
    , _isArgNeeded(a._isArgNeeded)
    , _callBackFunc(a._callBackFunc)
{
}

Arg::Arg(const std::string& name,
         const std::string& description,
         const bool isNeeded,
         const Value& defaultValue)
    : Value(defaultValue._str, name, description)
    , _isArgNeeded(isNeeded)
    , _callBackFunc(nullptr)
{
}

// Flag

Flag::Flag(const Flag& f)
    : _longFlag(f._longFlag)
    , _shortFlag(f._shortFlag)
    , _description(f._description)
    , _value(f._value)
    , _isSet(f._isSet)
    , _callBackFunc(f._callBackFunc)
{
}

Flag::Flag(const std::string& longFlag,
           const std::string& shortFlag,
           const std::string& description,
           const bool set)
    : _longFlag(longFlag)
    , _shortFlag(shortFlag)
    , _description(description)
    , _isSet(set)
    , _callBackFunc(nullptr)
{
}

Flag::Flag(const std::string& longFlag,
           const std::string& shortFlag,
           const std::string& description,
           const Value value)
    : Flag(longFlag, shortFlag, description)
{
    _value = value;
}

// ArgPars

ArgParse::ArgParse(int argc, char* argv[])
    : _argc(argc)
    , _argv(argv)
    , _programName(argv[0])
{
    add(Flag("--help", "-h", "Show this help."));
}

void ArgParse::add(Arg arg)
{
    _args.push_back(arg);
}

void ArgParse::add(Arg arg, CallBackFunc cbf)
{
    arg._callBackFunc = cbf;
    add(arg);
}

void ArgParse::add(Flag flag)
{
    static uint ndx = 0;
    std::string name = flag._longFlag;

    if (name.empty())
        do {
            name = "-" + flag._shortFlag + std::to_string(ndx++);
        } while (_flags.find(name) != _flags.end());

    _flags[name] = flag;
}

void ArgParse::add(Flag flag, CallBackFunc cbf)
{
    flag._callBackFunc = cbf;
    add(flag);
}




int ArgParse::find(std::string longFlag, std::string shortFlag)
{
//    std::string args;

//    if (!longFlag.empty) {
//        for (int adv = 1; adv < _argc; ++adv) {
//            std::string arg(_argv[adv]);
//            if (arg.find(longFlag)
//        }
//    }

//    std::cout << args << std::endl;
    return 0;
}

bool ArgParse::parse()
{
    find("","");
    for (int adv = 1; adv < _argc; ++adv) {
        bool isFlag = false;
        bool hasFlagValue = false;
        std::string value = "";
        std::string arg(_argv[adv]);

        std::cout << arg << std::endl;

        // Valid flags:
        //  -abc
        //  -abc valueForC
        //  -x
        //  -x valueForX
        //  --flag
        //  --flag valueForFlag
        // Arguments:
        //  argument
        //  -
        //  --

        assert(arg.size());
        if (arg[0] == '-')                  // '-___
        {
            isFlag = true;
            if (arg.size() == 1)            // '-'
            {
                isFlag = false;
                value = arg;
            }
            else if (arg[1] == '-')         // '--___
            {
                if (arg.size() == 2)        // '--'
                {
                    isFlag = false;
                    value = arg;
                }
                else                        // '--flag' valid long flag
                {
                    // TODO: check the Flag type: simple or need value
                    if (_flags.find(arg) == _flags.end()) {
                        // TODO: error
                        isFlag = false;
                        value = arg;
                    } else {
                        Flag& f = _flags[arg];
                        f._isSet = true;
                        std::cout << f._isSet << std::endl;
                        if (f._value.hasValue()) {
                            std::cout << "has value" << std::endl;
                        }
                    }
                }
            }
            else if (arg.size() == 2)       // '-_' valid short flag
            {
                // TODO: check the Flag type: simple or need value
            }
            else                            // '-___' valid short flags
            {
                // TODO: check the Flag type: simple or need value
            }
        }
        else                                // '___' argument
        {
            isFlag = false;
            value = arg;
        }

        if (isFlag) {

        } else {
            _args.push_back(Arg("", "", false, Value(value)));
        }
    }

    return true;
}

//bool compFlags (Flag& a, Flag& b) { return (a._shortFlag < b._shortFlag || ); }

const std::string ArgParse::showHelp()
{
    const std::string tab = "    ";
    std::stringstream help;

    // Print program name.
    help << "usage: " << _programName;

    // Print arguments after programname.
    for (auto const& it : _args) {
        const Arg& arg = it;
        if (!arg._name.empty())
            if (arg._isArgNeeded)
                help << " <" << arg._name << "> ";
            else
                help << " [<" << arg._name << ">] ";
    }

    // Print arguments.
    help << std::endl << std::endl << "Arguments:" << std::endl;

    for (auto const& it : _args) {
        const Arg& arg = it;
        if (!arg._name.empty()) {
            if (arg._isArgNeeded)
                help << tab << " <" << arg._name << "> ";
            else
                help << tab << " [<" << arg._name << ">] ";
            if (!arg._description.empty())
                help << tab << arg._description;
            help << std::endl;
        }
    }

    // Print option flags.
    help << std::endl << "Option flags:" << std::endl;

    for (auto const& it : _flags) {
#define HAS_NAME(CH, VALNAME) do { \
    if (flag._value._isValueNeeded) \
        help << CH << "<" << VALNAME << ">"; \
    else \
        help << CH << "[<" << VALNAME << ">]"; \
    } while(false);

#define PRINT_FLAG(FLAG) do { \
    help << FLAG; \
    if (!flag._value._name.empty()) \
        HAS_NAME(" ", flag._value._name); \
    if (flag._value._chooseList.size() > 0)  \
        HAS_NAME(" ", flag._value.getChoosesStr(hasLongFlag ? 1 : 0)); \
    } while(false);

        const Flag& flag = it.second;
        const bool hasShortFlag = !flag._shortFlag.empty();
        const bool hasLongFlag = !flag._longFlag.empty();

        help << tab;
        if (hasShortFlag)
            PRINT_FLAG(flag._shortFlag);
        if (hasLongFlag) {
            if (hasShortFlag)
                help << ", ";
            PRINT_FLAG(flag._longFlag);
        }
        if (hasShortFlag || hasLongFlag) {
            help << tab << flag._description << std::endl;
        }

#undef PRINT_FLAG
#undef HAS_NAME
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
