#ifndef _OBJLOADER_HPP_INCLUDED_
#define _OBJLOADER_HPP_INCLUDED_

#include <map>
#include <vector>

#include <iostream>

#include "core/math/vector3.hpp"
#include "core/math/vector2.hpp"
#include "mesh.hpp"
using mesh::Mesh32;
using mesh::Face32;

#include "core/string/string.hpp"

using core::math::Vector2f;
using core::math::Vector3f;

#define NAME_LENGTH_MAX 512
using namespace std;

class Material;
class Group;

class MtlFile : public File
{
private:
   vector<Material> materials;
public:
   bool Read();
};

class Group
{
public:
private:
   String_c groupName;
   //MtlFile material;
   String_c materialName;

   int32 groupNum;
   int32 numFaces;
};

class Material
{
private:
   String_c materialName;
   String_c textureFilename;
   String_c groupName;

   Vector3f ambientVec, diffuseVec, specularVec;
   float reflection;
   float refract;
   float transparency;
   float shine;
   float gloss;
   float refractIndex;

public:
   void SetAmbientVec( const Vector3f inAmb )
   {
      this->ambientVec = inAmb;
   }
   
   void SetDiffuseVec( const Vector3f inDiff )
   {
      this->diffuseVec = inDiff;
   }
   
   void SetSpecularVec( const Vector3f inSpec )
   {
      this->specularVec = inSpec;
   }
   
   void SetReflection( const float reflection )
   {
      this->reflection = reflection;
   }
   
   void SetTransparency( const float trans )
   {
      this->transparency = trans;
   }
   
   void SetShine( const float shine )
   {
      this->shine = shine;
   }
   
   void SetGloss( const float gloss )
   {
      this->gloss = gloss;
   }
   
   void SetRefractIndex( const float refractIndex )
   {
      this->refractIndex = refractIndex;
   }
   
   void SetRefract( const float refract )
   {
      this->refract = refract;
   }

   void SetMaterialName( const String_c name )
   {
      materialName = name;
   }

   void SetTextureFileName( const String_c texname )
   {
      textureFilename = texname;
   }
};

class ObjFile : public File
{
private:
   
   //String_c objectName;
   ////string mtlFilename;
   //int numMaterials;
   /*int numFaces, numVertices, numGroups;*/
   vector<MtlFile> MtlFileList;
   String_c materialFilePath;
public:
   Mesh32 mesh; // assume the PNT for obj file for the time being

   ObjFile();
   void SetMaterialFilePath( String_c path );
   bool Read();
};


#endif