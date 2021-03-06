/*
Open Asset Import Library (assimp)
----------------------------------------------------------------------

Copyright (c) 2006-2012, assimp team
All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.

* Neither the name of the assimp team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the assimp team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------*/
#ifndef _OBJMTLIMPORTER_HPP_INCLUDED_
#define _OBJMTLIMPORTER_HPP_INCLUDED_

#include <vector>
//using core::string::String_c;

#include "gfx/color4f.hpp"
using gfx::color4f::Color4f;

#include "OBJFile.hpp"

namespace model
{

  namespace objmtlimporter
  {
    // Loads the material description from a mtl file.
    class ObjMtlImporter
    {
    public:
       static const size_t BUFFERSIZE = 2048;
       typedef std::vector<char> DataArray_t;
       typedef std::vector<char>::iterator DataArrayIterator_t;
       typedef std::vector<char>::const_iterator ConstDataArrayIterator_t;
    private:

    	std::string m_strAbsPath;
    	//	Data iterator showing to the current position in data buffer
    	/*DataArrayIterator_t m_dataIterator;*/
      ConstDataArrayIterator_t m_dataIterator;
    	//	Data iterator to end of buffer
    	//DataArrayIterator_t m_dataIteratorEndOfBuffer;
      ConstDataArrayIterator_t m_dataIteratorEndOfBuffer;
    	//	USed model instance
    	objfile::Model *m_pModelInstance;
    	uint32 m_uiCurrentLine;
    	//	Helper buffer
    	char m_buffer[BUFFERSIZE];

      ObjMtlImporter(const ObjMtlImporter &other); // copy ctor, empty

      ObjMtlImporter &operator=(const ObjMtlImporter &other);
    	//	Load the whole material description
    	void Load();

    	void GetColorRGBA(Color4f *pColor);

    	void GetIlluminationModel(int32 &illum_model );

    	void GetFloatValue(float &value );
    	//	Creates a new material from loaded data.
    	void CreateMaterial();

    	void GetTextureName();
    	void GetTextureOption(bool &clamp);

    public:
    	ObjMtlImporter( const std::vector<char> &buffer, objfile::Model *pModel );
    	~ObjMtlImporter();
    };

  } // namespace objmtlimporter

} // namespace model

#endif
