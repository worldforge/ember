#ifndef _SBCHARACTERFRAMEDATA_H_
#define _SBCHARACTERFRAMEDATA_H_


#ifdef __cplusplus
extern "C" {
#endif 


// This struct is used by SBCharacter::GetFrameDataMarshalFriendly() and SBM_GetCharacter().
// It is useful for when you want a character's entire skeleton data for the current frame in one call.
// The data is laid out to be friendly for marshalling purposes when going over the managed barrier.

struct SBM_CharacterFrameDataMarshalFriendly
{
	char * m_name;
	float x;
	float y;
	float z;
	float rw;
	float rx;
	float ry;
	float rz;
	size_t m_numJoints;

	char ** jname;
	float * jx;
	float * jy;
	float * jz;
	float * jrw;
	float * jrx;
	float * jry;
	float * jrz;
};


#ifdef __cplusplus
}
#endif

#endif  // _SBCHARACTERFRAMEDATA_H_
