#include "me_ct_reach_IK.hpp"
#include <sr/sr_alg.h>


MeCtReachIK::MeCtReachIK(void)
{
	max_iteration = 30;
}

MeCtReachIK::~MeCtReachIK(void)
{
}

SrQuat MeCtReachIK::dampQuat( const SrQuat& prevQuat, const SrQuat& nextQuat, float damping_angle )
{
	SrQuat diff = prevQuat.inverse()*nextQuat;	
	diff.normalize();
	float angle = diff.angle() > damping_angle ? damping_angle : diff.angle();
	diff.set(diff.axis(),angle);
	SrQuat newQuat = prevQuat*diff;
	newQuat.normalize();
	return newQuat;
}

void MeCtReachIK::adjust()
{
	int reach = 0;
	int i,j;	
	for(i = 0; i < max_iteration; ++i)
	{
		//for(j = start_joint_index; j != manipulated_joint_index; ++j)
		for(j = manipulated_joint_index-1 ; j >= start_joint_index; j--) 
		{
			/*if(reach_destination()) 
			{
				reach = 1;
				break;
			}*/			
			ccdRotate(scenario->joint_pos_list.get(manipulated_joint_index), j);
		}
		if(reach) break;
	}

	// damping

	for (int i=0;i<scenario->joint_quat_list.size();i++)
	{
		MeCtIKScenarioJointInfo* info = &(scenario->joint_info_list.get(i));
		float damping_angle = (float)RAD(info->angular_speed_limit*getDt());
		scenario->joint_quat_list[i] = dampQuat(cur_quatList[i],scenario->joint_quat_list[i],damping_angle);		
	}
}





int MeCtReachIK::check_joint_limit( SrQuat* quat, int index )
{
	//return 0;	
	int modified = 0;
	MeCtIKScenarioJointInfo* info = &(scenario->joint_info_list.get(index));

	// feng : for some reason, the twist axis is aligned with local x-axis, instead of z-axis
	// therefore we need to do the "shifted" version of swingTwist conversion.
	// so we map x->z, y->x, z->y for computing swing-twist parameteriation.
	// all of the angle limits are adjust accordingly.
	{
		MeCtIKJointLimit& limit = info->joint_limit;
		gwiz::quat_t quat_st = gwiz::quat_t(quat->w, quat->x, quat->y, quat->z);			
		gwiz::vector_t st = quat2SwingTwist(quat_st);//quat_st.swingtwist();
		float sw_y = (float)st.x();
		float sw_z = (float)st.y();		

		float sw_limit_z, sw_limit_y;
		sw_limit_z = sw_z > 0 ? limit.x_limit[0] : limit.x_limit[1];
		sw_limit_y = sw_y > 0 ? limit.y_limit[0] : limit.y_limit[1];

		// project swing angles onto the joint limit ellipse
		if( sr_in_ellipse( sw_z, sw_y, sw_limit_z, sw_limit_y ) > 0.0 )	{
			sr_get_closest_on_ellipse( sw_limit_z, sw_limit_y, sw_z, sw_y );			
		}		
		// handle twist angle limit
		float tw = (float)st.z(); 
 		if( tw > limit.twist_limit[0] ) tw = limit.twist_limit[0];
 		if( tw < limit.twist_limit[1] ) tw = limit.twist_limit[1];
		
		gwiz::vector_t sr_tmp(sw_y, sw_z, tw);
		gwiz::quat_t ql = swingTwist2Quat( sr_tmp );//quat_t( sw_x, sw_y, tw);
		quat->set((float)ql.w(),(float)ql.x(),(float)ql.y(),(float)ql.z());
	}	
	return modified;
}






void MeCtReachIK::ccdRotatePositionAndOrientation( SrVec& src, int start_index )
{
	// (feng) To-Do : add orientation constraint in CCD algorithm
	SrVec v1, v2, v3, v4;
	SrVec v, i_target, i_src;
	//SrVec axis, r_axis;	
	SrVec r_axis, cross_v;
	SrQuat q;
	SrMat rot_src, rot_target;
	SrVec e[3], e_t[3]; // for orientation target
	SrMat mat, mat_inv;

	MeCtIKScenarioJointInfo* info = &(scenario->joint_info_list.get(start_index));
	float damping_angle = (float)RAD(info->angular_speed_limit*getDt());

	if(start_index == 0) mat_inv = scenario->gmat.inverse();
	else mat_inv = scenario->joint_global_mat_list.get(start_index-1).inverse();
	SrVec& pivot = scenario->joint_pos_list.get(start_index);
	pivot = pivot * mat_inv; // tranverse joint back to its local coordinate
	i_target = target.get(manipulated_joint_index) * mat_inv;
	i_src = src * mat_inv;
	
	rot_target = target_orientation.get(manipulated_joint_index).get_mat(rot_target);
	
	rot_target = rot_target * mat_inv;
	rot_src    = scenario->joint_global_mat_list.get(manipulated_joint_index) * mat_inv;

	// remove translation part in the rotation matrix
	rot_target.setl4(0.f,0.f,0.f,1.f);
	rot_src.setl4(0.f,0.f,0.f,1.f);	

	// to-do : solve for optimal angles that satisfies both position & orientation constraints
	
	
	
	q = scenario->joint_quat_list.get(start_index);
	q = mat * q;
	check_joint_limit(&q,start_index);
	scenario->joint_quat_list.set(start_index, q);
	get_limb_section_local_pos(start_index, -1);

}


