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
#include "ObjMtlImporter.h"
#include "ObjTools.h"
#include "ObjFileData.h"
#include "fast_atof.h"
#include "ParsingUtils.h"
#include "../include/assimp/material.h"
#include "../include/assimp/DefaultLogger.hpp"

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
    
    ObjFileMtlImporter::ObjFileMtlImporter( std::vector<char> &buffer, 
                                           const String_c & /*strAbsPath*/,
                                           ObjFile::Model *pModel ) :
        m_DataIt( buffer.begin() ),
        m_DataItEnd( buffer.end() ),
        m_pModel( pModel ),
        m_uiLine( 0 )
    {
        assert( NULL != m_pModel );
        if ( NULL == m_pModel->m_pDefaultMaterial )
        {
            m_pModel->m_pDefaultMaterial = new ObjFile::Material;
            m_pModel->m_pDefaultMaterial->MaterialName.Set( "default" );
        }
        load();
    }
    
    // -------------------------------------------------------------------
    //	Destructor
    ObjFileMtlImporter::~ObjFileMtlImporter()
    {
        // empty
    }
    
    // -------------------------------------------------------------------
    //	Private copy constructor
    ObjFileMtlImporter::ObjFileMtlImporter(const ObjFileMtlImporter & /* rOther */ )
    {
        // empty
    }
        
    // -------------------------------------------------------------------
    //	Private copy constructor
    ObjFileMtlImporter &ObjFileMtlImporter::operator = ( const ObjFileMtlImporter & /*rOther */ )
    {
        return *this;
    }
    
    // -------------------------------------------------------------------
    //	Loads the material description
    void ObjFileMtlImporter::Load()
    {
        if ( m_DataIt == m_DataItEnd )
            return;
    
        while ( m_DataIt != m_DataItEnd )
        {
            switch (*m_DataIt)
            {
            case 'k':
            case 'K':
                {
                    ++m_DataIt;
                    if (*m_DataIt == 'a') // Ambient color
                    {
                        ++m_DataIt;
                        getColorRGBA( &m_pModel->m_pCurrentMaterial->ambient );
                    }
                    else if (*m_DataIt == 'd')	// Diffuse color
                    {
                        ++m_DataIt;
                        getColorRGBA( &m_pModel->m_pCurrentMaterial->diffuse );
                    }
                    else if (*m_DataIt == 's')
                    {
                        ++m_DataIt;
                        getColorRGBA( &m_pModel->m_pCurrentMaterial->specular );
                    }
                    else if (*m_DataIt == 'e')
                    {
                        ++m_DataIt;
                        getColorRGBA( &m_pModel->m_pCurrentMaterial->emissive );
                    }
                    m_DataIt = skipLine<DataArrayIt>( m_DataIt, m_DataItEnd, m_uiLine );
                }
                break;
    
            case 'd':	// Alpha value
                {
                    ++m_DataIt;
                    getFloatValue( m_pModel->m_pCurrentMaterial->alpha );
                    m_DataIt = skipLine<DataArrayIt>( m_DataIt, m_DataItEnd, m_uiLine );
                }
                break;
    
            case 'N':
            case 'n':
                {
                    ++m_DataIt;
                    switch(*m_DataIt)
                    {
                    case 's':	// Specular exponent
                        ++m_DataIt;
                        GetFloatValue(m_pModel->m_pCurrentMaterial->shineness);
                        break;
                    case 'i':	// Index Of refraction
                        ++m_DataIt;
                        GetFloatValue(m_pModel->m_pCurrentMaterial->ior);
                        break;
                    case 'e':	// New material
                        CreateMaterial();
                        break;
                    }
                    m_DataIt = SkipLine<DataArrayIt>( m_DataIt, m_DataItEnd, m_uiLine );
                }
                break;
    
            case 'm':	// Texture
            case 'b':   // quick'n'dirty - for 'bump' sections
                {
                    GetTexture();
                    m_DataIt = SkipLine<DataArrayIt>( m_DataIt, m_DataItEnd, m_uiLine );
                }
                break;
    
            case 'i':	// Illumination model
                {
                    m_DataIt = GetNextToken<DataArrayIt>(m_DataIt, m_DataItEnd);
                    GetIlluminationModel( m_pModel->m_pCurrentMaterial->illumination_model );
                    m_DataIt = SkipLine<DataArrayIt>( m_DataIt, m_DataItEnd, m_uiLine );
                }
                break;
    
            default:
                {
                    m_DataIt = SkipLine<DataArrayIt>( m_DataIt, m_DataItEnd, m_uiLine );
                }
                break;
            }
        }
    }
    
    // -------------------------------------------------------------------
    //	Loads a color definition
    void ObjFileMtlImporter::getColorRGBA( aiColor3D *pColor )
    {
        assert( NULL != pColor );
        
        float r( 0.0f ), g( 0.0f ), b( 0.0f );
        m_DataIt = GetFloat<DataArrayIt>( m_DataIt, m_DataItEnd, r );
        pColor->r = r;
        
        // we have to check if color is default 0 with only one token
        if( !IsLineEnd( *m_DataIt ) ) {
            m_DataIt = GetFloat<DataArrayIt>( m_DataIt, m_DataItEnd, g );
            m_DataIt = GetFloat<DataArrayIt>( m_DataIt, m_DataItEnd, b );
        }
        pColor->g = g;
        pColor->b = b;
    }
    
    void ObjFileMtlImporter::GetIlluminationModel( int &illum_model )
    {
        m_DataIt = CopyNextWord<DataArrayIt>( m_DataIt, m_DataItEnd, m_buffer, BUFFERSIZE );
        illum_model = atoi(m_buffer);
    }
    
    void ObjFileMtlImporter::GetFloatValue( float &value )
    {
        m_DataIt = CopyNextWord<DataArrayIt>( m_DataIt, m_DataItEnd, m_buffer, BUFFERSIZE );
        value = (float) fast_atof(m_buffer);
    }
    
    // -------------------------------------------------------------------
    //	Creates a material from loaded data.
    void ObjFileMtlImporter::CreateMaterial()
    {	
        String_c line( "" );
        while( !IsLineEnd( *m_DataIt ) ) {
            line += *m_DataIt;
            ++m_DataIt;
        }
        
        std::vector<String_c> token;
        const unsigned int32 numToken = tokenize<String_c>( line, token, " " );
        String_c name( "" );
        if ( numToken == 1 ) {
            name = AI_DEFAULT_MATERIAL_NAME;
        } else {
            name = token[ 1 ];
        }
    
        std::map<String_c, ObjFile::Material*>::iterator it = m_pModel->m_MaterialMap.find( name );
        if ( m_pModel->m_MaterialMap.end() == it) {
            // New Material created
            m_pModel->m_pCurrentMaterial = new ObjFile::Material();	
            m_pModel->m_pCurrentMaterial->MaterialName.Set( name );
            m_pModel->m_MaterialLib.push_back( name );
            m_pModel->m_MaterialMap[ name ] = m_pModel->m_pCurrentMaterial;
        } else {
            // Use older material
            m_pModel->m_pCurrentMaterial = (*it).second;
        }
    }
    
    // -------------------------------------------------------------------
    //	Gets a texture name from data.
    void ObjFileMtlImporter::GetTexture() {
        aiString *out( NULL );
        int32 clampIndex = -1;
    
        const char *pPtr( &(*m_DataIt) );
        if ( !ASSIMP_strincmp( pPtr, DiffuseTexture.c_str(), DiffuseTexture.size() ) ) {
            // Diffuse texture
            out = & m_pModel->m_pCurrentMaterial->texture;
            clampIndex = ObjFile::Material::TextureDiffuseType;
        } else if ( !ASSIMP_strincmp( pPtr,AmbientTexture.c_str(),AmbientTexture.size() ) ) {
            // Ambient texture
            out = & m_pModel->m_pCurrentMaterial->textureAmbient;
            clampIndex = ObjFile::Material::TextureAmbientType;
        } else if (!ASSIMP_strincmp( pPtr, SpecularTexture.c_str(), SpecularTexture.size())) {
            // Specular texture
            out = & m_pModel->m_pCurrentMaterial->textureSpecular;
            clampIndex = ObjFile::Material::TextureSpecularType;
        } else if ( !ASSIMP_strincmp( pPtr, OpacityTexture.c_str(), OpacityTexture.size() ) ) {
            // Opacity texture
            out = & m_pModel->m_pCurrentMaterial->textureOpacity;
            clampIndex = ObjFile::Material::TextureOpacityType;
        } else if (!ASSIMP_strincmp( pPtr, EmmissiveTexture.c_str(), EmmissiveTexture.size())) {
            // Emissive texture
            out = & m_pModel->m_pCurrentMaterial->textureEmissive;
            clampIndex = ObjFile::Material::TextureEmissiveType;
        } else if ( !ASSIMP_strincmp( pPtr, BumpTexture1.c_str(), BumpTexture1.size() ) ||
                    !ASSIMP_strincmp( pPtr, BumpTexture2.c_str(), BumpTexture2.size() ) || 
                    !ASSIMP_strincmp( pPtr, BumpTexture3.c_str(), BumpTexture3.size() ) ) {
            // Bump texture 
            out = & m_pModel->m_pCurrentMaterial->textureBump;
            clampIndex = ObjFile::Material::TextureBumpType;
        } else if (!ASSIMP_strincmp( pPtr,NormalTexture.c_str(), NormalTexture.size())) { 
            // Normal map
            out = & m_pModel->m_pCurrentMaterial->textureNormal;
            clampIndex = ObjFile::Material::TextureNormalType;
        } else if (!ASSIMP_strincmp( pPtr, DisplacementTexture.c_str(), DisplacementTexture.size() ) ) {
            // Displacement texture
            out = &m_pModel->m_pCurrentMaterial->textureDisp;
            clampIndex = ObjFile::Material::TextureDispType;
        } else if (!ASSIMP_strincmp( pPtr, SpecularityTexture.c_str(),SpecularityTexture.size() ) ) {
            // Specularity scaling (glossiness)
            out = & m_pModel->m_pCurrentMaterial->textureSpecularity;
            clampIndex = ObjFile::Material::TextureSpecularityType;
        } else {
            DefaultLogger::get()->error("OBJ/MTL: Encountered unknown texture type");
            return;
        }
    
        bool clamp = false;
        getTextureOption(clamp);
        m_pModel->m_pCurrentMaterial->clamp[clampIndex] = clamp;
    
        std::string strTexture;
        m_DataIt = getName<DataArrayIt>( m_DataIt, m_DataItEnd, strTexture );
        out->Set( strTexture );
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
    void ObjFileMtlImporter::getTextureOption(bool &clamp)
    {
        m_DataIt = getNextToken<DataArrayIt>(m_DataIt, m_DataItEnd);
    
        //If there is any more texture option
        while (!isEndOfBuffer(m_DataIt, m_DataItEnd) && *m_DataIt == '-')
        {
            const char *pPtr( &(*m_DataIt) );
            //skip option key and value
            int skipToken = 1;
    
            if (!ASSIMP_strincmp(pPtr, ClampOption.c_str(), ClampOption.size()))
            {
                DataArrayIt it = getNextToken<DataArrayIt>(m_DataIt, m_DataItEnd);
                char value[3];
                CopyNextWord(it, m_DataItEnd, value, sizeof(value) / sizeof(*value));
                if (!ASSIMP_strincmp(value, "on", 2))
                {
                    clamp = true;
                }
    
                skipToken = 2;
            }
            else if (  !ASSIMP_strincmp(pPtr, BlendUOption.c_str(), BlendUOption.size())
                    || !ASSIMP_strincmp(pPtr, BlendVOption.c_str(), BlendVOption.size())
                    || !ASSIMP_strincmp(pPtr, BoostOption.c_str(), BoostOption.size())
                    || !ASSIMP_strincmp(pPtr, ResolutionOption.c_str(), ResolutionOption.size())
                    || !ASSIMP_strincmp(pPtr, BumpOption.c_str(), BumpOption.size())
                    || !ASSIMP_strincmp(pPtr, ChannelOption.c_str(), ChannelOption.size())
                    || !ASSIMP_strincmp(pPtr, TypeOption.c_str(), TypeOption.size()) )
            {
                skipToken = 2;
            }
            else if (!ASSIMP_strincmp(pPtr, ModifyMapOption.c_str(), ModifyMapOption.size()))
            {
                skipToken = 3;
            }
            else if (  !ASSIMP_strincmp(pPtr, OffsetOption.c_str(), OffsetOption.size())
                    || !ASSIMP_strincmp(pPtr, ScaleOption.c_str(), ScaleOption.size())
                    || !ASSIMP_strincmp(pPtr, TurbulenceOption.c_str(), TurbulenceOption.size())
                    )
            {
                skipToken = 4;
            }
    
            for (int32 i = 0; i < skipToken; ++i)
            {
                m_DataIt = GetNextToken<DataArrayIt>(m_DataIt, m_DataItEnd);
            }
        }
    }
    
  } // namespace objmtlimporter

} // namespace model

#endif // !! ASSIMP_BUILD_NO_OBJ_IMPORTER
