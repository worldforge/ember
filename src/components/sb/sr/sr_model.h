/*  sr_model.h - part of Motion Engine and SmartBody-lib
 *  Copyright (C) 2008  University of Southern California
 *
 *  SmartBody-lib is free software: you can redistribute it and/or
 *  modify it under the terms of the Lesser GNU General Public License
 *  as published by the Free Software Foundation, version 3 of the
 *  license.
 *
 *  SmartBody-lib is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Lesser GNU General Public License for more details.
 *
 *  You should have received a copy of the Lesser GNU General Public
 *  License along with SmarBody-lib.  If not, see:
 *      http://www.gnu.org/licenses/lgpl-3.0.txt
 *
 *  CONTRIBUTORS:
 *      Marcelo Kallmann, USC (currently UC Merced)
 */

# ifndef SR_MODEL_H
# define SR_MODEL_H

/** \file sr_model.h
 * 3d model based on a triangle list
 */

class SrCylinder;
class SrSphere;

# include <sb/SBTypes.h>
# include <sr/sr_box.h>
# include <sr/sr_vec.h>
# include <sr/sr_mat.h>
# include <sr/sr_vec2.h>
# include <sr/sr_line.h>
# include <sr/sr_string.h>
# include <sr/sr_material.h>
# include <sr/sr_string_array.h>
# include <sr/sr_shared_class.h>
#include <map>
#include <string>
#include <vector>

/*! \class SrModel sr_model.h
    \brief a model composed of triangular faces

    SrModel keeps arrays of vertices, normals, texture coordinates,
    materials, and face elements.
    These arrays are public and can be directly manipulated by the user.
    However please note that is the user responsability to maintain indices
    in a coherent way.
    Texture support is not finished. */