void MeCtReachIK::ccdRotate(SrVec& src, int start_index)
{
	SrVec v1, v2, v3, v4;
	SrVec v, i_target, i_src;
	//SrVec axis, r_axis;
	SrVec r_axis;
	SrQuat q;
	SrMat mat, mat_inv;
	if(start_index == 0) mat_inv = scenario->gmat.inverse();
	else mat_inv = scenario->joint_global_mat_list.get(start_index-1).inverse();
	SrVec& pivot = scenario->joint_pos_list.get(start_index);
	pivot = pivot * mat_inv; // tranverse joint back to its local coordinate
	i_target = target.get(manipulated_joint_index) * mat_inv;
	i_src = src * mat_inv;
	
	v1 = i_src - pivot;
	v1.normalize();

	MeCtIKScenarioJointInfo* info = &(scenario->joint_info_list.get(start_index));
	float damping_angle = (float)RAD(info->angular_speed_limit*getDt());

	if(scenario->joint_info_list.get(start_index).type == JOINT_TYPE_BALL)
	{
		v2 = i_target - pivot;
		v2.normalize();

		double dot_v = dot(v1, v2);
		if(dot_v >= 0.9999995000000f) 
		{
			if(start_index >= scenario->joint_pos_list.size()-1) return;
			dot_v = 1.0f;
		}
		else if(dot_v < -1.0f) dot_v = -1.0f;

		double angle = acos(dot_v);
		//if (angle > damping_angle) angle = damping_angle;		

		r_axis = cross(v2, v1);
		r_axis.normalize();

		mat.rot(r_axis, (float)-angle);
	}
	
	
	q = scenario->joint_quat_list.get(start_index);
	q = mat * q;

	check_joint_limit(&q,start_index);

	scenario->joint_quat_list.set(start_index, q);
	get_limb_section_local_pos(start_index, -1);
}


#define USE_PARTICLE_IK 0
void MeCtReachIK::update(MeCtIKScenario* scenario)
{
	this->scenario = scenario;
	int modified = 0;
	SrMat inv_end;

	SrQuat quat;
	SrMat mat;	
	init();	

	cur_quatList = scenario->joint_quat_list;
#if !USE_PARTICLE_IK
	adjust();	
#else
	particleIK();
#endif
}

void MeCtReachIK::calc_target(SrVec& orientation, SrVec& offset)
{	
	// don't care about incoming parameters, just set the target position & orientation from ik_scenario
	target.set(manipulated_joint_index,scenario->ik_offset);
	target_orientation.set(manipulated_joint_index,scenario->ik_quat_orientation);
}


// added by feng :
// Originally I thought all joints use the local Z axis as twist axis, but this is not the case for the smartbody skeleton.
// For arms, the twist axis is at local "X" axis.
// For legs, the twist axis is local "Z" axis.
// So if we want to convert arm joints quaternion into Swing-Twist, we need to invert ( shift ) XZ coordinate so we get correct results.
// For other limbs like legs, we use the original swing-twist conversion.
#define INVERT_XZ 
gwiz::vector_t MeCtReachIK::quat2SwingTwist( gwiz::quat_t& quat )
{
	const float EPSILON6 = 0.0000001f;

#ifdef INVERT_XZ	
	if( ( quat.w() < EPSILON6 )&&( quat.x() < EPSILON6 ) )	{
		return( gwiz::vector_t( 0.0, 0.0, M_PI ) );
	}
#else
	if( ( quat.w() < EPSILON6 )&&( quat.z() < EPSILON6 ) )	{
		return( gwiz::vector_t( M_PI, 0.0, 0.0 ) );
	}
#endif

	gwiz::quat_t q;
	if( quat.w() < 0.0 )	{
		q = quat.complement();
	}
	else	{
		q = quat;
	}

#ifdef INVERT_XZ
	double gamma = atan2( q.x(), q.w() );
	double beta = atan2( sqrt( q.z()*q.z() + q.y()*q.y() ), sqrt( q.x()*q.x() + q.w()*q.w() ) );
	double sinc = 1.0;
	if( beta > EPSILON6 )	{
		sinc = sin( beta )/beta;
	}
	double s = sin( gamma );
	double c = cos( gamma );
	double sinc2 = 2.0 / sinc;
	double swing_x = sinc2 * ( c * q.y() - s * q.z());
	double swing_y = sinc2 * ( s * q.y() + c * q.z());
	double twist = 2.0 * gamma;
#else
	double gamma = atan2( q.z(), q.w() );
	double beta = atan2( sqrt( q.x()*q.x() + q.y()*q.y() ), sqrt( q.z()*q.z() + q.w()*q.w() ) );
	double sinc = 1.0;
	if( beta > EPSILON6 )	{
		sinc = sin( beta )/beta;
	}
	double s = sin( gamma );
	double c = cos( gamma );
	double sinc2 = 2.0 / sinc;
	double swing_x = sinc2 * ( c * q.x() - s * q.y() );
	double swing_y = sinc2 * ( s * q.x() + c * q.y());
	double twist = 2.0 * gamma;
#endif
	return( gwiz::vector_t( ( swing_x ), ( swing_y ), ( twist ) ) );
}

