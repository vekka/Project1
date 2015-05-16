/*
---------------------------------------------------------------------------
Open Asset Import Library (assimp)
---------------------------------------------------------------------------

Copyright (c) 2006-2012, assimp team

All rights reserved.

Redistribution and use of this software in source and binary forms, 
with or without modification, are permitted provided that the following 
conditions are met:

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
---------------------------------------------------------------------------
*/


#ifndef ASSIMP_BUILD_NO_OBJ_IMPORTER

#include <stdlib.h>
#include "ObjMtlImporter.hpp"

#include "ObjTools.hpp"
using objtools::SkipLine;
using objtools::GetNextToken;
using objtools::GetFloat;
using objtools::CopyNextWord;
using objtools::GetName;
using objtools::IsEndOfBuffer;

using gfx::color::IDX_RED;
using gfx::color::IDX_BLUE;
using gfx::color::IDX_GREEN;
using gfx::color::IDX_ALPHA;

//#include "ObjFileData.h"
#include "../core/fast_atof.hpp"

#include "../core/StringComparison.hpp"
using assimp::ASSIMP_strincmp;

//#include "ParsingUtils.h"
//#include "../include/assimp/material.h"
//#include "../include/assimp/DefaultLogger.hpp"

namespace model
{
  
  namespace objmtlimporter
  {
    
    // Material specific token
    const String_c DiffuseTexture      = "map_Kd";
    const String_c AmbientTexture      = "map_Ka";
    const String_c SpecularTexture     = "map_Ks";
    const String_c OpacityTexture      = "map_d";
    const String_c EmmissiveTexture    = "map_emissive";
    const String_c BumpTexture1        = "map_bump";
    const String_c BumpTexture2        = "map_Bump";
    const String_c BumpTexture3        = "bump";
    const String_c NormalTexture       = "map_Kn";
    const String_c DisplacementTexture = "disp";
    const String_c SpecularityTexture  = "map_ns";
    
    // texture option specific token
    const String_c BlendUOption		= "-blendu";
    const String_c BlendVOption		= "-blendv";
    const String_c BoostOption		= "-boost";
    const String_c ModifyMapOption	= "-mm";
    const String_c OffsetOption		= "-o";
    const String_c ScaleOption		= "-s";
    const String_c TurbulenceOption	= "-t";
    const String_c ResolutionOption	= "-texres";
    const String_c ClampOption		= "-clamp";
    const String_c BumpOption			= "-bm";
    const String_c ChannelOption		= "-imfchan";
    const String_c TypeOption			= "-type";
    
    ObjMtlImporter::ObjMtlImporter(const std::vector<char> &buffer,
                                           const String_c & /*strAbsPath*/,
                                           objfile::Model *pModel) :
        m_dataIterator( buffer.begin() ),
        m_dataIteratorEndOfBuffer( buffer.end() ),
        m_pModelInstance( pModel ),
        m_uiCurrentLine( 0 )
    {
        assert( NULL != m_pModelInstance );

        if ( NULL == m_pModelInstance->m_pDefaultMaterial )
        {
            m_pModelInstance->m_pDefaultMaterial = new objfile::Material;
            m_pModelInstance->m_pDefaultMaterial->MaterialName = "default";
        }
        Load();
    }
    
    ObjMtlImporter::~ObjMtlImporter()
    {
    }
    
    ObjMtlImporter::ObjMtlImporter(const ObjMtlImporter & /* rOther */ )
    {
    }
        
    ObjMtlImporter &ObjMtlImporter::operator=( const ObjMtlImporter & /*rOther */ )
    {
        return *this;
    }
    
