#include "SMath/SQuaternion.h"

namespace sm{
#define PI (4.f * atan(1.f))
#define feps(f) (fabs(f)<1e-5?1:0)
glm::vec3 Quat2Eul(glm::quat qua){

	

	// pitch (y-axis rotation)
	float pitch = 0.f;
	const float sinp = +2.0f * (qua.w * qua.y - qua.z * qua.x);
	if (fabs(sinp) >= 1)
		pitch = static_cast<float>(copysign(PI / 2.0f, sinp)); // use 90 degrees if out of range
	else
		pitch = asin(sinp);
	
	//singularities process
	if (feps(sinp-1)){
		return glm::degrees(glm::vec3(
			copysign(atan2(qua.x,qua.w),sinp),
			pitch,
			0));
	}
	
	
	// roll (x-axis rotation)
	const float sinr_cosp = 2.0f * (qua.w * qua.x + qua.y * qua.z);
	const float cosr_cosp = 1.0f - 2.0f * (qua.x * qua.x + qua.y * qua.y);
	const float roll = atan2(sinr_cosp, cosr_cosp);

	

	// yaw (z-axis rotation)
	const float siny_cosp = +2.0f * (qua.w * qua.z + qua.x * qua.y);
	const float cosy_cosp = +1.0f - 2.0f * (qua.y * qua.y + qua.z * qua.z);
	const float yaw = atan2(siny_cosp, cosy_cosp);
	
	return glm::degrees(glm::vec3(roll,pitch,yaw));
}

glm::quat Eul2Quat(glm::vec3 euler_xyz_deg){
    // lab x - y - z rotate
    float yaw	= glm::radians(euler_xyz_deg.z) * 0.5f;
	float pitch = glm::radians(euler_xyz_deg.y) * 0.5f;
	float roll	= glm::radians(euler_xyz_deg.x) * 0.5f;

	float cy = cos(yaw);
	float sy = sin(yaw);
	float cp = cos(pitch);
	float sp = sin(pitch);
	float cr = cos(roll);
	float sr = sin(roll);
    return {
        cr * cp * cy + sr * sp * sy,
        sr * cp * cy - cr * sp * sy,
	    cr * sp * cy + sr * cp * sy,
	    cr * cp * sy - sr * sp * cy
    };
	// x = sr * cp * cy - cr * sp * sy;
	// y = cr * sp * cy + sr * cp * sy;
	// z = cr * cp * sy - sr * sp * cy;
	// w = cr * cp * cy + sr * sp * sy;
}

glm::quat LookAt(glm::vec3 up,glm::vec3 forward){
    //quat_cast ver:
	glm::vec3 axisZ = -glm::normalize(forward);
	glm::vec3 axisX =glm::normalize( glm::cross(up, axisZ));
	glm::vec3 axisY = glm::cross(axisZ, axisX);
	return glm::quat_cast(glm::mat3(axisX,axisY,axisZ));

}

}