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

static const char* g_solveStrs = "is solvable\0is not solvable\0";
static const int g_strSeparator = 12;

Solution solver(const float a, const float b, const float c)
{
    const float epsilon = (a == 0.0f) * 0.0000000001f;
    Solution solution;

    solution.discriminant = b * b - 4 * a * c;
    solution.solveStr = g_solveStrs + (solution.discriminant < 0) * g_strSeparator;

    solution.roots.x1 = solution.roots.x2 = (a == 0.0f && c == 0.0f) * b + (a == 0.0f) * (-c / b);

    solution.roots.x1 += (a != 0.0f) * ((-b + sqrt(solution.discriminant)) / (2.0f * (a + epsilon)));
    solution.roots.x2 += (a != 0.0f) * ((-b - sqrt(solution.discriminant)) / (2.0f * (a + epsilon)));

    return solution;
}