class SrModel : public SrSharedClass
 { public :
    /*! A static string containing the name "model" */
    SBAPI static const char* class_name;

    /*! The Face structure keeps 3 indices used to associate the
        information in V, N, and T to each face */
    struct Face 
     { 
#if defined(__ANDROID__) || defined(SB_IPHONE)
       unsigned short a,b,c;
       void validate () { }
       unsigned short& operator[] ( int i ) { return *((&a)+i); }
       friend SrOutput& operator<< ( SrOutput& o, const Face& f ) { 
                int ia,ib,ic; ia = f.a; ib = f.b; ic = f.c;
		return o<<ia<<srspc<<ib<<srspc<<ic; 
       }
       friend SrInput& operator>> ( SrInput& i, Face& f ) { 
          int ia,ib,ic;
          //return i>>f.a>>f.b>>f.c; 
          SrInput& t = i>>ia>>ib>>ic; 
          f.a = ia; f.b = ib; f.c = ic;
          return t;
       }
#else
       int a, b, c;
       void validate () { if (a<0) a=-a; if (b<0) b=-b; if (c<0) c=-c; }
       int& operator[] ( int i ) { return *((&a)+i); }
       friend SrOutput& operator<< ( SrOutput& o, const Face& f ) { return o<<f.a<<srspc<<f.b<<srspc<<f.c; }
       friend SrInput& operator>> ( SrInput& i, Face& f ) { return i>>f.a>>f.b>>f.c; }
#endif   
       void set ( int i, int j, int k ) { a=i; b=j; c=k; }    
     };

    SrArray<SrMaterial> M;  //!< Used materials
    SrArray<SrPnt>      V;  //!< Vertices coordinates
	SrArray<SrVec>      Vc; // color of vertices
    SrArray<SrVec>      N;  //!< Normals table		
	SrArray<SrVec>      Tangent, BiNormal; // tangent and bi normal vectors
    SrArray<SrPnt2>     T;  //!< Texture coordinates
    SrArray<Face>       F;  //!< Triangular faces indices to V
    SrArray<int>       Fm;  //!< Indices to the materials in M (size can be<F.size())
    SrArray<Face>      Fn;  //!< Indices to the normals in N (size can be<F.size())
    SrArray<Face>      Ft;  //!< Indices to the texture coordinates in T

	SrArray<SrPnt>      VOrig;  //!< Vertices original (if changed)

	std::map<std::string,std::string> mtlTextureNameMap; // map from material name to texture name
	std::map<std::string,std::string> mtlNormalTexNameMap;
	std::map<std::string,std::string> mtlSpecularTexNameMap;
	std::map<std::string,std::vector<int> > mtlFaceIndices;

	SrVec _translation;
	SrVec _rotation;
	float _scale;

    /*! Will be set to true (the default) if back face culling
        should be applied, and false otherwise */
    bool culling;

    /*! Stores a name description for the materials in M.
        Will have size 0 if no names defined, otherwise will have size M.size() */
    SrStringArray mtlnames;
    
    /*! May contain any desired name for the model. */
	SrString name;

    /*! Constructor lets all internal arrays as empty and culling is set to true */
    SBAPI SrModel ();

    /*! Virtual Destructor */
    SBAPI virtual ~SrModel ();

	SBAPI void computeTangentBiNormal(); // build the tangent and binormal vectors for each vertex. Used for tangent space normal mapping
	SBAPI void computeNormals();

    /*! Returns true if the model has no faces, and false otherwise */
    SBAPI bool empty () { return F.empty(); }

    /*! Sets to an empty model. Internal arrays are not compressed. */
    SBAPI void init ();

    /*! Compress all internal array buffers. */
    SBAPI void compress ();

    /*! Ensures that Fm, Fn, Ft arrays have the same size as F or have size 0. */
    SBAPI void validate ();
    
    /*! Removes unreferenced or duplicated materials. */
    SBAPI void remove_redundant_materials ();

    /*! Removes redundant normals, which are closer than the given angular distance. */
    SBAPI void remove_redundant_normals ( float dang=SR_TORAD(0.5f) );

	/*! Removes redundant normals, which are closer than the given angular distance. */
	SBAPI void remove_redundant_texcoord ();

    /*! Check and remove redundant vertices */
    SBAPI void merge_redundant_vertices ( float prec );

    /*! Reads a SrModel format. */
    SBAPI bool load ( SrInput &in );

    /*! Save in the SrModel format. */
    SBAPI bool save ( SrOutput &o ) const;

    /*! Imports .obj file format. If the import is succesfull, true
       is returned, and otherwise false is returned. */
    SBAPI bool import_obj ( const char* file );

	/*! Imports .ply file format. If the import is succesfull, true
       is returned, and otherwise false is returned. */
	SBAPI bool import_ply ( const char* file );

    /*! Exports .iv file format. If the export is succesfull, true
       is returned, and otherwise false is returned. */
    SBAPI bool export_iv ( const char* file );

    /*! Makes E to be an array containing the indices of the model edges. */
    SBAPI void make_edges ( SrArray<int>& E );
   
    /*! Count and return the mean number of edges adjacent to a vertex in the model. */
    SBAPI float count_mean_vertex_degree ();

    /*! Calculates the bounding box of this model. */
    SBAPI void get_bounding_box ( SrBox &box ) const;

    /*! Translate the position of each vertex of the model. */
    SBAPI void translate ( const SrVec &tr );
	SBAPI SrVec& translate () {return _translation;};

	/*! Rotatee the position of each vertex of the model. */
    SBAPI void rotate ( const SrVec &tr );

    /*! Scale each vertex of the model. */
    SBAPI void scale ( float factor );

    /*! Translates so that the bounding box center becomes (0,0,0). */
    SBAPI void centralize ();

    /*! Centralizes and scale to achieve maxcoord. */
    SBAPI void normalize ( float maxcoord );

    /*! Returns the number of common vertices between the two faces indices. */
    SBAPI int common_vertices_of_faces ( int i, int j );

    /*! Clear the N and Fn arrays, with compression. */
    SBAPI void flat ();

    /*! Clear materials and then set M and Fm so that all triangles use the
        same material m, with compression. */
    SBAPI void set_one_material ( const SrMaterial& m );

    /*! Clear material names, and the M and Fm array, with compression. */
    SBAPI void clear_materials ();

    /*! Clear the T and Ft array (with compression). */
    SBAPI void clear_textures ();

    /*! Generates normals smoothly, respecting the given crease
        angle in radians. Compression is called in the end.
        If the crease angle is <0, it is not considered and
        only an overall smooth is done. */
    SBAPI void smooth ( float crease_angle=SR_TORAD(35.0f) );

    /*! Calculates and returns the normalized normal of the given face index. */
    SBAPI SrVec face_normal ( int f ) const;

	SBAPI float face_area ( int f ) const;

    /*! Inverts faces orientations by swaping b and c indices, 
        and does the same to the normals. */
    SBAPI void invert_faces ();

    /*! Multiply all normals in N by -1 */
    SBAPI void invert_normals ();

    /*! Multiply arrays V and N with the given matrix */
    SBAPI void apply_transformation ( const SrMat& mat );

    /*! Add faces, materials and normals of m to SrModel */
    SBAPI void add_model ( const SrModel& m );

    /*! Copy operator */
    SBAPI void operator = ( const SrModel& m );

    /*! Make a box shape */
    SBAPI void make_box ( const SrBox& b );

    /*! Make a sphere shape */
    SBAPI void make_sphere ( const SrSphere& s, float resolution=1 );

    /*! Make a cylinder shape */
    SBAPI void make_cylinder ( const SrCylinder& c, float resolution=1, bool smooth=true );

    /*! Returns the index of the face intersecting with the line, or -1 if
        no face is found. In case several intersections are found, the closest
        to line.p1 is returned */
    SBAPI int pick_face ( const SrLine& line ) const;

	SBAPI SrVec intersectLine( const SrLine& line) const;

	SBAPI void saveOriginalVertices();
	SBAPI void restoreOriginalVertices();

	SBAPI void recalculateVertices();
 };

//================================ End of File =================================================

# endif // SR_MODEL_H
