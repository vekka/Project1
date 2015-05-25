#include <fstream>
using std::ifstream;
using std::ios_base;

#include <iostream>
using std::cerr;
using std::endl;

#include "oglshader.hpp"

namespace oglshader
{

   GLSLShader::GLSLShader()
   {
      m_totalShaders = 0;
      m_attributeMap.clear();
      m_uniformLocationMap.clear();
      m_shaders[VERTEX_SHADER] = 0; //tmp
      m_shaders[FRAGMENT_SHADER] = 0; //tmp
      m_shaders[GEOMETRY_SHADER] = 0; //tmp
   }

   GLSLShader::~GLSLShader()
   {
      m_attributeMap.clear();
      m_uniformLocationMap.clear();
   }

   void GLSLShader::Load(GLenum type, const std::string &filename)
   {
      ifstream stream;

      stream.open(filename.c_str(), ios_base::in);
      bool isOpen = stream.is_open();
      assert(isOpen);

      if (stream)
      {
         std::string line, buffer;
         while (std::getline(stream, line))
         {
            buffer.append(line);
            buffer.append("\r\n");
         }

         GLuint shader = glCreateShader(type);

         const char *ptmp = buffer.c_str();
         glShaderSource(shader, 1, &ptmp, NULL);

         //check whether the shader loads fine
         GLint status;
         glCompileShader(shader);
         glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
         if (status == GL_FALSE)
         {
            GLint infoLogLength;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
            GLchar *infoLog = new GLchar[infoLogLength];
            glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
            cerr << "Compile log: " << infoLog << endl;
            delete[] infoLog;
         }
         m_shaders[m_totalShaders++] = shader;
      }
      else
      {
         cerr << "Error loading shader: " << filename << endl;
      }

   }

   //An indexer that returns the location of the attribute
   GLuint GLSLShader::operator[](const std::string &attribute)
   {
      return m_attributeMap[attribute];
   }

   GLuint GLSLShader::operator()(const std::string &uniform)
   {
      return m_uniformLocationMap[uniform];
   }

   void GLSLShader::AddUniform(const std::string &uniform)
   {
      m_uniformLocationMap[uniform] = glGetUniformLocation(m_program, uniform.c_str());
   }

   void GLSLShader::Use()
   {
      glUseProgram(m_program);
   }

   void GLSLShader::Unuse()
   {
      glUseProgram(0);
   }

   void GLSLShader::AddAttribute(const std::string &attribute)
   {
      m_attributeMap[attribute] = glGetAttribLocation(m_program, attribute.c_str());

   }

   void GLSLShader::CreateAndLink()
   {
      m_program = glCreateProgram();
      if (m_shaders[VERTEX_SHADER] != 0) {
         glAttachShader(m_program, m_shaders[VERTEX_SHADER]);
      }
      if (m_shaders[FRAGMENT_SHADER] != 0) {
         glAttachShader(m_program, m_shaders[FRAGMENT_SHADER]);
      }
      if (m_shaders[GEOMETRY_SHADER] != 0) {
         glAttachShader(m_program, m_shaders[GEOMETRY_SHADER]);
      }

      //link and check whether the program links fine
      GLint status;
      glLinkProgram(m_program);
      glGetProgramiv(m_program, GL_LINK_STATUS, &status);
      if (status == GL_FALSE) {
         GLint infoLogLength;

         glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);
         GLchar *infoLog = new GLchar[infoLogLength];
         glGetProgramInfoLog(m_program, infoLogLength, NULL, infoLog);
         cerr << "Link log: " << infoLog << endl;
         delete[] infoLog;
      }

