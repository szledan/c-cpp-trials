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
#include <iostream>

using namespace argparse;

int main(int argc, char* argv[])
{
    ArgParse args(argc, argv);
    args.add(Arg("func", "Select benchmarked function.", ""));
    args.add(Fleg("--all", "-a", "Select all functions."));
    args.add(Fleg("--list", "-l", "List all functions."));
    args.add(Fleg("--chose", "-c", "Chose from [A|B|C].", Arg(), HasValueAfterFleg));
    args.add(Fleg("--file", "-f", "Set filename.", Arg("filename", "Source PNG filename.", "arg-pars.png"), HasNeededValueAfterFleg));
    args.add(Fleg("--version", "-v", "Get version.", Arg("", "", "3.1415"), HasNeededValueAfterFleg));
    args.add(Fleg("--run", "", "Run program."));
    if (!args.parse())
        std::cout  << args.showHelp() << std::endl;

    std::cout  << args.showHelp() << std::endl;

    bool b;
    args.checkAndReadFleg("--chose", &b);
    std::cout << b << std::endl;

    std::string fn;
    args.checkAndReadFleg("--file", &fn);
    std::cout << fn << std::endl;

    double v;
    args.checkAndReadFleg("--version", &v);
    std::cout << v << std::endl;

//    std::string str = args["func"];
//    bool isAll = stoi(args["all"]);
//    bool isList = stoi(args["list"]);

    return 0;
}