#include "Ogre.h"
#include "GroundCover.h"

String GroundCover::MESH_NAME = "big_foliage_mesh";
String GroundCover::NODE_NAME = "big_foliage_node";
String GroundCover::ENTITY_NAME = "big_foliage_entity";

GroundCover::GroundCover(SceneManager* manager,const Vector3& size,int sizeOfABlock, const Vector3 center)
{
	_manager = manager;
	_big_entity = NULL;
	_big_mesh = NULL;
	_big_node = NULL;
	_listener = NULL;
	_size = size;
	_x_blocks = (int) size.x / sizeOfABlock;
	_z_blocks = (int) size.z / sizeOfABlock;
	
	_center = center;

	Real x_size = sizeOfABlock;
	Real z_size = sizeOfABlock;

	// Default to near clip of 2 blocks, far clip of half of the map, FOV of 120 degrees
	setCullParameters(std::max(x_size,z_size) * 2.0,std::max(_size.x,_size.z) * 0.5,120.0);
}

// Add a mesh at the given position, with high and low LODs
void GroundCover::add(const Vector3& position,const String& high_mesh,const String& low_mesh)
{
	// See if we've already loaded the mesh
	Mesh* mesh = (Mesh*)MeshManager::getSingleton().getByName(high_mesh);
	if(!mesh)
	{
		// If not, load the high resolution version and set the low version as an LOD
		mesh = MeshManager::getSingleton().load(high_mesh);
        mesh->createManualLodLevel(_max_dist,low_mesh); 
	}
	
	// Remember these nodes so we can compile them later
	_items.push_back(std::pair<Vector3,Mesh*>(position,mesh));
}

// Cull and apply LODs depending on where the camera is
void GroundCover::update(Camera* camera)
{
	// Switch on/off the low LOD sub-meshes (i.e. the sub-entities) depending on where the camera is
	for(std::map<unsigned int,GroundCover::Block*>::iterator b = _blocks.begin();b != _blocks.end();b++)
	{
		// Reconfigure the high LOD mesh blocks near the camera
		if(b->second->cull(camera,_max_dist,_min_dist,_cos_view_angle,_listener))
		{
			GroundCover::Detail* det = b->second->getDetail();

			// If we're not already using a detail block...
			if(!det)
			{
				// Find an unused detail one
				for(std::vector<GroundCover::Detail*>::iterator i = _details.begin();i != _details.end();i++)
				{
					if(!(*i)->isInUse())
					{
						det = (*i);
						break;
					}
				}
			
				// Or create a new one
				if(!det)
				{
					det = new GroundCover::Detail(_manager,(int)_details.size());
					_details.push_back(det);
				}

				// Reconfigure the detail for the block that needs it
				b->second->reconfigureDetail(det,_listener);
			}
		}
	}
}

void GroundCover::setCullParameters(Real near,Real far,Real view_angle)
{
	_min_dist = near;
	_max_dist = far;
	_cos_view_angle = cosf(((view_angle * 0.5) * 3.14159) / 180.0);
}