    void ObjMtlImporter::Load()
    {
        if ( m_dataIterator == m_dataIteratorEndOfBuffer )
            return;
    
        while ( m_dataIterator != m_dataIteratorEndOfBuffer )
        {
            switch (*m_dataIterator)
            {
            case 'k':
            case 'K':
                {
                    ++m_dataIterator;
                    if (*m_dataIterator == 'a') // ambient color
                    {
                        ++m_dataIterator;
                        GetColorRGBA( &m_pModelInstance->m_pCurrentMaterial->ambientColor );
                    }
                    else if (*m_dataIterator == 'd')	// diffuse color
                    {
                        ++m_dataIterator;
                        GetColorRGBA( &m_pModelInstance->m_pCurrentMaterial->diffuseColor );
                    }
                    else if (*m_dataIterator == 's')
                    {
                        ++m_dataIterator;
                        GetColorRGBA( &m_pModelInstance->m_pCurrentMaterial->specularColor );
                    }
                    else if (*m_dataIterator == 'e')
                    {
                        ++m_dataIterator;
                        GetColorRGBA( &m_pModelInstance->m_pCurrentMaterial->emissiveColor );
                    }
                    m_dataIterator = SkipLine<ConstDataArrayIterator_t>( m_dataIterator, m_dataIteratorEndOfBuffer, m_uiCurrentLine );
                }
                break;
    
            case 'd':	// alpha value
                {
                    ++m_dataIterator;
                    GetFloatValue( m_pModelInstance->m_pCurrentMaterial->alpha );
                    m_dataIterator = SkipLine<ConstDataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, m_uiCurrentLine);
                }
                break;
    
            case 'N':
            case 'n':
                {
                    ++m_dataIterator;
                    switch(*m_dataIterator)
                    {
                    case 's':	// Specular exponent
                        ++m_dataIterator;
                        GetFloatValue(m_pModelInstance->m_pCurrentMaterial->shineness);
                        break;
                    case 'i':	// Index Of refraction
                        ++m_dataIterator;
                        GetFloatValue(m_pModelInstance->m_pCurrentMaterial->indexOfRefraction);
                        break;
                    case 'e':	// New material
                        CreateMaterial();
                        break;
                    }
                    m_dataIterator = SkipLine<ConstDataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, m_uiCurrentLine);
                }
                break;
    
            case 'm':	// Texture
            case 'b':   // quick'n'dirty - for 'bump' sections
                {
                    GetTextureName();
                    m_dataIterator = SkipLine<ConstDataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, m_uiCurrentLine);
                }
                break;
    
