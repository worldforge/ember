#include "SBAssetHandlerOgre.h"
#include <vhcl/vhcl.h>
#include <boost/version.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/algorithm/string.hpp>
#include <sb/SBMotion.h>
#include <sb/SBScene.h>
#include <sb/SBSkeleton.h>
#include <sb/SBJoint.h>
#include <sb/SBAssetManager.h>
#include <sbm/ParserOgre.h>
#ifdef EMBER_SB_GPU
	#include <sbm/GPU/SbmDeformableMeshGPU.h>
#endif

namespace SmartBody {

SBAssetHandlerOgre::SBAssetHandlerOgre()
{
	assetTypes.push_back("xml");
}

SBAssetHandlerOgre::~SBAssetHandlerOgre()
{
}

std::vector<SBAsset*> SBAssetHandlerOgre::getAssets(const std::string& path)
{
	std::vector<SBAsset*> assets;

	std::string convertedPath = checkPath(path);
	if (convertedPath == "")
		return assets;

	boost::filesystem::path p(convertedPath);
	std::string fileName = boost::filesystem::basename( p );
	std::string extension =  boost::filesystem::extension( p );

	SmartBody::SBSkeleton* skeleton = new SmartBody::SBSkeleton();			
	skeleton->skfilename(convertedPath.c_str());
	skeleton->setName(fileName + extension);

	double scale = 1.0;
	if (SmartBody::SBScene::getScene()->getAttribute("globalSkeletonScale"))
		scale = SmartBody::SBScene::getScene()->getDoubleAttribute("globalSkeletonScale");

	// is this a .skeleton.xml or a .mesh.xml file?
	if (convertedPath.find(".skeleton.xml") != std::string::npos)
	{
		std::vector<SmartBody::SBMotion*> motions;
		bool ok = ParserOgre::parse(*skeleton, motions, convertedPath, float(scale), true, true);

		if (ok)
		{
			assets.push_back(skeleton);
			for (size_t m = 0; m < motions.size(); m++)
			{
				assets.push_back(motions[m]);
			}
		}
	}
	else if (convertedPath.find(".mesh.xml") != std::string::npos)
	{
		// parse the mesh and skinweights in a separate pass (should be collapsed into only one pass...)
#if defined(EMBER_SB_GPU) && !defined (__ANDROID__) && !defined(SB_IPHONE) &&  !defined(__FLASHPLAYER__) && !defined(__native_client__)
		SbmDeformableMeshGPU* mesh = new SbmDeformableMeshGPU();
#else
		DeformableMesh* mesh = new DeformableMesh();
#endif
		std::vector<SrModel*> meshModelVec;
		std::vector<SkinWeight*> tempWeights;
		std::string skeletonName;
		bool ok = ParserOgre::parseSkinMesh(meshModelVec, mesh->skinWeights, convertedPath, skeletonName, 1.0, true, true);		
		if (ok)
		{
			SmartBody::SBSkeleton* skeleton = NULL;
			if (skeletonName != "")
			{
				// find the skeleton
				skeleton = SmartBody::SBScene::getScene()->getAssetManager()->getSkeleton(skeletonName);
				if (skeleton)
				{
					for (unsigned int i=0; i< mesh->skinWeights.size(); i++)
					{
						SkinWeight* sw = mesh->skinWeights[i];
						
						for (int k=0; k < skeleton->getNumJoints(); k++)
						{
							// manually add all joint names
							SmartBody::SBJoint* joint = skeleton->getJoint(k);
							sw->infJointName.push_back(joint->getName());
							sw->infJoint.push_back(joint);
							SrMat gmatZeroInv = joint->gmatZero().rigidInverse();						
							sw->bindPoseMat.push_back(gmatZeroInv);
						}
					}
				}

				if (skeleton)
				{
					for (size_t s = 0; s < mesh->skinWeights.size(); s++)
					{
						for (size_t i = 0; i < mesh->skinWeights[s]->jointNameIndex.size(); i++)
						{
							SmartBody::SBJoint* joint = skeleton->getJoint(mesh->skinWeights[s]->jointNameIndex[i]);
							mesh->skinWeights[s]->infJointName.push_back(joint->getName());
						}
					}
				}

			}
			
			mesh->setName(fileName + extension);
			for (unsigned int i = 0; i < meshModelVec.size(); i++)
			{
				//for (int j = 0; j < meshModelVec[i]->V.size(); j++)
				//{
				//	meshModelVec[i]->V[j] *= factor;
				//}
		
				if (meshModelVec[i]->Fn.size() == 0)
				{
					meshModelVec[i]->computeNormals();
				}

				SrSnModel* srSnModelStatic = new SrSnModel();
				srSnModelStatic->shape(*meshModelVec[i]);
				srSnModelStatic->shape().name = meshModelVec[i]->name;
				mesh->dMeshStatic_p.push_back(srSnModelStatic);
				srSnModelStatic->ref();

				SrSnModel* srSnModelDynamic = new SrSnModel();
				srSnModelDynamic->shape(*meshModelVec[i]);
				srSnModelDynamic->changed(true);
				srSnModelDynamic->visible(false);
				srSnModelDynamic->shape().name = meshModelVec[i]->name;
				mesh->dMeshDynamic_p.push_back(srSnModelDynamic);
				srSnModelDynamic->ref();
			
				delete meshModelVec[i];
			}
			assets.push_back(mesh);
		}
	}

	return assets;
}

};
