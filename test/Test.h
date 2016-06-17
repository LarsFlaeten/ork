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

#ifndef _ORK_TEST_
#define _ORK_TEST_

#include <string>
#include <vector>

using namespace std;

typedef void (*testFunction)();

class TestSuite
{
public:
    vector<testFunction> tests;

    vector<string> testNames;

    vector<int> testVersions;

    static TestSuite *getInstance();

private:
    static TestSuite *INSTANCE;
};

class Test
{
public:
    Test(const char *name, testFunction test, int majorVersion = 3);
};

void test(bool result, const char* file, int line);

#define TEST(x) void x(); Test _##x(#x, x); void x()

#define TEST4(x) void x(); Test _##x(#x, x, 4); void x()

#define ASSERT(x) test(x, __FILE__, __LINE__)

#endif
