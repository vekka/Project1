#ifndef _MD5MODEL_HPP_INCLUDED_
#define _MD5MODEL_HPP_INCLUDED_

#include "../core/math/vector2.hpp"
#include "../core/math/vector3.hpp"
#include "../core/math/quaternion.hpp"
#include "../core/fileio/file.hpp"

using core::fileio::File;
using core::math::Vector2f;
using core::math::Vector3f;
using core::math::Quaternion_f;

namespace model
{
	
namespace md5
{
	
struct MD5Joint
{
   char name[64];
   int32 parentID;
   Vector3f position;
   Quaternion_f orientation;
};

struct MD5Weight
{
  int32 joint;
  float bias;
  Vector3f pos;
};

struct MD5Vertex
{
  Vector2f st;
  int32 startWeight;
  int32 weightCount;
};

typedef uint32 triangle_t[3];

struct Mesh // maybe whole this should emigrate to Mesh-class?
{
   char shaderName[256];
   int32 numVertices;
   int32 numTriangles;
   int32 numWeights;
   // for bind pose
   MD5Vertex *vertices;
   triangle_t *triangles;
   MD5Weight *weights;

   //for material
   int32 textureID;

   //for rendering animated mesh
   Vector3f *positionBuffer; // should emigrate to Mesh?
   Vector3f *normalBuffer; // should emigrate to Mesh?
   Vector2f *tex2DBuffer; // should emigrate to Mesh?
   uint32 *indexBuffer;
};

// *** md5anim *** //

struct MD5JointInfo
{
  char name[64];
  int32 parent;
  int32 flags;
  int32 startIndex;
};

struct MD5BaseframeJoint
{
  Vector3f position;
  Quaternion_f orientation;
};

struct MD5BoundingBox // aabbox.hpp facilities could be useful here
{
  Vector3f min;
  Vector3f max;
};

struct MD5Anim
{
  int32 numFrames;
  int32 numJoints;
  int32 frameRate;

  MD5Joint **skeletonFrames;
  MD5BoundingBox *boundingboxes;
};

struct MD5AnimInfo
{
   int32 currentFrame;
   int32 nextFrame;
   double lastTime;
   double currentTime;
   double maxTime;
};

class MD5Mesh : File
{
private:
   Mesh *meshes;
   MD5Joint *joints;
   char *name;
   int32 numMeshes;
   int32 numJoints;
   bool isAnimated;
   int32 modelSize;//in bytes
public:
   bool LoadMesh( const char *filename );
   void ComputeVertexPositionsPerMesh( MD5Mesh *mesh, MD5Joint *sk );
   void FreeMesh();
   bool LoadAnim( const char *filename, MD5Anim *md5Anim );
   void Animate( const MD5Anim *, MD5AnimInfo *, double );
   void InterpolateSkeletons( const MD5Joint *skelA, const MD5Joint *skelB, const int32 numJoints, const float interp, MD5Joint *out );
};

} // namespace md5

} // namespace model

#endif