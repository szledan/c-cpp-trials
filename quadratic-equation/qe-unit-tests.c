/* Copyright (C) 2016, Szilard Ledan <szledan@gmail.com>
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "qe-solver.h"
#include <math.h>
#include <stdio.h>

static struct {
    int pass;
    int number;
} s_testResults = { 0, 0};

#define EPS 1000.0f
#define EPSILON_EQUAL(L, R) (trunc(L * EPS) / EPS == trunc(R * EPS) / EPS)

void testCase(const float a, const float b, const float c, const Roots expected, const int isNegativeTest)
{
    static const char* passStrs = "FAIL\0PASS\0";
    const Roots roots = solver(a, b, c).roots;
    const int x1EqX1 = EPSILON_EQUAL(roots.x1, expected.x1) || isNegativeTest;
    const int x2EqX2 = EPSILON_EQUAL(roots.x2, expected.x2) || isNegativeTest;
    const int pass =  x1EqX1 && x2EqX2;

    s_testResults.number++;
    s_testResults.pass += pass;

    printf("%d. test case: %+.2f %+.2f %+.2f   %+.4f=%d=%+.4f %+.4f=%d=%+.4f   %s\n", s_testResults.number, a, b, c, roots.x1, x1EqX1, expected.x1, roots.x2, x2EqX2, expected.x2, passStrs + pass * 5);
}

Roots expectedRoots(const float x1, const float x2)
{
    const Roots roots = {x1, x2};
    return roots;
}

int main()
{
    const float negativeNaN = sqrt(-1.0f);
    const float pi = 2.0f * asin(1.0f);
    const int negativeTest = 1;

    printf(" *** Unit tester for Quadratic equation solver without 'if' statement *** by Szilard Ledan, 2016.\n");
    printf("    Run unit tests\n");
    printf("n. test case: <a> <b> <c>   <x1>=<?>=<expected x1> <x2>=<?>=<expected x2>\n");

    /* Real test cases. */
    {
        /* Golden ratio { phi =def= (k + l) / l = l / k } */
        testCase(1.0f, -1.0f, -1.0f, expectedRoots(1.61803400516510009766f, -0.61803400516510009766f), !negativeTest);

        testCase(1.0f, 2.0f, -3.0f, expectedRoots(+1.0f, -3.0f), !negativeTest);
        testCase(-1.0f, 2.0f, 3.0f, expectedRoots(-1.0f, 3.0f), !negativeTest);
        testCase(1.0f, 5.0f, 6.0f, expectedRoots(-2.0f, -3.0f), !negativeTest);
        testCase(1.0f, -5.0f, 6.0f, expectedRoots(3.0f, 2.0f), !negativeTest);
        testCase(0.0f, 3.0f, 6.0f, expectedRoots(-2.0f, -2.0f), !negativeTest);
        testCase(0.0f, 3.0f, 0.0f, expectedRoots(3.0f, 3.0f), !negativeTest);
        testCase(-1.0f, 6.0f, 0.0f, expectedRoots(0.0f, 6.0f), !negativeTest);

        testCase(1.0f, pi, -2.0f * pi * pi, expectedRoots(pi, -2 * pi), !negativeTest);
        testCase((pi - 1.0f) / pi, pi, pi, expectedRoots(-pi / (pi - 1.0f), -pi), !negativeTest);
        testCase(pi, pi / 2, 0.0f, expectedRoots(0.0f, -pi / 2.0f / pi), !negativeTest);
        testCase(-pi, -pi / 2, 0.0f, expectedRoots(-pi / 2.0f / pi, 0.0f), !negativeTest);
    }

    /* Linear test cases. */
    {
        testCase(0.0f, 1.0f, 1.0f, expectedRoots(-1.0f, -1.0f), !negativeTest);
    }

    /* Negative test cases. */
    {
        testCase(0.0f, 0.0f, 0.0f, expectedRoots(negativeNaN, negativeNaN), negativeTest);
        testCase(0.0f, 0.0f, 1.0f, expectedRoots(-1.0f / 0.0f, -1.0f / 0.0f), negativeTest);
        testCase(0.0f, 0.0f, -1.0f, expectedRoots(1.0f / 0.0f, 1.0f / 0.0f), negativeTest);
        testCase(0.0f, 0.0f, 1985.0f, expectedRoots(-1985.0f / 0.0f, -1985.0f / 0.0f), negativeTest);
        testCase(1.0f, 1.0f, 1.0f, expectedRoots(negativeNaN, negativeNaN), negativeTest);
    }

    printf("Test run totals:\n");
    printf("    Pass: %d/%d (%.1f%%)\n", s_testResults.pass, s_testResults.number, 100 * (float)s_testResults.pass / (float)s_testResults.number);
    return 0;
}
