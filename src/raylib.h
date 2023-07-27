/**********************************************************************************************
*
*   raylib v4.6-dev - A simple and easy-to-use library to enjoy videogames programming (www.raylib.com)
*
*   FEATURES:
*       - NO external dependencies, all required libraries included with raylib
*       - Multiplatform: Windows, Linux, FreeBSD, OpenBSD, NetBSD, DragonFly,
*                        MacOS, Haiku, Android, Raspberry Pi, DRM native, HTML5.
*       - Written in plain C code (C99) in PascalCase/camelCase notation
*       - Hardware accelerated with OpenGL (1.1, 2.1, 3.3, 4.3 or ES2 - choose at compile)
*       - Unique OpenGL abstraction layer (usable as standalone module): [rlgl]
*       - Multiple Fonts formats supported (TTF, XNA fonts, AngelCode fonts)
*       - Outstanding texture formats support, including compressed formats (DXT, ETC, ASTC)
*       - Full 3d support for 3d Shapes, Models, Billboards, Heightmaps and more!
*       - Flexible Materials system, supporting classic maps and PBR maps
*       - Animated 3D models supported (skeletal bones animation) (IQM)
*       - Shaders support, including RlModel shaders and Postprocessing shaders
*       - Powerful math module for Vector, RlMatrix and RlQuaternion operations: [raymath]
*       - Audio loading and playing with streaming support (WAV, OGG, MP3, FLAC, XM, MOD)
*       - VR stereo rendering with configurable HMD device parameters
*       - Bindings to multiple programming languages available!
*
*   NOTES:
*       - One default RlFont is loaded on RL_InitWindow()->LoadFontDefault() [core, text]
*       - One default RlTexture2D is loaded on rlglInit(), 1x1 white pixel R8G8B8A8 [rlgl] (OpenGL 3.3 or ES2)
*       - One default RlShader is loaded on rlglInit()->rlLoadShaderDefault() [rlgl] (OpenGL 3.3 or ES2)
*       - One default RenderBatch is loaded on rlglInit()->rlLoadRenderBatch() [rlgl] (OpenGL 3.3 or ES2)
*
*   DEPENDENCIES (included):
*       [rcore] rglfw (Camilla Löwy - github.com/glfw/glfw) for window/context management and input (PLATFORM_DESKTOP)
*       [rlgl] glad (David Herberth - github.com/Dav1dde/glad) for OpenGL 3.3 extensions loading (PLATFORM_DESKTOP)
*       [raudio] miniaudio (David Reid - github.com/mackron/miniaudio) for audio device/context management
*
*   OPTIONAL DEPENDENCIES (included):
*       [rcore] msf_gif (Miles Fogle) for GIF recording
*       [rcore] sinfl (Micha Mettke) for DEFLATE decompression algorithm
*       [rcore] sdefl (Micha Mettke) for DEFLATE compression algorithm
*       [rtextures] stb_image (Sean Barret) for images loading (BMP, TGA, PNG, JPEG, HDR...)
*       [rtextures] stb_image_write (Sean Barret) for image writing (BMP, TGA, PNG, JPG)
*       [rtextures] stb_image_resize (Sean Barret) for image resizing algorithms
*       [rtext] stb_truetype (Sean Barret) for ttf fonts loading
*       [rtext] stb_rect_pack (Sean Barret) for rectangles packing
*       [rmodels] par_shapes (Philip Rideout) for parametric 3d shapes generation
*       [rmodels] tinyobj_loader_c (Syoyo Fujita) for models loading (OBJ, MTL)
*       [rmodels] cgltf (Johannes Kuhlmann) for models loading (glTF)
*       [rmodels] Model3D (bzt) for models loading (M3D, https://bztsrc.gitlab.io/model3d)
*       [raudio] dr_wav (David Reid) for WAV audio file loading
*       [raudio] dr_flac (David Reid) for FLAC audio file loading
*       [raudio] dr_mp3 (David Reid) for MP3 audio file loading
*       [raudio] stb_vorbis (Sean Barret) for OGG audio loading
*       [raudio] jar_xm (Joshua Reisenauer) for XM audio module loading
*       [raudio] jar_mod (Joshua Reisenauer) for MOD audio module loading
*
*
*   LICENSE: zlib/libpng
*
*   raylib is licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software:
*
*   Copyright (c) 2013-2023 Ramon Santamaria (@raysan5)
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

#ifndef RAYLIB_H
#define RAYLIB_H

#include <stdarg.h>     // Required for: va_list - Only used by TraceLogCallback

#define RAYLIB_VERSION_MAJOR 4
#define RAYLIB_VERSION_MINOR 6
#define RAYLIB_VERSION_PATCH 0
#define RAYLIB_VERSION  "4.6-dev"

// Function specifiers in case library is build/used as a shared library (Windows)
// NOTE: Microsoft specifiers to tell compiler that symbols are imported/exported from a .dll
#if defined(_WIN32)
    #if defined(BUILD_LIBTYPE_SHARED)
        #if defined(__TINYC__)
            #define __declspec(x) __attribute__((x))
        #endif
        #define RLAPI __declspec(dllexport)     // We are building the library as a Win32 shared library (.dll)
    #elif defined(USE_LIBTYPE_SHARED)
        #define RLAPI __declspec(dllimport)     // We are using the library as a Win32 shared library (.dll)
    #endif
#endif

#ifndef RLAPI
    #define RLAPI       // Functions defined as 'extern' by default (implicit specifiers)
#endif

//----------------------------------------------------------------------------------
// Some basic Defines
//----------------------------------------------------------------------------------
#ifndef PI
    #define PI 3.14159265358979323846f
#endif
#ifndef DEG2RAD
    #define DEG2RAD (PI/180.0f)
#endif
#ifndef RAD2DEG
    #define RAD2DEG (180.0f/PI)
#endif

// Allow custom memory allocators
// NOTE: Require recompiling raylib sources
#ifndef RL_MALLOC
    #define RL_MALLOC(sz)       malloc(sz)
#endif
#ifndef RL_CALLOC
    #define RL_CALLOC(n,sz)     calloc(n,sz)
#endif
#ifndef RL_REALLOC
    #define RL_REALLOC(ptr,sz)  realloc(ptr,sz)
#endif
#ifndef RL_FREE
    #define RL_FREE(ptr)        free(ptr)
#endif

// NOTE: MSVC C++ compiler does not support compound literals (C99 feature)
// Plain structures in C++ (without constructors) can be initialized with { }
// This is called aggregate initialization (C++11 feature)
#if defined(__cplusplus)
    #define CLITERAL(type)      type
#else
    #define CLITERAL(type)      (type)
#endif

// Some compilers (mostly macos clang) default to C++98,
// where aggregate initialization can't be used
// So, give a more clear error stating how to fix this
#if !defined(_MSC_VER) && (defined(__cplusplus) && __cplusplus < 201103L)
    #error "C++11 or later is required. Add -std=c++11"
#endif

// NOTE: We set some defines with some data types declared by raylib
// Other modules (raymath, rlgl) also require some of those types, so,
// to be able to use those other modules as standalone (not depending on raylib)
// this defines are very useful for internal check and avoid type (re)definitions
#define RL_COLOR_TYPE
#define RL_RECTANGLE_TYPE
#define RL_VECTOR2_TYPE
#define RL_VECTOR3_TYPE
#define RL_VECTOR4_TYPE
#define RL_QUATERNION_TYPE
#define RL_MATRIX_TYPE

// Some Basic Colors
// NOTE: Custom raylib color palette for amazing visuals on WHITE background
#define LIGHTGRAY  CLITERAL(RlColor){ 200, 200, 200, 255 }   // Light Gray
#define GRAY       CLITERAL(RlColor){ 130, 130, 130, 255 }   // Gray
#define DARKGRAY   CLITERAL(RlColor){ 80, 80, 80, 255 }      // Dark Gray
#define YELLOW     CLITERAL(RlColor){ 253, 249, 0, 255 }     // Yellow
#define GOLD       CLITERAL(RlColor){ 255, 203, 0, 255 }     // Gold
#define ORANGE     CLITERAL(RlColor){ 255, 161, 0, 255 }     // Orange
#define PINK       CLITERAL(RlColor){ 255, 109, 194, 255 }   // Pink
#define RED        CLITERAL(RlColor){ 230, 41, 55, 255 }     // Red
#define MAROON     CLITERAL(RlColor){ 190, 33, 55, 255 }     // Maroon
#define GREEN      CLITERAL(RlColor){ 0, 228, 48, 255 }      // Green
#define LIME       CLITERAL(RlColor){ 0, 158, 47, 255 }      // Lime
#define DARKGREEN  CLITERAL(RlColor){ 0, 117, 44, 255 }      // Dark Green
#define SKYBLUE    CLITERAL(RlColor){ 102, 191, 255, 255 }   // Sky Blue
#define BLUE       CLITERAL(RlColor){ 0, 121, 241, 255 }     // Blue
#define DARKBLUE   CLITERAL(RlColor){ 0, 82, 172, 255 }      // Dark Blue
#define PURPLE     CLITERAL(RlColor){ 200, 122, 255, 255 }   // Purple
#define VIOLET     CLITERAL(RlColor){ 135, 60, 190, 255 }    // Violet
#define DARKPURPLE CLITERAL(RlColor){ 112, 31, 126, 255 }    // Dark Purple
#define BEIGE      CLITERAL(RlColor){ 211, 176, 131, 255 }   // Beige
#define BROWN      CLITERAL(RlColor){ 127, 106, 79, 255 }    // Brown
#define DARKBROWN  CLITERAL(RlColor){ 76, 63, 47, 255 }      // Dark Brown

#define WHITE      CLITERAL(RlColor){ 255, 255, 255, 255 }   // White
#define BLACK      CLITERAL(RlColor){ 0, 0, 0, 255 }         // Black
#define BLANK      CLITERAL(RlColor){ 0, 0, 0, 0 }           // Blank (Transparent)
#define MAGENTA    CLITERAL(RlColor){ 255, 0, 255, 255 }     // Magenta
#define RAYWHITE   CLITERAL(RlColor){ 245, 245, 245, 255 }   // My own White (raylib logo)

//----------------------------------------------------------------------------------
// Structures Definition
//----------------------------------------------------------------------------------
// Boolean type
#if (defined(__STDC__) && __STDC_VERSION__ >= 199901L) || (defined(_MSC_VER) && _MSC_VER >= 1800)
    #include <stdbool.h>
#elif !defined(__cplusplus) && !defined(bool)
    typedef enum bool { false = 0, true = !false } bool;
    #define RL_BOOL_TYPE
#endif

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

// RlVector4, 4 components
typedef struct RlVector4 {
    float x;                // Vector x component
    float y;                // Vector y component
    float z;                // Vector z component
    float w;                // Vector w component
} RlVector4;

// RlQuaternion, 4 components (RlVector4 alias)
typedef RlVector4 RlQuaternion;

// RlMatrix, 4x4 components, column major, OpenGL style, right-handed
typedef struct RlMatrix {
    float m0, m4, m8, m12;  // RlMatrix first row (4 components)
    float m1, m5, m9, m13;  // RlMatrix second row (4 components)
    float m2, m6, m10, m14; // RlMatrix third row (4 components)
    float m3, m7, m11, m15; // RlMatrix fourth row (4 components)
} RlMatrix;

// RlColor, 4 components, R8G8B8A8 (32bit)
typedef struct RlColor {
    unsigned char r;        // RlColor red value
    unsigned char g;        // RlColor green value
    unsigned char b;        // RlColor blue value
    unsigned char a;        // RlColor alpha value
} RlColor;

// RlRectangle, 4 components
typedef struct RlRectangle {
    float x;                // RlRectangle top-left corner position x
    float y;                // RlRectangle top-left corner position y
    float width;            // RlRectangle width
    float height;           // RlRectangle height
} RlRectangle;

// RlImage, pixel data stored in CPU memory (RAM)
typedef struct RlImage {
    void *data;             // RlImage raw data
    int width;              // RlImage base width
    int height;             // RlImage base height
    int mipmaps;            // Mipmap levels, 1 by default
    int format;             // Data format (RlPixelFormat type)
} RlImage;

// RlTexture, tex data stored in GPU memory (VRAM)
typedef struct RlTexture {
    unsigned int id;        // OpenGL texture id
    int width;              // RlTexture base width
    int height;             // RlTexture base height
    int mipmaps;            // Mipmap levels, 1 by default
    int format;             // Data format (RlPixelFormat type)
} RlTexture;

// RlTexture2D, same as RlTexture
typedef RlTexture RlTexture2D;

// RlTextureCubemap, same as RlTexture
typedef RlTexture RlTextureCubemap;

// RlRenderTexture, fbo for texture rendering
typedef struct RlRenderTexture {
    unsigned int id;        // OpenGL framebuffer object id
    RlTexture texture;        // RlColor buffer attachment texture
    RlTexture depth;          // Depth buffer attachment texture
} RlRenderTexture;

// RlRenderTexture2D, same as RlRenderTexture
typedef RlRenderTexture RlRenderTexture2D;

// RlNPatchInfo, n-patch layout info
typedef struct RlNPatchInfo {
    RlRectangle source;       // RlTexture source rectangle
    int left;               // Left border offset
    int top;                // Top border offset
    int right;              // Right border offset
    int bottom;             // Bottom border offset
    int layout;             // Layout of the n-patch: 3x3, 1x3 or 3x1
} RlNPatchInfo;

// RlGlyphInfo, font characters glyphs info
typedef struct RlGlyphInfo {
    int value;              // Character value (Unicode)
    int offsetX;            // Character offset X when drawing
    int offsetY;            // Character offset Y when drawing
    int advanceX;           // Character advance position X
    RlImage image;            // Character image data
} RlGlyphInfo;

// RlFont, font texture and RlGlyphInfo array data
typedef struct RlFont {
    int baseSize;           // Base size (default chars height)
    int glyphCount;         // Number of glyph characters
    int glyphPadding;       // Padding around the glyph characters
    RlTexture2D texture;      // RlTexture atlas containing the glyphs
    RlRectangle *recs;        // Rectangles in texture for the glyphs
    RlGlyphInfo *glyphs;      // Glyphs info data
} RlFont;

// RlCamera, defines position/orientation in 3d space
typedef struct RlCamera3D {
    RlVector3 position;       // RlCamera position
    RlVector3 target;         // RlCamera target it looks-at
    RlVector3 up;             // RlCamera up vector (rotation over its axis)
    float fovy;             // RlCamera field-of-view aperture in Y (degrees) in perspective, used as near plane width in orthographic
    int projection;         // RlCamera projection: RL_CAMERA_PERSPECTIVE or RL_CAMERA_ORTHOGRAPHIC
} RlCamera3D;

typedef RlCamera3D RlCamera;    // RlCamera type fallback, defaults to RlCamera3D

// RlCamera2D, defines position/orientation in 2d space
typedef struct RlCamera2D {
    RlVector2 offset;         // RlCamera offset (displacement from target)
    RlVector2 target;         // RlCamera target (rotation and zoom origin)
    float rotation;         // RlCamera rotation in degrees
    float zoom;             // RlCamera zoom (scaling), should be 1.0f by default
} RlCamera2D;

// RlMesh, vertex data and vao/vbo
typedef struct RlMesh {
    int vertexCount;        // Number of vertices stored in arrays
    int triangleCount;      // Number of triangles stored (indexed or not)

    // Vertex attributes data
    float *vertices;        // Vertex position (XYZ - 3 components per vertex) (shader-location = 0)
    float *texcoords;       // Vertex texture coordinates (UV - 2 components per vertex) (shader-location = 1)
    float *texcoords2;      // Vertex texture second coordinates (UV - 2 components per vertex) (shader-location = 5)
    float *normals;         // Vertex normals (XYZ - 3 components per vertex) (shader-location = 2)
    float *tangents;        // Vertex tangents (XYZW - 4 components per vertex) (shader-location = 4)
    unsigned char *colors;      // Vertex colors (RGBA - 4 components per vertex) (shader-location = 3)
    unsigned short *indices;    // Vertex indices (in case vertex data comes indexed)

    // Animation vertex data
    float *animVertices;    // Animated vertex positions (after bones transformations)
    float *animNormals;     // Animated normals (after bones transformations)
    unsigned char *boneIds; // Vertex bone ids, max 255 bone ids, up to 4 bones influence by vertex (skinning)
    float *boneWeights;     // Vertex bone weight, up to 4 bones influence by vertex (skinning)

    // OpenGL identifiers
    unsigned int vaoId;     // OpenGL Vertex Array Object id
    unsigned int *vboId;    // OpenGL Vertex Buffer Objects id (default vertex data)
} RlMesh;

// RlShader
typedef struct RlShader {
    unsigned int id;        // RlShader program id
    int *locs;              // RlShader locations array (RL_MAX_SHADER_LOCATIONS)
} RlShader;

// RlMaterialMap
typedef struct RlMaterialMap {
    RlTexture2D texture;      // RlMaterial map texture
    RlColor color;            // RlMaterial map color
    float value;            // RlMaterial map value
} RlMaterialMap;

// RlMaterial, includes shader and maps
typedef struct RlMaterial {
    RlShader shader;          // RlMaterial shader
    RlMaterialMap *maps;      // RlMaterial maps array (MAX_MATERIAL_MAPS)
    float params[4];        // RlMaterial generic parameters (if required)
} RlMaterial;

// RlTransform, vertex transformation data
typedef struct RlTransform {
    RlVector3 translation;    // Translation
    RlQuaternion rotation;    // Rotation
    RlVector3 scale;          // Scale
} RlTransform;

// Bone, skeletal animation bone
typedef struct RlBoneInfo {
    char name[32];          // Bone name
    int parent;             // Bone parent
} RlBoneInfo;

// RlModel, meshes, materials and animation data
typedef struct RlModel {
    RlMatrix transform;       // Local transform matrix

    int meshCount;          // Number of meshes
    int materialCount;      // Number of materials
    RlMesh *meshes;           // Meshes array
    RlMaterial *materials;    // Materials array
    int *meshMaterial;      // RlMesh material number

    // Animation data
    int boneCount;          // Number of bones
    RlBoneInfo *bones;        // Bones information (skeleton)
    RlTransform *bindPose;    // Bones base transformation (pose)
} RlModel;

// RlModelAnimation
typedef struct RlModelAnimation {
    int boneCount;          // Number of bones
    int frameCount;         // Number of animation frames
    RlBoneInfo *bones;        // Bones information (skeleton)
    RlTransform **framePoses; // Poses array by frame
    char name[32];          // Animation name
} RlModelAnimation;

// RlRay, ray for raycasting
typedef struct RlRay {
    RlVector3 position;       // RlRay position (origin)
    RlVector3 direction;      // RlRay direction
} RlRay;

// RlRayCollision, ray hit information
typedef struct RlRayCollision {
    bool hit;               // Did the ray hit something?
    float distance;         // Distance to the nearest hit
    RlVector3 point;          // Point of the nearest hit
    RlVector3 normal;         // Surface normal of hit
} RlRayCollision;

// RlBoundingBox
typedef struct RlBoundingBox {
    RlVector3 min;            // Minimum vertex box-corner
    RlVector3 max;            // Maximum vertex box-corner
} RlBoundingBox;

// RlWave, audio wave data
typedef struct RlWave {
    unsigned int frameCount;    // Total number of frames (considering channels)
    unsigned int sampleRate;    // Frequency (samples per second)
    unsigned int sampleSize;    // Bit depth (bits per sample): 8, 16, 32 (24 not supported)
    unsigned int channels;      // Number of channels (1-mono, 2-stereo, ...)
    void *data;                 // Buffer data pointer
} RlWave;

// Opaque structs declaration
// NOTE: Actual structs are defined internally in raudio module
typedef struct rAudioBuffer rAudioBuffer;
typedef struct rAudioProcessor rAudioProcessor;

// RlAudioStream, custom audio stream
typedef struct RlAudioStream {
    rAudioBuffer *buffer;       // Pointer to internal data used by the audio system
    rAudioProcessor *processor; // Pointer to internal data processor, useful for audio effects

    unsigned int sampleRate;    // Frequency (samples per second)
    unsigned int sampleSize;    // Bit depth (bits per sample): 8, 16, 32 (24 not supported)
    unsigned int channels;      // Number of channels (1-mono, 2-stereo, ...)
} RlAudioStream;

// RlSound
typedef struct RlSound {
    RlAudioStream stream;         // Audio stream
    unsigned int frameCount;    // Total number of frames (considering channels)
} RlSound;

// RlMusic, audio stream, anything longer than ~10 seconds should be streamed
typedef struct RlMusic {
    RlAudioStream stream;         // Audio stream
    unsigned int frameCount;    // Total number of frames (considering channels)
    bool looping;               // RlMusic looping enable

    int ctxType;                // Type of music context (audio filetype)
    void *ctxData;              // Audio context data, depends on type
} RlMusic;

// RlVrDeviceInfo, Head-Mounted-Display device parameters
typedef struct RlVrDeviceInfo {
    int hResolution;                // Horizontal resolution in pixels
    int vResolution;                // Vertical resolution in pixels
    float hScreenSize;              // Horizontal size in meters
    float vScreenSize;              // Vertical size in meters
    float vScreenCenter;            // Screen center in meters
    float eyeToScreenDistance;      // Distance between eye and display in meters
    float lensSeparationDistance;   // Lens separation distance in meters
    float interpupillaryDistance;   // IPD (distance between pupils) in meters
    float lensDistortionValues[4];  // Lens distortion constant parameters
    float chromaAbCorrection[4];    // Chromatic aberration correction parameters
} RlVrDeviceInfo;

// RlVrStereoConfig, VR stereo rendering configuration for simulator
typedef struct RlVrStereoConfig {
    RlMatrix projection[2];           // VR projection matrices (per eye)
    RlMatrix viewOffset[2];           // VR view offset matrices (per eye)
    float leftLensCenter[2];        // VR left lens center
    float rightLensCenter[2];       // VR right lens center
    float leftScreenCenter[2];      // VR left screen center
    float rightScreenCenter[2];     // VR right screen center
    float scale[2];                 // VR distortion scale
    float scaleIn[2];               // VR distortion scale in
} RlVrStereoConfig;

// File path list
typedef struct RlFilePathList {
    unsigned int capacity;          // Filepaths max entries
    unsigned int count;             // Filepaths entries count
    char **paths;                   // Filepaths entries
} RlFilePathList;

//----------------------------------------------------------------------------------
// Enumerators Definition
//----------------------------------------------------------------------------------
// System/Window config flags
// NOTE: Every bit registers one state (use it with bit masks)
// By default all flags are set to 0
typedef enum {
    RL_FLAG_VSYNC_HINT         = 0x00000040,   // Set to try enabling V-Sync on GPU
    RL_FLAG_FULLSCREEN_MODE    = 0x00000002,   // Set to run program in fullscreen
    RL_FLAG_WINDOW_RESIZABLE   = 0x00000004,   // Set to allow resizable window
    RL_FLAG_WINDOW_UNDECORATED = 0x00000008,   // Set to disable window decoration (frame and buttons)
    RL_FLAG_WINDOW_HIDDEN      = 0x00000080,   // Set to hide window
    RL_FLAG_WINDOW_MINIMIZED   = 0x00000200,   // Set to minimize window (iconify)
    RL_FLAG_WINDOW_MAXIMIZED   = 0x00000400,   // Set to maximize window (expanded to monitor)
    RL_FLAG_WINDOW_UNFOCUSED   = 0x00000800,   // Set to window non focused
    RL_FLAG_WINDOW_TOPMOST     = 0x00001000,   // Set to window always on top
    RL_FLAG_WINDOW_ALWAYS_RUN  = 0x00000100,   // Set to allow windows running while minimized
    RL_FLAG_WINDOW_TRANSPARENT = 0x00000010,   // Set to allow transparent framebuffer
    RL_FLAG_WINDOW_HIGHDPI     = 0x00002000,   // Set to support HighDPI
    RL_FLAG_WINDOW_MOUSE_PASSTHROUGH = 0x00004000, // Set to support mouse passthrough, only supported when RL_FLAG_WINDOW_UNDECORATED
    RL_FLAG_MSAA_4X_HINT       = 0x00000020,   // Set to try enabling MSAA 4X
    RL_FLAG_INTERLACED_HINT    = 0x00010000    // Set to try enabling interlaced video format (for V3D)
} ConfigFlags;

// Trace log level
// NOTE: Organized by priority level
typedef enum {
    RL_LOG_ALL = 0,        // Display all logs
    RL_LOG_TRACE,          // Trace logging, intended for internal use only
    RL_LOG_DEBUG,          // Debug logging, used for internal debugging, it should be disabled on release builds
    RL_LOG_INFO,           // Info logging, used for program execution info
    RL_LOG_WARNING,        // Warning logging, used on recoverable failures
    RL_LOG_ERROR,          // Error logging, used on unrecoverable failures
    RL_LOG_FATAL,          // Fatal logging, used to abort program: exit(EXIT_FAILURE)
    RL_LOG_NONE            // Disable logging
} TraceLogLevel;

// Keyboard keys (US keyboard layout)
// NOTE: Use RL_GetKeyPressed() to allow redefining
// required keys for alternative layouts
typedef enum {
    RL_KEY_NULL            = 0,        // Key: NULL, used for no key pressed
    // Alphanumeric keys
    RL_KEY_APOSTROPHE      = 39,       // Key: '
    RL_KEY_COMMA           = 44,       // Key: ,
    RL_KEY_MINUS           = 45,       // Key: -
    RL_KEY_PERIOD          = 46,       // Key: .
    RL_KEY_SLASH           = 47,       // Key: /
    RL_KEY_ZERO            = 48,       // Key: 0
    RL_KEY_ONE             = 49,       // Key: 1
    RL_KEY_TWO             = 50,       // Key: 2
    RL_KEY_THREE           = 51,       // Key: 3
    RL_KEY_FOUR            = 52,       // Key: 4
    RL_KEY_FIVE            = 53,       // Key: 5
    RL_KEY_SIX             = 54,       // Key: 6
    RL_KEY_SEVEN           = 55,       // Key: 7
    RL_KEY_EIGHT           = 56,       // Key: 8
    RL_KEY_NINE            = 57,       // Key: 9
    RL_KEY_SEMICOLON       = 59,       // Key: ;
    RL_KEY_EQUAL           = 61,       // Key: =
    RL_KEY_A               = 65,       // Key: A | a
    RL_KEY_B               = 66,       // Key: B | b
    RL_KEY_C               = 67,       // Key: C | c
    RL_KEY_D               = 68,       // Key: D | d
    RL_KEY_E               = 69,       // Key: E | e
    RL_KEY_F               = 70,       // Key: F | f
    RL_KEY_G               = 71,       // Key: G | g
    RL_KEY_H               = 72,       // Key: H | h
    RL_KEY_I               = 73,       // Key: I | i
    RL_KEY_J               = 74,       // Key: J | j
    RL_KEY_K               = 75,       // Key: K | k
    RL_KEY_L               = 76,       // Key: L | l
    RL_KEY_M               = 77,       // Key: M | m
    RL_KEY_N               = 78,       // Key: N | n
    RL_KEY_O               = 79,       // Key: O | o
    RL_KEY_P               = 80,       // Key: P | p
    RL_KEY_Q               = 81,       // Key: Q | q
    RL_KEY_R               = 82,       // Key: R | r
    RL_KEY_S               = 83,       // Key: S | s
    RL_KEY_T               = 84,       // Key: T | t
    RL_KEY_U               = 85,       // Key: U | u
    RL_KEY_V               = 86,       // Key: V | v
    RL_KEY_W               = 87,       // Key: W | w
    RL_KEY_X               = 88,       // Key: X | x
    RL_KEY_Y               = 89,       // Key: Y | y
    RL_KEY_Z               = 90,       // Key: Z | z
    RL_KEY_LEFT_BRACKET    = 91,       // Key: [
    RL_KEY_BACKSLASH       = 92,       // Key: '\'
    RL_KEY_RIGHT_BRACKET   = 93,       // Key: ]
    RL_KEY_GRAVE           = 96,       // Key: `
    // Function keys
    RL_KEY_SPACE           = 32,       // Key: Space
    RL_KEY_ESCAPE          = 256,      // Key: Esc
    RL_KEY_ENTER           = 257,      // Key: Enter
    RL_KEY_TAB             = 258,      // Key: Tab
    RL_KEY_BACKSPACE       = 259,      // Key: Backspace
    RL_KEY_INSERT          = 260,      // Key: Ins
    RL_KEY_DELETE          = 261,      // Key: Del
    RL_KEY_RIGHT           = 262,      // Key: Cursor right
    RL_KEY_LEFT            = 263,      // Key: Cursor left
    RL_KEY_DOWN            = 264,      // Key: Cursor down
    RL_KEY_UP              = 265,      // Key: Cursor up
    RL_KEY_PAGE_UP         = 266,      // Key: Page up
    RL_KEY_PAGE_DOWN       = 267,      // Key: Page down
    RL_KEY_HOME            = 268,      // Key: Home
    RL_KEY_END             = 269,      // Key: End
    RL_KEY_CAPS_LOCK       = 280,      // Key: Caps lock
    RL_KEY_SCROLL_LOCK     = 281,      // Key: Scroll down
    RL_KEY_NUM_LOCK        = 282,      // Key: Num lock
    RL_KEY_PRINT_SCREEN    = 283,      // Key: Print screen
    RL_KEY_PAUSE           = 284,      // Key: Pause
    RL_KEY_F1              = 290,      // Key: F1
    RL_KEY_F2              = 291,      // Key: F2
    RL_KEY_F3              = 292,      // Key: F3
    RL_KEY_F4              = 293,      // Key: F4
    RL_KEY_F5              = 294,      // Key: F5
    RL_KEY_F6              = 295,      // Key: F6
    RL_KEY_F7              = 296,      // Key: F7
    RL_KEY_F8              = 297,      // Key: F8
    RL_KEY_F9              = 298,      // Key: F9
    RL_KEY_F10             = 299,      // Key: F10
    RL_KEY_F11             = 300,      // Key: F11
    RL_KEY_F12             = 301,      // Key: F12
    RL_KEY_LEFT_SHIFT      = 340,      // Key: Shift left
    RL_KEY_LEFT_CONTROL    = 341,      // Key: Control left
    RL_KEY_LEFT_ALT        = 342,      // Key: Alt left
    RL_KEY_LEFT_SUPER      = 343,      // Key: Super left
    RL_KEY_RIGHT_SHIFT     = 344,      // Key: Shift right
    RL_KEY_RIGHT_CONTROL   = 345,      // Key: Control right
    RL_KEY_RIGHT_ALT       = 346,      // Key: Alt right
    RL_KEY_RIGHT_SUPER     = 347,      // Key: Super right
    RL_KEY_KB_MENU         = 348,      // Key: KB menu
    // Keypad keys
    RL_KEY_KP_0            = 320,      // Key: Keypad 0
    RL_KEY_KP_1            = 321,      // Key: Keypad 1
    RL_KEY_KP_2            = 322,      // Key: Keypad 2
    RL_KEY_KP_3            = 323,      // Key: Keypad 3
    RL_KEY_KP_4            = 324,      // Key: Keypad 4
    RL_KEY_KP_5            = 325,      // Key: Keypad 5
    RL_KEY_KP_6            = 326,      // Key: Keypad 6
    RL_KEY_KP_7            = 327,      // Key: Keypad 7
    RL_KEY_KP_8            = 328,      // Key: Keypad 8
    RL_KEY_KP_9            = 329,      // Key: Keypad 9
    RL_KEY_KP_DECIMAL      = 330,      // Key: Keypad .
    RL_KEY_KP_DIVIDE       = 331,      // Key: Keypad /
    RL_KEY_KP_MULTIPLY     = 332,      // Key: Keypad *
    RL_KEY_KP_SUBTRACT     = 333,      // Key: Keypad -
    RL_KEY_KP_ADD          = 334,      // Key: Keypad +
    RL_KEY_KP_ENTER        = 335,      // Key: Keypad Enter
    RL_KEY_KP_EQUAL        = 336,      // Key: Keypad =
    // Android key buttons
    RL_KEY_BACK            = 4,        // Key: Android back button
    RL_KEY_MENU            = 82,       // Key: Android menu button
    RL_KEY_VOLUME_UP       = 24,       // Key: Android volume up button
    RL_KEY_VOLUME_DOWN     = 25        // Key: Android volume down button
} RlKeyboardKey;

// Add backwards compatibility support for deprecated names
#define MOUSE_LEFT_BUTTON   RL_MOUSE_BUTTON_LEFT
#define MOUSE_RIGHT_BUTTON  RL_MOUSE_BUTTON_RIGHT
#define MOUSE_MIDDLE_BUTTON RL_MOUSE_BUTTON_MIDDLE

// Mouse buttons
typedef enum {
    RL_MOUSE_BUTTON_LEFT    = 0,       // Mouse button left
    RL_MOUSE_BUTTON_RIGHT   = 1,       // Mouse button right
    RL_MOUSE_BUTTON_MIDDLE  = 2,       // Mouse button middle (pressed wheel)
    RL_MOUSE_BUTTON_SIDE    = 3,       // Mouse button side (advanced mouse device)
    RL_MOUSE_BUTTON_EXTRA   = 4,       // Mouse button extra (advanced mouse device)
    RL_MOUSE_BUTTON_FORWARD = 5,       // Mouse button forward (advanced mouse device)
    RL_MOUSE_BUTTON_BACK    = 6,       // Mouse button back (advanced mouse device)
} RlMouseButton;

// Mouse cursor
typedef enum {
    RL_MOUSE_CURSOR_DEFAULT       = 0,     // Default pointer shape
    RL_MOUSE_CURSOR_ARROW         = 1,     // Arrow shape
    RL_MOUSE_CURSOR_IBEAM         = 2,     // Text writing cursor shape
    RL_MOUSE_CURSOR_CROSSHAIR     = 3,     // Cross shape
    RL_MOUSE_CURSOR_POINTING_HAND = 4,     // Pointing hand cursor
    RL_MOUSE_CURSOR_RESIZE_EW     = 5,     // Horizontal resize/move arrow shape
    RL_MOUSE_CURSOR_RESIZE_NS     = 6,     // Vertical resize/move arrow shape
    RL_MOUSE_CURSOR_RESIZE_NWSE   = 7,     // Top-left to bottom-right diagonal resize/move arrow shape
    RL_MOUSE_CURSOR_RESIZE_NESW   = 8,     // The top-right to bottom-left diagonal resize/move arrow shape
    RL_MOUSE_CURSOR_RESIZE_ALL    = 9,     // The omnidirectional resize/move cursor shape
    RL_MOUSE_CURSOR_NOT_ALLOWED   = 10     // The operation-not-allowed shape
} RlMouseCursor;

// Gamepad buttons
typedef enum {
    RL_GAMEPAD_BUTTON_UNKNOWN = 0,         // Unknown button, just for error checking
    RL_GAMEPAD_BUTTON_LEFT_FACE_UP,        // Gamepad left DPAD up button
    RL_GAMEPAD_BUTTON_LEFT_FACE_RIGHT,     // Gamepad left DPAD right button
    RL_GAMEPAD_BUTTON_LEFT_FACE_DOWN,      // Gamepad left DPAD down button
    RL_GAMEPAD_BUTTON_LEFT_FACE_LEFT,      // Gamepad left DPAD left button
    RL_GAMEPAD_BUTTON_RIGHT_FACE_UP,       // Gamepad right button up (i.e. PS3: Triangle, Xbox: Y)
    RL_GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,    // Gamepad right button right (i.e. PS3: Square, Xbox: X)
    RL_GAMEPAD_BUTTON_RIGHT_FACE_DOWN,     // Gamepad right button down (i.e. PS3: Cross, Xbox: A)
    RL_GAMEPAD_BUTTON_RIGHT_FACE_LEFT,     // Gamepad right button left (i.e. PS3: Circle, Xbox: B)
    RL_GAMEPAD_BUTTON_LEFT_TRIGGER_1,      // Gamepad top/back trigger left (first), it could be a trailing button
    RL_GAMEPAD_BUTTON_LEFT_TRIGGER_2,      // Gamepad top/back trigger left (second), it could be a trailing button
    RL_GAMEPAD_BUTTON_RIGHT_TRIGGER_1,     // Gamepad top/back trigger right (one), it could be a trailing button
    RL_GAMEPAD_BUTTON_RIGHT_TRIGGER_2,     // Gamepad top/back trigger right (second), it could be a trailing button
    RL_GAMEPAD_BUTTON_MIDDLE_LEFT,         // Gamepad center buttons, left one (i.e. PS3: Select)
    RL_GAMEPAD_BUTTON_MIDDLE,              // Gamepad center buttons, middle one (i.e. PS3: PS, Xbox: XBOX)
    RL_GAMEPAD_BUTTON_MIDDLE_RIGHT,        // Gamepad center buttons, right one (i.e. PS3: Start)
    RL_GAMEPAD_BUTTON_LEFT_THUMB,          // Gamepad joystick pressed button left
    RL_GAMEPAD_BUTTON_RIGHT_THUMB          // Gamepad joystick pressed button right
} RlGamepadButton;

// Gamepad axis
typedef enum {
    RL_GAMEPAD_AXIS_LEFT_X        = 0,     // Gamepad left stick X axis
    RL_GAMEPAD_AXIS_LEFT_Y        = 1,     // Gamepad left stick Y axis
    RL_GAMEPAD_AXIS_RIGHT_X       = 2,     // Gamepad right stick X axis
    RL_GAMEPAD_AXIS_RIGHT_Y       = 3,     // Gamepad right stick Y axis
    RL_GAMEPAD_AXIS_LEFT_TRIGGER  = 4,     // Gamepad back trigger left, pressure level: [1..-1]
    RL_GAMEPAD_AXIS_RIGHT_TRIGGER = 5      // Gamepad back trigger right, pressure level: [1..-1]
} RlGamepadAxis;

// RlMaterial map index
typedef enum {
    RL_MATERIAL_MAP_ALBEDO = 0,        // Albedo material (same as: MATERIAL_MAP_DIFFUSE)
    RL_MATERIAL_MAP_METALNESS,         // Metalness material (same as: MATERIAL_MAP_SPECULAR)
    RL_MATERIAL_MAP_NORMAL,            // Normal material
    RL_MATERIAL_MAP_ROUGHNESS,         // Roughness material
    RL_MATERIAL_MAP_OCCLUSION,         // Ambient occlusion material
    RL_MATERIAL_MAP_EMISSION,          // Emission material
    RL_MATERIAL_MAP_HEIGHT,            // Heightmap material
    RL_MATERIAL_MAP_CUBEMAP,           // Cubemap material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
    RL_MATERIAL_MAP_IRRADIANCE,        // Irradiance material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
    RL_MATERIAL_MAP_PREFILTER,         // Prefilter material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
    RL_MATERIAL_MAP_BRDF               // Brdf material
} RlMaterialMapIndex;

#define MATERIAL_MAP_DIFFUSE      RL_MATERIAL_MAP_ALBEDO
#define MATERIAL_MAP_SPECULAR     RL_MATERIAL_MAP_METALNESS

// RlShader location index
typedef enum {
    RL_SHADER_LOC_VERTEX_POSITION = 0, // RlShader location: vertex attribute: position
    RL_SHADER_LOC_VERTEX_TEXCOORD01,   // RlShader location: vertex attribute: texcoord01
    RL_SHADER_LOC_VERTEX_TEXCOORD02,   // RlShader location: vertex attribute: texcoord02
    RL_SHADER_LOC_VERTEX_NORMAL,       // RlShader location: vertex attribute: normal
    RL_SHADER_LOC_VERTEX_TANGENT,      // RlShader location: vertex attribute: tangent
    RL_SHADER_LOC_VERTEX_COLOR,        // RlShader location: vertex attribute: color
    RL_SHADER_LOC_MATRIX_MVP,          // RlShader location: matrix uniform: model-view-projection
    RL_SHADER_LOC_MATRIX_VIEW,         // RlShader location: matrix uniform: view (camera transform)
    RL_SHADER_LOC_MATRIX_PROJECTION,   // RlShader location: matrix uniform: projection
    RL_SHADER_LOC_MATRIX_MODEL,        // RlShader location: matrix uniform: model (transform)
    RL_SHADER_LOC_MATRIX_NORMAL,       // RlShader location: matrix uniform: normal
    RL_SHADER_LOC_VECTOR_VIEW,         // RlShader location: vector uniform: view
    RL_SHADER_LOC_COLOR_DIFFUSE,       // RlShader location: vector uniform: diffuse color
    RL_SHADER_LOC_COLOR_SPECULAR,      // RlShader location: vector uniform: specular color
    RL_SHADER_LOC_COLOR_AMBIENT,       // RlShader location: vector uniform: ambient color
    RL_SHADER_LOC_MAP_ALBEDO,          // RlShader location: sampler2d texture: albedo (same as: RL_SHADER_LOC_MAP_DIFFUSE)
    RL_SHADER_LOC_MAP_METALNESS,       // RlShader location: sampler2d texture: metalness (same as: RL_SHADER_LOC_MAP_SPECULAR)
    RL_SHADER_LOC_MAP_NORMAL,          // RlShader location: sampler2d texture: normal
    RL_SHADER_LOC_MAP_ROUGHNESS,       // RlShader location: sampler2d texture: roughness
    RL_SHADER_LOC_MAP_OCCLUSION,       // RlShader location: sampler2d texture: occlusion
    RL_SHADER_LOC_MAP_EMISSION,        // RlShader location: sampler2d texture: emission
    RL_SHADER_LOC_MAP_HEIGHT,          // RlShader location: sampler2d texture: height
    RL_SHADER_LOC_MAP_CUBEMAP,         // RlShader location: samplerCube texture: cubemap
    RL_SHADER_LOC_MAP_IRRADIANCE,      // RlShader location: samplerCube texture: irradiance
    RL_SHADER_LOC_MAP_PREFILTER,       // RlShader location: samplerCube texture: prefilter
    RL_SHADER_LOC_MAP_BRDF             // RlShader location: sampler2d texture: brdf
} RlShaderLocationIndex;

#define RL_SHADER_LOC_MAP_DIFFUSE      RL_SHADER_LOC_MAP_ALBEDO
#define RL_SHADER_LOC_MAP_SPECULAR     RL_SHADER_LOC_MAP_METALNESS

// RlShader uniform data type
typedef enum {
    RL_SHADER_UNIFORM_FLOAT = 0,       // RlShader uniform type: float
    RL_SHADER_UNIFORM_VEC2,            // RlShader uniform type: vec2 (2 float)
    RL_SHADER_UNIFORM_VEC3,            // RlShader uniform type: vec3 (3 float)
    RL_SHADER_UNIFORM_VEC4,            // RlShader uniform type: vec4 (4 float)
    RL_SHADER_UNIFORM_INT,             // RlShader uniform type: int
    RL_SHADER_UNIFORM_IVEC2,           // RlShader uniform type: ivec2 (2 int)
    RL_SHADER_UNIFORM_IVEC3,           // RlShader uniform type: ivec3 (3 int)
    RL_SHADER_UNIFORM_IVEC4,           // RlShader uniform type: ivec4 (4 int)
    RL_SHADER_UNIFORM_SAMPLER2D        // RlShader uniform type: sampler2d
} RlShaderUniformDataType;

// RlShader attribute data types
typedef enum {
    RL_SHADER_ATTRIB_FLOAT = 0,        // RlShader attribute type: float
    RL_SHADER_ATTRIB_VEC2,             // RlShader attribute type: vec2 (2 float)
    RL_SHADER_ATTRIB_VEC3,             // RlShader attribute type: vec3 (3 float)
    RL_SHADER_ATTRIB_VEC4              // RlShader attribute type: vec4 (4 float)
} RlShaderAttributeDataType;

// Pixel formats
// NOTE: Support depends on OpenGL version and platform
typedef enum {
    RL_PIXELFORMAT_UNCOMPRESSED_GRAYSCALE = 1, // 8 bit per pixel (no alpha)
    RL_PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA,    // 8*2 bpp (2 channels)
    RL_PIXELFORMAT_UNCOMPRESSED_R5G6B5,        // 16 bpp
    RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8,        // 24 bpp
    RL_PIXELFORMAT_UNCOMPRESSED_R5G5B5A1,      // 16 bpp (1 bit alpha)
    RL_PIXELFORMAT_UNCOMPRESSED_R4G4B4A4,      // 16 bpp (4 bit alpha)
    RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,      // 32 bpp
    RL_PIXELFORMAT_UNCOMPRESSED_R32,           // 32 bpp (1 channel - float)
    RL_PIXELFORMAT_UNCOMPRESSED_R32G32B32,     // 32*3 bpp (3 channels - float)
    RL_PIXELFORMAT_UNCOMPRESSED_R32G32B32A32,  // 32*4 bpp (4 channels - float)
    RL_PIXELFORMAT_COMPRESSED_DXT1_RGB,        // 4 bpp (no alpha)
    RL_PIXELFORMAT_COMPRESSED_DXT1_RGBA,       // 4 bpp (1 bit alpha)
    RL_PIXELFORMAT_COMPRESSED_DXT3_RGBA,       // 8 bpp
    RL_PIXELFORMAT_COMPRESSED_DXT5_RGBA,       // 8 bpp
    RL_PIXELFORMAT_COMPRESSED_ETC1_RGB,        // 4 bpp
    RL_PIXELFORMAT_COMPRESSED_ETC2_RGB,        // 4 bpp
    RL_PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA,   // 8 bpp
    RL_PIXELFORMAT_COMPRESSED_PVRT_RGB,        // 4 bpp
    RL_PIXELFORMAT_COMPRESSED_PVRT_RGBA,       // 4 bpp
    RL_PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA,   // 8 bpp
    RL_PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA    // 2 bpp
} RlPixelFormat;

// RlTexture parameters: filter mode
// NOTE 1: Filtering considers mipmaps if available in the texture
// NOTE 2: Filter is accordingly set for minification and magnification
typedef enum {
    RL_TEXTURE_FILTER_POINT = 0,               // No filter, just pixel approximation
    RL_TEXTURE_FILTER_BILINEAR,                // Linear filtering
    RL_TEXTURE_FILTER_TRILINEAR,               // Trilinear filtering (linear with mipmaps)
    RL_TEXTURE_FILTER_ANISOTROPIC_4X,          // Anisotropic filtering 4x
    RL_TEXTURE_FILTER_ANISOTROPIC_8X,          // Anisotropic filtering 8x
    RL_TEXTURE_FILTER_ANISOTROPIC_16X,         // Anisotropic filtering 16x
} TextureFilter;

// RlTexture parameters: wrap mode
typedef enum {
    RL_TEXTURE_WRAP_REPEAT = 0,                // Repeats texture in tiled mode
    RL_TEXTURE_WRAP_CLAMP,                     // Clamps texture to edge pixel in tiled mode
    RL_TEXTURE_WRAP_MIRROR_REPEAT,             // Mirrors and repeats the texture in tiled mode
    RL_TEXTURE_WRAP_MIRROR_CLAMP               // Mirrors and clamps to border the texture in tiled mode
} TextureWrap;

// Cubemap layouts
typedef enum {
    RL_CUBEMAP_LAYOUT_AUTO_DETECT = 0,         // Automatically detect layout type
    RL_CUBEMAP_LAYOUT_LINE_VERTICAL,           // Layout is defined by a vertical line with faces
    RL_CUBEMAP_LAYOUT_LINE_HORIZONTAL,         // Layout is defined by a horizontal line with faces
    RL_CUBEMAP_LAYOUT_CROSS_THREE_BY_FOUR,     // Layout is defined by a 3x4 cross with cubemap faces
    RL_CUBEMAP_LAYOUT_CROSS_FOUR_BY_THREE,     // Layout is defined by a 4x3 cross with cubemap faces
    RL_CUBEMAP_LAYOUT_PANORAMA                 // Layout is defined by a panorama image (equirrectangular map)
} RlCubemapLayout;

// RlFont type, defines generation method
typedef enum {
    RL_FONT_DEFAULT = 0,               // Default font generation, anti-aliased
    RL_FONT_BITMAP,                    // Bitmap font generation, no anti-aliasing
    RL_FONT_SDF                        // SDF font generation, requires external shader
} RlFontType;

// RlColor blending modes (pre-defined)
typedef enum {
    RL_BLEND_ALPHA = 0,                // Blend textures considering alpha (default)
    RL_BLEND_ADDITIVE,                 // Blend textures adding colors
    RL_BLEND_MULTIPLIED,               // Blend textures multiplying colors
    RL_BLEND_ADD_COLORS,               // Blend textures adding colors (alternative)
    RL_BLEND_SUBTRACT_COLORS,          // Blend textures subtracting colors (alternative)
    RL_BLEND_ALPHA_PREMULTIPLY,        // Blend premultiplied textures considering alpha
    RL_BLEND_CUSTOM,                   // Blend textures using custom src/dst factors (use rlSetBlendFactors())
    RL_BLEND_CUSTOM_SEPARATE           // Blend textures using custom rgb/alpha separate src/dst factors (use rlSetBlendFactorsSeparate())
} RlBlendMode;

// RlGesture
// NOTE: Provided as bit-wise flags to enable only desired gestures
typedef enum {
    RL_GESTURE_NONE        = 0,        // No gesture
    RL_GESTURE_TAP         = 1,        // Tap gesture
    RL_GESTURE_DOUBLETAP   = 2,        // Double tap gesture
    RL_GESTURE_HOLD        = 4,        // Hold gesture
    RL_GESTURE_DRAG        = 8,        // Drag gesture
    RL_GESTURE_SWIPE_RIGHT = 16,       // Swipe right gesture
    RL_GESTURE_SWIPE_LEFT  = 32,       // Swipe left gesture
    RL_GESTURE_SWIPE_UP    = 64,       // Swipe up gesture
    RL_GESTURE_SWIPE_DOWN  = 128,      // Swipe down gesture
    RL_GESTURE_PINCH_IN    = 256,      // Pinch in gesture
    RL_GESTURE_PINCH_OUT   = 512       // Pinch out gesture
} RlGesture;

// RlCamera system modes
typedef enum {
    RL_CAMERA_CUSTOM = 0,              // Custom camera
    RL_CAMERA_FREE,                    // Free camera
    RL_CAMERA_ORBITAL,                 // Orbital camera
    RL_CAMERA_FIRST_PERSON,            // First person camera
    RL_CAMERA_THIRD_PERSON             // Third person camera
} RlCameraMode;

// RlCamera projection
typedef enum {
    RL_CAMERA_PERSPECTIVE = 0,         // Perspective projection
    RL_CAMERA_ORTHOGRAPHIC             // Orthographic projection
} RlCameraProjection;

// N-patch layout
typedef enum {
    RL_NPATCH_NINE_PATCH = 0,          // Npatch layout: 3x3 tiles
    RL_NPATCH_THREE_PATCH_VERTICAL,    // Npatch layout: 1x3 tiles
    RL_NPATCH_THREE_PATCH_HORIZONTAL   // Npatch layout: 3x1 tiles
} NPatchLayout;

#ifndef RAYLIB_COREDATA_STRUCT
#define RAYLIB_COREDATA_STRUCT
// Core global state context data
typedef struct CoreData {
    struct {
#if defined(PLATFORM_DESKTOP) || defined(PLATFORM_WEB)
        GLFWwindow *handle;                 // GLFW window handle (graphic device)
#endif
#if defined(PLATFORM_RPI)
        EGL_DISPMANX_WINDOW_T handle;       // Native window handle (graphic device)
#endif
#if defined(PLATFORM_ANDROID) || defined(PLATFORM_RPI) || defined(PLATFORM_DRM)
#if defined(PLATFORM_DRM)
        int fd;                             // File descriptor for /dev/dri/...
        drmModeConnector *connector;        // Direct Rendering Manager (DRM) mode connector
        drmModeCrtc *crtc;                  // CRT Controller
        int modeIndex;                      // Index of the used mode of connector->modes
        struct gbm_device *gbmDevice;       // GBM device
        struct gbm_surface *gbmSurface;     // GBM surface
        struct gbm_bo *prevBO;              // Previous GBM buffer object (during frame swapping)
        uint32_t prevFB;                    // Previous GBM framebufer (during frame swapping)
#endif  // PLATFORM_DRM
        EGLDisplay device;                  // Native display device (physical screen connection)
        EGLSurface surface;                 // Surface to draw on, framebuffers (connected to context)
        EGLContext context;                 // Graphic context, mode in which drawing can be done
        EGLConfig config;                   // Graphic config
#endif
        const char *title;                  // Window text title const pointer
        unsigned int flags;                 // Configuration flags (bit based), keeps window state
        bool ready;                         // Check if window has been initialized successfully
        bool fullscreen;                    // Check if fullscreen mode is enabled
        bool shouldClose;                   // Check if window set for closing
        bool resizedLastFrame;              // Check if window has been resized last frame
        bool eventWaiting;                  // Wait for events before ending frame

        Point position;                     // Window position on screen (required on fullscreen toggle)
        Size display;                       // Display width and height (monitor, device-screen, LCD, ...)
        Size screen;                        // Screen width and height (used render area)
        Size currentFbo;                    // Current render width and height (depends on active fbo)
        Size render;                        // Framebuffer width and height (render area, including black bars if required)
        Point renderOffset;                 // Offset from render area (must be divided by 2)
        RlMatrix screenScale;                 // RlMatrix to scale screen (framebuffer rendering)

        char **dropFilepaths;         // Store dropped files paths pointers (provided by GLFW)
        unsigned int dropFileCount;         // Count dropped files strings

    } Window;
#if defined(PLATFORM_ANDROID)
    struct {
        bool appEnabled;                    // Flag to detect if app is active ** = true
        struct android_app *app;            // Android activity
        struct android_poll_source *source; // Android events polling source
        bool contextRebindRequired;         // Used to know context rebind required
    } Android;
#endif
    struct {
        const char *basePath;               // Base path for data storage
    } Storage;
    struct {
#if defined(PLATFORM_RPI) || defined(PLATFORM_DRM)
        InputEventWorker eventWorker[10];   // List of worker threads for every monitored "/dev/input/event<N>"
#endif
        struct {
            int exitKey;                    // Default exit key
            char currentKeyState[MAX_KEYBOARD_KEYS];        // Registers current frame key state
            char previousKeyState[MAX_KEYBOARD_KEYS];       // Registers previous frame key state

            int keyPressedQueue[MAX_RL_KEY_PRESSED_QUEUE];     // Input keys queue
            int keyPressedQueueCount;       // Input keys queue count

            int charPressedQueue[MAX_CHAR_PRESSED_QUEUE];   // Input characters queue (unicode)
            int charPressedQueueCount;      // Input characters queue count

#if defined(PLATFORM_RPI) || defined(PLATFORM_DRM)
            int defaultMode;                // Default keyboard mode
#if defined(SUPPORT_SSH_KEYBOARD_RPI)
            bool evtMode;                   // Keyboard in event mode
#endif
            int defaultFileFlags;           // Default IO file flags
            struct termios defaultSettings; // Default keyboard settings
            int fd;                         // File descriptor for the evdev keyboard
#endif
        } Keyboard;
        struct {
            RlVector2 offset;                 // Mouse offset
            RlVector2 scale;                  // Mouse scaling
            RlVector2 currentPosition;        // Mouse position on screen
            RlVector2 previousPosition;       // Previous mouse position

            int cursor;                     // Tracks current mouse cursor
            bool cursorHidden;              // Track if cursor is hidden
            bool cursorOnScreen;            // Tracks if cursor is inside client area

            char currentButtonState[MAX_MOUSE_BUTTONS];     // Registers current mouse button state
            char previousButtonState[MAX_MOUSE_BUTTONS];    // Registers previous mouse button state
            RlVector2 currentWheelMove;       // Registers current mouse wheel variation
            RlVector2 previousWheelMove;      // Registers previous mouse wheel variation
#if defined(PLATFORM_RPI) || defined(PLATFORM_DRM)
            RlVector2 eventWheelMove;         // Registers the event mouse wheel variation
            // NOTE: currentButtonState[] can't be written directly due to multithreading, app could miss the update
            char currentButtonStateEvdev[MAX_MOUSE_BUTTONS]; // Holds the new mouse state for the next polling event to grab
#endif
        } Mouse;
        struct {
            int pointCount;                             // Number of touch points active
            int pointId[MAX_TOUCH_POINTS];              // Point identifiers
            RlVector2 position[MAX_TOUCH_POINTS];         // Touch position on screen
            char currentTouchState[MAX_TOUCH_POINTS];   // Registers current touch state
            char previousTouchState[MAX_TOUCH_POINTS];  // Registers previous touch state
        } Touch;
        struct {
            int lastButtonPressed;          // Register last gamepad button pressed
            int axisCount;                  // Register number of available gamepad axis
            bool ready[MAX_GAMEPADS];       // Flag to know if gamepad is ready
            char name[MAX_GAMEPADS][64];    // Gamepad name holder
            char currentButtonState[MAX_GAMEPADS][MAX_GAMEPAD_BUTTONS];     // Current gamepad buttons state
            char previousButtonState[MAX_GAMEPADS][MAX_GAMEPAD_BUTTONS];    // Previous gamepad buttons state
            float axisState[MAX_GAMEPADS][MAX_GAMEPAD_AXIS];                // Gamepad axis state
#if defined(PLATFORM_RPI) || defined(PLATFORM_DRM)
            pthread_t threadId;             // Gamepad reading thread id
            int streamId[MAX_GAMEPADS];     // Gamepad device file descriptor
#endif
        } Gamepad;
    } Input;
    struct {
        double current;                     // Current time measure
        double previous;                    // Previous time measure
        double update;                      // Time measure for frame update
        double draw;                        // Time measure for frame draw
        double frame;                       // Time measure for one frame
        double target;                      // Desired time for one frame, if 0 not applied
#if defined(PLATFORM_ANDROID) || defined(PLATFORM_RPI) || defined(PLATFORM_DRM)
        unsigned long long base;            // Base time measure for hi-res timer
#endif
        unsigned int frameCounter;          // Frame counter
    } Time;
} CoreData;
#endif // RAYLIB_COREDATA_STRUCT

// Callbacks to hook some internal functions
// WARNING: These callbacks are intended for advance users
typedef void (*TraceLogCallback)(int logLevel, const char *text, va_list args);  // Logging: Redirect trace log messages
typedef unsigned char *(*LoadFileDataCallback)(const char *fileName, unsigned int *bytesRead);      // FileIO: Load binary data
typedef bool (*SaveFileDataCallback)(const char *fileName, void *data, unsigned int bytesToWrite);  // FileIO: Save binary data
typedef char *(*LoadFileTextCallback)(const char *fileName);            // FileIO: Load text data
typedef bool (*SaveFileTextCallback)(const char *fileName, char *text); // FileIO: Save text data

//------------------------------------------------------------------------------------
// Global Variables Definition
//------------------------------------------------------------------------------------
// It's lonely here...

//------------------------------------------------------------------------------------
// Window and Graphics Device Functions (Module: core)
//------------------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif

struct CoreData;        // Core global variables context
RLAPI CoreData *RL_GetCoreData(void); // Get internal data reference

// Window-related functions
RLAPI void RL_InitWindow(int width, int height, const char *title);  // Initialize window and OpenGL context
RLAPI bool RL_WindowShouldClose(void);                               // Check if KEY_ESCAPE pressed or Close icon pressed
RLAPI void RL_CloseWindow(void);                                     // Close window and unload OpenGL context
RLAPI bool RL_IsWindowReady(void);                                   // Check if window has been initialized successfully
RLAPI bool RL_IsWindowFullscreen(void);                              // Check if window is currently fullscreen
RLAPI bool RL_IsWindowHidden(void);                                  // Check if window is currently hidden (only PLATFORM_DESKTOP)
RLAPI bool RL_IsWindowMinimized(void);                               // Check if window is currently minimized (only PLATFORM_DESKTOP)
RLAPI bool RL_IsWindowMaximized(void);                               // Check if window is currently maximized (only PLATFORM_DESKTOP)
RLAPI bool RL_IsWindowFocused(void);                                 // Check if window is currently focused (only PLATFORM_DESKTOP)
RLAPI bool RL_IsWindowResized(void);                                 // Check if window has been resized last frame
RLAPI bool RL_IsWindowState(unsigned int flag);                      // Check if one specific window flag is enabled
RLAPI void RL_SetWindowState(unsigned int flags);                    // Set window configuration state using flags (only PLATFORM_DESKTOP)
RLAPI void RL_ClearWindowState(unsigned int flags);                  // Clear window configuration state flags
RLAPI void RL_ToggleFullscreen(void);                                // Toggle window state: fullscreen/windowed (only PLATFORM_DESKTOP)
RLAPI void RL_MaximizeWindow(void);                                  // Set window state: maximized, if resizable (only PLATFORM_DESKTOP)
RLAPI void RL_MinimizeWindow(void);                                  // Set window state: minimized, if resizable (only PLATFORM_DESKTOP)
RLAPI void RL_RestoreWindow(void);                                   // Set window state: not minimized/maximized (only PLATFORM_DESKTOP)
RLAPI void RL_SetWindowIcon(RlImage image);                            // Set icon for window (single image, RGBA 32bit, only PLATFORM_DESKTOP)
RLAPI void RL_SetWindowIcons(RlImage *images, int count);              // Set icon for window (multiple images, RGBA 32bit, only PLATFORM_DESKTOP)
RLAPI void RL_SetWindowTitle(const char *title);                     // Set title for window (only PLATFORM_DESKTOP)
RLAPI void RL_SetWindowPosition(int x, int y);                       // Set window position on screen (only PLATFORM_DESKTOP)
RLAPI void RL_SetWindowMonitor(int monitor);                         // Set monitor for the current window (fullscreen mode)
RLAPI void RL_SetWindowMinSize(int width, int height);               // Set window minimum dimensions (for RL_FLAG_WINDOW_RESIZABLE)
RLAPI void RL_SetWindowSize(int width, int height);                  // Set window dimensions
RLAPI void RL_SetWindowOpacity(float opacity);                       // Set window opacity [0.0f..1.0f] (only PLATFORM_DESKTOP)
RLAPI void RL_SetWindowFocused(void);                                // Set window focused (only PLATFORM_DESKTOP)
RLAPI void *RL_GetWindowHandle(void);                                // Get native window handle
RLAPI int RL_GetScreenWidth(void);                                   // Get current screen width
RLAPI int RL_GetScreenHeight(void);                                  // Get current screen height
RLAPI int RL_GetRenderWidth(void);                                   // Get current render width (it considers HiDPI)
RLAPI int RL_GetRenderHeight(void);                                  // Get current render height (it considers HiDPI)
RLAPI int RL_GetMonitorCount(void);                                  // Get number of connected monitors
RLAPI int RL_GetCurrentMonitor(void);                                // Get current connected monitor
RLAPI RlVector2 RL_GetMonitorPosition(int monitor);                    // Get specified monitor position
RLAPI int RL_GetMonitorWidth(int monitor);                           // Get specified monitor width (current video mode used by monitor)
RLAPI int RL_GetMonitorHeight(int monitor);                          // Get specified monitor height (current video mode used by monitor)
RLAPI int RL_GetMonitorPhysicalWidth(int monitor);                   // Get specified monitor physical width in millimetres
RLAPI int RL_GetMonitorPhysicalHeight(int monitor);                  // Get specified monitor physical height in millimetres
RLAPI int RL_GetMonitorRefreshRate(int monitor);                     // Get specified monitor refresh rate
RLAPI RlVector2 RL_GetWindowPosition(void);                            // Get window position XY on monitor
RLAPI RlVector2 RL_GetWindowScaleDPI(void);                            // Get window scale DPI factor
RLAPI const char *RL_GetMonitorName(int monitor);                    // Get the human-readable, UTF-8 encoded name of the specified monitor
RLAPI void RL_SetClipboardText(const char *text);                    // Set clipboard text content
RLAPI const char *RL_GetClipboardText(void);                         // Get clipboard text content
RLAPI void RL_EnableEventWaiting(void);                              // Enable waiting for events on RL_EndDrawing(), no automatic event polling
RLAPI void RL_DisableEventWaiting(void);                             // Disable waiting for events on RL_EndDrawing(), automatic events polling

// Custom frame control functions
// NOTE: Those functions are intended for advance users that want full control over the frame processing
// By default RL_EndDrawing() does this job: draws everything + RL_SwapScreenBuffer() + manage frame timing + RL_PollInputEvents()
// To avoid that behaviour and control frame processes manually, enable in config.h: SUPPORT_CUSTOM_FRAME_CONTROL
RLAPI void RL_SwapScreenBuffer(void);                                // Swap back buffer with front buffer (screen drawing)
RLAPI void RL_PollInputEvents(void);                                 // Register all input events
RLAPI void RL_WaitTime(double seconds);                              // Wait for some time (halt program execution)

// Cursor-related functions
RLAPI void RL_ShowCursor(void);                                      // Shows cursor
RLAPI void RL_HideCursor(void);                                      // Hides cursor
RLAPI bool RL_IsCursorHidden(void);                                  // Check if cursor is not visible
RLAPI void RL_EnableCursor(void);                                    // Enables cursor (unlock cursor)
RLAPI void RL_DisableCursor(void);                                   // Disables cursor (lock cursor)
RLAPI bool RL_IsCursorOnScreen(void);                                // Check if cursor is on the screen

// Drawing-related functions
RLAPI void RL_ClearBackground(RlColor color);                          // Set background color (framebuffer clear color)
RLAPI void RL_BeginDrawing(void);                                    // Setup canvas (framebuffer) to start drawing
RLAPI void RL_EndDrawing(void);                                      // End canvas drawing and swap buffers (double buffering)
RLAPI void RL_BeginMode2D(RlCamera2D camera);                          // Begin 2D mode with custom camera (2D)
RLAPI void RL_EndMode2D(void);                                       // Ends 2D mode with custom camera
RLAPI void RL_BeginMode3D(RlCamera3D camera);                          // Begin 3D mode with custom camera (3D)
RLAPI void RL_EndMode3D(void);                                       // Ends 3D mode and returns to default 2D orthographic mode
RLAPI void RL_BeginTextureMode(RlRenderTexture2D target);              // Begin drawing to render texture
RLAPI void RL_EndTextureMode(void);                                  // Ends drawing to render texture
RLAPI void RL_BeginShaderMode(RlShader shader);                        // Begin custom shader drawing
RLAPI void RL_EndShaderMode(void);                                   // End custom shader drawing (use default shader)
RLAPI void RL_BeginBlendMode(int mode);                              // Begin blending mode (alpha, additive, multiplied, subtract, custom)
RLAPI void RL_EndBlendMode(void);                                    // End blending mode (reset to default: alpha blending)
RLAPI void RL_BeginScissorMode(int x, int y, int width, int height); // Begin scissor mode (define screen area for following drawing)
RLAPI void RL_EndScissorMode(void);                                  // End scissor mode
RLAPI void RL_BeginVrStereoMode(RlVrStereoConfig config);              // Begin stereo rendering (requires VR simulator)
RLAPI void RL_EndVrStereoMode(void);                                 // End stereo rendering (requires VR simulator)

// VR stereo config functions for VR simulator
RLAPI RlVrStereoConfig RL_LoadVrStereoConfig(RlVrDeviceInfo device);     // Load VR stereo config for VR simulator device parameters
RLAPI void RL_UnloadVrStereoConfig(RlVrStereoConfig config);           // Unload VR stereo config

// RlShader management functions
// NOTE: RlShader functionality is not available on OpenGL 1.1
RLAPI RlShader RL_LoadShader(const char *vsFileName, const char *fsFileName);   // Load shader from files and bind default locations
RLAPI RlShader RL_LoadShaderFromMemory(const char *vsCode, const char *fsCode); // Load shader from code strings and bind default locations
RLAPI bool RL_IsShaderReady(RlShader shader);                                   // Check if a shader is ready
RLAPI int RL_GetShaderLocation(RlShader shader, const char *uniformName);       // Get shader uniform location
RLAPI int RL_GetShaderLocationAttrib(RlShader shader, const char *attribName);  // Get shader attribute location
RLAPI void RL_SetShaderValue(RlShader shader, int locIndex, const void *value, int uniformType);               // Set shader uniform value
RLAPI void RL_SetShaderValueV(RlShader shader, int locIndex, const void *value, int uniformType, int count);   // Set shader uniform value vector
RLAPI void RL_SetShaderValueMatrix(RlShader shader, int locIndex, RlMatrix mat);         // Set shader uniform value (matrix 4x4)
RLAPI void RL_SetShaderValueTexture(RlShader shader, int locIndex, RlTexture2D texture); // Set shader uniform value for texture (sampler2d)
RLAPI void RL_UnloadShader(RlShader shader);                                    // Unload shader from GPU memory (VRAM)

// Screen-space-related functions
RLAPI RlRay RL_GetMouseRay(RlVector2 mousePosition, RlCamera camera);      // Get a ray trace from mouse position
RLAPI RlMatrix RL_GetCameraMatrix(RlCamera camera);                      // Get camera transform matrix (view matrix)
RLAPI RlMatrix RL_GetCameraMatrix2D(RlCamera2D camera);                  // Get camera 2d transform matrix
RLAPI RlVector2 RL_GetWorldToScreen(RlVector3 position, RlCamera camera);  // Get the screen space position for a 3d world space position
RLAPI RlVector2 RL_GetScreenToWorld2D(RlVector2 position, RlCamera2D camera); // Get the world space position for a 2d camera screen space position
RLAPI RlVector2 RL_GetWorldToScreenEx(RlVector3 position, RlCamera camera, int width, int height); // Get size position for a 3d world space position
RLAPI RlVector2 RL_GetWorldToScreen2D(RlVector2 position, RlCamera2D camera); // Get the screen space position for a 2d camera world space position

// Timing-related functions
RLAPI void RL_SetTargetFPS(int fps);                                 // Set target FPS (maximum)
RLAPI int RL_GetFPS(void);                                           // Get current FPS
RLAPI float RL_GetFrameTime(void);                                   // Get time in seconds for last frame drawn (delta time)
RLAPI double RL_GetTime(void);                                       // Get elapsed time in seconds since RL_InitWindow()

// Misc. functions
RLAPI int RL_GetRandomValue(int min, int max);                       // Get a random value between min and max (both included)
RLAPI void RL_SetRandomSeed(unsigned int seed);                      // Set the seed for the random number generator
RLAPI void RL_TakeScreenshot(const char *fileName);                  // Takes a screenshot of current screen (filename extension defines format)
RLAPI void RL_SetConfigFlags(unsigned int flags);                    // Setup init configuration flags (view FLAGS)

RLAPI void RL_TraceLog(int logLevel, const char *text, ...);         // Show trace log messages (RL_LOG_DEBUG, RL_LOG_INFO, RL_LOG_WARNING, RL_LOG_ERROR...)
RLAPI void RL_SetTraceLogLevel(int logLevel);                        // Set the current threshold (minimum) log level
RLAPI void *RL_MemAlloc(unsigned int size);                          // Internal memory allocator
RLAPI void *RL_MemRealloc(void *ptr, unsigned int size);             // Internal memory reallocator
RLAPI void RL_MemFree(void *ptr);                                    // Internal memory free

RLAPI void RL_OpenURL(const char *url);                              // Open URL with default system browser (if available)

// Set custom callbacks
// WARNING: Callbacks setup is intended for advance users
RLAPI void RL_SetTraceLogCallback(TraceLogCallback callback);         // Set custom trace log
RLAPI void RL_SetLoadFileDataCallback(LoadFileDataCallback callback); // Set custom file binary data loader
RLAPI void RL_SetSaveFileDataCallback(SaveFileDataCallback callback); // Set custom file binary data saver
RLAPI void RL_SetLoadFileTextCallback(LoadFileTextCallback callback); // Set custom file text data loader
RLAPI void RL_SetSaveFileTextCallback(SaveFileTextCallback callback); // Set custom file text data saver

// Files management functions
RLAPI unsigned char *RL_LoadFileData(const char *fileName, unsigned int *bytesRead);       // Load file data as byte array (read)
RLAPI void RL_UnloadFileData(unsigned char *data);                   // Unload file data allocated by RL_LoadFileData()
RLAPI bool RL_SaveFileData(const char *fileName, void *data, unsigned int bytesToWrite);   // Save data to file from byte array (write), returns true on success
RLAPI bool RL_ExportDataAsCode(const unsigned char *data, unsigned int size, const char *fileName); // Export data to code (.h), returns true on success
RLAPI char *RL_LoadFileText(const char *fileName);                   // Load text data from file (read), returns a '\0' terminated string
RLAPI void RL_UnloadFileText(char *text);                            // Unload file text data allocated by RL_LoadFileText()
RLAPI bool RL_SaveFileText(const char *fileName, char *text);        // Save text data to file (write), string must be '\0' terminated, returns true on success
RLAPI bool RL_FileExists(const char *fileName);                      // Check if file exists
RLAPI bool RL_DirectoryExists(const char *dirPath);                  // Check if a directory path exists
RLAPI bool RL_IsFileExtension(const char *fileName, const char *ext); // Check file extension (including point: .png, .wav)
RLAPI int RL_GetFileLength(const char *fileName);                    // Get file length in bytes (NOTE: GetFileSize() conflicts with windows.h)
RLAPI const char *RL_GetFileExtension(const char *fileName);         // Get pointer to extension for a filename string (includes dot: '.png')
RLAPI const char *RL_GetFileName(const char *filePath);              // Get pointer to filename for a path string
RLAPI const char *RL_GetFileNameWithoutExt(const char *filePath);    // Get filename string without extension (uses static string)
RLAPI const char *RL_GetDirectoryPath(const char *filePath);         // Get full path for a given fileName with path (uses static string)
RLAPI const char *RL_GetPrevDirectoryPath(const char *dirPath);      // Get previous directory path for a given path (uses static string)
RLAPI const char *RL_GetWorkingDirectory(void);                      // Get current working directory (uses static string)
RLAPI const char *RL_GetApplicationDirectory(void);                  // Get the directory if the running application (uses static string)
RLAPI bool RL_ChangeDirectory(const char *dir);                      // Change working directory, return true on success
RLAPI bool RL_IsPathFile(const char *path);                          // Check if a given path is a file or a directory
RLAPI RlFilePathList RL_LoadDirectoryFiles(const char *dirPath);       // Load directory filepaths
RLAPI RlFilePathList RL_LoadDirectoryFilesEx(const char *basePath, const char *filter, bool scanSubdirs); // Load directory filepaths with extension filtering and recursive directory scan
RLAPI void RL_UnloadDirectoryFiles(RlFilePathList files);              // Unload filepaths
RLAPI bool RL_IsFileDropped(void);                                   // Check if a file has been dropped into window
RLAPI RlFilePathList RL_LoadDroppedFiles(void);                        // Load dropped filepaths
RLAPI void RL_UnloadDroppedFiles(RlFilePathList files);                // Unload dropped filepaths
RLAPI long RL_GetFileModTime(const char *fileName);                  // Get file modification time (last write time)

// Compression/Encoding functionality
RLAPI unsigned char *RL_CompressData(const unsigned char *data, int dataSize, int *compDataSize);        // Compress data (DEFLATE algorithm), memory must be RL_MemFree()
RLAPI unsigned char *RL_DecompressData(const unsigned char *compData, int compDataSize, int *dataSize);  // Decompress data (DEFLATE algorithm), memory must be RL_MemFree()
RLAPI char *RL_EncodeDataBase64(const unsigned char *data, int dataSize, int *outputSize);               // Encode data to Base64 string, memory must be RL_MemFree()
RLAPI unsigned char *RL_DecodeDataBase64(const unsigned char *data, int *outputSize);                    // Decode Base64 string data, memory must be RL_MemFree()

//------------------------------------------------------------------------------------
// Input Handling Functions (Module: core)
//------------------------------------------------------------------------------------

// Input-related functions: keyboard
RLAPI bool RL_IsKeyPressed(int key);                             // Check if a key has been pressed once
RLAPI bool RL_IsKeyDown(int key);                                // Check if a key is being pressed
RLAPI bool RL_IsKeyReleased(int key);                            // Check if a key has been released once
RLAPI bool RL_IsKeyUp(int key);                                  // Check if a key is NOT being pressed
RLAPI void RL_SetExitKey(int key);                               // Set a custom key to exit program (default is ESC)
RLAPI int RL_GetKeyPressed(void);                                // Get key pressed (keycode), call it multiple times for keys queued, returns 0 when the queue is empty
RLAPI int RL_GetCharPressed(void);                               // Get char pressed (unicode), call it multiple times for chars queued, returns 0 when the queue is empty

// Input-related functions: gamepads
RLAPI bool RL_IsGamepadAvailable(int gamepad);                   // Check if a gamepad is available
RLAPI const char *RL_GetGamepadName(int gamepad);                // Get gamepad internal name id
RLAPI bool RL_IsGamepadButtonPressed(int gamepad, int button);   // Check if a gamepad button has been pressed once
RLAPI bool RL_IsGamepadButtonDown(int gamepad, int button);      // Check if a gamepad button is being pressed
RLAPI bool RL_IsGamepadButtonReleased(int gamepad, int button);  // Check if a gamepad button has been released once
RLAPI bool RL_IsGamepadButtonUp(int gamepad, int button);        // Check if a gamepad button is NOT being pressed
RLAPI int RL_GetGamepadButtonPressed(void);                      // Get the last gamepad button pressed
RLAPI int RL_GetGamepadAxisCount(int gamepad);                   // Get gamepad axis count for a gamepad
RLAPI float RL_GetGamepadAxisMovement(int gamepad, int axis);    // Get axis movement value for a gamepad axis
RLAPI int RL_SetGamepadMappings(const char *mappings);           // Set internal gamepad mappings (SDL_GameControllerDB)

// Input-related functions: mouse
RLAPI bool RL_IsMouseButtonPressed(int button);                  // Check if a mouse button has been pressed once
RLAPI bool RL_IsMouseButtonDown(int button);                     // Check if a mouse button is being pressed
RLAPI bool RL_IsMouseButtonReleased(int button);                 // Check if a mouse button has been released once
RLAPI bool RL_IsMouseButtonUp(int button);                       // Check if a mouse button is NOT being pressed
RLAPI int RL_GetMouseX(void);                                    // Get mouse position X
RLAPI int RL_GetMouseY(void);                                    // Get mouse position Y
RLAPI RlVector2 RL_GetMousePosition(void);                         // Get mouse position XY
RLAPI RlVector2 RL_GetMouseDelta(void);                            // Get mouse delta between frames
RLAPI void RL_SetMousePosition(int x, int y);                    // Set mouse position XY
RLAPI void RL_SetMouseOffset(int offsetX, int offsetY);          // Set mouse offset
RLAPI void RL_SetMouseScale(float scaleX, float scaleY);         // Set mouse scaling
RLAPI float RL_GetMouseWheelMove(void);                          // Get mouse wheel movement for X or Y, whichever is larger
RLAPI RlVector2 RL_GetMouseWheelMoveV(void);                       // Get mouse wheel movement for both X and Y
RLAPI void RL_SetMouseCursor(int cursor);                        // Set mouse cursor

// Input-related functions: touch
RLAPI int RL_GetTouchX(void);                                    // Get touch position X for touch point 0 (relative to screen size)
RLAPI int RL_GetTouchY(void);                                    // Get touch position Y for touch point 0 (relative to screen size)
RLAPI RlVector2 RL_GetTouchPosition(int index);                    // Get touch position XY for a touch point index (relative to screen size)
RLAPI int RL_GetTouchPointId(int index);                         // Get touch point identifier for given index
RLAPI int RL_GetTouchPointCount(void);                           // Get number of touch points

//------------------------------------------------------------------------------------
// Gestures and Touch Handling Functions (Module: rgestures)
//------------------------------------------------------------------------------------
RLAPI void RL_SetGesturesEnabled(unsigned int flags);      // Enable a set of gestures using flags
RLAPI bool RL_IsGestureDetected(int gesture);              // Check if a gesture have been detected
RLAPI int RL_GetGestureDetected(void);                     // Get latest detected gesture
RLAPI float RL_GetGestureHoldDuration(void);               // Get gesture hold time in milliseconds
RLAPI RlVector2 RL_GetGestureDragVector(void);               // Get gesture drag vector
RLAPI float RL_GetGestureDragAngle(void);                  // Get gesture drag angle
RLAPI RlVector2 RL_GetGesturePinchVector(void);              // Get gesture pinch delta
RLAPI float RL_GetGesturePinchAngle(void);                 // Get gesture pinch angle

//------------------------------------------------------------------------------------
// RlCamera System Functions (Module: rcamera)
//------------------------------------------------------------------------------------
RLAPI void RL_UpdateCamera(RlCamera *camera, int mode);      // Update camera position for selected mode
RLAPI void RL_UpdateCameraPro(RlCamera *camera, RlVector3 movement, RlVector3 rotation, float zoom); // Update camera movement/rotation

//------------------------------------------------------------------------------------
// Basic Shapes Drawing Functions (Module: shapes)
//------------------------------------------------------------------------------------
// Set texture and rectangle to be used on shapes drawing
// NOTE: It can be useful when using basic shapes and one single font,
// defining a font char white rectangle would allow drawing everything in a single draw call
RLAPI void SetShapesTexture(RlTexture2D texture, RlRectangle source);       // Set texture and rectangle to be used on shapes drawing

// Basic shapes drawing functions
RLAPI void RL_DrawPixel(int posX, int posY, RlColor color);                                                   // Draw a pixel
RLAPI void RL_DrawPixelV(RlVector2 position, RlColor color);                                                    // Draw a pixel (Vector version)
RLAPI void RL_DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, RlColor color);                // Draw a line
RLAPI void RL_DrawLineV(RlVector2 startPos, RlVector2 endPos, RlColor color);                                     // Draw a line (Vector version)
RLAPI void RL_DrawLineEx(RlVector2 startPos, RlVector2 endPos, float thick, RlColor color);                       // Draw a line defining thickness
RLAPI void RL_DrawLineBezier(RlVector2 startPos, RlVector2 endPos, float thick, RlColor color);                   // Draw a line using cubic-bezier curves in-out
RLAPI void RL_DrawLineBezierQuad(RlVector2 startPos, RlVector2 endPos, RlVector2 controlPos, float thick, RlColor color); // Draw line using quadratic bezier curves with a control point
RLAPI void RL_DrawLineBezierCubic(RlVector2 startPos, RlVector2 endPos, RlVector2 startControlPos, RlVector2 endControlPos, float thick, RlColor color); // Draw line using cubic bezier curves with 2 control points
RLAPI void RL_DrawLineStrip(RlVector2 *points, int pointCount, RlColor color);                                  // Draw lines sequence
RLAPI void RL_DrawCircle(int centerX, int centerY, float radius, RlColor color);                              // Draw a color-filled circle
RLAPI void RL_DrawCircleSector(RlVector2 center, float radius, float startAngle, float endAngle, int segments, RlColor color);      // Draw a piece of a circle
RLAPI void RL_DrawCircleSectorLines(RlVector2 center, float radius, float startAngle, float endAngle, int segments, RlColor color); // Draw circle sector outline
RLAPI void RL_DrawCircleGradient(int centerX, int centerY, float radius, RlColor color1, RlColor color2);       // Draw a gradient-filled circle
RLAPI void RL_DrawCircleV(RlVector2 center, float radius, RlColor color);                                       // Draw a color-filled circle (Vector version)
RLAPI void RL_DrawCircleLines(int centerX, int centerY, float radius, RlColor color);                         // Draw circle outline
RLAPI void RL_DrawEllipse(int centerX, int centerY, float radiusH, float radiusV, RlColor color);             // Draw ellipse
RLAPI void RL_DrawEllipseLines(int centerX, int centerY, float radiusH, float radiusV, RlColor color);        // Draw ellipse outline
RLAPI void RL_DrawRing(RlVector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, RlColor color); // Draw ring
RLAPI void RL_DrawRingLines(RlVector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, RlColor color);    // Draw ring outline
RLAPI void RL_DrawRectangle(int posX, int posY, int width, int height, RlColor color);                        // Draw a color-filled rectangle
RLAPI void RL_DrawRectangleV(RlVector2 position, RlVector2 size, RlColor color);                                  // Draw a color-filled rectangle (Vector version)
RLAPI void RL_DrawRectangleRec(RlRectangle rec, RlColor color);                                                 // Draw a color-filled rectangle
RLAPI void RL_DrawRectanglePro(RlRectangle rec, RlVector2 origin, float rotation, RlColor color);                 // Draw a color-filled rectangle with pro parameters
RLAPI void RL_DrawRectangleGradientV(int posX, int posY, int width, int height, RlColor color1, RlColor color2);// Draw a vertical-gradient-filled rectangle
RLAPI void RL_DrawRectangleGradientH(int posX, int posY, int width, int height, RlColor color1, RlColor color2);// Draw a horizontal-gradient-filled rectangle
RLAPI void RL_DrawRectangleGradientEx(RlRectangle rec, RlColor col1, RlColor col2, RlColor col3, RlColor col4);       // Draw a gradient-filled rectangle with custom vertex colors
RLAPI void RL_DrawRectangleLines(int posX, int posY, int width, int height, RlColor color);                   // Draw rectangle outline
RLAPI void RL_DrawRectangleLinesEx(RlRectangle rec, float lineThick, RlColor color);                            // Draw rectangle outline with extended parameters
RLAPI void RL_DrawRectangleRounded(RlRectangle rec, float roundness, int segments, RlColor color);              // Draw rectangle with rounded edges
RLAPI void RL_DrawRectangleRoundedLines(RlRectangle rec, float roundness, int segments, float lineThick, RlColor color); // Draw rectangle with rounded edges outline
RLAPI void RL_DrawTriangle(RlVector2 v1, RlVector2 v2, RlVector2 v3, RlColor color);                                // Draw a color-filled triangle (vertex in counter-clockwise order!)
RLAPI void RL_DrawTriangleLines(RlVector2 v1, RlVector2 v2, RlVector2 v3, RlColor color);                           // Draw triangle outline (vertex in counter-clockwise order!)
RLAPI void RL_DrawTriangleFan(RlVector2 *points, int pointCount, RlColor color);                                // Draw a triangle fan defined by points (first vertex is the center)
RLAPI void RL_DrawTriangleStrip(RlVector2 *points, int pointCount, RlColor color);                              // Draw a triangle strip defined by points
RLAPI void RL_DrawPoly(RlVector2 center, int sides, float radius, float rotation, RlColor color);               // Draw a regular polygon (Vector version)
RLAPI void RL_DrawPolyLines(RlVector2 center, int sides, float radius, float rotation, RlColor color);          // Draw a polygon outline of n sides
RLAPI void RL_DrawPolyLinesEx(RlVector2 center, int sides, float radius, float rotation, float lineThick, RlColor color); // Draw a polygon outline of n sides with extended parameters

// Basic shapes collision detection functions
RLAPI bool RL_CheckCollisionRecs(RlRectangle rec1, RlRectangle rec2);                                           // Check collision between two rectangles
RLAPI bool RL_CheckCollisionCircles(RlVector2 center1, float radius1, RlVector2 center2, float radius2);        // Check collision between two circles
RLAPI bool RL_CheckCollisionCircleRec(RlVector2 center, float radius, RlRectangle rec);                         // Check collision between circle and rectangle
RLAPI bool RL_CheckCollisionPointRec(RlVector2 point, RlRectangle rec);                                         // Check if point is inside rectangle
RLAPI bool RL_CheckCollisionPointCircle(RlVector2 point, RlVector2 center, float radius);                       // Check if point is inside circle
RLAPI bool RL_CheckCollisionPointTriangle(RlVector2 point, RlVector2 p1, RlVector2 p2, RlVector2 p3);               // Check if point is inside a triangle
RLAPI bool RL_CheckCollisionPointPoly(RlVector2 point, RlVector2 *points, int pointCount);                      // Check if point is within a polygon described by array of vertices
RLAPI bool RL_CheckCollisionLines(RlVector2 startPos1, RlVector2 endPos1, RlVector2 startPos2, RlVector2 endPos2, RlVector2 *collisionPoint); // Check the collision between two lines defined by two points each, returns collision point by reference
RLAPI bool RL_CheckCollisionPointLine(RlVector2 point, RlVector2 p1, RlVector2 p2, int threshold);                // Check if point belongs to line created between two points [p1] and [p2] with defined margin in pixels [threshold]
RLAPI RlRectangle RL_GetCollisionRec(RlRectangle rec1, RlRectangle rec2);                                         // Get collision rectangle for two rectangles collision

//------------------------------------------------------------------------------------
// RlTexture Loading and Drawing Functions (Module: textures)
//------------------------------------------------------------------------------------

// RlImage loading functions
// NOTE: These functions do not require GPU access
RLAPI RlImage RL_LoadImage(const char *fileName);                                                             // Load image from file into CPU memory (RAM)
RLAPI RlImage RL_LoadImageRaw(const char *fileName, int width, int height, int format, int headerSize);       // Load image from RAW file data
RLAPI RlImage RL_LoadImageAnim(const char *fileName, int *frames);                                            // Load image sequence from file (frames appended to image.data)
RLAPI RlImage RL_LoadImageFromMemory(const char *fileType, const unsigned char *fileData, int dataSize);      // Load image from memory buffer, fileType refers to extension: i.e. '.png'
RLAPI RlImage RL_LoadImageFromTexture(RlTexture2D texture);                                                     // Load image from GPU texture data
RLAPI RlImage RL_LoadImageFromScreen(void);                                                                   // Load image from screen buffer and (screenshot)
RLAPI bool RL_IsImageReady(RlImage image);                                                                    // Check if an image is ready
RLAPI void RL_UnloadImage(RlImage image);                                                                     // Unload image from CPU memory (RAM)
RLAPI bool RL_ExportImage(RlImage image, const char *fileName);                                               // Export image data to file, returns true on success
RLAPI unsigned char *RL_ExportImageToMemory(RlImage image, const char *fileType, int *fileSize);              // Export image to memory buffer
RLAPI bool RL_ExportImageAsCode(RlImage image, const char *fileName);                                         // Export image as code file defining an array of bytes, returns true on success

// RlImage generation functions
RLAPI RlImage RL_GenImageColor(int width, int height, RlColor color);                                           // Generate image: plain color
RLAPI RlImage RL_GenImageGradientLinear(int width, int height, int direction, RlColor start, RlColor end);        // Generate image: linear gradient, direction in degrees [0..360], 0=Vertical gradient
RLAPI RlImage RL_GenImageGradientRadial(int width, int height, float density, RlColor inner, RlColor outer);      // Generate image: radial gradient
RLAPI RlImage RL_GenImageGradientSquare(int width, int height, float density, RlColor inner, RlColor outer);      // Generate image: square gradient
RLAPI RlImage RL_GenImageChecked(int width, int height, int checksX, int checksY, RlColor col1, RlColor col2);    // Generate image: checked
RLAPI RlImage RL_GenImageWhiteNoise(int width, int height, float factor);                                     // Generate image: white noise
RLAPI RlImage RL_GenImagePerlinNoise(int width, int height, int offsetX, int offsetY, float scale);           // Generate image: perlin noise
RLAPI RlImage RL_GenImageCellular(int width, int height, int tileSize);                                       // Generate image: cellular algorithm, bigger tileSize means bigger cells
RLAPI RlImage RL_GenImageText(int width, int height, const char *text);                                       // Generate image: grayscale image from text data

// RlImage manipulation functions
RLAPI RlImage RL_ImageCopy(RlImage image);                                                                      // Create an image duplicate (useful for transformations)
RLAPI RlImage RL_ImageFromImage(RlImage image, RlRectangle rec);                                                  // Create an image from another image piece
RLAPI RlImage RL_ImageText(const char *text, int fontSize, RlColor color);                                      // Create an image from text (default font)
RLAPI RlImage RL_ImageTextEx(RlFont font, const char *text, float fontSize, float spacing, RlColor tint);         // Create an image from text (custom sprite font)
RLAPI void RL_ImageFormat(RlImage *image, int newFormat);                                                     // Convert image data to desired format
RLAPI void RL_ImageToPOT(RlImage *image, RlColor fill);                                                         // Convert image to POT (power-of-two)
RLAPI void RL_ImageCrop(RlImage *image, RlRectangle crop);                                                      // Crop an image to a defined rectangle
RLAPI void RL_ImageAlphaCrop(RlImage *image, float threshold);                                                // Crop image depending on alpha value
RLAPI void RL_ImageAlphaClear(RlImage *image, RlColor color, float threshold);                                  // Clear alpha channel to desired color
RLAPI void RL_ImageAlphaMask(RlImage *image, RlImage alphaMask);                                                // Apply alpha mask to image
RLAPI void RL_ImageAlphaPremultiply(RlImage *image);                                                          // Premultiply alpha channel
RLAPI void RL_ImageBlurGaussian(RlImage *image, int blurSize);                                                // Apply Gaussian blur using a box blur approximation
RLAPI void RL_ImageResize(RlImage *image, int newWidth, int newHeight);                                       // Resize image (Bicubic scaling algorithm)
RLAPI void RL_ImageResizeNN(RlImage *image, int newWidth,int newHeight);                                      // Resize image (Nearest-Neighbor scaling algorithm)
RLAPI void RL_ImageResizeCanvas(RlImage *image, int newWidth, int newHeight, int offsetX, int offsetY, RlColor fill);  // Resize canvas and fill with color
RLAPI void RL_ImageMipmaps(RlImage *image);                                                                   // Compute all mipmap levels for a provided image
RLAPI void RL_ImageDither(RlImage *image, int rBpp, int gBpp, int bBpp, int aBpp);                            // Dither image data to 16bpp or lower (Floyd-Steinberg dithering)
RLAPI void RL_ImageFlipVertical(RlImage *image);                                                              // Flip image vertically
RLAPI void RL_ImageFlipHorizontal(RlImage *image);                                                            // Flip image horizontally
RLAPI void RL_ImageRotate(RlImage *image, int degrees);                                                       // Rotate image by input angle in degrees (-359 to 359)
RLAPI void RL_ImageRotateCW(RlImage *image);                                                                  // Rotate image clockwise 90deg
RLAPI void RL_ImageRotateCCW(RlImage *image);                                                                 // Rotate image counter-clockwise 90deg
RLAPI void RL_ImageColorTint(RlImage *image, RlColor color);                                                    // Modify image color: tint
RLAPI void RL_ImageColorInvert(RlImage *image);                                                               // Modify image color: invert
RLAPI void RL_ImageColorGrayscale(RlImage *image);                                                            // Modify image color: grayscale
RLAPI void RL_ImageColorContrast(RlImage *image, float contrast);                                             // Modify image color: contrast (-100 to 100)
RLAPI void RL_ImageColorBrightness(RlImage *image, int brightness);                                           // Modify image color: brightness (-255 to 255)
RLAPI void RL_ImageColorReplace(RlImage *image, RlColor color, RlColor replace);                                  // Modify image color: replace color
RLAPI RlColor *RL_LoadImageColors(RlImage image);                                                               // Load color data from image as a RlColor array (RGBA - 32bit)
RLAPI RlColor *RL_LoadImagePalette(RlImage image, int maxPaletteSize, int *colorCount);                         // Load colors palette from image as a RlColor array (RGBA - 32bit)
RLAPI void RL_UnloadImageColors(RlColor *colors);                                                             // Unload color data loaded with RL_LoadImageColors()
RLAPI void RL_UnloadImagePalette(RlColor *colors);                                                            // Unload colors palette loaded with RL_LoadImagePalette()
RLAPI RlRectangle RL_GetImageAlphaBorder(RlImage image, float threshold);                                       // Get image alpha border rectangle
RLAPI RlColor RL_GetImageColor(RlImage image, int x, int y);                                                    // Get image pixel color at (x, y) position

// RlImage drawing functions
// NOTE: RlImage software-rendering functions (CPU)
RLAPI void RL_ImageClearBackground(RlImage *dst, RlColor color);                                                // Clear image background with given color
RLAPI void RL_ImageDrawPixel(RlImage *dst, int posX, int posY, RlColor color);                                  // Draw pixel within an image
RLAPI void RL_ImageDrawPixelV(RlImage *dst, RlVector2 position, RlColor color);                                   // Draw pixel within an image (Vector version)
RLAPI void RL_ImageDrawLine(RlImage *dst, int startPosX, int startPosY, int endPosX, int endPosY, RlColor color); // Draw line within an image
RLAPI void RL_ImageDrawLineV(RlImage *dst, RlVector2 start, RlVector2 end, RlColor color);                          // Draw line within an image (Vector version)
RLAPI void RL_ImageDrawCircle(RlImage *dst, int centerX, int centerY, int radius, RlColor color);               // Draw a filled circle within an image
RLAPI void RL_ImageDrawCircleV(RlImage *dst, RlVector2 center, int radius, RlColor color);                        // Draw a filled circle within an image (Vector version)
RLAPI void RL_ImageDrawCircleLines(RlImage *dst, int centerX, int centerY, int radius, RlColor color);          // Draw circle outline within an image
RLAPI void RL_ImageDrawCircleLinesV(RlImage *dst, RlVector2 center, int radius, RlColor color);                   // Draw circle outline within an image (Vector version)
RLAPI void RL_ImageDrawRectangle(RlImage *dst, int posX, int posY, int width, int height, RlColor color);       // Draw rectangle within an image
RLAPI void RL_ImageDrawRectangleV(RlImage *dst, RlVector2 position, RlVector2 size, RlColor color);                 // Draw rectangle within an image (Vector version)
RLAPI void RL_ImageDrawRectangleRec(RlImage *dst, RlRectangle rec, RlColor color);                                // Draw rectangle within an image
RLAPI void RL_ImageDrawRectangleLines(RlImage *dst, RlRectangle rec, int thick, RlColor color);                   // Draw rectangle lines within an image
RLAPI void RL_ImageDraw(RlImage *dst, RlImage src, RlRectangle srcRec, RlRectangle dstRec, RlColor tint);             // Draw a source image within a destination image (tint applied to source)
RLAPI void RL_ImageDrawText(RlImage *dst, const char *text, int posX, int posY, int fontSize, RlColor color);   // Draw text (using default font) within an image (destination)
RLAPI void RL_ImageDrawTextEx(RlImage *dst, RlFont font, const char *text, RlVector2 position, float fontSize, float spacing, RlColor tint); // Draw text (custom sprite font) within an image (destination)

// RlTexture loading functions
// NOTE: These functions require GPU access
RLAPI RlTexture2D RL_LoadTexture(const char *fileName);                                                       // Load texture from file into GPU memory (VRAM)
RLAPI RlTexture2D RL_LoadTextureFromImage(RlImage image);                                                       // Load texture from image data
RLAPI RlTextureCubemap RL_LoadTextureCubemap(RlImage image, int layout);                                        // Load cubemap from image, multiple image cubemap layouts supported
RLAPI RlRenderTexture2D RL_LoadRenderTexture(int width, int height);                                          // Load texture for rendering (framebuffer)
RLAPI bool RL_IsTextureReady(RlTexture2D texture);                                                            // Check if a texture is ready
RLAPI void RL_UnloadTexture(RlTexture2D texture);                                                             // Unload texture from GPU memory (VRAM)
RLAPI bool RL_IsRenderTextureReady(RlRenderTexture2D target);                                                       // Check if a render texture is ready
RLAPI void RL_UnloadRenderTexture(RlRenderTexture2D target);                                                  // Unload render texture from GPU memory (VRAM)
RLAPI void RL_UpdateTexture(RlTexture2D texture, const void *pixels);                                         // Update GPU texture with new data
RLAPI void RL_UpdateTextureRec(RlTexture2D texture, RlRectangle rec, const void *pixels);                       // Update GPU texture rectangle with new data

// RlTexture configuration functions
RLAPI void RL_GenTextureMipmaps(RlTexture2D *texture);                                                        // Generate GPU mipmaps for a texture
RLAPI void RL_SetTextureFilter(RlTexture2D texture, int filter);                                              // Set texture scaling filter mode
RLAPI void RL_SetTextureWrap(RlTexture2D texture, int wrap);                                                  // Set texture wrapping mode

// RlTexture drawing functions
RLAPI void RL_DrawTexture(RlTexture2D texture, int posX, int posY, RlColor tint);                               // Draw a RlTexture2D
RLAPI void RL_DrawTextureV(RlTexture2D texture, RlVector2 position, RlColor tint);                                // Draw a RlTexture2D with position defined as RlVector2
RLAPI void RL_DrawTextureEx(RlTexture2D texture, RlVector2 position, float rotation, float scale, RlColor tint);  // Draw a RlTexture2D with extended parameters
RLAPI void RL_DrawTextureRec(RlTexture2D texture, RlRectangle source, RlVector2 position, RlColor tint);            // Draw a part of a texture defined by a rectangle
RLAPI void RL_DrawTexturePro(RlTexture2D texture, RlRectangle source, RlRectangle dest, RlVector2 origin, float rotation, RlColor tint); // Draw a part of a texture defined by a rectangle with 'pro' parameters
RLAPI void RL_DrawTextureNPatch(RlTexture2D texture, RlNPatchInfo nPatchInfo, RlRectangle dest, RlVector2 origin, float rotation, RlColor tint); // Draws a texture (or part of it) that stretches or shrinks nicely

// RlColor/pixel related functions
RLAPI RlColor RL_Fade(RlColor color, float alpha);                                 // Get color with alpha applied, alpha goes from 0.0f to 1.0f
RLAPI int RL_ColorToInt(RlColor color);                                          // Get hexadecimal value for a RlColor
RLAPI RlVector4 RL_ColorNormalize(RlColor color);                                  // Get RlColor normalized as float [0..1]
RLAPI RlColor RL_ColorFromNormalized(RlVector4 normalized);                        // Get RlColor from normalized values [0..1]
RLAPI RlVector3 RL_ColorToHSV(RlColor color);                                      // Get HSV values for a RlColor, hue [0..360], saturation/value [0..1]
RLAPI RlColor RL_ColorFromHSV(float hue, float saturation, float value);         // Get a RlColor from HSV values, hue [0..360], saturation/value [0..1]
RLAPI RlColor RL_ColorTint(RlColor color, RlColor tint);                             // Get color multiplied with another color
RLAPI RlColor RL_ColorBrightness(RlColor color, float factor);                     // Get color with brightness correction, brightness factor goes from -1.0f to 1.0f
RLAPI RlColor RL_ColorContrast(RlColor color, float contrast);                     // Get color with contrast correction, contrast values between -1.0f and 1.0f
RLAPI RlColor RL_ColorAlpha(RlColor color, float alpha);                           // Get color with alpha applied, alpha goes from 0.0f to 1.0f
RLAPI RlColor RL_ColorAlphaBlend(RlColor dst, RlColor src, RlColor tint);              // Get src alpha-blended into dst color with tint
RLAPI RlColor RL_GetColor(unsigned int hexValue);                                // Get RlColor structure from hexadecimal value
RLAPI RlColor RL_GetPixelColor(void *srcPtr, int format);                        // Get RlColor from a source pixel pointer of certain format
RLAPI void RL_SetPixelColor(void *dstPtr, RlColor color, int format);            // Set color formatted into destination pixel pointer
RLAPI int RL_GetPixelDataSize(int width, int height, int format);              // Get pixel data size in bytes for certain format

//------------------------------------------------------------------------------------
// RlFont Loading and Text Drawing Functions (Module: text)
//------------------------------------------------------------------------------------

// RlFont loading/unloading functions
RLAPI RlFont RL_GetFontDefault(void);                                                            // Get the default RlFont
RLAPI RlFont RL_LoadFont(const char *fileName);                                                  // Load font from file into GPU memory (VRAM)
RLAPI RlFont RL_LoadFontEx(const char *fileName, int fontSize, int *fontChars, int glyphCount);  // Load font from file with extended parameters, use NULL for fontChars and 0 for glyphCount to load the default character set
RLAPI RlFont RL_LoadFontFromImage(RlImage image, RlColor key, int firstChar);                        // Load font from RlImage (XNA style)
RLAPI RlFont RL_LoadFontFromMemory(const char *fileType, const unsigned char *fileData, int dataSize, int fontSize, int *fontChars, int glyphCount); // Load font from memory buffer, fileType refers to extension: i.e. '.ttf'
RLAPI bool RL_IsFontReady(RlFont font);                                                          // Check if a font is ready
RLAPI RlGlyphInfo *RL_LoadFontData(const unsigned char *fileData, int dataSize, int fontSize, int *fontChars, int glyphCount, int type); // Load font data for further use
RLAPI RlImage RL_GenImageFontAtlas(const RlGlyphInfo *chars, RlRectangle **recs, int glyphCount, int fontSize, int padding, int packMethod); // Generate image font atlas using chars info
RLAPI void RL_UnloadFontData(RlGlyphInfo *chars, int glyphCount);                                // Unload font chars info data (RAM)
RLAPI void RL_UnloadFont(RlFont font);                                                           // Unload font from GPU memory (VRAM)
RLAPI bool RL_ExportFontAsCode(RlFont font, const char *fileName);                               // Export font as code file, returns true on success

// Text drawing functions
RLAPI void RL_DrawFPS(int posX, int posY);                                                     // Draw current FPS
RLAPI void RL_DrawText(const char *text, int posX, int posY, int fontSize, RlColor color);       // Draw text (using default font)
RLAPI void RL_DrawTextEx(RlFont font, const char *text, RlVector2 position, float fontSize, float spacing, RlColor tint); // Draw text using font and additional parameters
RLAPI void RL_DrawTextPro(RlFont font, const char *text, RlVector2 position, RlVector2 origin, float rotation, float fontSize, float spacing, RlColor tint); // Draw text using RlFont and pro parameters (rotation)
RLAPI void RL_DrawTextCodepoint(RlFont font, int codepoint, RlVector2 position, float fontSize, RlColor tint); // Draw one character (codepoint)
RLAPI void RL_DrawTextCodepoints(RlFont font, const int *codepoints, int count, RlVector2 position, float fontSize, float spacing, RlColor tint); // Draw multiple character (codepoint)

// Text font info functions
RLAPI void RL_SetTextLineSpacing(int spacing);                                                 // Set vertical line spacing when drawing with line-breaks
RLAPI int RL_MeasureText(const char *text, int fontSize);                                      // Measure string width for default font
RLAPI RlVector2 RL_MeasureTextEx(RlFont font, const char *text, float fontSize, float spacing);    // Measure string size for RlFont
RLAPI int RL_GetGlyphIndex(RlFont font, int codepoint);                                          // Get glyph index position in font for a codepoint (unicode character), fallback to '?' if not found
RLAPI RlGlyphInfo RL_GetGlyphInfo(RlFont font, int codepoint);                                     // Get glyph font info data for a codepoint (unicode character), fallback to '?' if not found
RLAPI RlRectangle RL_GetGlyphAtlasRec(RlFont font, int codepoint);                                 // Get glyph rectangle in font atlas for a codepoint (unicode character), fallback to '?' if not found

// Text codepoints management functions (unicode characters)
RLAPI char *RL_LoadUTF8(const int *codepoints, int length);                // Load UTF-8 text encoded from codepoints array
RLAPI void RL_UnloadUTF8(char *text);                                      // Unload UTF-8 text encoded from codepoints array
RLAPI int *RL_LoadCodepoints(const char *text, int *count);                // Load all codepoints from a UTF-8 text string, codepoints count returned by parameter
RLAPI void RL_UnloadCodepoints(int *codepoints);                           // Unload codepoints data from memory
RLAPI int RL_GetCodepointCount(const char *text);                          // Get total number of codepoints in a UTF-8 encoded string
RLAPI int RL_GetCodepoint(const char *text, int *codepointSize);           // Get next codepoint in a UTF-8 encoded string, 0x3f('?') is returned on failure
RLAPI int RL_GetCodepointNext(const char *text, int *codepointSize);       // Get next codepoint in a UTF-8 encoded string, 0x3f('?') is returned on failure
RLAPI int RL_GetCodepointPrevious(const char *text, int *codepointSize);   // Get previous codepoint in a UTF-8 encoded string, 0x3f('?') is returned on failure
RLAPI const char *RL_CodepointToUTF8(int codepoint, int *utf8Size);        // Encode one codepoint into UTF-8 byte array (array length returned as parameter)

// Text strings management functions (no UTF-8 strings, only byte chars)
// NOTE: Some strings allocate memory internally for returned strings, just be careful!
RLAPI int RL_TextCopy(char *dst, const char *src);                                             // Copy one string to another, returns bytes copied
RLAPI bool RL_TextIsEqual(const char *text1, const char *text2);                               // Check if two text string are equal
RLAPI unsigned int RL_TextLength(const char *text);                                            // Get text length, checks for '\0' ending
RLAPI const char *RL_TextFormat(const char *text, ...);                                        // Text formatting with variables (sprintf() style)
RLAPI const char *RL_TextSubtext(const char *text, int position, int length);                  // Get a piece of a text string
RLAPI char *RL_TextReplace(char *text, const char *replace, const char *by);                   // Replace text string (WARNING: memory must be freed!)
RLAPI char *RL_TextInsert(const char *text, const char *insert, int position);                 // Insert text in a position (WARNING: memory must be freed!)
RLAPI const char *RL_TextJoin(const char **textList, int count, const char *delimiter);        // Join text strings with delimiter
RLAPI const char **RL_TextSplit(const char *text, char delimiter, int *count);                 // Split text into multiple strings
RLAPI void RL_TextAppend(char *text, const char *append, int *position);                       // Append text at specific position and move cursor!
RLAPI int RL_TextFindIndex(const char *text, const char *find);                                // Find first text occurrence within a string
RLAPI const char *RL_TextToUpper(const char *text);                      // Get upper case version of provided string
RLAPI const char *RL_TextToLower(const char *text);                      // Get lower case version of provided string
RLAPI const char *RL_TextToPascal(const char *text);                     // Get Pascal case notation version of provided string
RLAPI int RL_TextToInteger(const char *text);                            // Get integer value from text (negative values not supported)

//------------------------------------------------------------------------------------
// Basic 3d Shapes Drawing Functions (Module: models)
//------------------------------------------------------------------------------------

// Basic geometric 3D shapes drawing functions
RLAPI void RL_DrawLine3D(RlVector3 startPos, RlVector3 endPos, RlColor color);                                    // Draw a line in 3D world space
RLAPI void RL_DrawPoint3D(RlVector3 position, RlColor color);                                                   // Draw a point in 3D space, actually a small line
RLAPI void RL_DrawCircle3D(RlVector3 center, float radius, RlVector3 rotationAxis, float rotationAngle, RlColor color); // Draw a circle in 3D world space
RLAPI void RL_DrawTriangle3D(RlVector3 v1, RlVector3 v2, RlVector3 v3, RlColor color);                              // Draw a color-filled triangle (vertex in counter-clockwise order!)
RLAPI void RL_DrawTriangleStrip3D(RlVector3 *points, int pointCount, RlColor color);                            // Draw a triangle strip defined by points
RLAPI void RL_DrawCube(RlVector3 position, float width, float height, float length, RlColor color);             // Draw cube
RLAPI void RL_DrawCubeV(RlVector3 position, RlVector3 size, RlColor color);                                       // Draw cube (Vector version)
RLAPI void RL_DrawCubeWires(RlVector3 position, float width, float height, float length, RlColor color);        // Draw cube wires
RLAPI void RL_DrawCubeWiresV(RlVector3 position, RlVector3 size, RlColor color);                                  // Draw cube wires (Vector version)
RLAPI void RL_DrawSphere(RlVector3 centerPos, float radius, RlColor color);                                     // Draw sphere
RLAPI void RL_DrawSphereEx(RlVector3 centerPos, float radius, int rings, int slices, RlColor color);            // Draw sphere with extended parameters
RLAPI void RL_DrawSphereWires(RlVector3 centerPos, float radius, int rings, int slices, RlColor color);         // Draw sphere wires
RLAPI void RL_DrawCylinder(RlVector3 position, float radiusTop, float radiusBottom, float height, int slices, RlColor color); // Draw a cylinder/cone
RLAPI void RL_DrawCylinderEx(RlVector3 startPos, RlVector3 endPos, float startRadius, float endRadius, int sides, RlColor color); // Draw a cylinder with base at startPos and top at endPos
RLAPI void RL_DrawCylinderWires(RlVector3 position, float radiusTop, float radiusBottom, float height, int slices, RlColor color); // Draw a cylinder/cone wires
RLAPI void RL_DrawCylinderWiresEx(RlVector3 startPos, RlVector3 endPos, float startRadius, float endRadius, int sides, RlColor color); // Draw a cylinder wires with base at startPos and top at endPos
RLAPI void RL_DrawCapsule(RlVector3 startPos, RlVector3 endPos, float radius, int slices, int rings, RlColor color); // Draw a capsule with the center of its sphere caps at startPos and endPos
RLAPI void RL_DrawCapsuleWires(RlVector3 startPos, RlVector3 endPos, float radius, int slices, int rings, RlColor color); // Draw capsule wireframe with the center of its sphere caps at startPos and endPos
RLAPI void RL_DrawPlane(RlVector3 centerPos, RlVector2 size, RlColor color);                                      // Draw a plane XZ
RLAPI void RL_DrawRay(RlRay ray, RlColor color);                                                                // Draw a ray line
RLAPI void RL_DrawGrid(int slices, float spacing);                                                          // Draw a grid (centered at (0, 0, 0))

//------------------------------------------------------------------------------------
// RlModel 3d Loading and Drawing Functions (Module: models)
//------------------------------------------------------------------------------------

// RlModel management functions
RLAPI RlModel RL_LoadModel(const char *fileName);                                                // Load model from files (meshes and materials)
RLAPI RlModel RL_LoadModelFromMesh(RlMesh mesh);                                                   // Load model from generated mesh (default material)
RLAPI bool RL_IsModelReady(RlModel model);                                                       // Check if a model is ready
RLAPI void RL_UnloadModel(RlModel model);                                                        // Unload model (including meshes) from memory (RAM and/or VRAM)
RLAPI RlBoundingBox RL_GetModelBoundingBox(RlModel model);                                         // Compute model bounding box limits (considers all meshes)

// RlModel drawing functions
RLAPI void RL_DrawModel(RlModel model, RlVector3 position, float scale, RlColor tint);               // Draw a model (with texture if set)
RLAPI void RL_DrawModelEx(RlModel model, RlVector3 position, RlVector3 rotationAxis, float rotationAngle, RlVector3 scale, RlColor tint); // Draw a model with extended parameters
RLAPI void RL_DrawModelWires(RlModel model, RlVector3 position, float scale, RlColor tint);          // Draw a model wires (with texture if set)
RLAPI void RL_DrawModelWiresEx(RlModel model, RlVector3 position, RlVector3 rotationAxis, float rotationAngle, RlVector3 scale, RlColor tint); // Draw a model wires (with texture if set) with extended parameters
RLAPI void RL_DrawBoundingBox(RlBoundingBox box, RlColor color);                                   // Draw bounding box (wires)
RLAPI void RL_DrawBillboard(RlCamera camera, RlTexture2D texture, RlVector3 position, float size, RlColor tint);   // Draw a billboard texture
RLAPI void RL_DrawBillboardRec(RlCamera camera, RlTexture2D texture, RlRectangle source, RlVector3 position, RlVector2 size, RlColor tint); // Draw a billboard texture defined by source
RLAPI void RL_DrawBillboardPro(RlCamera camera, RlTexture2D texture, RlRectangle source, RlVector3 position, RlVector3 up, RlVector2 size, RlVector2 origin, float rotation, RlColor tint); // Draw a billboard texture defined by source and rotation

// RlMesh management functions
RLAPI void RL_UploadMesh(RlMesh *mesh, bool dynamic);                                            // Upload mesh vertex data in GPU and provide VAO/VBO ids
RLAPI void RL_UpdateMeshBuffer(RlMesh mesh, int index, const void *data, int dataSize, int offset); // Update mesh vertex data in GPU for a specific buffer index
RLAPI void RL_UnloadMesh(RlMesh mesh);                                                           // Unload mesh data from CPU and GPU
RLAPI void RL_DrawMesh(RlMesh mesh, RlMaterial material, RlMatrix transform);                        // Draw a 3d mesh with material and transform
RLAPI void RL_DrawMeshInstanced(RlMesh mesh, RlMaterial material, const RlMatrix *transforms, int instances); // Draw multiple mesh instances with material and different transforms
RLAPI bool RL_ExportMesh(RlMesh mesh, const char *fileName);                                     // Export mesh data to file, returns true on success
RLAPI RlBoundingBox RL_GetMeshBoundingBox(RlMesh mesh);                                            // Compute mesh bounding box limits
RLAPI void RL_GenMeshTangents(RlMesh *mesh);                                                     // Compute mesh tangents

// RlMesh generation functions
RLAPI RlMesh RL_GenMeshPoly(int sides, float radius);                                            // Generate polygonal mesh
RLAPI RlMesh RL_GenMeshPlane(float width, float length, int resX, int resZ);                     // Generate plane mesh (with subdivisions)
RLAPI RlMesh RL_GenMeshCube(float width, float height, float length);                            // Generate cuboid mesh
RLAPI RlMesh RL_GenMeshSphere(float radius, int rings, int slices);                              // Generate sphere mesh (standard sphere)
RLAPI RlMesh RL_GenMeshHemiSphere(float radius, int rings, int slices);                          // Generate half-sphere mesh (no bottom cap)
RLAPI RlMesh RL_GenMeshCylinder(float radius, float height, int slices);                         // Generate cylinder mesh
RLAPI RlMesh RL_GenMeshCone(float radius, float height, int slices);                             // Generate cone/pyramid mesh
RLAPI RlMesh RL_GenMeshTorus(float radius, float size, int radSeg, int sides);                   // Generate torus mesh
RLAPI RlMesh RL_GenMeshKnot(float radius, float size, int radSeg, int sides);                    // Generate trefoil knot mesh
RLAPI RlMesh RL_GenMeshHeightmap(RlImage heightmap, RlVector3 size);                                 // Generate heightmap mesh from image data
RLAPI RlMesh RL_GenMeshCubicmap(RlImage cubicmap, RlVector3 cubeSize);                               // Generate cubes-based map mesh from image data

// RlMaterial loading/unloading functions
RLAPI RlMaterial *RL_LoadMaterials(const char *fileName, int *materialCount);                    // Load materials from model file
RLAPI RlMaterial RL_LoadMaterialDefault(void);                                                   // Load default material (Supports: DIFFUSE, SPECULAR, NORMAL maps)
RLAPI bool RL_IsMaterialReady(RlMaterial material);                                              // Check if a material is ready
RLAPI void RL_UnloadMaterial(RlMaterial material);                                               // Unload material from GPU memory (VRAM)
RLAPI void RL_SetMaterialTexture(RlMaterial *material, int mapType, RlTexture2D texture);          // Set texture for a material map type (MATERIAL_MAP_DIFFUSE, MATERIAL_MAP_SPECULAR...)
RLAPI void RL_SetModelMeshMaterial(RlModel *model, int meshId, int materialId);                  // Set material for a mesh

// RlModel animations loading/unloading functions
RLAPI RlModelAnimation *RL_LoadModelAnimations(const char *fileName, unsigned int *animCount);   // Load model animations from file
RLAPI void RL_UpdateModelAnimation(RlModel model, RlModelAnimation anim, int frame);               // Update model animation pose
RLAPI void RL_UnloadModelAnimation(RlModelAnimation anim);                                       // Unload animation data
RLAPI void RL_UnloadModelAnimations(RlModelAnimation *animations, unsigned int count);           // Unload animation array data
RLAPI bool RL_IsModelAnimationValid(RlModel model, RlModelAnimation anim);                         // Check model animation skeleton match

// Collision detection functions
RLAPI bool RL_CheckCollisionSpheres(RlVector3 center1, float radius1, RlVector3 center2, float radius2);   // Check collision between two spheres
RLAPI bool RL_CheckCollisionBoxes(RlBoundingBox box1, RlBoundingBox box2);                                 // Check collision between two bounding boxes
RLAPI bool RL_CheckCollisionBoxSphere(RlBoundingBox box, RlVector3 center, float radius);                  // Check collision between box and sphere
RLAPI RlRayCollision RL_GetRayCollisionSphere(RlRay ray, RlVector3 center, float radius);                    // Get collision info between ray and sphere
RLAPI RlRayCollision RL_GetRayCollisionBox(RlRay ray, RlBoundingBox box);                                    // Get collision info between ray and box
RLAPI RlRayCollision RL_GetRayCollisionMesh(RlRay ray, RlMesh mesh, RlMatrix transform);                       // Get collision info between ray and mesh
RLAPI RlRayCollision RL_GetRayCollisionTriangle(RlRay ray, RlVector3 p1, RlVector3 p2, RlVector3 p3);            // Get collision info between ray and triangle
RLAPI RlRayCollision RL_GetRayCollisionQuad(RlRay ray, RlVector3 p1, RlVector3 p2, RlVector3 p3, RlVector3 p4);    // Get collision info between ray and quad

//------------------------------------------------------------------------------------
// Audio Loading and Playing Functions (Module: audio)
//------------------------------------------------------------------------------------
typedef void (*RL_AudioCallback)(void *bufferData, unsigned int frames);

// Audio device management functions
RLAPI void RL_InitAudioDevice(void);                                     // Initialize audio device and context
RLAPI void RL_CloseAudioDevice(void);                                    // Close the audio device and context
RLAPI bool RL_IsAudioDeviceReady(void);                                  // Check if audio device has been initialized successfully
RLAPI void RL_SetMasterVolume(float volume);                             // Set master volume (listener)

// RlWave/RlSound loading/unloading functions
RLAPI RlWave RL_LoadWave(const char *fileName);                            // Load wave data from file
RLAPI RlWave RL_LoadWaveFromMemory(const char *fileType, const unsigned char *fileData, int dataSize); // Load wave from memory buffer, fileType refers to extension: i.e. '.wav'
RLAPI bool RL_IsWaveReady(RlWave wave);                                    // Checks if wave data is ready
RLAPI RlSound RL_LoadSound(const char *fileName);                          // Load sound from file
RLAPI RlSound RL_LoadSoundFromWave(RlWave wave);                             // Load sound from wave data
RLAPI bool RL_IsSoundReady(RlSound sound);                                 // Checks if a sound is ready
RLAPI void RL_UpdateSound(RlSound sound, const void *data, int sampleCount); // Update sound buffer with new data
RLAPI void RL_UnloadWave(RlWave wave);                                     // Unload wave data
RLAPI void RL_UnloadSound(RlSound sound);                                  // Unload sound
RLAPI bool RL_ExportWave(RlWave wave, const char *fileName);               // Export wave data to file, returns true on success
RLAPI bool RL_ExportWaveAsCode(RlWave wave, const char *fileName);         // Export wave sample data to code (.h), returns true on success

// RlWave/RlSound management functions
RLAPI void RL_PlaySound(RlSound sound);                                    // Play a sound
RLAPI void RL_StopSound(RlSound sound);                                    // Stop playing a sound
RLAPI void RL_PauseSound(RlSound sound);                                   // Pause a sound
RLAPI void RL_ResumeSound(RlSound sound);                                  // Resume a paused sound
RLAPI bool RL_IsSoundPlaying(RlSound sound);                               // Check if a sound is currently playing
RLAPI void RL_SetSoundVolume(RlSound sound, float volume);                 // Set volume for a sound (1.0 is max level)
RLAPI void RL_SetSoundPitch(RlSound sound, float pitch);                   // Set pitch for a sound (1.0 is base level)
RLAPI void RL_SetSoundPan(RlSound sound, float pan);                       // Set pan for a sound (0.5 is center)
RLAPI RlWave RL_WaveCopy(RlWave wave);                                       // Copy a wave to a new wave
RLAPI void RL_WaveCrop(RlWave *wave, int initSample, int finalSample);     // Crop a wave to defined samples range
RLAPI void RL_WaveFormat(RlWave *wave, int sampleRate, int sampleSize, int channels); // Convert wave data to desired format
RLAPI float *RL_LoadWaveSamples(RlWave wave);                              // Load samples data from wave as a 32bit float data array
RLAPI void RL_UnloadWaveSamples(float *samples);                         // Unload samples data loaded with RL_LoadWaveSamples()

// RlMusic management functions
RLAPI RlMusic RL_LoadMusicStream(const char *fileName);                    // Load music stream from file
RLAPI RlMusic RL_LoadMusicStreamFromMemory(const char *fileType, const unsigned char *data, int dataSize); // Load music stream from data
RLAPI bool RL_IsMusicReady(RlMusic music);                                 // Checks if a music stream is ready
RLAPI void RL_UnloadMusicStream(RlMusic music);                            // Unload music stream
RLAPI void RL_PlayMusicStream(RlMusic music);                              // Start music playing
RLAPI bool RL_IsMusicStreamPlaying(RlMusic music);                         // Check if music is playing
RLAPI void RL_UpdateMusicStream(RlMusic music);                            // Updates buffers for music streaming
RLAPI void RL_StopMusicStream(RlMusic music);                              // Stop music playing
RLAPI void RL_PauseMusicStream(RlMusic music);                             // Pause music playing
RLAPI void RL_ResumeMusicStream(RlMusic music);                            // Resume playing paused music
RLAPI void RL_SeekMusicStream(RlMusic music, float position);              // Seek music to a position (in seconds)
RLAPI void RL_SetMusicVolume(RlMusic music, float volume);                 // Set volume for music (1.0 is max level)
RLAPI void RL_SetMusicPitch(RlMusic music, float pitch);                   // Set pitch for a music (1.0 is base level)
RLAPI void RL_SetMusicPan(RlMusic music, float pan);                       // Set pan for a music (0.5 is center)
RLAPI float RL_GetMusicTimeLength(RlMusic music);                          // Get music time length (in seconds)
RLAPI float RL_GetMusicTimePlayed(RlMusic music);                          // Get current music time played (in seconds)

// RlAudioStream management functions
RLAPI RlAudioStream RL_LoadAudioStream(unsigned int sampleRate, unsigned int sampleSize, unsigned int channels); // Load audio stream (to stream raw audio pcm data)
RLAPI bool RL_IsAudioStreamReady(RlAudioStream stream);                    // Checks if an audio stream is ready
RLAPI void RL_UnloadAudioStream(RlAudioStream stream);                     // Unload audio stream and free memory
RLAPI void RL_UpdateAudioStream(RlAudioStream stream, const void *data, int frameCount); // Update audio stream buffers with data
RLAPI bool RL_IsAudioStreamProcessed(RlAudioStream stream);                // Check if any audio stream buffers requires refill
RLAPI void RL_PlayAudioStream(RlAudioStream stream);                       // Play audio stream
RLAPI void RL_PauseAudioStream(RlAudioStream stream);                      // Pause audio stream
RLAPI void RL_ResumeAudioStream(RlAudioStream stream);                     // Resume audio stream
RLAPI bool RL_IsAudioStreamPlaying(RlAudioStream stream);                  // Check if audio stream is playing
RLAPI void RL_StopAudioStream(RlAudioStream stream);                       // Stop audio stream
RLAPI void RL_SetAudioStreamVolume(RlAudioStream stream, float volume);    // Set volume for audio stream (1.0 is max level)
RLAPI void RL_SetAudioStreamPitch(RlAudioStream stream, float pitch);      // Set pitch for audio stream (1.0 is base level)
RLAPI void RL_SetAudioStreamPan(RlAudioStream stream, float pan);          // Set pan for audio stream (0.5 is centered)
RLAPI void RL_SetAudioStreamBufferSizeDefault(int size);                 // Default size for new audio streams
RLAPI void RL_SetAudioStreamCallback(RlAudioStream stream, RL_AudioCallback callback);  // Audio thread callback to request new data

RLAPI void RL_AttachAudioStreamProcessor(RlAudioStream stream, RL_AudioCallback processor); // Attach audio stream processor to stream, receives the samples as <float>s
RLAPI void RL_DetachAudioStreamProcessor(RlAudioStream stream, RL_AudioCallback processor); // Detach audio stream processor from stream

RLAPI void RL_AttachAudioMixedProcessor(RL_AudioCallback processor); // Attach audio stream processor to the entire audio pipeline, receives the samples as <float>s
RLAPI void RL_DetachAudioMixedProcessor(RL_AudioCallback processor); // Detach audio stream processor from the entire audio pipeline

#if defined(__cplusplus)
}
#endif

#endif // RAYLIB_H
