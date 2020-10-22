#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OVectorDefs.h"

class OMatrixStack;
class OState;

/**
 @brief Class that represents a camera on the scene.
*/
class OAPI OCamera
{
public:
	static const OVector3F DefaultPosition;
	static const OVector3F DefaultOrientation;
	static constexpr float DefaultFieldOfView	= 45.0f;
	static constexpr float DefaultAspectRatio	= 4.0f / 3;
	static constexpr float DefaultZNear		= 1.0f;
	static constexpr float DefaultZFar		= 5.0f;

	/**
	 @brief Class constructor.

	 The constructor takes in all the attributes of the scene camera.

	 @param aFieldOfViewDeg Angle of the camera's field of view in degrees.
	 @param aAspectRatio The aspect ration of the screen (width/height).
	 @param aZNear Nearest camera depth.
	 @param aZFar Farthest camera depth.

	 @note The default values for position and orietation are (0, 0, 0) and (0, 0, -1), respectively.
	*/
	OCamera(float aFieldOfViewDeg = DefaultFieldOfView, float aAspectRatio = DefaultAspectRatio, 
		float aZNear = DefaultZNear, float aZFar = DefaultZFar);

	/**
	 @brief Class constructor.

	 The constructor takes in all the attributes of the scene camera.

	 @param aPosition Camera position.
	 @param aOrientation Camera orientation vector containing Euler angles: rotations around axes x, y and z. 
			    The camera always faces the positive z-axis direction from it's perspective.
	 @param aFieldOfViewDeg Angle of the camera's field of view in degrees.
	 @param aAspectRatio The aspect ration of the screen (width/height).
	 @param aZNear Nearest camera depth.
	 @param aZFar Farthest camera depth.
	*/
	OCamera(const OVector3F& aPosition, const OVector3F& aOrientation, 
		float aFieldOfViewDeg = DefaultFieldOfView, float aAspectRatio = DefaultAspectRatio, 
		float aZNear = DefaultZNear, float aZFar = DefaultZFar);

	/**
	 @brief Deleted copy constructor.
	 */
	OCamera(const OCamera& aOther) = delete;

	/**
	 @brief Move constructor.
	 */
	OCamera(OCamera&& aOther);

	/**
	 @brief Class destructor.
	*/
	virtual ~OCamera();

	/**
	 @brief Deleted copy assignment operator.
	 */
	OCamera& operator=(const OCamera& aOther) = delete;

	/**
	 @brief Move assignment operator.
	 */
	OCamera& operator=(OCamera&& aOther);

	/**
	 @brief Sets the camera field of view.
	 @param aValueDeg Field of view in degrees.
	*/
	void setFieldOfView(float aValueDeg);
	
	/**
	 @brief Sets camera aspect ratio (width/height).
	*/
	void setAspectRatio(float aValue);
	
	/**
	 @brief Sets camera depth limits.
	 @param aZNear Nearest camera depth.
	 @param aZFar Farthest camera depth.
	*/
	void setCameraLimits(float aZNear, float aZFar);
	
	/**
	 @brief Set camera position.
	 @param aPosition Camera position coordinates.
	*/
	void setPosition(const OVector3F &aPosition);

	/**
	 @brief Change camera position using a displacement vector.
	 @param aDisplacement Displacement vector.
	 */
	void changePosition(const OVector3F &aDisplacement);

	/**
	 @brief Set camera orientation.
	 @param aOrientation New orientation vector given in Euler angles.
	*/
	void setOrientation(const OVector3F &aOrientation);

	/**
	 @brief Returns the camera field of view.
	 @return Field of view in degrees.
	*/
	float fieldOfViewDegrees() const;

	/**
	 @brief Returns screen aspect ratio.
	 @return Screen aspect ratio (width/height)
	*/
	float aspectRatio() const;

	/**
	 @brief Returns camera nearest limit.
	*/
	float nearLimit() const;
	
	/**
	 @brief Returns camera farthest limit.
	*/
	float farLimit() const;
	
	/**
	 @brief Retrieves the camera position from the state object. 
	 @return Camera position coordinates.
	*/
	const OVector3F& position() const;
	
	/**
	 @brief Retrieves the camera orientation from the state object.
	 @return Camera orientation vector in terms of Euler angles.
	*/
	const OVector3F& orientation() const;

	/**
	 @brief Direct access to the camera state object.
	 */
	OState* state();

	/**
	 @brief Calculates the perspective and camera transformations.
	 @return Matrix stack containing the transformation matrix.
	 */
	OMatrixStack* transform();
	
private:
	/**
	 @cond HIDDEN
	 */
	struct Impl;
	Impl* _impl = nullptr;
	/**
	 @endcond
	 */
};

inline OCamera::OCamera(OCamera&& aOther)
{
	_impl = aOther._impl;
	aOther._impl = nullptr;
}

