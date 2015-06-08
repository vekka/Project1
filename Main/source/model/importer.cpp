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

#include "importer.hpp"
#include "core/memory/scopedptr.hpp"


//namespace baseimporter
//{
//
//   BaseImporter::BaseImporter()
//  
//   {
//      // nothing to do here
//   }
//
//   // ------------------------------------------------------------------------------------------------
//   // Destructor, private as well
//   BaseImporter::~BaseImporter()
//   {
//      // nothing to do here
//   }
//
//   Scene* BaseImporter::ReadFile(const Importer* pImp, const std::string& pFile, File* pIOHandler)
//   {
//      //progress = pImp->GetProgressHandler();
//      //assert(progress);
//
//      // Gather configuration properties for this run
//      //SetupProperties(pImp);
//
//      // Construct a file system filter to improve our success ratio at reading external files
//      //FileSystemFilter filter(pFile, pIOHandler);
//
//      // create a scene object to hold the data  
//      ScopeGuard<Scene> sc(new Scene() );
//   
//      // dispatch importing
//      try
//      {
//        InternReadFile(pFile, sc, pIOHandler);
//      }
//      catch (const std::exception& err)	
//      {
//      //   // extract error description
//      //   mErrorText = err.what();
//      //   DefaultLogger::get()->error(mErrorText);
//         return NULL;
//      }
//
//      // return what we gathered from the import. 
//      sc.dismiss();
//      return sc;
//   }
//
//}

namespace importer
{

   Importer::Importer()
   {
   //   // allocate the pimpl first
   //   pimpl = new ImporterPimpl();

   //   pimpl->mScene = NULL;
   //   pimpl->mErrorString = "";

   //   // Allocate a default IO handler
   //   pimpl->mIOHandler = new DefaultIOSystem;
   //   pimpl->mIsDefaultHandler = true;
   //   pimpl->bExtraVerbose = false; // disable extra verbose mode by default

   //   pimpl->mProgressHandler = new DefaultProgressHandler();
   //   pimpl->mIsDefaultProgressHandler = true;

   //   GetImporterInstanceList(pimpl->mImporter);
   //   GetPostProcessingStepInstanceList(pimpl->mPostProcessingSteps);

   //   // Allocate a SharedPostProcessInfo object and store pointers to it in all post-process steps in the list.
   //   pimpl->mPPShared = new SharedPostProcessInfo();
   //   for (std::vector<BaseProcess*>::iterator it = pimpl->mPostProcessingSteps.begin();
   //      it != pimpl->mPostProcessingSteps.end();
   //      ++it)	{

   //      (*it)->SetSharedData(pimpl->mPPShared);
   //   }
   }


   Scene* Importer::ReadFile(const std::string& pFile, core::fileio::File* file)
         {
 
            // create a scene object to hold the data  
            ScopeGuard<Scene> sc(new Scene() );
         
            try
            {
              objFile.InternReadFile(pFile, sc, file);
            }
            catch (const std::exception& err)	
            {
            //   // extract error description
            //   mErrorText = err.what();
            //   DefaultLogger::get()->error(mErrorText);
               return NULL;
            }
      
            // return what we gathered from the import. 
            sc.dismiss();
            return sc;
         }

} // namespace importer