// Compile all the items in the list into the single big mesh/node/entity
void GroundCover::compile()
{
	LogManager::getSingleton().logMessage(LML_NORMAL,"Items to compile = %u",_items.size());

	// Run through all the potential blocks in the system looking for
	// items that we've added to the _items list
	Real x_size = _size.x / (Real)_x_blocks;
	Real z_size = _size.z / (Real)_z_blocks;
	for(unsigned int z = 0;z < _z_blocks;z++)
	{
		for(unsigned int x = 0;x < _x_blocks;x++)
		{
			
			// Work out the position of the block
			Real x_position = (x_size * x) - (_size.x / 2) + (_center.x);
			Real z_position = (z_size * z) - (_size.x / 2) + (_center.z);
			Vector3 block_pos(x_position ,0.0,z_position);

			// Check all the added items
			for(std::vector<std::pair<Vector3,Mesh*> >::iterator i = _items.begin();i != _items.end();i++)
			{
				// If it's within this block
				Vector3 node_pos = i->first;
				if((node_pos.x >= block_pos.x) &&
				   (node_pos.x < block_pos.x + x_size) &&
				   (node_pos.z >= block_pos.z) &&
				   (node_pos.z < block_pos.z + z_size))
				{
					// Try to find this block
					unsigned int key = (((x & 0xFFFF) << 16)|(z & 0xFFFF));
					std::map<unsigned int,GroundCover::Block*>::iterator m = _blocks.find(key);
					if(m == _blocks.end())
					{
						// Create a new one
						_blocks[key] = new GroundCover::Block(key,Vector3(block_pos.x + (x_size * 0.5),block_pos.y,block_pos.z + (z_size * 0.5)));
					}
					// Add the item to the existing block
					_blocks[key]->add(i->first,i->second);
				}
			}
		}
	}

	LogManager::getSingleton().logMessage(LML_NORMAL,"Total blocks = %u",_blocks.size());

	// Bin the big stuff if it already exists
	if(_big_node) _manager->getRootSceneNode()->removeAndDestroyChild(_big_node->getName());
	if(_big_entity) _manager->removeEntity(_big_entity->getName());
	if(_big_mesh) MeshManager::getSingleton().unload(_big_mesh);
		
	// Build the low LOD versions of all the blocks into the giant foliage mesh
	_big_mesh = MeshManager::getSingleton().createManual(GroundCover::MESH_NAME);
	for(std::map<unsigned int,GroundCover::Block*>::iterator b = _blocks.begin();b != _blocks.end();b++)
	{
		b->second->build(_big_mesh);
	}
	_big_mesh->_setBounds(AxisAlignedBox(Vector3::ZERO,_size));
	_big_mesh->load();
	_big_mesh->touch();

	_big_entity = _manager->createEntity(GroundCover::ENTITY_NAME,GroundCover::MESH_NAME);
	_big_entity->setCastShadows(false);
	_big_entity->setRenderQueueGroup(RENDER_QUEUE_7);

	_big_node = _manager->getRootSceneNode()->createChildSceneNode(GroundCover::NODE_NAME);
	_big_node->attachObject(_big_entity);

	// Set the blocks sub-entities from the sub-meshes, so we can switch them on and off
	for(std::map<unsigned int,GroundCover::Block*>::iterator b = _blocks.begin();b != _blocks.end();b++)
	{
		b->second->setSubEntities(_big_entity);
	}

	// Clear the list of items added
	_items.clear();
}

GroundCover::~GroundCover()
{
 	LogManager::getSingleton().logMessage(LML_NORMAL,"Using %u detail blocks",_details.size());

	for(std::map<unsigned int,GroundCover::Block*>::iterator m = _blocks.begin();m != _blocks.end();m++)
	{
		delete m->second;
	}

	for(std::vector<GroundCover::Detail*>::iterator d = _details.begin();d != _details.end();d++)
	{
		delete (*d);
	}

	// Bin the big stuff if it exists
	if(_big_node) _manager->getRootSceneNode()->removeAndDestroyChild(_big_node->getName());
	if(_big_entity) _manager->removeEntity(_big_entity->getName());
	if(_big_mesh)
	{
		MeshManager::getSingleton().unload(_big_mesh);
		delete _big_mesh;
	}
}

GroundCover::Block::Block(unsigned int key,const Vector3& center)
{
	_key = key;
	_visible = true;
	_center = center;
	_detail = NULL;
}

void GroundCover::Block::add(const Vector3& position,Mesh* mesh)
{
	// String mesh_name = mesh->getName();
	// LogManager::getSingleton().logMessage(LML_NORMAL,"Adding item %s to block at (%f,%f,%f), using mesh %s",node->getName().c_str(),_center.x,_center.y,_center.z,mesh_name.c_str());

	// Stick the mesh into this block
	_meshes.push_back(std::pair<Vector3,Mesh*>(position,mesh));
}

bool GroundCover::Block::cull(Camera* camera,Real max_dist,Real min_dist,Real cos_view_angle,GroundCover::DetailListener* listener)
{
	// Do we need a detail block creating
	bool rc = false;

	// Switch the block's sub-entities off if the camera's too far away
	Vector3 camPos = camera->getDerivedPosition();
	Vector3 toCamera(_center.x - camPos.x,0,_center.z - camPos.z);
	Real sqrlen = toCamera.squaredLength();
	Real sqrmin;
	if(sqrlen > (max_dist * max_dist)) goto jump_hide;

	// Switch the block off if it's behind the camera and further away than the minimum distance
	sqrmin = min_dist * min_dist;
	if(sqrlen > sqrmin)
	{
		toCamera.normalise();
		Real dot = toCamera.dotProduct(camera->getDirection());
		if(dot < cos_view_angle) goto jump_hide;
	}

	// Hide it and return true if it's too close, so the caller will build a high-detail version
	if(sqrlen < sqrmin)
	{
		rc = true;
		goto jump_hide;
	}

	// By default, show the block
	if(!_visible)
	{
		_visible = true;
		for(std::vector<SubEntity*>::iterator se = _subentities.begin();se != _subentities.end();se++) (*se)->setVisible(true);
	}
	return rc; 

	// Hide the block
jump_hide:
	if(_visible)
	{
		_visible = false;
		for(std::vector<SubEntity*>::iterator se = _subentities.begin();se != _subentities.end();se++) (*se)->setVisible(false);
	}
	// If we don't need a detail block then free any we're currently using
	if(!rc)
	{
		if(_detail)
		{
			_detail->setInUse(false,listener);
			_detail = NULL;
		}
	}
	return rc; 
}

