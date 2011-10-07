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

#include "vesMapper.h"

// VES includes
#include "vesMaterial.h"
#include "vesRenderStage.h"
#include "vesShaderProgram.h"
#include "vesTriangleData.h"
#include "vesVertexAttributeKeys.h"

#ifdef ANDROID
# include <GLES2/gl2.h>
# include <GLES2/gl2ext.h>
#else
# include <OpenGLES/ES2/gl.h>
# include <OpenGLES/ES2/glext.h>
#endif

// C++ includes
#include <vector>

class vesMapper::vesInternal
{
public:
  ~vesInternal()
  {
    this->m_bufferVertexAttributeMap.clear();
  }

  std::map<unsigned int, std::vector<int> > m_bufferVertexAttributeMap;
};



vesMapper::vesMapper() : vsgBoundedObject(),
  m_initialized(false),
  m_data       (0x0),
  m_internal   (0x0)
{
  this->m_internal = new vesInternal();
}


vesMapper::~vesMapper()
{
  if (this->m_initialized) {
    // \todo: Need to implement release graphics resources.
  }

  delete this->m_internal; this->m_internal = 0x0;
}


void vesMapper::computeBounds()
{
  vesVector3f min = this->m_data->GetMin();
  vesVector3f max = this->m_data->GetMax();

  this->setBounds(min, max);

  this->setBoundsDirty(false);
}


void vesMapper::setData(vesTriangleData *data)
{
  if (data) {
    this->m_data = data;
  }
}


void vesMapper::normalize()
{
  float r = this->boundsRadius();

  this->m_normalizedMatrix =
      makeScaleMatrix4x4(1/r,1/r,1/r)*
      makeTranslationMatrix4x4(-this->boundsCenter());

  this->setBoundsCenter(transformPoint3f(this->m_normalizedMatrix, this->boundsCenter()));
  this->setBoundsSize(transformPoint3f(this->m_normalizedMatrix, this->boundsSize()));
}


void vesMapper::render(const vesRenderState &renderState)
{
  if (!this->m_initialized) {
    this->setupDrawObjects(renderState);
  }

  std::map<unsigned int, std::vector<int> >::const_iterator constItr
    = this->m_internal->m_bufferVertexAttributeMap.begin();

  for (constItr; constItr != this->m_internal->m_bufferVertexAttributeMap.end(); ++constItr) {
    glBindBuffer(GL_ARRAY_BUFFER, constItr->first);
    for (size_t i = 0; i < constItr->second.size(); ++i) {
      renderState.m_material->bindVertexData(renderState, constItr->second[i]);
    }
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_buffer[2]);
  glDrawElements(GL_TRIANGLES, this->m_data->GetTriangles().size() * 3,
                 GL_UNSIGNED_SHORT, (void*)0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_buffer[3]);
  glDrawElements(GL_LINES, this->m_data->GetLines().size() * 2,
                 GL_UNSIGNED_SHORT, (void*)0);

  constItr = this->m_internal->m_bufferVertexAttributeMap.begin();
  for (constItr; constItr != this->m_internal->m_bufferVertexAttributeMap.end(); ++constItr) {
    glBindBuffer(GL_ARRAY_BUFFER, constItr->first);
    for (size_t i = 0; i < constItr->second.size(); ++i) {
      renderState.m_material->unbindVertexData(renderState, constItr->second[i]);
    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void vesMapper::setupDrawObjects(const vesRenderState &renderState)
{
  const int numberOfFloats = 6;
  size_t sizeOfPositions = (this->m_data->GetPoints().size() * numberOfFloats * sizeof(float));

  // \todo: Put a GL log.
  glGenBuffers(4, &this->m_buffer[0]);

  glBindBuffer(GL_ARRAY_BUFFER, this->m_buffer[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeOfPositions,
               &this->m_data->GetPoints()[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, this->m_buffer[1]);
  glBufferData(GL_ARRAY_BUFFER, this->m_data->GetVertexColors().size() * sizeof(float) * 3,
               &this->m_data->GetVertexColors()[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_buffer[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               this->m_data->GetTriangles().size() *sizeof(unsigned short) * 3,
               &this->m_data->GetTriangles()[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_buffer[3]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               this->m_data->GetLines().size() *sizeof(unsigned short) * 2,
               &this->m_data->GetLines()[0], GL_STATIC_DRAW);

  // \todo: Hard coded for now.
  this->m_internal->m_bufferVertexAttributeMap[this->m_buffer[0]].push_back(
    vesVertexAttributeKeys::Position);
  this->m_internal->m_bufferVertexAttributeMap[this->m_buffer[0]].push_back(
    vesVertexAttributeKeys::Normal);
  this->m_internal->m_bufferVertexAttributeMap[this->m_buffer[1]].push_back(
    vesVertexAttributeKeys::Color);

  this->m_initialized = true;
}