gwiz::quat_t MeCtReachIK::swingTwist2Quat( gwiz::vector_t& sw )
{
	using namespace gwiz;
#ifdef INVERT_XZ
	quat_t swing = quat_t( vector_t( 0.0, sw.x(), sw.y() ) );	
    quat_t result = swing * quat_t( sw.z(), vector_t( 1.0, 0.0, 0.0 ), 1);
#else
	quat_t swing = quat_t(vector_t( sw.x(), sw.y(), 0.0) );	
	quat_t result = swing * quat_t( sw.z(), vector_t( 0.0, 0.0, 1.0 ), 1);
#endif
	
	return result;
}

#if USE_PARTICLE_IK
// Added by feng 
// Particle IK : method from "Real-Time Motion Retargeting to Highly Varied User-Created Morphologies"
// It gives a more "springy" IK solution and may look more natural than CCD sometimes. 
// However, the result is not nearly as good as traditional CCD when adding the joint angle limits.

void MeCtReachIK::particleIK()
{
	// implement a particle based method for IK
	SrArray<SrVec> particleList; // particle list of the joint chain	
	particleList = scenario->joint_pos_list;		
	// compute bone length constraint
	SrArray<float> constraintList;
	constraintList.size(particleList.size());
	for (int i=0;i<particleList.size()-1;i++)
		constraintList[i] = (particleList[i] - particleList[i+1]).len();

	SrVec targetPos = target.get(manipulated_joint_index);

	const int nMaxIter = 5;
	int num_particles = particleList.size();
	for (int iter=0;iter<nMaxIter;iter++)
	{
		SrVec dv = targetPos - particleList[num_particles-1];
		particleList[num_particles-1] += dv;		
		for (int k=(num_particles-1);k>=2;k--)
		{
			dv = particleList[k - 1] - particleList[k];
			dv *= 0.5f - constraintList[k - 1] / dv.len() * 0.5f;
			particleList[k] += dv;
			particleList[k - 1] -= dv;			
		}
		dv = particleList[1] - particleList[0];
		dv *= 1.0f - constraintList[0] / dv.len();
		particleList[1] += dv;
	}
	matchSkeleton(particleList);

	for (int i=0;i<scenario->joint_quat_list.size();i++)
	{
		MeCtIKScenarioJointInfo* info = &(scenario->joint_info_list.get(i));
		float damping_angle = (float)RAD(info->angular_speed_limit*getDt());
		scenario->joint_quat_list[i] = dampQuat(cur_quatList[i],scenario->joint_quat_list[i],damping_angle);		
	}
}

void MeCtReachIK::matchSkeleton( SrArray<SrVec>& particleList )
{
	SrMat mat,mat_inv;
	SrVec v, i_target, i_src, v1, v2, r_axis;
	SrQuat q;

	for (int i=0;i<particleList.size()-1;i++)
	{
		int start_index = i;
		if(start_index == 0) mat_inv = scenario->gmat.inverse();
		else mat_inv = scenario->joint_global_mat_list.get(start_index-1).inverse();

		SrVec pivot = scenario->joint_pos_list.get(start_index);
		pivot = pivot * mat_inv; // tranverse joint back to its local coordinate
		i_target = particleList.get(start_index+1) * mat_inv;
		i_src = scenario->joint_pos_list.get(start_index+1) * mat_inv;

		v1 = i_src - pivot;
		v1.normalize();
		v2 = i_target - pivot;
		v2.normalize();

		double dot_v = dot(v1, v2);
		if(dot_v >= 0.9999995000000f) 
		{			
			dot_v = 1.0f;			
		}
		else if(dot_v < -1.0f) dot_v = -1.0f;

		double angle = acos(dot_v);
		MeCtIKScenarioJointInfo* info = &(scenario->joint_info_list.get(i));
		float damping_angle = (float)RAD(info->angular_speed_limit*getDt());

		if (angle > damping_angle) angle = damping_angle;		
		r_axis = cross(v2, v1);
		r_axis.normalize();
		mat.rot(r_axis, (float)-angle);

		q = scenario->joint_quat_list.get(start_index);
		q = mat * q;
		check_joint_limit(&q,start_index);
		scenario->joint_quat_list.set(start_index, q);
		get_limb_section_local_pos(start_index, -1);
	}
}
#endif
