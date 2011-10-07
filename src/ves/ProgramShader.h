/*========================================================================
  VES --- VTK OpenGL ES Rendering Toolkit

      http://www.kitware.com/ves

  Copyright 2011 Kitware, Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 ========================================================================*/
// .NAME ProgramShader - Defines a collection of individual shaders.
// .SECTION Description
// ProgramShader [refer 31.4.6]

#ifndef __ProgramShader_h
#define __ProgramShader_h

#include "vsgShaderNode.h"
#include "vesSetGet.h"

#include <vector>

class ProgramShaderInternal;
class vesShader;

/**
 * \class ProgramShader
 * \brief Defines a collection of shaders.
 *
 * ProgramShader [refer 31.4.6]
 *
 * \todo Currently an empty class with no render method.
 */

class ProgramShader : public vsgShaderNode
{
public:
  ProgramShader();
  ~ProgramShader();

  vesSetGetVectorMacro(Shaders,vesShader*)

  /** Render.
   * \todo Document these methods.
   */
  bool read();

  /** Render.
   * \todo Document these methods.
   */
  void render(Painter *render);


private:
  ProgramShaderInternal *m_internal;
};

#endif // __ProgramShader_h