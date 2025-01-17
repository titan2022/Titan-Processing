#include <array>
#include <cmath>
#include <opencv2/calib3d.hpp>
#include <string>
#include <tuple>
#include <vector>


#include "util/Vector3D.hpp"
#include <opencv2/core.hpp>


using namespace titan;

Vector3D::Vector3D(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3D::Vector3D(double x, double y, double z, CoordinateSystem coordinateSystem)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->coordinateSystem = coordinateSystem;
}

Vector3D::Vector3D(cv::Vec3d v)
{
	x = v[0];
	y = v[1];
	z = v[2];
}

Vector3D::Vector3D(cv::Vec3d v, CoordinateSystem _coordinateSystem)
{
	x = v[0];
	y = v[1];
	z = v[2];
	coordinateSystem = _coordinateSystem;
}

Vector3D::Vector3D(std::vector<double> v)
{
	x = v[0];
	y = v[1];
	z = v[2];
}

Vector3D::Vector3D(std::vector<double> v, CoordinateSystem _coordinateSystem)
{
	x = v[0];
	y = v[1];
	z = v[2];
	coordinateSystem = _coordinateSystem;
}

Vector3D::Vector3D(double (&v)[])
{
	x = v[0];
	y = v[1];
	z = v[2];
}

Vector3D::Vector3D(double (&v)[], CoordinateSystem _coordinateSystem)
{
	x = v[0];
	y = v[1];
	z = v[2];
	coordinateSystem = _coordinateSystem;
}

struct StructVector3D
{
	double x;
	double y;
	double z;
};
Vector3D::Vector3D(const char *d)
{
	const StructVector3D *structVec = reinterpret_cast<const StructVector3D *>(d);
	x = structVec->x;
	y = structVec->y;
	z = structVec->z;
}

Vector3D::Vector3D()
{
	x = 0;
	y = 0;
	z = 0;
}

double Vector3D::getX()
{
	return x;
}

double Vector3D::getY()
{
	return y;
}

double Vector3D::getZ()
{
	return z;
}

/**
 * @brief Returns rotated vector, all units are radians
 *
 * @param angle In Radians
 * @return Vector3D Rotated Vector
 */
Vector3D Vector3D::getRotatedX(double angle)
{
	return Vector3D(x, y * cos(angle) - z * sin(angle), y * sin(angle) + z * cos(angle));
}

/**
 * @brief Returns rotated vector, all units are radians
 *
 * @param angle In Radians
 * @return Vector3D Rotated Vector
 */
Vector3D Vector3D::getRotatedY(double angle)
{
	return Vector3D(x * cos(angle) + z * sin(angle), y, x * -sin(angle) + z * cos(angle));
}

/**
 * @brief Returns rotated vector, all units are radians
 *
 * @param angle In Radians
 * @return Vector3D Rotated Vector
 */
Vector3D Vector3D::getRotatedZ(double angle)
{
	return Vector3D(x * cos(angle) - y * sin(angle), x * sin(angle) + y * cos(angle), z);
}

/**
 * @brief Returns rotated vector in order of ZYX, all units are raiduans
 *
 * @param xAngle In Radians
 * @param yAngle In Radians
 * @param zAngle In Radians
 * @return Vector3D Rotated Vector
 */
Vector3D Vector3D::getRotated(double xAngle, double yAngle, double zAngle)
{
	return getRotatedZ(zAngle).getRotatedY(yAngle).getRotatedX(xAngle);
}

/**
 * @brief Returns magnitude or absolute length of vector
 *
 * @return double
 */
double Vector3D::getMagnitude()
{
	return sqrt(x * x + y * y + z * z);
}

/**
 * @brief Returns normalized vector (unit length of 1)
 *
 * @return Vector3D
 */
Vector3D Vector3D::getNormalized()
{
	double mag = getMagnitude();
	Vector3D normalized(x / mag, y / mag, z / mag);
	return normalized;
}

