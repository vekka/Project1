#include "objLoader.hpp"

using core::string::String_c;

bool MtlFile::Read()
{
   assert( isOpen );

   vector<String_c> tokens;
   String_c token, lineC;
   int newmtlCount = -1;
   bool materialOpen = true;
   //materials = NULL;

   while( ReadLine(lineC) )
	{
	   lineC.Tokenize(tokens);

      // skip comments
      if( tokens[0] == NULL || tokens[0] == '#' )
			continue;
      
      else if ( tokens[0] == "newmtl" ) // material
      {
         Material newMtl;
         materialOpen = true;
         newmtlCount++;        
       
         newMtl.SetAmbientVec( Vector3f( 0.2f, 0.2f, 0.2f) );
         newMtl.SetDiffuseVec( Vector3f( 0.8f, 0.8f, 0.8f ) );
         newMtl.SetSpecularVec( Vector3f( 1.0f, 1.0f, 1.0f ) );
         newMtl.SetReflection( 0.0f );
         newMtl.SetTransparency( 1.0f );
         newMtl.SetGloss( 98.0f );
         newMtl.SetShine( 0.0f );
         newMtl.SetRefractIndex( 1.0f );
         newMtl.SetRefract( 0.0f );

         newMtl.SetMaterialName( tokens[1] );

         materials.push_back( newMtl );
      }
      else if ( tokens[0] == "Ka" && materialOpen ) // ambient
      {
         Vector3f temp;

         temp[0] = tokens[1].StringToFloat();
         temp[1] = tokens[2].StringToFloat();
         temp[2] = tokens[3].StringToFloat();      
         materials[newmtlCount].SetAmbientVec( temp );
      }
      else if ( tokens[0] == "Kd" && materialOpen ) // diffuse
      {
         Vector3f temp;
         temp[0] = tokens[1].StringToFloat();
         temp[1] = tokens[2].StringToFloat();
         temp[2] = tokens[3].StringToFloat();      
         materials[newmtlCount].SetDiffuseVec( temp );
      }
      else if ( tokens[0] == "Ks" && materialOpen ) // specular
      {
         Vector3f temp;
         temp[0] = tokens[1].StringToFloat();
         temp[1] = tokens[2].StringToFloat();
         temp[2] = tokens[3].StringToFloat();      
         materials[newmtlCount].SetSpecularVec( temp );
      }
      else if ( tokens[0] == "Ns" && materialOpen ) // shine
      {
         float t = tokens[1].StringToFloat();
         materials[newmtlCount].SetShine( tokens[1].StringToFloat() );
      }
		else if( tokens[0] == "d" && materialOpen ) // transparency
		{
         materials[newmtlCount].SetTransparency( tokens[1].StringToFloat() );
		}
      else if( tokens[0] == "r" && materialOpen ) // reflection
		{
         materials[newmtlCount].SetReflection( tokens[1].StringToFloat() );
		}
		else if( tokens[0] == "Ni" && materialOpen ) //refract index
		{
			materials[newmtlCount].SetRefractIndex( tokens[1].StringToFloat() );
		}
		else if( tokens[0] == "illum" && materialOpen ) // illumination type
		{
         // type of lighting technique ??
		}
		else if( tokens[0] == "map_Kd" && materialOpen ) // texture map
		{
         String_c textureFileName = tokens[1];
         //must remove /n character
         textureFileName = textureFileName( 0, textureFileName.GetSize()-1  );
         materials[newmtlCount].SetTextureFileName( textureFileName );
		}
		//else
  //       cout << "Unknown tag in material file ! " << endl;  

      tokens.clear();
   }

   return true;
}

ObjFile::ObjFile()
{
}

void ObjFile::SetMaterialFilePath( String_c inPath )
{
   this->materialFilePath = inPath;
}

