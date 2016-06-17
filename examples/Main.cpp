/*
 * Ork: a small object-oriented OpenGL Rendering Kernel.
 * Copyright (c) 2008-2010 INRIA
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.
 */

/*
 * Authors: Eric Bruneton, Antoine Begault, Guillaume Piolat.
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
