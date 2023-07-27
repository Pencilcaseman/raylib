/*******************************************************************************************
*
*   rcamera - Basic camera system with support for multiple camera modes
*
*   CONFIGURATION:
*       #define RCAMERA_IMPLEMENTATION
*           Generates the implementation of the library into the included file.
*           If not defined, the library is in header only mode and can be included in other headers
*           or source files without problems. But only ONE file should hold the implementation.
*
*       #define RCAMERA_STANDALONE
*           If defined, the library can be used as standalone as a camera system but some
*           functions must be redefined to manage inputs accordingly.
*
*   CONTRIBUTORS:
*       Ramon Santamaria:   Supervision, review, update and maintenance
*       Christoph Wagner:   Complete redesign, using raymath (2022)
*       Marc Palau:         Initial implementation (2014)
*
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2022-2023 Christoph Wagner (@Crydsch) & Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#ifndef RCAMERA_H
#define RCAMERA_H

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Function specifiers definition
#ifndef RLAPI
    #define RLAPI       // Functions defined as 'extern' by default (implicit specifiers)
#endif

#if defined(RCAMERA_STANDALONE)
    #define CAMERA_CULL_DISTANCE_NEAR      0.01
    #define CAMERA_CULL_DISTANCE_FAR    1000.0
#else
    #define CAMERA_CULL_DISTANCE_NEAR   RL_CULL_DISTANCE_NEAR
    #define CAMERA_CULL_DISTANCE_FAR    RL_CULL_DISTANCE_FAR
#endif

//----------------------------------------------------------------------------------
// Types and Structures Definition
// NOTE: Below types are required for standalone usage
//----------------------------------------------------------------------------------
#if defined(RCAMERA_STANDALONE)
    // RlVector2, 2 components
    typedef struct RlVector2 {
        float x;                // Vector x component
        float y;                // Vector y component
    } RlVector2;

    // RlVector3, 3 components
    typedef struct RlVector3 {
        float x;                // Vector x component
        float y;                // Vector y component
        float z;                // Vector z component
    } RlVector3;

    // RlMatrix, 4x4 components, column major, OpenGL style, right-handed
    typedef struct RlMatrix {
        float m0, m4, m8, m12;  // RlMatrix first row (4 components)
        float m1, m5, m9, m13;  // RlMatrix second row (4 components)
        float m2, m6, m10, m14; // RlMatrix third row (4 components)
        float m3, m7, m11, m15; // RlMatrix fourth row (4 components)
    } RlMatrix;

    // RlCamera type, defines a camera position/orientation in 3d space
    typedef struct RlCamera3D {
        RlVector3 position;       // RlCamera position
        RlVector3 target;         // RlCamera target it looks-at
        RlVector3 up;             // RlCamera up vector (rotation over its axis)
        float fovy;             // RlCamera field-of-view apperture in Y (degrees) in perspective, used as near plane width in orthographic
        int projection;         // RlCamera projection type: RL_CAMERA_PERSPECTIVE or RL_CAMERA_ORTHOGRAPHIC
    } RlCamera3D;

    typedef RlCamera3D RlCamera;    // RlCamera type fallback, defaults to RlCamera3D

    // RlCamera projection
    typedef enum {
        RL_CAMERA_PERSPECTIVE = 0, // Perspective projection
        RL_CAMERA_ORTHOGRAPHIC     // Orthographic projection
    } RlCameraProjection;

    // RlCamera system modes
    typedef enum {
        RL_CAMERA_CUSTOM = 0,      // RlCamera custom, controlled by user (RL_UpdateCamera() does nothing)
        RL_CAMERA_FREE,            // RlCamera free mode
        RL_CAMERA_ORBITAL,         // RlCamera orbital, around target, zoom supported
        RL_CAMERA_FIRST_PERSON,    // RlCamera first person
        RL_CAMERA_THIRD_PERSON     // RlCamera third person
    } RlCameraMode;
#endif

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif

RLAPI RlVector3 GetCameraForward(RlCamera *camera);
RLAPI RlVector3 GetCameraUp(RlCamera *camera);
RLAPI RlVector3 GetCameraRight(RlCamera *camera);

// RlCamera movement
RLAPI void CameraMoveForward(RlCamera *camera, float distance, bool moveInWorldPlane);
RLAPI void CameraMoveUp(RlCamera *camera, float distance);
RLAPI void CameraMoveRight(RlCamera *camera, float distance, bool moveInWorldPlane);
RLAPI void CameraMoveToTarget(RlCamera *camera, float delta);

// RlCamera rotation
RLAPI void CameraYaw(RlCamera *camera, float angle, bool rotateAroundTarget);
RLAPI void CameraPitch(RlCamera *camera, float angle, bool lockView, bool rotateAroundTarget, bool rotateUp);
RLAPI void CameraRoll(RlCamera *camera, float angle);

RLAPI RlMatrix GetCameraViewMatrix(RlCamera *camera);
RLAPI RlMatrix GetCameraProjectionMatrix(RlCamera* camera, float aspect);

#if defined(__cplusplus)
}
#endif

#endif // RCAMERA_H


/***********************************************************************************
*
*   CAMERA IMPLEMENTATION
*
************************************************************************************/