bool ObjFile::Read()
{
   assert( isOpen );

   if (this->GetSize() == 0)
      return false;
   String_c buffer;
   vector<String_c> tokens;
   String_c token;

   int currentGroupIndex = -1;
 
   while ( ReadLine(buffer) )
   {
      buffer.Tokenize( tokens );

      if (tokens[0] == "#" ) {}
      else if( tokens[0] == "v"  )
      {
         Vector3f vtemp;
         vtemp[0] = tokens[1].StringToFloat();
         vtemp[1] = tokens[2].StringToFloat();
         vtemp[2] = tokens[3].StringToFloat();
         mesh.AddVertex( vtemp );
      }
      else if( tokens[0] == "vn" )
      {
         Vector3f vntemp;
         vntemp[0] = tokens[1].StringToFloat();
         vntemp[1] = tokens[2].StringToFloat();
         vntemp[2] = tokens[3].StringToFloat();
         mesh.AddVertexNormal(vntemp);
      }
      else if( tokens[0] == "g" )
      {			      
         //Group newGroup;
         //currentGroupIndex++;
         //newGroup.groupName = tokens[1];
         //newGroup.groupNum = currentGroupIndex;
     
      }
      else if( tokens[0] == "mtllib" )
      {
         //Material file is listed as file name only in obj file:
         // "mtllib castle.mtl" For example. Therefore need to specify path to this file location. Smarter solution probably exists
         

         //create new file, fill it with materials and put it into material list
         MtlFile newMtlFile;
         String_c materialFileLocation = materialFilePath + tokens[1]( 0, tokens[1].GetSize() - 1 );

         newMtlFile.Open( materialFileLocation );         
         newMtlFile.Read();
         newMtlFile.Close();

         MtlFileList.push_back( newMtlFile );
     
      }
      else if( tokens[0]== "vt" )
      {
         if (!mesh.HasTexture2())
            mesh.ToggleHasTexture2();

         Vector2f vttemp;
         vttemp[0] = tokens[1].StringToFloat();
         vttemp[1] = tokens[2].StringToFloat();
  
         mesh.AddVertexTexture2( vttemp );
      }
      else if( tokens[0] == "f" )
      {
         //triangular face:
         String_c test;
         Face32 newFace;
         //need vertexIndexCount
         int32 vertexIndexCount = 0;

         //3 attributes
         //f 1/2/3 4/5/6 7/8/9

         //2 attributes( V + N )
         //f 1//2 3//4 5//6 

         //2 attributes( V + U )
         //f 1/2 3/4 5/6

         //1 attribute( V )
         //f 1 2 3


         for ( int i = 1; i < newFace.GetNumVertices() + 1; i++ )
         {
            int idx;
            //newFace.vertexIndex[vertexIndexCount] = tokens[i].StringToInt();

            newFace.AddVertexIndex( tokens[i].StringToInt() );
            
            if ((idx = tokens[i].FindFirst('/')) != -1)
            {
               if (tokens[i][idx+1] == '/')
               {
                  tokens[i] = tokens[i](idx+2);
                  //newFace.normalIndex[vertexIndexCount] = tokens[i].StringToInt();
                  newFace.AddNormalIndex( tokens[i].StringToInt() );
               }
               else if (core::IsADigit(tokens[i][idx+1]))
               {
                  tokens[i] = tokens[i](idx+1);
                  //newFace.textureIndex[vertexIndexCount] = tokens[i].StringToInt();
                  newFace.AddTextureIndex( tokens[i].StringToInt() );
                  if ((idx = tokens[i].FindFirst('/')) != -1)
                  {
                     tokens[i] = tokens[i](idx+1);
                     //newFace.normalIndex[vertexIndexCount] = tokens[i].StringToInt();
                     newFace.AddNormalIndex( tokens[i].StringToInt() );
                  }
               }
            }

            //vertexIndexCount++;
         }


         //if ( tokens[i+1][1] == '/' )


         //newFace.vertexIndex[vertexIndexCount] = tokens[i+1].StringToInt();


         //ParseIndices( newFace.vertexIndex,newFace.normalIndex, 
         //   newFace.textureIndex, test );

         mesh.AddFace( newFace );
         //if( currentGroupIndex > 0 )
         // this->groupsMap[currentGroupIndex].numFaces++;
      }
     tokens.clear();
   }//WHILE

   return true; 
}

//void ObjFile::AddMaterial( Material mat )
//{
//   this->materials.
//}