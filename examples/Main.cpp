/*
 * Ork: a small object-oriented OpenGL Rendering Kernel.
 * Website : http://ork.gforge.inria.fr/
 * Copyright (c) 2008-2015 INRIA - LJK (CNRS - Grenoble University)
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, 
 * this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors 
 * may be used to endorse or promote products derived from this software without 
 * specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/*
 * Ork is distributed under the BSD3 Licence. 
 * For any assistance, feedback and remarks, you can check out the 
 * mailing list on the project page : 
 * http://ork.gforge.inria.fr/
 */
/*
 * Main authors: Eric Bruneton, Antoine Begault, Guillaume Piolat.
 */

#include <vector>
#include <assert.h>
#include <cstdio>
#include <cstring>

#include "examples/Main.h"

using namespace std;

class MainFunctions
{
public:
    vector<const char*> names;

    vector<MainFunction::mainFunction> functions;
};

static MainFunctions *mainFunctions = NULL;

MainFunction::MainFunction(const char* name, mainFunction f)
{
    if (mainFunctions == NULL) {
        mainFunctions = new MainFunctions();
    }
    mainFunctions->names.push_back(name);
    mainFunctions->functions.push_back(f);
}

int mainFunction(int argc, char* argv[])
{
    assert(mainFunctions != NULL);
    char funcName[50];
    if (argc > 1) {
        sprintf(funcName, "%s", argv[1]);
    } else {
        sprintf(funcName, "test");
    }
    for (unsigned int i = 0; i < mainFunctions->names.size(); ++i) {
        if (strcmp(funcName, mainFunctions->names[i]) == 0) {
            return mainFunctions->functions[i](argc, argv);
        }
    }
    printf("Unknown command line argument '%s'\n", argv[1]);
    printf("Must be one of:\n");
    for (unsigned int i = 0; i < mainFunctions->names.size(); ++i) {
        printf("%s\n", mainFunctions->names[i]);
    }
    return 0;
}

int main(int argc, char* argv[])
{
    return mainFunction(argc, argv);
}