#if defined(RCAMERA_IMPLEMENTATION)

#include "raymath.h"        // Required for vector maths:
                            // Vector3Add()
                            // Vector3Subtract()
                            // Vector3Scale()
                            // Vector3Normalize()
                            // Vector3Distance()
                            // Vector3CrossProduct()
                            // Vector3RotateByAxisAngle()
                            // Vector3Angle()
                            // Vector3Negate()
                            // MatrixLookAt()
                            // MatrixPerspective()
                            // MatrixOrtho()
                            // MatrixIdentity()

// raylib required functionality:
                            // RL_GetMouseDelta()
                            // RL_GetMouseWheelMove()
                            // RL_IsKeyDown()
                            // RL_IsKeyPressed()
                            // RL_GetFrameTime()

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
#define CAMERA_MOVE_SPEED                               0.09f
#define CAMERA_ROTATION_SPEED                           0.03f

// RlCamera mouse movement sensitivity
#define CAMERA_MOUSE_MOVE_SENSITIVITY                   0.003f    // TODO: it should be independant of framerate
#define CAMERA_MOUSE_SCROLL_SENSITIVITY                 1.5f

#define CAMERA_ORBITAL_SPEED                            0.5f       // Radians per second


#define CAMERA_FIRST_PERSON_STEP_TRIGONOMETRIC_DIVIDER  8.0f
#define CAMERA_FIRST_PERSON_STEP_DIVIDER                30.0f
#define CAMERA_FIRST_PERSON_WAVING_DIVIDER              200.0f

// PLAYER (used by camera)
#define PLAYER_MOVEMENT_SENSITIVITY                     20.0f

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module specific Functions Declaration
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
// Returns the cameras forward vector (normalized)
RlVector3 GetCameraForward(RlCamera *camera)
{
    return Vector3Normalize(Vector3Subtract(camera->target, camera->position));
}

// Returns the cameras up vector (normalized)
// Note: The up vector might not be perpendicular to the forward vector
RlVector3 GetCameraUp(RlCamera *camera)
{
    return Vector3Normalize(camera->up);
}

// Returns the cameras right vector (normalized)
RlVector3 GetCameraRight(RlCamera *camera)
{
    RlVector3 forward = GetCameraForward(camera);
    RlVector3 up = GetCameraUp(camera);

    return Vector3CrossProduct(forward, up);
}

// Moves the camera in its forward direction
void CameraMoveForward(RlCamera *camera, float distance, bool moveInWorldPlane)
{
    RlVector3 forward = GetCameraForward(camera);

    if (moveInWorldPlane)
    {
        // Project vector onto world plane
        forward.y = 0;
        forward = Vector3Normalize(forward);
    }

    // Scale by distance
    forward = Vector3Scale(forward, distance);

    // Move position and target
    camera->position = Vector3Add(camera->position, forward);
    camera->target = Vector3Add(camera->target, forward);
}

