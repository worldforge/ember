
# ifndef SK_POS_CFG_H
# define SK_POS_CFG_H

# include "sr_cfg_manager.h"
# include "sr_cfg_planner.h"
# include <sr/sr_vec.h>
# include <vector>
# include <sb/SBColObject.h>

// basic interface for the object to be grabbed
class ColObject
{
public:
	virtual SrVec getCentroid() = 0; // get the center of the object
	virtual bool  isCollided(const SrVec& inPos) = 0; // check if a point is inside the object	
	virtual bool isCollided(const SrVec& p1, const SrVec& p2) = 0;
	virtual bool isCollided(std::vector<SrVec>& lineSeg) = 0;
};

class ColSphere : public ColObject
{
protected:
	float radius;
	SrVec center;
public:
	ColSphere() {};
	~ColSphere() {};
	void setSphere(const SrVec& pt, const float r);
	void setCenter(const SrVec& pt) { center = pt; }
	virtual SrVec getCentroid(); // get the center of the object
	virtual bool  isCollided(const SrVec& inPos); // check if a point is inside the object	
	virtual bool isCollided(const SrVec& p1, const SrVec& p2);
	virtual bool isCollided(std::vector<SrVec>& lineSeg);
};

typedef std::vector<ColObject*> VecOfColObj;

typedef std::vector<SBGeomObject*> VecOfSbmColObj;

//=================================== SkPosCfg =====================================

class SkPosCfgManager;

class SkPosCfg : public SrVec
 { private :
    // its manager:
    SkPosCfgManager* _cman;
    // used only if in motion sync mode:
    float _t;

   public :
    SkPosCfg ( SkPosCfgManager* cman );
    SkPosCfg ( const SkPosCfg& c );
   ~SkPosCfg ();

    void random ();
    bool valid ();
    bool monotone ( const SkPosCfg& c2 ) const;
    void time ( float t ) { _t=t; }
    float time () const { return _t; }
    void update_points ();
    void get ();
    void apply (); // handles attachments
    void copy ( const SkPosCfg& c2 );  
    float dist ( SkPosCfg& c2 );
    void interp ( const SkPosCfg& c2, float t, SkPosCfg& c );
    friend float dist ( SkPosCfg& c1, SkPosCfg& c2 ) { return c1.dist(c2); }
    friend void interp ( const SkPosCfg& c1, const SkPosCfg& c2, float t, SkPosCfg& c )
                { ((SkPosCfg&)c1).interp(c2,t,c); }
    
    friend SrOutput& operator<< ( SrOutput& out, const SkPosCfg& c );
    friend SrInput& operator>> ( SrInput& inp, SkPosCfg& c );
  };

//======================================= EOF =====================================

# endif // SK_ARM_CFG_H