      glDeleteShader(m_shaders[VERTEX_SHADER]);
      glDeleteShader(m_shaders[FRAGMENT_SHADER]);
      glDeleteShader(m_shaders[GEOMETRY_SHADER]);
   }

   void GLSLShader::DeleteProgram()
   {
      glDeleteProgram(m_program);
   }

   void GLSLShader::AddUniformData(const char* variableName, const void *_array, eVectorType type, int32 numElementsToModify)
   {
      assert(numElementsToModify > 0);

      int32 uniformHandle = m_uniformLocationMap[variableName];
      switch (type)
      {
         //uniforms for scalars, 1 in number
      case TYPE_FVEC1:
         glUniform1fv(uniformHandle, numElementsToModify, (const GLfloat*)_array);
         break;
      case TYPE_UIVEC1:
         glUniform1uiv(uniformHandle, numElementsToModify, (const GLuint*)_array);
         break;
      case TYPE_IVEC1:
         glUniform1iv(uniformHandle, numElementsToModify, (const GLint*)_array);
         break;

         //uniforms for scalars, 2 in number. But uses vector form for more than 2 scalars...
      case TYPE_FVEC2:
         glUniform2fv(uniformHandle, numElementsToModify, (GLfloat*)_array);
         break;
      case TYPE_UIVEC2:
         glUniform2uiv(uniformHandle, numElementsToModify, (GLuint*)_array);
         break;
      case TYPE_IVEC2:
         glUniform2iv(uniformHandle, numElementsToModify, (GLint*)_array);
         break;

      case TYPE_FVEC3:
         glUniform3fv(uniformHandle, numElementsToModify, (GLfloat*)_array);
         break;
      case TYPE_UIVEC3:
         glUniform3uiv(uniformHandle, numElementsToModify, (GLuint*)_array);
         break;
      case TYPE_IVEC3:
         glUniform3iv(uniformHandle, numElementsToModify, (GLint*)_array);
         break;

      case TYPE_FVEC4:
         glUniform4fv(uniformHandle, numElementsToModify, (GLfloat*)_array);
         break;
      case TYPE_UIVEC4:
         glUniform4uiv(uniformHandle, numElementsToModify, (GLuint*)_array);
         break;
      case TYPE_IVEC4:
         glUniform4iv(uniformHandle, numElementsToModify, (GLint*)_array);
         break;
      }
   }

   // assume for the sake of simplification. Assume that 4x4 float matrix is most common, but should be generalized in distant future
   void GLSLShader::AddUniformData(const char* variableName, const void *_array, eMatrixType type, int32 n, bool transposed)
   {
      // n = number of matrices to modify
      int32 uniformHandle = m_uniformLocationMap[variableName];
      // TODO: make glUniform for scalars into vector form glUniform*v, because it is easier to extend for 1, 2, 3 and 4(instead of supplying them individually)
      // note that scalars are interpreted as vectors, even just 1 float is
      switch (type)
      {
         // uniforms for matrices	
      case TYPE_FMAT2:
         glUniformMatrix2fv(uniformHandle, n, transposed, (const GLfloat*)_array);
         break;
      case TYPE_FMAT3:
         glUniformMatrix3fv(uniformHandle, n, transposed, (const GLfloat*)_array);
         break;
      case TYPE_FMAT4:
         glUniformMatrix4fv(uniformHandle, n, transposed, (const GLfloat*)_array);
         break;
      case TYPE_FMAT2x3:
         glUniformMatrix2x3fv(uniformHandle, n, transposed, (const GLfloat*)_array);
         break;
      case TYPE_FMAT2x4:
         glUniformMatrix2x4fv(uniformHandle, n, transposed, (const GLfloat*)_array);
         break;
      case TYPE_FMAT3x2:
         glUniformMatrix3x2fv(uniformHandle, n, transposed, (const GLfloat*)_array);
         break;
      case TYPE_FMAT3x4:
         glUniformMatrix3x4fv(uniformHandle, n, transposed, (const GLfloat*)_array);
         break;
      case TYPE_FMAT4x2:
         glUniformMatrix4x2fv(uniformHandle, n, transposed, (const GLfloat*)_array);
         break;
      case TYPE_FMAT4x3:
         glUniformMatrix4x3fv(uniformHandle, n, transposed, (const GLfloat*)_array);
         break;
      }
   }

   //void GLSLShader::GetCompilationStatus(std::string &outStatus) const
   //{
   //   GLint status;
   //   glGetShaderiv( handle, GL_COMPILE_STATUS, &status );
   //   if( status == 0 )
   //   {
   //      char *strShaderType;
   //      char *statusLogString;
   //      GLint statusLogLen;
   //
   //      glGetShaderiv( handle, GL_INFO_LOG_LENGTH, &statusLogLen );
   //      statusLogString = new char[statusLogLen+1];
   //
   //      glGetShaderInfoLog( handle, statusLogLen, NULL, statusLogString );
   //      strShaderType = NULL;
   //      switch( type )
   //      {
   //      case GL_VERTEX_SHADER: strShaderType = "vertex";
   //         break;
   //      case GL_GEOMETRY_SHADER: strShaderType = "geometry";
   //         break;
   //      case GL_FRAGMENT_SHADER: strShaderType = "fragment";
   //         break;
   //      }
   //
   //      outStatus = strShaderType;
   //      outStatus.Append( ": " );
   //      outStatus.Append( statusLogString );
   //      outStatus.Append( "\n" );
   //
   //      delete[] statusLogString;
   //   }
   //}

   /////////////// -- class ShaderExecutable -- ///////////////
   //
   //ShaderExecutable::ShaderExecutable()
   //{
   //
   //}
   //
   //ShaderExecutable::~ShaderExecutable()
   //{
   //
   //}
   //
   //bool ShaderExecutable::Create()
   //{
   //   handle = glCreateProgram();
   //   if( handle == 0 )
   //      return false;
   //   return true;
   //}
   //
   //void ShaderExecutable::AddUniformLocation( const char *name )
   //{
   //	uint32 temp = glGetUniformLocation(handle, name);
   //	//assert(uniformHandle != -1);
   //   assert(temp != GL_INVALID_VALUE);
   //   assert(temp != GL_INVALID_OPERATION);
   //   handleMap[name] = temp;
   //	//uniformHandles.push_back(loc);
   //}
   //
   //void ShaderExecutable::Attach( const uint32 shaderHandle ) const
   //{
   //   glAttachShader( handle, shaderHandle );
   //}
   //
   //void ShaderExecutable::Detach( const uint32 shaderHandle ) const
   //{
   //   glDetachShader( handle, shaderHandle );
   //}
   //
   //void ShaderExecutable::Link() const
   //{
   //   glLinkProgram( handle );
   //}
   //
   //uint32 ShaderExecutable::GetHandle() const
   //{
   //   return handle;
   //}
   //

   //
   //void ShaderExecutable::AddUniformSampler(const char* variableName, eOpaqueType sampler)
   //{
   //	//int32 uniformHandle = handleMap[variableName];
   //	//switch (sampler)
   //	//{
   //	//case TYPE_SAMPLER_1D:
   //	//case TYPE_SAMPLER_2D:
   //	//case TYPE_SAMPLER_3D:
   //	//case TYPE_SAMPLER_1D_ARRAY:
   //	//case TYPE_SAMPLER_2D_ARRAY:
   //	//	//TODO: bind textures to shader
   //	//	glActiveTexture(GL_TEXTURE0 + ident->order);
   //	//	glBindTexture(((GLBTexture*)val)->target, ((GLBTexture*)val)->globj);
   //	//	glUniform1i(ident->location, ident->order); //TODO: order might collide between shaders
   //	//	break;
   //	//default:
   //	//	return GLB_UNIMPLEMENTED;
   //	//}
   //}

} // namespace oglshader