// Moves the camera in its up direction
void CameraMoveUp(RlCamera *camera, float distance)
{
    RlVector3 up = GetCameraUp(camera);

    // Scale by distance
    up = Vector3Scale(up, distance);

    // Move position and target
    camera->position = Vector3Add(camera->position, up);
    camera->target = Vector3Add(camera->target, up);
}

// Moves the camera target in its current right direction
void CameraMoveRight(RlCamera *camera, float distance, bool moveInWorldPlane)
{
    RlVector3 right = GetCameraRight(camera);

    if (moveInWorldPlane)
    {
        // Project vector onto world plane
        right.y = 0;
        right = Vector3Normalize(right);
    }

    // Scale by distance
    right = Vector3Scale(right, distance);

    // Move position and target
    camera->position = Vector3Add(camera->position, right);
    camera->target = Vector3Add(camera->target, right);
}

// Moves the camera position closer/farther to/from the camera target
void CameraMoveToTarget(RlCamera *camera, float delta)
{
    float distance = Vector3Distance(camera->position, camera->target);

    // Apply delta
    distance += delta;

    // Distance must be greater than 0
    if (distance <= 0) distance = 0.001f;

    // Set new distance by moving the position along the forward vector
    RlVector3 forward = GetCameraForward(camera);
    camera->position = Vector3Add(camera->target, Vector3Scale(forward, -distance));
}

// Rotates the camera around its up vector
// Yaw is "looking left and right"
// If rotateAroundTarget is false, the camera rotates around its position
// Note: angle must be provided in radians
void CameraYaw(RlCamera *camera, float angle, bool rotateAroundTarget)
{
    // Rotation axis
    RlVector3 up = GetCameraUp(camera);

    // View vector
    RlVector3 targetPosition = Vector3Subtract(camera->target, camera->position);

    // Rotate view vector around up axis
    targetPosition = Vector3RotateByAxisAngle(targetPosition, up, angle);

    if (rotateAroundTarget)
    {
        // Move position relative to target
        camera->position = Vector3Subtract(camera->target, targetPosition);
    }
    else // rotate around camera.position
    {
        // Move target relative to position
        camera->target = Vector3Add(camera->position, targetPosition);
    }
}

// Rotates the camera around its right vector, pitch is "looking up and down"
//  - lockView prevents camera overrotation (aka "somersaults")
//  - rotateAroundTarget defines if rotation is around target or around its position
//  - rotateUp rotates the up direction as well (typically only usefull in RL_CAMERA_FREE)
// NOTE: angle must be provided in radians
void CameraPitch(RlCamera *camera, float angle, bool lockView, bool rotateAroundTarget, bool rotateUp)
{
    // Up direction
    RlVector3 up = GetCameraUp(camera);

    // View vector
    RlVector3 targetPosition = Vector3Subtract(camera->target, camera->position);

    if (lockView)
    {
        // In these camera modes we clamp the Pitch angle
        // to allow only viewing straight up or down.

        // Clamp view up
        float maxAngleUp = Vector3Angle(up, targetPosition);
        maxAngleUp -= 0.001f; // avoid numerical errors
        if (angle > maxAngleUp) angle = maxAngleUp;

        // Clamp view down
        float maxAngleDown = Vector3Angle(Vector3Negate(up), targetPosition);
        maxAngleDown *= -1.0f; // downwards angle is negative
        maxAngleDown += 0.001f; // avoid numerical errors
        if (angle < maxAngleDown) angle = maxAngleDown;
    }

    // Rotation axis
    RlVector3 right = GetCameraRight(camera);

    // Rotate view vector around right axis
    targetPosition = Vector3RotateByAxisAngle(targetPosition, right, angle);

    if (rotateAroundTarget)
    {
        // Move position relative to target
        camera->position = Vector3Subtract(camera->target, targetPosition);
    }
    else // rotate around camera.position
    {
        // Move target relative to position
        camera->target = Vector3Add(camera->position, targetPosition);
    }

    if (rotateUp)
    {
        // Rotate up direction around right axis
        camera->up = Vector3RotateByAxisAngle(camera->up, right, angle);
    }
}

