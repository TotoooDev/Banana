#include <Physics/Helpers.h>

namespace Banana
{
	rp3d::Vector3 RP3DVec3(glm::vec3 vec)
	{
		return rp3d::Vector3(vec.x, vec.y, vec.z);
	}

    glm::vec3 GLMVec3(rp3d::Vector3 vec)
    {
        return glm::vec3(vec.x, vec.y, vec.z);
    }

    glm::mat4 GLMTransform(rp3d::Transform transform)
    {
        float mat[16];
        transform.getOpenGLMatrix(mat);
        glm::mat4 matrix(
            mat[ 0], mat[ 1], mat[ 2], mat[ 3],
            mat[ 4], mat[ 5], mat[ 6], mat[ 7],
            mat[ 8], mat[ 9], mat[10], mat[11],
            mat[12], mat[13], mat[14], mat[15]
        );
        return matrix;
    }

	glm::vec3 QuaternionToEuler(rp3d::Quaternion quaternion)
	{
        glm::vec3 angles;

        // roll (x-axis rotation)
        double sinr_cosp = 2 * (quaternion.w * quaternion.x + quaternion.y * quaternion.z);
        double cosr_cosp = 1 - 2 * (quaternion.x * quaternion.x + quaternion.y * quaternion.y);
        angles.x = std::atan2(sinr_cosp, cosr_cosp);

        // pitch (y-axis rotation)
        double sinp = std::sqrt(1 + 2 * (quaternion.w * quaternion.y - quaternion.x * quaternion.z));
        double cosp = std::sqrt(1 - 2 * (quaternion.w * quaternion.y - quaternion.x * quaternion.z));
        angles.y = 2 * std::atan2(sinp, cosp) - 3.14159265359f / 2;

        // yaw (z-axis rotation)
        double siny_cosp = 2 * (quaternion.w * quaternion.z + quaternion.x * quaternion.y);
        double cosy_cosp = 1 - 2 * (quaternion.y * quaternion.y + quaternion.z * quaternion.z);
        angles.z = std::atan2(siny_cosp, cosy_cosp);

        return angles;
	}

    rp3d::Quaternion EulerToQuaternion(glm::vec3 euler)
    {
        // Abbreviations for the various angular functions

        double cr = cos(euler.z * 0.5);
        double sr = sin(euler.z * 0.5);
        double cp = cos(euler.y * 0.5);
        double sp = sin(euler.y * 0.5);
        double cy = cos(euler.x * 0.5);
        double sy = sin(euler.x * 0.5);

        rp3d::Quaternion q;
        q.w = cr * cp * cy + sr * sp * sy;
        q.x = sr * cp * cy - cr * sp * sy;
        q.y = cr * sp * cy + sr * cp * sy;
        q.z = cr * cp * sy - sr * sp * cy;

        return q;
    }
}