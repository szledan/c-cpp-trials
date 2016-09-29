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

#include <stdio.h>

int main()
{
    char q;

    printf(" *** Quadratic equation solver without 'if' statement *** by Szilard Ledan, 2016.\n");
    printf("    ax² + bx +c = 0\n");
    printf("  Question is, witch x solves this equation if we know a, b and c coefficients.\n");

    do {
        float a, b, c;
        Solution solution;
        printf("Coefficients:\n");
        printf(" a = "); scanf("%f", &a);
        printf(" b = "); scanf("%f", &b);
        printf(" c = "); scanf("%f", &c);

        solution = solver(a, b, c);

        printf("Eqation: %+.2fx² %+.2fx %+.2f = 0,    -->    ", a, b, c);
        printf("quadratic formula: x₁,₂ = (%+.2f ± sqrt(%+.2f) / %+.2f.\n", -b, solution.discriminant, 2.0f * a);

        printf("Your equation %s.\n x1 = %f\n x2 = %f\n", solution.solveStr, solution.roots.x1, solution.roots.x2);

        printf("[q]uit >"); scanf("%s", &q);
    } while (q != 'q');

    printf("  Goodbye!\n");
    return 0;
}