void GroundCover::Block::setSubEntities(Entity* entity)
{
	// Calculate which sub-entities are using our sub-mesh so we can switch them on or off
	unsigned int c = entity->getNumSubEntities();
	for(unsigned int i = 0;i < c;i++)
	{
		SubEntity* se = entity->getSubEntity(i);
		for(std::vector<SubMesh*>::iterator sm = _submeshes.begin();sm != _submeshes.end();sm++)
		{
			if((*sm) == se->getSubMesh()) _subentities.push_back(se);
		}		
	}
}

void GroundCover::Block::build(Mesh* mesh)
{
	// Clear the list of sub-entities and sub-meshes
	_subentities.clear();
	_submeshes.clear();

	// Loop through all the meshes in this block, and all the sub-meshes
	// thereof, building a list of all the different materials. We'll need
	// to create one sub-mesh for each material
	std::map<String,int> materialList;
	for(std::vector<std::pair<Vector3,Mesh*> >::iterator i = _meshes.begin();i != _meshes.end();i++)
	{
		if(i->second->getNumLodLevels() > 1)
		{
			Mesh* lod = i->second->getLodLevel(1).manualMesh;
			if(lod)
			{
				unsigned short subs = lod->getNumSubMeshes();
				for(unsigned short s = 0;s < subs;s++)
				{
					String mat = lod->getSubMesh(s)->getMaterialName();
					int cnt = 0;
					if(materialList.find(mat) != materialList.end()) cnt = materialList[mat];
					materialList[mat] = cnt + 1;
				}
			}
		}
	}

	// Loop through all the materials we found...
	for(std::map<String,int>::iterator j = materialList.begin();j != materialList.end();j++)
	{
		// LogManager::getSingleton().logMessage(LML_NORMAL,"Block %u uses material %s %i times",_key,j->first.c_str(),j->second);

		size_t vrt_count = 0;
		size_t idx_count = 0;

		// First pass, add up all the vertices and all the indices in all the submeshes using this material
		for(std::vector<std::pair<Vector3,Mesh*> >::iterator i = _meshes.begin();i != _meshes.end();i++)
		{
			if(i->second->getNumLodLevels() > 1)
			{
				Mesh* lod = i->second->getLodLevel(1).manualMesh;
				if(lod)
				{
					unsigned short subs = lod->getNumSubMeshes();
					for(unsigned short s = 0;s < subs;s++)
					{
						SubMesh* sm = lod->getSubMesh(s);
						if(sm->getMaterialName() == j->first)
						{
							idx_count += (sm->indexData)?sm->indexData->indexCount:0;
							vrt_count += (sm->useSharedVertices)?lod->sharedVertexData->vertexCount:(sm->vertexData)?sm->vertexData->vertexCount:0;
						}
					}
				}
			}
		}

		// Next pass, actually create the submesh for this material and copy over the vertices and indices
		// LogManager::getSingleton().logMessage(LML_NORMAL,"We'll need %u vertices and %u indices for this mesh",vrt_count,idx_count);

		SubMesh* submesh = mesh->createSubMesh();
		_submeshes.push_back(submesh);
		
		submesh->useSharedVertices = false;
		submesh->vertexData = new VertexData();
		submesh->setMaterialName(j->first);

		unsigned int idxOffs = 0;
		unsigned int vrtOffs = 0;

	    // Set up Vertex Declaration
		VertexDeclaration* vertexDecl = submesh->vertexData->vertexDeclaration;
		size_t currOffset = 0;

		// We always need positions
		vertexDecl->addElement(0, currOffset, VET_FLOAT3, VES_POSITION);
		currOffset += VertexElement::getTypeSize(VET_FLOAT3);
       
		// Normals
//		vertexDecl->addElement(0, currOffset, VET_FLOAT3, VES_NORMAL);
//		currOffset += VertexElement::getTypeSize(VET_FLOAT3);

		// 2D texture coords
		vertexDecl->addElement(0, currOffset, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);
		currOffset += VertexElement::getTypeSize(VET_FLOAT2);

		// Number of vertices
		submesh->vertexData->vertexCount = vrt_count;

		// Allocate vertex buffer
		HardwareVertexBufferSharedPtr vrtBuf = 
			HardwareBufferManager::getSingleton().
				createVertexBuffer(vertexDecl->getVertexSize(0), submesh->vertexData->vertexCount,
					HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
 
		// Set up the binding (one source only)
		VertexBufferBinding* binding = submesh->vertexData->vertexBufferBinding;
		binding->setBinding(0, vrtBuf);

		// Lock the buffer so we can write to it
		unsigned char* vrtPtr = static_cast<unsigned char*>(vrtBuf->lock(HardwareBuffer::HBL_READ_ONLY));

		// Create the index data
		submesh->indexData->indexCount = idx_count;
		submesh->indexData->indexBuffer = HardwareBufferManager::getSingleton().
			createIndexBuffer(HardwareIndexBuffer::IT_16BIT,
				submesh->indexData->indexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);

		// Get the index buffer and lock it
		HardwareIndexBufferSharedPtr idxBuf = submesh->indexData->indexBuffer;
		unsigned short* idxPtr = static_cast<unsigned short*>(idxBuf->lock(HardwareBuffer::HBL_DISCARD) );

		// Loop through the meshes again, copying the data across
		for(std::vector<std::pair<Vector3,Mesh*> >::iterator i = _meshes.begin();i != _meshes.end();i++)
		{
			if(i->second->getNumLodLevels() > 1)
			{
				Mesh* lod = i->second->getLodLevel(1).manualMesh;
				if(lod)
				{
					unsigned short subs = lod->getNumSubMeshes();
					for(unsigned short s = 0;s < subs;s++)
					{
						SubMesh* sm = lod->getSubMesh(s);
						if(sm->getMaterialName() == j->first)
						{
							copyIndexData(sm->indexData,submesh->indexData,idxPtr,&idxOffs,vrtOffs);
							copyVertexData(i->first,(sm->useSharedVertices)?lod->sharedVertexData:sm->vertexData,submesh->vertexData,vrtPtr,&vrtOffs);

							// LogManager::getSingleton().logMessage(LML_NORMAL,"Vert offset = %u, Idx offset = %u",vrtOffs,idxOffs);
						}
					}
				}
			}
		}

		// Unlock the vertex buffer
		vrtBuf->unlock();

		// Unlock the index buffer
		idxBuf->unlock();
	}
}

void GroundCover::Block::copyVertexData(const Vector3& pos,VertexData* src,VertexData* dst,unsigned char* dstPtr,unsigned int* dstOffset)
{
	unsigned int offset = *dstOffset;

	/* POSITIONS */

	// Find the positional element in both buffers
	const VertexElement* srcPosElem = src->vertexDeclaration->findElementBySemantic(VES_POSITION);
	const VertexElement* dstPosElem = dst->vertexDeclaration->findElementBySemantic(VES_POSITION);

	// Find the buffers used for the elements
	HardwareVertexBufferSharedPtr srcPosBuf = src->vertexBufferBinding->getBuffer(srcPosElem->getSource());
	HardwareVertexBufferSharedPtr dstPosBuf = dst->vertexBufferBinding->getBuffer(dstPosElem->getSource());

	// Lock the source buffer, destination is already locked
	unsigned char* srcPtr = static_cast<unsigned char*>(srcPosBuf->lock(HardwareBuffer::HBL_READ_ONLY));

	// Work out where the base pointer should be
	unsigned char* ptr = dstPtr + (dstPosBuf->getVertexSize() * offset);

	// Copy the vertices
	Real* pSrcVert;
	Real* pDstVert;
	
	for(size_t i = 0; i < src->vertexCount; ++i, srcPtr += srcPosBuf->getVertexSize())
	{
		srcPosElem->baseVertexPointerToElement(srcPtr,&pSrcVert);
		dstPosElem->baseVertexPointerToElement(ptr,&pDstVert);

		Real x = (*pSrcVert++) + pos.x;
		Real y = (*pSrcVert++) + pos.y;
		Real z = (*pSrcVert++) + pos.z;

		// LogManager::getSingleton().logMessage(LML_NORMAL,"Added vertex (%f,%f,%f)",x,y,z);

		*pDstVert++ = x;
		*pDstVert++ = y;
		*pDstVert++ = z;

		ptr += dstPosBuf->getVertexSize();
    }
	srcPosBuf->unlock();

	/* NORMALS */
/*
	// Find the normal element in both buffers
	const VertexElement* srcNrmElem = src->vertexDeclaration->findElementBySemantic(VES_NORMAL);
	const VertexElement* dstNrmElem = dst->vertexDeclaration->findElementBySemantic(VES_NORMAL);

	// Find the buffers used for the elements
	HardwareVertexBufferSharedPtr srcNrmBuf = src->vertexBufferBinding->getBuffer(srcNrmElem->getSource());
	HardwareVertexBufferSharedPtr dstNrmBuf = dst->vertexBufferBinding->getBuffer(dstNrmElem->getSource());

	// Lock the source buffer, destination is already locked
	srcPtr = static_cast<unsigned char*>(srcNrmBuf->lock(HardwareBuffer::HBL_READ_ONLY));

	// Work out where the base pointer should be
	ptr = dstPtr + (dstNrmBuf->getVertexSize() * offset);

	// Copy the vertices
	for(size_t i = 0; i < src->vertexCount; ++i, srcPtr += srcNrmBuf->getVertexSize())
	{
		srcNrmElem->baseVertexPointerToElement(srcPtr,&pSrcVert);
		dstNrmElem->baseVertexPointerToElement(ptr,&pDstVert);

		*pDstVert++ = *pSrcVert++;
		*pDstVert++ = *pSrcVert++;
		*pDstVert++ = *pSrcVert++;

		ptr += dstNrmBuf->getVertexSize();
    }
	srcNrmBuf->unlock();
*/
	/* TEXTURE COORDINATES */

	// Find the texture element in both buffers
	const VertexElement* srcTexElem = src->vertexDeclaration->findElementBySemantic(VES_TEXTURE_COORDINATES);
	const VertexElement* dstTexElem = dst->vertexDeclaration->findElementBySemantic(VES_TEXTURE_COORDINATES);

	// Find the buffers used for the elements
	HardwareVertexBufferSharedPtr srcTexBuf = src->vertexBufferBinding->getBuffer(srcTexElem->getSource());
	HardwareVertexBufferSharedPtr dstTexBuf = dst->vertexBufferBinding->getBuffer(dstTexElem->getSource());

	// Lock the source buffer, destination is already locked
	srcPtr = static_cast<unsigned char*>(srcTexBuf->lock(HardwareBuffer::HBL_READ_ONLY));

	// Work out where the base pointer should be
	ptr = dstPtr + (dstTexBuf->getVertexSize() * offset);

	// Copy the vertices
	for(size_t i = 0; i < src->vertexCount; ++i, srcPtr += srcTexBuf->getVertexSize())
	{
		srcTexElem->baseVertexPointerToElement(srcPtr,&pSrcVert);
		dstTexElem->baseVertexPointerToElement(ptr,&pDstVert);

		*pDstVert++ = *pSrcVert++;
		*pDstVert++ = *pSrcVert++;

		ptr += dstTexBuf->getVertexSize();
    }
	srcTexBuf->unlock();

	// Increment the offset (measured in vertices)
	*dstOffset = offset + src->vertexCount;
}

void GroundCover::Block::copyIndexData(IndexData* src,IndexData* dst,unsigned short* dstPtr,unsigned int* dstOffset,unsigned int vrtOffset)
{
	unsigned int offset = *dstOffset;
	unsigned short* pDst = dstPtr + offset;

	size_t numTris = src->indexCount / 3;
	unsigned short* pShort;
	unsigned int* pInt;
	Ogre::HardwareIndexBufferSharedPtr ibuf = src->indexBuffer;
	bool use32bitindexes = (ibuf->getType() == HardwareIndexBuffer::IT_32BIT);
	if (use32bitindexes)
	{
		pInt = static_cast<unsigned int*>(ibuf->lock(HardwareBuffer::HBL_READ_ONLY));
	}
	else
	{
		pShort = static_cast<unsigned short*>(ibuf->lock(HardwareBuffer::HBL_READ_ONLY));
	}

	for(size_t i = 0; i < numTris; ++i)
	{
		unsigned short i0 = (use32bitindexes? *pInt++ : *pShort++) + vrtOffset;
		unsigned short i1 = (use32bitindexes? *pInt++ : *pShort++) + vrtOffset;
		unsigned short i2 = (use32bitindexes? *pInt++ : *pShort++) + vrtOffset;

		// LogManager::getSingleton().logMessage(LML_NORMAL,"Added index (%u,%u,%u)",i0,i1,i2);

		*pDst++ = i0;
		*pDst++ = i1;
		*pDst++ = i2;
	}
	ibuf->unlock();

	// Increase the offset (measured in indices)
	*dstOffset = offset + src->indexCount;
}

void GroundCover::Block::reconfigureDetail(GroundCover::Detail* detail,GroundCover::DetailListener* listener)
{
	if(_detail != detail)
	{
		_detail = detail;
		_detail->setInUse(true,listener);
		_detail->setCenter(_center);

		for(std::vector<std::pair<Vector3,Mesh*> >::iterator i = _meshes.begin();i != _meshes.end();i++)
		{
			_detail->addMesh(i->first,i->second,listener);
		}
	}
}

GroundCover::Block::~Block()
{
}

GroundCover::Detail::Detail(SceneManager* manager,int id)
{
	_manager = manager;
	_in_use = false;
	_center = Vector3::ZERO;
	_node = _manager->getRootSceneNode()->createChildSceneNode("gcd_"+StringConverter::toString(id));
}

void GroundCover::Detail::addMesh(const Vector3& position,Mesh* mesh,GroundCover::DetailListener* listener)
{
	std::map<String,std::vector<std::pair<bool,SceneNode*> >* >::iterator i = _children.find(mesh->getName());
	std::vector<std::pair<bool,SceneNode*> >* nodes = NULL;
	if(i == _children.end())
	{
		nodes = new std::vector<std::pair<bool,SceneNode*> >;
		_children[mesh->getName()] = nodes;
	}
	else nodes = i->second;

	SceneNode* node = NULL;
	for(std::vector<std::pair<bool,SceneNode*> >::iterator j = nodes->begin();j != nodes->end();j++)
	{
		if(!(j->first))
		{
			j->first = true;
			j->second->setVisible(true);
			if(listener) listener->nodeShown(j->second,true);
			node = j->second;
			break;
		}
	}

	if(!node)
	{
		node = _node->createChildSceneNode();
		nodes->push_back(std::pair<bool,SceneNode*>(true,node));

		Entity* ent = _manager->createEntity(_node->getName() + "_" + mesh->getName() + "_" + StringConverter::toString((unsigned int)nodes->size()),mesh->getName());
		ent->setCastShadows(false);
		ent->setRenderQueueGroup(RENDER_QUEUE_8);
		node->attachObject(ent);
		node->setPosition(position - _center);

		if(listener) listener->nodeCreated(node,position);
	}
	else
	{
		node->setPosition(position - _center);

		if(listener) listener->nodeMoved(node,position);
	}
}

void GroundCover::Detail::setInUse(bool in_use,GroundCover::DetailListener* listener)
{
	_in_use = in_use; 
	_node->setVisible(_in_use,!_in_use);

	// If we're setting this detail block to be in use then hide everything
	// attached to it so we can re-jig it when we reconfigure the block
	if(_in_use)
	{
		for(std::map<String,std::vector<std::pair<bool,SceneNode*> >* >::iterator i = _children.begin();i != _children.end();i++)
		{
			for(std::vector<std::pair<bool,SceneNode*> >::iterator j = i->second->begin();j != i->second->end();j++)
			{
				j->first = false;
				j->second->setVisible(false);
				if(listener) listener->nodeShown(j->second,false);
			}
		}
	}
}

void GroundCover::Detail::setCenter(const Vector3& center)
{
	_center = center;
	_node->setPosition(_center);
}

GroundCover::Detail::~Detail()
{
	LogManager::getSingleton().logMessage(LML_NORMAL,"Items in child map: %u",_children.size());

	for(std::map<String,std::vector<std::pair<bool,SceneNode*> >* >::iterator i = _children.begin();i != _children.end();i++)
	{
		LogManager::getSingleton().logMessage(LML_NORMAL,"Items in node list %s: %u",i->first.c_str(),i->second->size());
		delete i->second;
	}
}