std::string Vector3D::toString()
{
	std::string s = "(";
	std::string xStr = std::to_string(getX());
	std::string yStr = std::to_string(getY());
	std::string zStr = std::to_string(getZ());
	s += xStr + ", ";
	s += yStr + ", ";
	s += zStr;
	s += ")";
	return s;
}

cv::Vec<double, 3> Vector3D::toCV()
{
	return cv::Vec<double, 3>{x, y, z};
}

std::array<double, 3> Vector3D::toArray()
{
	std::array<double, 3> arr;
	arr[0] = x;
	arr[1] = y;
	arr[2] = z;
	return arr;
}

Vector3D Vector3D::fromWPILibPosition(double x, double y, double z, double fieldLength, double fieldWidth)
{
	return Vector3D(x - fieldLength/2.0, z, -(y - fieldWidth/2.0), CoordinateSystem::THREEJS);
}

/**
 * @brief Rotates vector using 3x3 rotation matrix
 *
 * @param mat 3x3 rotation matrix
 * @return Vector3D Rotated vector
 */
Vector3D Vector3D::matToVec(double (&mat)[])
{
	// Source: http://motion.pratt.duke.edu/RoboticSystems/3DRotations.html#Converting-from-a-rotation-matrix
	// Temporarily copied from: https://github.com/titan2022/Jetson-Vision-2023/blob/apriltags/py/detector_client.py
	// 3x3 matrices follow row-major order (index = row * 3 + col)
	double theta = acos((mat[0] + mat[4] + mat[8] - 1) / 2);
	Vector3D pre(0, 0, 0);
	if (sin(theta) < 1e-3)
	{
		return pre; // TODO: impliment special case of theta == pi
	}

	double rk = (2 * sin(theta));
	pre.setX((mat[4] - mat[2]) / rk); // -5, -7
	pre.setY((mat[8] - mat[6]) / rk); // -1, -3
	pre.setZ((mat[3] - mat[1]) / rk); // 3, 1

	Vector3D result(pre[0] * theta, pre[1] * theta, pre[2] * theta);
	return result;
}

double Vector3D::setX(const double value)
{
	x = value;
	return x;
}

double Vector3D::setY(const double value)
{
	y = value;
	return y;
}

double Vector3D::setZ(const double value)
{
	z = value;
	return z;
}

/**
 * @brief Normalizes the vector (turns magnitude to 1)
 *
 * @return Vector3D Normalized vector
 */
Vector3D Vector3D::normalize()
{
	Vector3D normalized = getNormalized();
	x = normalized.getX();
	y = normalized.getY();
	z = normalized.getZ();
	return normalized;
}

void Vector3D::rotateX(double angle)
{
	*this = getRotatedX(angle);
}

void Vector3D::rotateY(double angle)
{
	*this = getRotatedY(angle);
}

void Vector3D::rotateZ(double angle)
{
	*this = getRotatedZ(angle);
}

/**
 * @brief Rotates vector in order of ZYX, all units are raiduans
 *
 * @param xAngle In Radians
 * @param yAngle In Radians
 * @param zAngle In Radians
 * @return Vector3D Rotated Vector
 */
void Vector3D::rotate(double xAngle, double yAngle, double zAngle)
{
	rotateZ(zAngle);
	rotateY(yAngle);
	rotateX(xAngle);
}

/**
 * @brief Rotates vector in order of ZYX, all units are raiduans
 *
 * @param vec Rotation vector
 * @return Vector3D Rotated Vector
 */
void Vector3D::rotate(Vector3D &vec)
{
	rotateZ(vec.getZ());
	rotateY(vec.getY());
	rotateX(vec.getX());
}

Vector3D &Vector3D::operator=(const Vector3D &v)
{
	setX(v.x);
	setY(v.y);
	setZ(v.z);
	return *this;
}

const Vector3D Vector3D::operator+(const Vector3D &v)
{
	return Vector3D(x + v.x, y + v.y, z + v.z);
}

const Vector3D Vector3D::operator-(const Vector3D &v)
{
	return Vector3D(x - v.x, y - v.y, z - v.z);
}