// Rotates the camera around its forward vector
// Roll is "turning your head sideways to the left or right"
// Note: angle must be provided in radians
void CameraRoll(RlCamera *camera, float angle)
{
    // Rotation axis
    RlVector3 forward = GetCameraForward(camera);

    // Rotate up direction around forward axis
    camera->up = Vector3RotateByAxisAngle(camera->up, forward, angle);
}

// Returns the camera view matrix
RlMatrix GetCameraViewMatrix(RlCamera *camera)
{
    return MatrixLookAt(camera->position, camera->target, camera->up);
}

// Returns the camera projection matrix
RlMatrix GetCameraProjectionMatrix(RlCamera *camera, float aspect)
{
    if (camera->projection == RL_CAMERA_PERSPECTIVE)
    {
        return MatrixPerspective(camera->fovy*DEG2RAD, aspect, CAMERA_CULL_DISTANCE_NEAR, CAMERA_CULL_DISTANCE_FAR);
    }
    else if (camera->projection == RL_CAMERA_ORTHOGRAPHIC)
    {
        double top = camera->fovy/2.0;
        double right = top*aspect;

        return MatrixOrtho(-right, right, -top, top, CAMERA_CULL_DISTANCE_NEAR, CAMERA_CULL_DISTANCE_FAR);
    }

    return MatrixIdentity();
}

