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
using objtools::tokenize;

//#include "ObjFileData.h"
#include "../core/fast_atof.hpp"

//#include "ParsingUtils.h"
#include "material.hpp"
//#include "../include/assimp/DefaultLogger.hpp"

namespace model
{

  namespace objmtlimporter
  {

    // Material specific token
    const std::string DiffuseTexture      = "map_Kd";
    const std::string AmbientTexture      = "map_Ka";
    const std::string SpecularTexture     = "map_Ks";
    const std::string OpacityTexture      = "map_d";
    const std::string EmmissiveTexture    = "map_emissive";
    const std::string BumpTexture1        = "map_bump";
    const std::string BumpTexture2        = "map_Bump";
    const std::string BumpTexture3        = "bump";
    const std::string NormalTexture       = "map_Kn";
    const std::string DisplacementTexture = "disp";
    const std::string SpecularityTexture  = "map_ns";

    // m_texture option specific token
    const std::string BlendUOption		= "-blendu";
    const std::string BlendVOption		= "-blendv";
    const std::string BoostOption		= "-boost";
    const std::string ModifyMapOption	= "-mm";
    const std::string OffsetOption		= "-o";
    const std::string ScaleOption		= "-s";
    const std::string TurbulenceOption	= "-t";
    const std::string ResolutionOption	= "-texres";
    const std::string ClampOption		= "-clamp";
    const std::string BumpOption			= "-bm";
    const std::string ChannelOption		= "-imfchan";
    const std::string TypeOption			= "-type";

    ObjMtlImporter::ObjMtlImporter(const std::vector<char> &buffer, objfile::Model *pModel) :
        m_dataIterator( buffer.begin() ),
        m_dataIteratorEndOfBuffer( buffer.end() ),
        m_pModelInstance( pModel ),
        m_uiCurrentLine( 0 )
    {
        assert( NULL != m_pModelInstance );

        if ( NULL == m_pModelInstance->m_pDefaultMaterial )
        {
            m_pModelInstance->m_pDefaultMaterial = new objfile::ObjMaterial;
            m_pModelInstance->m_pDefaultMaterial->m_materialName = "default";
        }
        Load();
    }

    const size_t ObjMtlImporter::BUFFERSIZE;

    ObjMtlImporter::~ObjMtlImporter()
    {
    }

    ObjMtlImporter::ObjMtlImporter(const ObjMtlImporter &other )
    {
    }