Vector3D &Vector3D::operator+=(Vector3D const &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3D &Vector3D::operator-=(Vector3D const &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3D Vector3D::operator*(const double s)
{
	return Vector3D(x * s, y * s, z * s);
}

Vector3D Vector3D::operator/(const double s)
{
	return Vector3D(x / s, y / s, z / s);
}

Vector3D &Vector3D::operator*=(const double s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3D &Vector3D::operator/=(const double s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

const double Vector3D::operator*(const Vector3D &v)
{
	return x * v.x + y * v.y + z * v.z;
}

const bool Vector3D::operator==(const Vector3D &v)
{
	return x == v.x && y == v.y && z == v.z;
}

const bool Vector3D::operator!=(const Vector3D &v)
{
	return !(*this == v);
}

/**
 * @brief Gets value. 0 = x, 1 = y, 2 = z
 *
 * @param index From 0 to 2
 * @return const double
 */
const double Vector3D::operator[](const int index)
{
	switch (index)
	{
	case 0:
		return x;
		break;
	case 1:
		return y;
		break;
	case 2:
		return z;
		break;
	default:
		break;
	}

	return 0;
}

///// Coordinate system /////

std::string titan::nameOfCoordinateSystem(CoordinateSystem coordinateSystem)
{
	switch(coordinateSystem) {
		case UNKNOWN: return "UNKNOWN";
		case THREEJS: return "THREEJS";
		case OPENCV: return "OPENCV";
		case WPILIB: return "WPILIB";
	}
}

///// Incorrect use of Vector3D for euler angles /////

EulerAngles Vector3D::coerceToEulerAngles()
{
	return {x, y, z, coordinateSystem};
}

Vector3D EulerAngles::coerceToVector3D()
{
	return {x, y, z, coordinateSystem};
}

///// Translations /////

Translation Translation::convertToCoordinateSystem(CoordinateSystem target)
{
	if (coordinateSystem == CoordinateSystem::OPENCV && target == CoordinateSystem::THREEJS)
	{
		return {-x, -y, z, target};
	}
	else
	{
		throw std::domain_error(std::string("conversion between translations of ") + nameOfCoordinateSystem(coordinateSystem) 
			+ " coordinate system and " + nameOfCoordinateSystem(target)
			+ " coordinate system is not yet supported.");
	}
}

///// Rotation matrix /////

RotationMatrix RotationMatrix::convertToCoordinateSystem(CoordinateSystem target)
{
	if (coordinateSystem == CoordinateSystem::OPENCV && target == CoordinateSystem::THREEJS)
    {
        // Flip x and y axes
        cv::Mat S = (cv::Mat_<double>(3,3) << 
            -1,  0,  0,
             0, -1,  0,
             0,  0,  1);

        // Apply axis flipping to the rotation matrix
        cv::Mat flipped_rotation = data * S;

        // Define a compensation matrix to negate pitch and roll
        cv::Mat compensation = (cv::Mat_<double>(3,3) << 
            1,  0,  0,
            0, -1,  0,
            0,  0, -1);

        // Apply compensation
        cv::Mat compensated_rotation = flipped_rotation * compensation;

        return RotationMatrix(compensated_rotation, target);
    }
	else
	{
		throw std::domain_error(std::string("conversion between rotation matrices of ") + nameOfCoordinateSystem(coordinateSystem) 
			+ " coordinate system and " + nameOfCoordinateSystem(target)
			+ " coordinate system is not yet supported.");
	}
}

RotationMatrix RotationMatrix::fromRotationVector(cv::Vec3d rvec, CoordinateSystem coordinateSystem)
{
	cv::Mat data;
	cv::Rodrigues(rvec, data);
	return RotationMatrix(data, coordinateSystem);
}

///// Rotation quaternions /////

RotationQuaternion RotationQuaternion::convertToCoordinateSystem(CoordinateSystem target)
{
	RotationQuaternion quat = *this;

	if (coordinateSystem == CoordinateSystem::WPILIB && target == CoordinateSystem::THREEJS)
	{
		double ninety = M_PI / 2;
		quat = quat * RotationQuaternion::fromAxisAngle(Vector3D(1, 0, 0), ninety*3); 
		quat.y = -quat.y;
		quat = quat * RotationQuaternion::fromAxisAngle(Vector3D(1, 0, 0), ninety); 
		quat = quat * RotationQuaternion::fromAxisAngle(Vector3D(0, 1, 0), ninety); 
	}
	else
	{
		throw std::domain_error(std::string("conversion between rotation quaternions of ") + nameOfCoordinateSystem(coordinateSystem) 
			+ " coordinate system and " + nameOfCoordinateSystem(target)
			+ " coordinate system is not yet supported.");
	}

	return quat;
}

RotationQuaternion RotationQuaternion::operator*(RotationQuaternion b)
{
	// https://github.com/mrdoob/three.js/blob/d9ca4dc0104b05c6421e22d30346abafd66894c4/src/math/Quaternion.js#L499

	double qax = this->x, qay = this->y, qaz = this->z, qaw = this->w;
	double qbx = b.x, qby = b.y, qbz = b.z, qbw = b.w;

	RotationQuaternion retval;

	retval.x = qax * qbw + qaw * qbx + qay * qbz - qaz * qby;
	retval.y = qay * qbw + qaw * qby + qaz * qbx - qax * qbz;
	retval.z = qaz * qbw + qaw * qbz + qax * qby - qay * qbx;
	retval.w = qaw * qbw - qax * qbx - qay * qby - qaz * qbz;

	return retval;
}

double RotationQuaternion::dotProduct(RotationQuaternion other)
{
	return this->w * other.w + this->x * other.x + this->y * other.y + this->z * other.z;
}

bool RotationQuaternion::isEquivalent(RotationQuaternion other)
{
	constexpr double TOLERANCE = 0.00001;
	double dot = dotProduct(other);
	return (1 - dot < TOLERANCE) || (-1 - dot < TOLERANCE); // Dot product is approximately -1 or 1
}

RotationQuaternion RotationQuaternion::fromAxisAngle(Vector3D axis, double angle)
{
	// https://github.com/mrdoob/three.js/blob/d9ca4dc0104b05c6421e22d30346abafd66894c4/src/math/Quaternion.js#L275
	// http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/index.htm

	// assumes axis is normalized
	double halfAngle = angle / 2;
	double s = sin(halfAngle);

	RotationQuaternion quat;

	quat.x = axis.getX() * s;
	quat.y = axis.getY() * s;
	quat.z = axis.getZ() * s;
	quat.w = cos(halfAngle);

	return quat;
}

///// Euler angles /////

EulerAngles EulerAngles::convertToCoordinateSystem(CoordinateSystem target)
{
	EulerAngles angles = *this;

	if (0) { }
	else
	{
		throw std::domain_error(std::string("conversion between euler angles of ") + nameOfCoordinateSystem(coordinateSystem) 
			+ " coordinate system and " + nameOfCoordinateSystem(target)
			+ " coordinate system is not yet supported.");
	}

	return angles;
}

///// Transforms /////

Transform::Transform(Translation position, RotationMatrix orientation)
{
	cv::Mat R = orientation.data;

    // 2) Construct 4x4 homogeneous matrix (initialize to identity)
    cv::Mat T = cv::Mat::eye(4, 4, CV_64F);

    // 3) Copy rotation block into the top-left
    R.copyTo(T(cv::Rect(0, 0, 3, 3)));  // Region of interest: columns 0..2, rows 0..2

    // 4) Set translation in the top-right
    T.at<double>(0, 3) = position.getX();
    T.at<double>(1, 3) = position.getY();
    T.at<double>(2, 3) = position.getZ();

	// Because it's an identity matrix the bottom-right is already a 1

    data = T;
	coordinateSystem = orientation.coordinateSystem;
}

Transform::Transform(cv::Mat data, CoordinateSystem coordinateSystem)
	: data(data), coordinateSystem(coordinateSystem)
{ }

Translation Transform::getPosition()
{
	double x = data.at<double>(0, 3);
	double y = data.at<double>(1, 3);
	double z = data.at<double>(2, 3);

	return Translation(x, y, z, coordinateSystem);
}

RotationMatrix Transform::getOrientation()
{
	return {data(cv::Rect(0, 0, 3, 3)).clone(), coordinateSystem}; // the rotation matrix is the top left corner of the transform
}

Transform Transform::inv()
{
	return {data.inv(), coordinateSystem};
}

Transform Transform::operator*(Transform other)
{
	return {data * other.data, coordinateSystem};
}

///// Euler angles <-> Rotation matrix /////

RotationMatrix EulerAngles::toRotationMatrix()
{
    // https://chatgpt.com/share/67884caa-0790-8013-a1d1-d7d813321c8c
	// https://www.eecs.qmul.ac.uk/~gslabaugh/publications/euler.pdf

	// Precompute sines and cosines
    double cx = std::cos(x);
    double sx = std::sin(x);
    double cy = std::cos(y);
    double sy = std::sin(y);
    double cz = std::cos(z);
    double sz = std::sin(z);

    // Rotation about X-axis (pitch)
    cv::Mat Rx = (cv::Mat_<double>(3, 3) << 
        1,    0,    0,
        0,   cx,  -sx,
        0,   sx,   cx
    );

    // Rotation about Y-axis (yaw)
    cv::Mat Ry = (cv::Mat_<double>(3, 3) << 
         cy,   0,   sy,
          0,   1,    0,
        -sy,   0,   cy
    );

    // Rotation about Z-axis (roll)
    cv::Mat Rz = (cv::Mat_<double>(3, 3) << 
        cz,  -sz,   0,
        sz,   cz,   0,
         0,    0,   1
    );

    // The combined rotation: Rz * Ry * Rx
    cv::Mat R = Rz * Ry * Rx;
    return {R, coordinateSystem};
}

EulerAngles RotationMatrix::toEulerAngles()
{
	// https://chatgpt.com/share/6789d1b4-3930-8013-9c9e-19e2dfb30754
    double roll, pitch, yaw;

    // Check for gimbal lock
    if (fabs(data.at<double>(2,0)) != 1)
    {
        yaw = -asin(data.at<double>(2,0));
        double cos_yaw = cos(yaw);
        pitch = atan2(data.at<double>(2,1) / cos_yaw, data.at<double>(2,2) / cos_yaw);
        roll = atan2(data.at<double>(1,0) / cos_yaw, data.at<double>(0,0) / cos_yaw);
    }
    else
    {
        // Gimbal lock occurs
        roll = 0;
        if (data.at<double>(2,0) == -1)
        {
            yaw = M_PI / 2;
            pitch = roll + atan2(data.at<double>(0,1), data.at<double>(0,2));
        }
        else
        {
            yaw = -M_PI / 2;
            pitch = -roll + atan2(-data.at<double>(0,1), -data.at<double>(0,2));
        }
    }

    return {pitch, yaw, roll, coordinateSystem};
}

#if 0
EulerAngles RotationMatrix::toEulerAngles()
{
	// https://www.eecs.qmul.ac.uk/~gslabaugh/publications/euler.pdf

    // Sanity check: must be 3x3 and double
	// Actually don't do this so we can apply this to transforms too
    // CV_Assert(R.rows == 3 && R.cols == 3 && R.type() == CV_64F);

    // Extract elements for clarity
    double r11 = data.at<double>(0,0);
    double r12 = data.at<double>(0,1);
    double r13 = data.at<double>(0,2);
    double r21 = data.at<double>(1,0);
    double r22 = data.at<double>(1,1);
    double r23 = data.at<double>(1,2);
    double r31 = data.at<double>(2,0);
    double r32 = data.at<double>(2,1);
    double r33 = data.at<double>(2,2);

	// phi = pitch = x
	// theta = yaw = y
    // psi = roll = z

	//   row      col:1                                     2                                                3
	//     1 [cos yaw cos pitch, sin roll sin yaw cos pitch - cos roll sin pitch, cos roll sin yaw cos pitch + sin roll sin pitch]
	// R = 2 [cos yaw sin pitch, sin roll sin yaw sin pitch + cos roll cos pitch, cos roll sin yaw sin pitch - sin roll cos pitch]
	//     3 [- sin yaw,         sin pitch cos yaw,                                cos roll cos yaw                              ]
    
	double roll, yaw, pitch;

	// theta = yaw = y
	// r20 = - sin yaw
	yaw = -std::asin(r31);

	if(r31 != 1 && r31 != -1) { // if cos(yaw) != 0
		double cosyaw = std::cos(yaw);

		// psi = roll = x
		// r32 / r33 = psi
		// Naively, psi = arctan(r32 / r33)
		// But "One must be careful in interpreting Equation 2. If cos(θ) > 0, then ψ =
		//      atan2(R32, R33). However, when cos(θ) < 0, ψ = atan2(−R32, −R33)."
		roll  = std::atan2(r32 / cosyaw, r33 / cosyaw);

		// phi = pitch = z
		// r21 / r11 = phi
		// Naively, phi = arctan (r21 / r11)
		// But similarly we need to use atan2 and divide the arguments by cosyaw
		pitch   = std::atan2(r21 / cosyaw, r11 / cosyaw);
	} else if (r31 == 1) { // if yaw = -pi/2
		// arbitrarily let pitch = 0
		pitch = 0;

		// roll - pitch = atan2(r12, r13)
		// => roll = pitch + atan2(r12, r13)
		// and pitch = 0
		roll = std::atan2(r12, r13);
	} else { // r31 = -1, so yaw = pi/2
		// arbitrarily let pitch = 0
		pitch = 0;

		// roll + pitch = atan2(-r12, -r13)
		// => roll = -pitch + atan2(-r12, -r13)
		// and pitch = 0
		roll = std::atan2(-r12, -r13);
	}

    return {pitch, yaw, roll, coordinateSystem};
}
#endif

RotationMatrix RotationMatrix::operator*(RotationMatrix other)
{
	return {data * other.data, coordinateSystem};
}

RotationMatrix RotationMatrix::operator*(cv::Mat other)
{
	return {data * other, coordinateSystem};
}

///// Euler angles <-> Rotation quaternion /////

EulerAngles RotationQuaternion::toEulerAngles()
{
	//code from https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Source_code_2

	//x-axis
	double sinr_cosp = 2*(w*x+y*z);
    double cosr_cosp = 1-2*(x*x+y*y);
    double pitch_x = std::atan2(sinr_cosp, cosr_cosp);

	//y-axis
    double sinp = std::sqrt(1+2*(w*y-x*z));
    double cosp = std::sqrt(1-2*(w*y-x*z));
    double yaw_y = 2 * std::atan2(sinp, cosp) - M_PI / 2;

    //z-axis
    double siny_cosp = 2*(w*z+x*y);
    double cosy_cosp = 1-2*(y*y+z*z);
    double roll_z = std::atan2(siny_cosp, cosy_cosp);

	return {pitch_x, yaw_y, roll_z, coordinateSystem};
}

RotationQuaternion EulerAngles::toRotationQuaternion()
{
	// code from https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Source_code

	double cr = cos(x * 0.5);
	double sr = sin(x * 0.5);
	double cp = cos(y * 0.5);
	double sp = sin(y * 0.5);
	double cy = cos(z * 0.5);
	double sy = sin(z * 0.5);

	double w = cr * cp * cy + sr * sp * sy;
	double x = sr * cp * cy - cr * sp * sy;
	double y = cr * sp * cy + sr * cp * sy;
	double z = cr * cp * sy - sr * sp * cy;
	return {w, x, y, z, coordinateSystem};
}