#if !defined(RCAMERA_STANDALONE)
// Update camera position for selected mode
// RlCamera mode: RL_CAMERA_FREE, RL_CAMERA_FIRST_PERSON, RL_CAMERA_THIRD_PERSON, RL_CAMERA_ORBITAL or CUSTOM
void RL_UpdateCamera(RlCamera *camera, int mode)
{
    RlVector2 mousePositionDelta = RL_GetMouseDelta();

    bool moveInWorldPlane = ((mode == RL_CAMERA_FIRST_PERSON) || (mode == RL_CAMERA_THIRD_PERSON));
    bool rotateAroundTarget = ((mode == RL_CAMERA_THIRD_PERSON) || (mode == RL_CAMERA_ORBITAL));
    bool lockView = ((mode == RL_CAMERA_FIRST_PERSON) || (mode == RL_CAMERA_THIRD_PERSON) || (mode == RL_CAMERA_ORBITAL));
    bool rotateUp = (mode == RL_CAMERA_FREE);

    if (mode == RL_CAMERA_ORBITAL)
    {
        // Orbital can just orbit
        RlMatrix rotation = MatrixRotate(GetCameraUp(camera), CAMERA_ORBITAL_SPEED*RL_GetFrameTime());
        RlVector3 view = Vector3Subtract(camera->position, camera->target);
        view = Vector3Transform(view, rotation);
        camera->position = Vector3Add(camera->target, view);
    }
    else
    {
        // RlCamera rotation
        if (RL_IsKeyDown(RL_KEY_DOWN)) CameraPitch(camera, -CAMERA_ROTATION_SPEED, lockView, rotateAroundTarget, rotateUp);
        if (RL_IsKeyDown(RL_KEY_UP)) CameraPitch(camera, CAMERA_ROTATION_SPEED, lockView, rotateAroundTarget, rotateUp);
        if (RL_IsKeyDown(RL_KEY_RIGHT)) CameraYaw(camera, -CAMERA_ROTATION_SPEED, rotateAroundTarget);
        if (RL_IsKeyDown(RL_KEY_LEFT)) CameraYaw(camera, CAMERA_ROTATION_SPEED, rotateAroundTarget);
        if (RL_IsKeyDown(RL_KEY_Q)) CameraRoll(camera, -CAMERA_ROTATION_SPEED);
        if (RL_IsKeyDown(RL_KEY_E)) CameraRoll(camera, CAMERA_ROTATION_SPEED);

        // RlCamera movement
        if (!RL_IsGamepadAvailable(0))
        {
            // Mouse/Keyboard support
            CameraYaw(camera, -mousePositionDelta.x*CAMERA_MOUSE_MOVE_SENSITIVITY, rotateAroundTarget);
            CameraPitch(camera, -mousePositionDelta.y*CAMERA_MOUSE_MOVE_SENSITIVITY, lockView, rotateAroundTarget, rotateUp);

            if (RL_IsKeyDown(RL_KEY_W)) CameraMoveForward(camera, CAMERA_MOVE_SPEED, moveInWorldPlane);
            if (RL_IsKeyDown(RL_KEY_A)) CameraMoveRight(camera, -CAMERA_MOVE_SPEED, moveInWorldPlane);
            if (RL_IsKeyDown(RL_KEY_S)) CameraMoveForward(camera, -CAMERA_MOVE_SPEED, moveInWorldPlane);
            if (RL_IsKeyDown(RL_KEY_D)) CameraMoveRight(camera, CAMERA_MOVE_SPEED, moveInWorldPlane);
        }
        else
        {
            // Gamepad controller support
            CameraYaw(camera, -(RL_GetGamepadAxisMovement(0, RL_GAMEPAD_AXIS_RIGHT_X) * 2)*CAMERA_MOUSE_MOVE_SENSITIVITY, rotateAroundTarget);
            CameraPitch(camera, -(RL_GetGamepadAxisMovement(0, RL_GAMEPAD_AXIS_RIGHT_Y) * 2)*CAMERA_MOUSE_MOVE_SENSITIVITY, lockView, rotateAroundTarget, rotateUp);

            if (RL_GetGamepadAxisMovement(0, RL_GAMEPAD_AXIS_LEFT_Y) <= -0.25f) CameraMoveForward(camera, CAMERA_MOVE_SPEED, moveInWorldPlane);
            if (RL_GetGamepadAxisMovement(0, RL_GAMEPAD_AXIS_LEFT_X) <= -0.25f) CameraMoveRight(camera, -CAMERA_MOVE_SPEED, moveInWorldPlane);
            if (RL_GetGamepadAxisMovement(0, RL_GAMEPAD_AXIS_LEFT_Y) >= 0.25f) CameraMoveForward(camera, -CAMERA_MOVE_SPEED, moveInWorldPlane);
            if (RL_GetGamepadAxisMovement(0, RL_GAMEPAD_AXIS_LEFT_X) >= 0.25f) CameraMoveRight(camera, CAMERA_MOVE_SPEED, moveInWorldPlane);
        }

        //if (RL_IsKeyDown(KEY_SPACE)) CameraMoveUp(camera, CAMERA_MOVE_SPEED);
        //if (RL_IsKeyDown(KEY_LEFT_CONTROL)) CameraMoveUp(camera, -CAMERA_MOVE_SPEED);
    }

    if ((mode == RL_CAMERA_THIRD_PERSON) || (mode == RL_CAMERA_ORBITAL))
    {
        // Zoom target distance
        CameraMoveToTarget(camera, -RL_GetMouseWheelMove());
        if (RL_IsKeyPressed(RL_KEY_KP_SUBTRACT)) CameraMoveToTarget(camera, 2.0f);
        if (RL_IsKeyPressed(RL_KEY_KP_ADD)) CameraMoveToTarget(camera, -2.0f);
    }
}
#endif // !RCAMERA_STANDALONE

// Update camera movement, movement/rotation values should be provided by user
void RL_UpdateCameraPro(RlCamera *camera, RlVector3 movement, RlVector3 rotation, float zoom)
{
    // Required values
    // movement.x - Move forward/backward
    // movement.y - Move right/left
    // movement.z - Move up/down
    // rotation.x - yaw
    // rotation.y - pitch
    // rotation.z - roll
    // zoom - Move towards target

    bool lockView = true;
    bool rotateAroundTarget = false;
    bool rotateUp = false;
    bool moveInWorldPlane = true;

    // RlCamera rotation
    CameraPitch(camera, -rotation.y*DEG2RAD, lockView, rotateAroundTarget, rotateUp);
    CameraYaw(camera, -rotation.x*DEG2RAD, rotateAroundTarget);
    CameraRoll(camera, rotation.z*DEG2RAD);

    // RlCamera movement
    CameraMoveForward(camera, movement.x, moveInWorldPlane);
    CameraMoveRight(camera, movement.y, moveInWorldPlane);
    CameraMoveUp(camera, movement.z);

    // Zoom target distance
    CameraMoveToTarget(camera, zoom);
}

#endif // RCAMERA_IMPLEMENTATION