    ObjMtlImporter &ObjMtlImporter::operator=( const ObjMtlImporter &other )
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
                        GetColorRGBA( &m_pModelInstance->m_pCurrentMaterial->m_ambientColor );
                    }
                    else if (*m_dataIterator == 'd')	// diffuse color
                    {
                        ++m_dataIterator;
                        GetColorRGBA( &m_pModelInstance->m_pCurrentMaterial->m_diffuseColor );
                    }
                    else if (*m_dataIterator == 's')
                    {
                        ++m_dataIterator;
                        GetColorRGBA( &m_pModelInstance->m_pCurrentMaterial->m_specularColor );
                    }
                    else if (*m_dataIterator == 'e')
                    {
                        ++m_dataIterator;
                        GetColorRGBA( &m_pModelInstance->m_pCurrentMaterial->m_emissiveColor );
                    }
                    m_dataIterator = SkipLine<ConstDataArrayIterator_t>( m_dataIterator, m_dataIteratorEndOfBuffer, m_uiCurrentLine );
                }
                break;

            case 'd':	// alpha value
                {
                    ++m_dataIterator;
                    GetFloatValue( m_pModelInstance->m_pCurrentMaterial->m_alpha );
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
                        GetFloatValue(m_pModelInstance->m_pCurrentMaterial->m_shineness);
                        break;
                    case 'i':	// Index Of refraction
                        ++m_dataIterator;
                        GetFloatValue(m_pModelInstance->m_pCurrentMaterial->m_indexOfRefraction);
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
                    GetIlluminationModel( m_pModelInstance->m_pCurrentMaterial->m_illuminationModel );
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

    void ObjMtlImporter::GetColorRGBA( Color4f *pColor )
    {
        assert( NULL != pColor );

        float r( 0.0f ), g( 0.0f ), b( 0.0f );
        m_dataIterator = GetFloat<ConstDataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, r);
        pColor->r = r;

        // we have to check if color is default 0 with only one token
        if( !core::IsLineEnd( *m_dataIterator ) ) {
           m_dataIterator = GetFloat<ConstDataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, g);
           m_dataIterator = GetFloat<ConstDataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, b);
        }

        pColor->g = g;
        pColor->b = b;
    }

    void ObjMtlImporter::GetIlluminationModel( int32 &illum_model )
    {
        m_dataIterator = CopyNextWord<ConstDataArrayIterator_t>( m_dataIterator, m_dataIteratorEndOfBuffer, m_buffer, BUFFERSIZE );
        illum_model = atoi(m_buffer);
    }

    void ObjMtlImporter::GetFloatValue( float &value )
    {
       m_dataIterator = CopyNextWord<ConstDataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer, m_buffer, BUFFERSIZE);
        value = (float)core::fast_atof(m_buffer);
    }

    void ObjMtlImporter::CreateMaterial()
    {
        std::string line( "" );
        while( !core::IsLineEnd( *m_dataIterator ) ) {
            line += *m_dataIterator;
            ++m_dataIterator;
        }

        std::vector<std::string> token;
        //int32 string<T, TAlloc>::Tokenize(TContainer &ret, const T* const delimiter, const int32 count,
           //const bool ignoreEmptyTokens, const bool keepSeparators) const

        const uint32 numToken = tokenize<std::string>(line, token, " "); //tokenize<std::string>line.Tokenize(token);
        std::string name( "" );
        if ( numToken == 1 ) {
            name = material::Material::DEFAULT_MATERIAL_NAME;
        } else {
            name = token[1];
        }

        std::map<std::string, objfile::ObjMaterial*>::iterator it = m_pModelInstance->m_materialMap.find( name );
        if ( m_pModelInstance->m_materialMap.end() == it) {
            // New Material created
           m_pModelInstance->m_pCurrentMaterial = new objfile::ObjMaterial();
            m_pModelInstance->m_pCurrentMaterial->m_materialName = name;
            m_pModelInstance->m_materialLib.push_back( name );
            m_pModelInstance->m_materialMap[ name ] = m_pModelInstance->m_pCurrentMaterial;
        } else {
            // Use older material
            m_pModelInstance->m_pCurrentMaterial = (*it).second;
        }
    }

    void ObjMtlImporter::GetTextureName()
    {
        std::string *out( NULL );
        int32 clampIndex = -1;

        const char *pPtr( &(*m_dataIterator) );
        if ( !strncmp( pPtr, DiffuseTexture.c_str(), DiffuseTexture.size() ) ) {
            // Diffuse m_texture
            out = & m_pModelInstance->m_pCurrentMaterial->m_texture;
            clampIndex = objfile::ObjMaterial::TEXTURE_TYPE_DIFFUSE;
        }
        else if (!strncmp(pPtr, AmbientTexture.c_str(), AmbientTexture.size())) {
            // Ambient m_texture
            out = & m_pModelInstance->m_pCurrentMaterial->m_textureAmbient;
            clampIndex = objfile::ObjMaterial::TEXTURE_TYPE_AMBIENT;
        }
        else if (!strncmp(pPtr, SpecularTexture.c_str(), SpecularTexture.size())) {
            // Specular m_texture
            out = & m_pModelInstance->m_pCurrentMaterial->textureSpecular;
            clampIndex = objfile::ObjMaterial::TEXTURE_TYPE_SPECULAR;
        }
        else if (!strncmp(pPtr, OpacityTexture.c_str(), OpacityTexture.size())) {
            // Opacity m_texture
            out = & m_pModelInstance->m_pCurrentMaterial->m_textureOpacity;
            clampIndex = objfile::ObjMaterial::TEXTURE_TYPE_OPACITY;
        }
        else if (!strncmp(pPtr, EmmissiveTexture.c_str(), EmmissiveTexture.size())) {
            // Emissive m_texture
            out = & m_pModelInstance->m_pCurrentMaterial->m_textureEmissive;
            clampIndex = objfile::ObjMaterial::TEXTURE_TYPE_EMISSIVE;
        }
        else if (!strncmp(pPtr, BumpTexture1.c_str(), BumpTexture1.size()) ||
           !strncmp(pPtr, BumpTexture2.c_str(), BumpTexture2.size()) ||
           !strncmp(pPtr, BumpTexture3.c_str(), BumpTexture3.size())) {
            // Bump m_texture
            out = & m_pModelInstance->m_pCurrentMaterial->m_textureBump;
            clampIndex = objfile::ObjMaterial::TEXTURE_TYPE_BUMP;
        }
        else if (!strncmp(pPtr, NormalTexture.c_str(), NormalTexture.size())) {
            // Normal map
            out = & m_pModelInstance->m_pCurrentMaterial->m_textureNormal;
            clampIndex = objfile::ObjMaterial::TEXTURE_TYPE_NORMALS;
        }
        else if (!strncmp(pPtr, DisplacementTexture.c_str(), DisplacementTexture.size())) {
            // Displacement m_texture
            out = &m_pModelInstance->m_pCurrentMaterial->m_textureDisplacement;
            clampIndex = objfile::ObjMaterial::TEXTURE_TYPE_DISPLACEMENT;
        }
        else if (!strncmp(pPtr, SpecularityTexture.c_str(), SpecularityTexture.size())) {
            // Specularity scaling (glossiness)
            out = & m_pModelInstance->m_pCurrentMaterial->m_textureSpecularity;
            clampIndex = objfile::ObjMaterial::TEXTURE_TYPE_SHININESS;
        } else {
            //DefaultLogger::get()->error("OBJ/MTL: Encountered unknown m_texture type");
            return;
        }

        bool clamp = false;
        GetTextureOption(clamp);
        m_pModelInstance->m_pCurrentMaterial->m_clamp[clampIndex] = clamp;

        std::string strTexture;
        m_dataIterator = GetName<ConstDataArrayIterator_t>( m_dataIterator, m_dataIteratorEndOfBuffer, strTexture );
        *out = strTexture;
    }

    /* Texture Option
     * /////////////////////////////////////////////////////////////////////////////
     * According to http://en.wikipedia.org/wiki/Wavefront_.obj_file#Texture_options
     * Texture map statement can contains various m_texture option, for example:
     *
     *	map_Ka -o 1 1 1 some.png
     *	map_Kd -clamp on some.png
     *
     * So we need to parse and skip these options, and leave the last part which is
     * the url of image, otherwise we will get a wrong url like "-clamp on some.png".
     *
     * Because Material supports clamp option, so we also want to return it
     */
    void ObjMtlImporter::GetTextureOption(bool &clamp)
    {
        m_dataIterator = GetNextToken<ConstDataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer);

        //If there is any more m_texture option
        while (!IsEndOfBuffer(m_dataIterator, m_dataIteratorEndOfBuffer) && *m_dataIterator == '-')
        {
            const char *pPtr( &(*m_dataIterator) );
            //skip option key and value
            int32 skipToken = 1;

            if (!strncmp(pPtr, ClampOption.c_str(), ClampOption.size()))
            {
                ConstDataArrayIterator_t it = GetNextToken<ConstDataArrayIterator_t>(m_dataIterator, m_dataIteratorEndOfBuffer);
                char value[3];
                CopyNextWord(it, m_dataIteratorEndOfBuffer, value, sizeof(value) / sizeof(*value));
                if (!strncmp(value, "on", 2))
                {
                    clamp = true;
                }

                skipToken = 2;
            }
            else if (!strncmp(pPtr, BlendUOption.c_str(), BlendUOption.size())
               || !strncmp(pPtr, BlendVOption.c_str(), BlendVOption.size())
               || !strncmp(pPtr, BoostOption.c_str(), BoostOption.size())
               || !strncmp(pPtr, ResolutionOption.c_str(), ResolutionOption.size())
               || !strncmp(pPtr, BumpOption.c_str(), BumpOption.size())
               || !strncmp(pPtr, ChannelOption.c_str(), ChannelOption.size())
               || !strncmp(pPtr, TypeOption.c_str(), TypeOption.size()))
            {
                skipToken = 2;
            }
            else if (!strncmp(pPtr, ModifyMapOption.c_str(), ModifyMapOption.size()))
            {
                skipToken = 3;
            }
            else if (!strncmp(pPtr, OffsetOption.c_str(), OffsetOption.size())
               || !strncmp(pPtr, ScaleOption.c_str(), ScaleOption.size())
               || !strncmp(pPtr, TurbulenceOption.c_str(), TurbulenceOption.size())
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