            case 'i':	// Illumination model
                {
                   m_dataIterator = GetNextToken<ConstDataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer);
                    GetIlluminationModel( m_pModelInstance->m_pCurrentMaterial->illuminationModel );
                    m_dataIterator = SkipLine<ConstDataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, m_uiCurrentLine);
                }
                break;
    
            default:
                {
                   m_dataIterator = SkipLine<ConstDataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, m_uiCurrentLine);
                }
                break;
            }
        }
    }
    
    void ObjMtlImporter::GetColorRGBA( Colorf *pColor )
    {
        assert( NULL != pColor );
        
        float r( 0.0f ), g( 0.0f ), b( 0.0f );
        m_dataIterator = GetFloat<ConstDataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, r);
        (*pColor)[IDX_RED] = r;
        
        // we have to check if color is default 0 with only one token
        if( !IsLineEnd( *m_dataIterator ) ) {
           m_dataIterator = GetFloat<ConstDataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, g);
           m_dataIterator = GetFloat<ConstDataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, b);
        }

        (*pColor)[IDX_GREEN] = g;
        (*pColor)[IDX_BLUE] = b;
    }
    
    void ObjMtlImporter::GetIlluminationModel( int &illum_model )
    {
        m_dataIterator = CopyNextWord<ConstDataArrayIterator_t>( m_dataIterator, m_dataIteratorEndOfBuffer, m_buffer, BUFFERSIZE );
        illum_model = atoi(m_buffer);
    }
    
    void ObjMtlImporter::GetFloatValue( float &value )
    {
       m_dataIterator = CopyNextWord<ConstDataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, m_buffer, BUFFERSIZE);
        value = (float)assimp::fast_atof(m_buffer);
    }
    
    void ObjMtlImporter::CreateMaterial()
    {	
        String_c line( "" );
        while( !IsLineEnd( *m_dataIterator ) ) {
            line += *m_dataIterator;
            ++m_dataIterator;
        }
        
        std::vector<String_c> token;
        //int32 String<T, TAlloc>::Tokenize(TContainer &ret, const T* const delimiter, const int32 count,
           //const bool ignoreEmptyTokens, const bool keepSeparators) const

        const uint32 numToken = line.Tokenize(token); //tokenize<String_c>( line, token, " " );
        String_c name( "" );
        if ( numToken == 1 ) {
            name = objparser::ObjFileParser::DEFAULT_MATERIAL_NAME;
        } else {
            name = token[1];
        }
    
        std::map<String_c, objfile::Material*>::iterator it = m_pModelInstance->m_MaterialMap.find( name );
        if ( m_pModelInstance->m_MaterialMap.end() == it) {
            // New Material created
           m_pModelInstance->m_pCurrentMaterial = new objfile::Material();
            m_pModelInstance->m_pCurrentMaterial->MaterialName = name;
            m_pModelInstance->m_MaterialLib.push_back( name );
            m_pModelInstance->m_MaterialMap[ name ] = m_pModelInstance->m_pCurrentMaterial;
        } else {
            // Use older material
            m_pModelInstance->m_pCurrentMaterial = (*it).second;
        }
    }
    
    void ObjMtlImporter::GetTextureName() {
        String_c *out( NULL );
        int32 clampIndex = -1;
    
        const char *pPtr( &(*m_dataIterator) );
        if ( !ASSIMP_strincmp( pPtr, DiffuseTexture.CString(), DiffuseTexture.GetSize() ) ) {
            // Diffuse texture
            out = & m_pModelInstance->m_pCurrentMaterial->texture;
            clampIndex = objfile::Material::TextureDiffuseType;
        }
        else if (!ASSIMP_strincmp(pPtr, AmbientTexture.CString(), AmbientTexture.GetSize() ) ) {
            // Ambient texture
            out = & m_pModelInstance->m_pCurrentMaterial->textureAmbient;
            clampIndex = objfile::Material::TextureAmbientType;
        }
        else if (!ASSIMP_strincmp(pPtr, SpecularTexture.CString(), SpecularTexture.GetSize())) {
            // Specular texture
            out = & m_pModelInstance->m_pCurrentMaterial->textureSpecular;
            clampIndex = objfile::Material::TextureSpecularType;
        }
        else if (!ASSIMP_strincmp(pPtr, OpacityTexture.CString(), OpacityTexture.GetSize())) {
            // Opacity texture
            out = & m_pModelInstance->m_pCurrentMaterial->textureOpacity;
            clampIndex = objfile::Material::TextureOpacityType;
        }
        else if (!ASSIMP_strincmp(pPtr, EmmissiveTexture.CString(), EmmissiveTexture.GetSize())) {
            // Emissive texture
            out = & m_pModelInstance->m_pCurrentMaterial->textureEmissive;
            clampIndex = objfile::Material::TextureEmissiveType;
        }
        else if (!ASSIMP_strincmp(pPtr, BumpTexture1.CString(), BumpTexture1.GetSize()) ||
           !ASSIMP_strincmp(pPtr, BumpTexture2.CString(), BumpTexture2.GetSize()) ||
           !ASSIMP_strincmp(pPtr, BumpTexture3.CString(), BumpTexture3.GetSize())) {
            // Bump texture 
            out = & m_pModelInstance->m_pCurrentMaterial->textureBump;
            clampIndex = objfile::Material::TextureBumpType;
        }
        else if (!ASSIMP_strincmp(pPtr, NormalTexture.CString(), NormalTexture.GetSize())) {
            // Normal map
            out = & m_pModelInstance->m_pCurrentMaterial->textureNormal;
            clampIndex = objfile::Material::TextureNormalType;
        }
        else if (!ASSIMP_strincmp(pPtr, DisplacementTexture.CString(), DisplacementTexture.GetSize())) {
            // Displacement texture
            out = &m_pModelInstance->m_pCurrentMaterial->textureDisp;
            clampIndex = objfile::Material::TextureDispType;
        }
        else if (!ASSIMP_strincmp(pPtr, SpecularityTexture.CString(), SpecularityTexture.GetSize())) {
            // Specularity scaling (glossiness)
            out = & m_pModelInstance->m_pCurrentMaterial->textureSpecularity;
            clampIndex = objfile::Material::TextureSpecularityType;
        } else {
            //DefaultLogger::get()->error("OBJ/MTL: Encountered unknown texture type");
            return;
        }
    
        bool clamp = false;
        GetTextureOption(clamp);
        m_pModelInstance->m_pCurrentMaterial->clamp[clampIndex] = clamp;
    
        String_c strTexture;
        m_dataIterator = GetName<ConstDataArrayIterator_t>( m_dataIterator, m_dataIteratorEndOfBuffer, strTexture );
        *out = strTexture;
    }
    
    /* /////////////////////////////////////////////////////////////////////////////
     * Texture Option
     * /////////////////////////////////////////////////////////////////////////////
     * According to http://en.wikipedia.org/wiki/Wavefront_.obj_file#Texture_options
     * Texture map statement can contains various texture option, for example:
     *
     *	map_Ka -o 1 1 1 some.png
     *	map_Kd -clamp on some.png
     *
     * So we need to parse and skip these options, and leave the last part which is 
     * the url of image, otherwise we will get a wrong url like "-clamp on some.png".
     *
     * Because aiMaterial supports clamp option, so we also want to return it
     * /////////////////////////////////////////////////////////////////////////////
     */
    void ObjMtlImporter::GetTextureOption(bool &clamp)
    {
        m_dataIterator = GetNextToken<ConstDataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer);
    
        //If there is any more texture option
        while (!IsEndOfBuffer(m_dataIterator, m_dataIteratorEndOfBuffer) && *m_dataIterator == '-')
        {
            const char *pPtr( &(*m_dataIterator) );
            //skip option key and value
            int skipToken = 1;
    
            if (!ASSIMP_strincmp(pPtr, ClampOption.CString(), ClampOption.GetSize()))
            {
                ConstDataArrayIterator_t it = GetNextToken<ConstDataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer);
                char value[3];
                CopyNextWord(it, m_dataIteratorEndOfBuffer, value, sizeof(value) / sizeof(*value));
                if (!ASSIMP_strincmp(value, "on", 2))
                {
                    clamp = true;
                }
    
                skipToken = 2;
            }
            else if (  !ASSIMP_strincmp(pPtr, BlendUOption.CString(), BlendUOption.GetSize())
               || !ASSIMP_strincmp(pPtr, BlendVOption.CString(), BlendVOption.GetSize())
               || !ASSIMP_strincmp(pPtr, BoostOption.CString(), BoostOption.GetSize())
               || !ASSIMP_strincmp(pPtr, ResolutionOption.CString(), ResolutionOption.GetSize())
               || !ASSIMP_strincmp(pPtr, BumpOption.CString(), BumpOption.GetSize())
               || !ASSIMP_strincmp(pPtr, ChannelOption.CString(), ChannelOption.GetSize())
               || !ASSIMP_strincmp(pPtr, TypeOption.CString(), TypeOption.GetSize()))
            {
                skipToken = 2;
            }
            else if (!ASSIMP_strincmp(pPtr, ModifyMapOption.CString(), ModifyMapOption.GetSize()))
            {
                skipToken = 3;
            }
            else if (!ASSIMP_strincmp(pPtr, OffsetOption.CString(), OffsetOption.GetSize())
               || !ASSIMP_strincmp(pPtr, ScaleOption.CString(), ScaleOption.GetSize())
               || !ASSIMP_strincmp(pPtr, TurbulenceOption.CString(), TurbulenceOption.GetSize())
                    )
            {
                skipToken = 4;
            }
    
            for (int32 i = 0; i < skipToken; ++i)
            {
                m_dataIterator = GetNextToken<ConstDataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer);
            }
        }
    }
    
  } // namespace objmtlimporter

} // namespace model

#endif // !! ASSIMP_BUILD_NO_OBJ_IMPORTER
