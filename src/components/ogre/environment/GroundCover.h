#ifndef GROUNDCOVER_H
#define GROUNDCOVER_H

#include <vector>
#include <map>

using namespace Ogre;

class GroundCover
{
public:
	class DetailListener
	{
	public:
		DetailListener(){}
		virtual ~DetailListener(){}

		virtual void nodeCreated(Ogre::SceneNode* node,const Ogre::Vector3& position){}
		virtual void nodeMoved(Ogre::SceneNode* node,const Ogre::Vector3& position){}
		virtual void nodeShown(Ogre::SceneNode* node,bool show){}
	};

private:
	class Detail
	{
	public:
		Detail(Ogre::SceneManager* manager,int id);
		~Detail();

		void setInUse(bool in_use,GroundCover::DetailListener* listener);
		bool isInUse(){return _in_use;}

		void addMesh(const Ogre::Vector3& position,Ogre::Mesh* mesh,GroundCover::DetailListener* listener);
		void setCenter(const Ogre::Vector3& center);

	protected:
		bool _in_use;
		Ogre::Vector3 _center;
		Ogre::SceneNode* _node;
		Ogre::SceneManager* _manager;

		// Map of mesh names and the list of child nodes that use them
		std::map<String,std::vector<std::pair<bool,Ogre::SceneNode*> >* > _children;
	};

	class Block
	{
	public:
		Block(unsigned int key,const Vector3& center);
		~Block();

		void add(const Ogre::Vector3& position,Ogre::Mesh* mesh);
		void build(Ogre::Mesh* mesh);
		bool cull(Ogre::Camera* camera,Ogre::Real max_dist,Ogre::Real min_dist,Ogre::Real cos_view_angle,GroundCover::DetailListener* listener);
		void setSubEntities(Ogre::Entity* entity);
		GroundCover::Detail* getDetail(){return _detail;}
		void reconfigureDetail(GroundCover::Detail* detail,GroundCover::DetailListener* listener);

	protected:
		unsigned int _key;
		bool _visible;
		std::vector<std::pair<Ogre::Vector3,Ogre::Mesh*> > _meshes;
		Ogre::Vector3 _center;
		std::vector<Ogre::SubMesh*> _submeshes;
		std::vector<Ogre::SubEntity*> _subentities;
		GroundCover::Detail* _detail;

	private:
		void copyVertexData(const Ogre::Vector3& pos,Ogre::VertexData* src,VertexData* dst,unsigned char* dstPtr,unsigned int* dstOffset);
		void copyIndexData(Ogre::IndexData* src,Ogre::IndexData* dst,unsigned short* dstPtr,unsigned int* dstOffset,unsigned int vrtOffset);
	};

public:
	static Ogre::String MESH_NAME;
	static Ogre::String ENTITY_NAME;
	static Ogre::String NODE_NAME;

	GroundCover(SceneManager* manager,const Vector3& size,int sizeOfABlock, const Ogre::Vector3 center);
	~GroundCover();

	void add(const Ogre::Vector3& position,const Ogre::String& high_mesh,const Ogre::String& low_mesh); // Add a mesh at the given position
	void update(Ogre::Camera* camera); // Cull and apply LODs depending on where the camera is
	void compile(); // Compile all the items in the list into the single big mesh/node/entity

	void setCullParameters(Ogre::Real near,Ogre::Real far,Ogre::Real view_angle);

	void setDetailListener(GroundCover::DetailListener* listener = NULL){_listener = listener;}
	GroundCover::DetailListener* getDetailListener(){return _listener;}

protected:
	Ogre::SceneManager* _manager; // What manages the whole scene

	Ogre::Entity*		_big_entity; // The big foliage entity that holds all the low LOD stuff for the distance
	Ogre::SceneNode*	_big_node; // The big foliage scene node
	Ogre::Mesh*			_big_mesh; // The big foliage mesh

	Ogre::Vector3		_size; // The size of the world our foliage will exist in	
	unsigned int		_x_blocks,_z_blocks; // The number of blocks in the Z and Z directions
	Ogre::Vector3 _center; //the center of the groundcover
	Ogre::Real _max_dist,_min_dist,_cos_view_angle;
	

	std::vector<std::pair<Ogre::Vector3,Ogre::Mesh*> > _items; // List of meshes and their positions, for use in compilation
	std::map<unsigned int,GroundCover::Block*> _blocks; 
		// Chopped up versions of all the blocks, so we can construct the high detail local stuff
		// Indexed by; MAKELONG(x_block,z_block)

	std::vector<GroundCover::Detail*> _details;
		// A number of detail entities. 
		// These will get recycled by the blocks and will contain a high-water mark
		// amount of the different meshes needed, the blocks will hide show and move
		// those entities as required to match their setup.

	GroundCover::DetailListener* _listener;
};

#endif
