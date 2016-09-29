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

#ifndef QE_SOLVER_H
#define QE_SOLVER_H

//extern const char* g_solveStrs;
//extern const int g_strSeparator;

typedef struct {
    float x1;
    float x2;
} Roots;

typedef struct {
    Roots roots;
    const char* solveStr;
    float discriminant;
} Solution;

Solution solver(const float a, const float b, const float c);

#endif /* QE_SOLVER_H */
