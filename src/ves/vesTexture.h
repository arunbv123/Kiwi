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

#ifndef __vesTexture_h
#define __vesTexture_h

#include "vsg/Utility/vsgMacro.h"
#include "vsg/Utility/vsgTypes.h"

class vesTextureInternal;
class vesShaderProgram;

class vesTexture
{
public:
  vesTexture(vesShaderProgram *program, SFImage image);
  virtual ~vesTexture();

  void load();
  void Render();

  vesShaderProgram *ShaderProgram;
  SFImage Image;
  unsigned int texID;
  bool loaded;

private:
  vesTextureInternal *_internal;
};
#endif // __vesTexture_h
