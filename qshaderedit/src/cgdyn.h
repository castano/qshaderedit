
#ifndef CGDYN_H
#define CGDYN_H

#if defined(_cg_h)
#	error("do not include 'cg.h' directly.")
#endif
#define _cg_h

#if defined(_cggl_h)
#	error("do not include 'cgGL.h' directly.")
#endif
#define _cggl_h

#if defined(CG_VERSION_NUM)
#	error("CG_VERSION_NUM already defined.")
#endif


// Type definitions.
typedef int CGbool;

#define CG_FALSE ((CGbool)0)
#define CG_TRUE ((CGbool)1)

typedef struct _CGcontext * CGcontext;
typedef struct _CGprogram * CGprogram;
typedef struct _CGparameter * CGparameter;
typedef struct _CGeffect * CGeffect;
typedef struct _CGtechnique * CGtechnique;
typedef struct _CGpass * CGpass;
typedef struct _CGstate * CGstate;
typedef struct _CGstateassignment * CGstateassignment;
typedef struct _CGannotation * CGannotation;
typedef void *CGhandle;



// Cg parameter classes.
typedef enum
{
	CG_PARAMETERCLASS_UNKNOWN = 0,
	CG_PARAMETERCLASS_SCALAR,
	CG_PARAMETERCLASS_VECTOR,
	CG_PARAMETERCLASS_MATRIX,
	CG_PARAMETERCLASS_STRUCT,
	CG_PARAMETERCLASS_ARRAY,
	CG_PARAMETERCLASS_SAMPLER,
	CG_PARAMETERCLASS_OBJECT
} CGparameterclass;

// Cg types.
typedef enum
{
	CG_UNKNOWN_TYPE,
	CG_STRUCT,
	CG_ARRAY,

	CG_TYPE_START_ENUM = 1024,
	CG_HALF,
	CG_HALF2,
	CG_HALF3,
	CG_HALF4,
	CG_HALF1x1,
	CG_HALF1x2,
	CG_HALF1x3,
	CG_HALF1x4,
	CG_HALF2x1,
	CG_HALF2x2,
	CG_HALF2x3,
	CG_HALF2x4,
	CG_HALF3x1,
	CG_HALF3x2,
	CG_HALF3x3,
	CG_HALF3x4,
	CG_HALF4x1,
	CG_HALF4x2,
	CG_HALF4x3,
	CG_HALF4x4,
	CG_FLOAT,
	CG_FLOAT2,
	CG_FLOAT3,
	CG_FLOAT4,
	CG_FLOAT1x1,
	CG_FLOAT1x2,
	CG_FLOAT1x3,
	CG_FLOAT1x4,
	CG_FLOAT2x1,
	CG_FLOAT2x2,
	CG_FLOAT2x3,
	CG_FLOAT2x4,
	CG_FLOAT3x1,
	CG_FLOAT3x2,
	CG_FLOAT3x3,
	CG_FLOAT3x4,
	CG_FLOAT4x1,
	CG_FLOAT4x2,
	CG_FLOAT4x3,
	CG_FLOAT4x4,
	CG_SAMPLER1D,
	CG_SAMPLER2D,
	CG_SAMPLER3D,
	CG_SAMPLERRECT,
	CG_SAMPLERCUBE,
	CG_FIXED,
	CG_FIXED2,
	CG_FIXED3,
	CG_FIXED4,
	CG_FIXED1x1,
	CG_FIXED1x2,
	CG_FIXED1x3,
	CG_FIXED1x4,
	CG_FIXED2x1,
	CG_FIXED2x2,
	CG_FIXED2x3,
	CG_FIXED2x4,
	CG_FIXED3x1,
	CG_FIXED3x2,
	CG_FIXED3x3,
	CG_FIXED3x4,
	CG_FIXED4x1,
	CG_FIXED4x2,
	CG_FIXED4x3,
	CG_FIXED4x4,
	CG_HALF1,
	CG_FLOAT1,
	CG_FIXED1,
	CG_INT,
	CG_INT1,
	CG_INT2,
	CG_INT3,
	CG_INT4,
	CG_INT1x1,
	CG_INT1x2,
	CG_INT1x3,
	CG_INT1x4,
	CG_INT2x1,
	CG_INT2x2,
	CG_INT2x3,
	CG_INT2x4,
	CG_INT3x1,
	CG_INT3x2,
	CG_INT3x3,
	CG_INT3x4,
	CG_INT4x1,
	CG_INT4x2,
	CG_INT4x3,
	CG_INT4x4,
	CG_BOOL,
	CG_BOOL1,
	CG_BOOL2,
	CG_BOOL3,
	CG_BOOL4,
	CG_BOOL1x1,
	CG_BOOL1x2,
	CG_BOOL1x3,
	CG_BOOL1x4,
	CG_BOOL2x1,
	CG_BOOL2x2,
	CG_BOOL2x3,
	CG_BOOL2x4,
	CG_BOOL3x1,
	CG_BOOL3x2,
	CG_BOOL3x3,
	CG_BOOL3x4,
	CG_BOOL4x1,
	CG_BOOL4x2,
	CG_BOOL4x3,
	CG_BOOL4x4,
	CG_STRING,
	CG_PROGRAM_TYPE,
	CG_TEXTURE
} CGtype;

// Cg enums.
typedef enum
{
	CG_UNKNOWN = 4096,
	CG_IN = 4097,
	CG_OUT = 4098,
	CG_INOUT = 4099,
	CG_MIXED = 4100,
	CG_VARYING = 4101,
	CG_UNIFORM = 4102,
	CG_CONSTANT = 4103,
	CG_PROGRAM_SOURCE = 4104,
	CG_PROGRAM_ENTRY = 4105,
	CG_COMPILED_PROGRAM = 4106,
	CG_PROGRAM_PROFILE = 4107,
	CG_GLOBAL = 4108,
	CG_PROGRAM = 4109,
	CG_DEFAULT = 4110,
	CG_ERROR = 4111,
	CG_SOURCE = 4112,
	CG_OBJECT = 4113,
	CG_COMPILE_MANUAL = 4114,
	CG_COMPILE_IMMEDIATE = 4115,
	CG_COMPILE_LAZY = 4116,
	CG_CURRENT = 4117,
	CG_LITERAL = 4118,
	CG_VERSION = 4119,
	CG_ROW_MAJOR = 4120,
	CG_COLUMN_MAJOR = 4121,
} CGenum;

// Cg errors.
typedef enum
{
	CG_NO_ERROR = 0,
	CG_COMPILER_ERROR, 
	CG_INVALID_PARAMETER_ERROR,
	CG_INVALID_PROFILE_ERROR,
	CG_PROGRAM_LOAD_ERROR,
	CG_PROGRAM_BIND_ERROR, 
	CG_PROGRAM_NOT_LOADED_ERROR,
	CG_UNSUPPORTED_GL_EXTENSION_ERROR, 
	CG_INVALID_VALUE_TYPE_ERROR, 
	CG_NOT_MATRIX_PARAM_ERROR, 
	CG_INVALID_ENUMERANT_ERROR, 
	CG_NOT_4x4_MATRIX_ERROR, 
	CG_FILE_READ_ERROR, 
	CG_FILE_WRITE_ERROR, 
	CG_NVPARSE_ERROR, 
	CG_MEMORY_ALLOC_ERROR, 
	CG_INVALID_CONTEXT_HANDLE_ERROR, 
	CG_INVALID_PROGRAM_HANDLE_ERROR, 
	CG_INVALID_PARAM_HANDLE_ERROR, 
	CG_UNKNOWN_PROFILE_ERROR, 
	CG_VAR_ARG_ERROR, 
	CG_INVALID_DIMENSION_ERROR, 
	CG_ARRAY_PARAM_ERROR, 
	CG_OUT_OF_ARRAY_BOUNDS_ERROR, 
	CG_CONFLICTING_TYPES_ERROR, 
	CG_CONFLICTING_PARAMETER_TYPES_ERROR, 
	CG_PARAMETER_IS_NOT_SHARED_ERROR, 
	CG_INVALID_PARAMETER_VARIABILITY_ERROR, 
	CG_CANNOT_DESTROY_PARAMETER_ERROR, 
	CG_NOT_ROOT_PARAMETER_ERROR, 
	CG_PARAMETERS_DO_NOT_MATCH_ERROR, 
	CG_IS_NOT_PROGRAM_PARAMETER_ERROR, 
	CG_INVALID_PARAMETER_TYPE_ERROR, 
	CG_PARAMETER_IS_NOT_RESIZABLE_ARRAY_ERROR, 
	CG_INVALID_SIZE_ERROR, 
	CG_BIND_CREATES_CYCLE_ERROR = 35,
	CG_ARRAY_TYPES_DO_NOT_MATCH_ERROR, 
	CG_ARRAY_DIMENSIONS_DO_NOT_MATCH_ERROR, 
	CG_ARRAY_HAS_WRONG_DIMENSION_ERROR, 
	CG_TYPE_IS_NOT_DEFINED_IN_PROGRAM_ERROR, 
	CG_INVALID_EFFECT_HANDLE_ERROR,
	CG_INVALID_STATE_HANDLE_ERROR,
	CG_INVALID_STATE_ASSIGNMENT_HANDLE_ERROR,
	CG_INVALID_PASS_HANDLE_ERROR,
	CG_INVALID_ANNOTATION_HANDLE_ERROR,
	CG_INVALID_TECHNIQUE_HANDLE_ERROR,
	CG_INVALID_PARAMETER_HANDLE_ERROR,
	CG_STATE_ASSIGNMENT_TYPE_MISMATCH_ERROR,
	CG_INVALID_FUNCTION_HANDLE_ERROR,
	CG_INVALID_TECHNIQUE_ERROR,
	CG_INVALID_POINTER_ERROR,
	CG_NOT_ENOUGH_DATA_ERROR,
	CG_NON_NUMERIC_PARAMETER_ERROR,
	CG_ARRAY_SIZE_MISMATCH_ERROR,
	CG_CANNOT_SET_NON_UNIFORM_PARAMETER_ERROR,
	CG_DUPLICATE_NAME_ERROR,
} CGerror;

// CgGL enums.
typedef enum
{
	CG_GL_MATRIX_IDENTITY = 0,
	CG_GL_MATRIX_TRANSPOSE = 1,
	CG_GL_MATRIX_INVERSE = 2,
	CG_GL_MATRIX_INVERSE_TRANSPOSE = 3,
	
	CG_GL_MODELVIEW_MATRIX,
	CG_GL_PROJECTION_MATRIX,
	CG_GL_TEXTURE_MATRIX,
	CG_GL_MODELVIEW_PROJECTION_MATRIX,
	
	CG_GL_VERTEX,
	CG_GL_FRAGMENT,
} CGGLenum;


#ifdef __cplusplus
extern "C" {
#endif

#if WIN32
#define CGAPI __stdcall
#else
#define CGAPI
#endif
	
typedef CGcontext (CGAPI * _cgCreateContext)(void);
typedef void (CGAPI * _cgDestroyContext)(CGcontext ctx); 
typedef const char * (CGAPI * _cgGetParameterName)(CGparameter param);
typedef CGtype (CGAPI * _cgGetParameterType)(CGparameter param);
typedef CGtype (CGAPI * _cgGetParameterBaseType)(CGparameter param);
typedef CGparameterclass (CGAPI * _cgGetParameterClass)(CGparameter param);
typedef int (CGAPI * _cgGetParameterRows)(CGparameter param);
typedef int (CGAPI * _cgGetParameterColumns)(CGparameter param);
typedef const char * (CGAPI * _cgGetParameterSemantic)(CGparameter param);
typedef CGenum (CGAPI * _cgGetParameterVariability)(CGparameter param);
typedef CGbool (CGAPI * _cgIsParameterReferenced)(CGparameter param);
typedef CGbool (CGAPI * _cgIsParameterUsed)(CGparameter param, CGhandle handle);
typedef const char * (CGAPI * _cgGetStringParameterValue)(CGparameter param);
typedef CGannotation (CGAPI * _cgGetNamedParameterAnnotation)(CGparameter, const char *);
typedef CGstateassignment (CGAPI * _cgGetNamedSamplerStateAssignment)(CGparameter, const char *);
typedef CGparameter (CGAPI * _cgGetTextureStateAssignmentValue)(CGstateassignment);
typedef CGparameter (CGAPI * _cgGetSamplerStateAssignmentValue)(CGstateassignment);
typedef const float * (CGAPI * _cgGetFloatAnnotationValues)(CGannotation, int *nvalues);
typedef const int * (CGAPI * _cgGetIntAnnotationValues)(CGannotation, int *nvalues);
typedef const char * (CGAPI * _cgGetStringAnnotationValue)(CGannotation);
typedef const CGbool * (CGAPI * _cgGetBoolAnnotationValues)(CGannotation, int *nvalues);
typedef const char * (CGAPI * _cgGetAnnotationName)(CGannotation);
typedef CGtype (CGAPI * _cgGetAnnotationType)(CGannotation);
typedef const double * (CGAPI * _cgGetParameterValues)(CGparameter param, CGenum value_type, int *nvalues);
typedef CGannotation (CGAPI * _cgGetFirstParameterAnnotation)(CGparameter);
typedef CGannotation (CGAPI * _cgGetNextAnnotation)(CGannotation);
typedef CGeffect (CGAPI * _cgCreateEffect)(CGcontext, const char *code, const char **args);
typedef void (CGAPI * _cgDestroyEffect)(CGeffect);
typedef const char * (CGAPI * _cgGetLastListing)(CGcontext ctx);
typedef CGtechnique (CGAPI * _cgGetFirstTechnique)(CGeffect);
typedef CGtechnique (CGAPI * _cgGetNextTechnique)(CGtechnique);
typedef CGbool (CGAPI * _cgValidateTechnique)(CGtechnique);
typedef const char * (CGAPI * _cgGetTechniqueName)(CGtechnique);
typedef CGpass (CGAPI * _cgGetFirstPass)(CGtechnique);
typedef CGpass (CGAPI * _cgGetNextPass)(CGpass);
typedef CGerror (CGAPI * _cgGetError)(void);
typedef const char * (CGAPI * _cgGetErrorString)(CGerror error);
typedef const char * (CGAPI * _cgGetString)(CGenum sname);
typedef CGparameter (CGAPI * _cgGetFirstLeafEffectParameter)(CGeffect);
typedef CGparameter (CGAPI * _cgGetNextLeafParameter)(CGparameter current);
typedef void (CGAPI * _cgSetParameter1f)(CGparameter param, float x);
typedef void (CGAPI * _cgSetParameter2f)(CGparameter param, float x, float y);
typedef void (CGAPI * _cgSetParameter1i)(CGparameter param, int x);
typedef void (CGAPI * _cgSetParameter1d)(CGparameter param, double x);
typedef void (CGAPI * _cgSetParameter2d)(CGparameter param, double x, double y);
typedef void (CGAPI * _cgSetParameter3d)(CGparameter param, double x, double y, double z);
typedef void (CGAPI * _cgSetParameter4d)(CGparameter param, double x, double y, double z, double w);
typedef void (CGAPI * _cgSetPassState)(CGpass);
typedef void (CGAPI * _cgResetPassState)(CGpass);

typedef void (CGAPI * _cgGLSetStateMatrixParameter)(CGparameter param, CGGLenum matrix, CGGLenum transform);
typedef void (CGAPI * _cgGLSetupSampler)(CGparameter param, GLuint texobj);
typedef void (CGAPI * _cgGLRegisterStates)(CGcontext);
typedef void (CGAPI * _cgGLSetManageTextureParameters)(CGcontext ctx, CGbool flag);


static _cgCreateContext cgCreateContext = NULL;
static _cgDestroyContext cgDestroyContext = NULL;
static _cgGetParameterName cgGetParameterName = NULL;
static _cgGetParameterType cgGetParameterType = NULL;
static _cgGetParameterBaseType cgGetParameterBaseType = NULL;
static _cgGetParameterClass cgGetParameterClass = NULL;
static _cgGetParameterRows cgGetParameterRows = NULL;
static _cgGetParameterColumns cgGetParameterColumns = NULL;
static _cgGetParameterSemantic cgGetParameterSemantic = NULL;
static _cgGetParameterVariability cgGetParameterVariability = NULL;
static _cgIsParameterReferenced cgIsParameterReferenced = NULL;
static _cgIsParameterUsed cgIsParameterUsed = NULL;
static _cgGetStringParameterValue cgGetStringParameterValue = NULL;
static _cgGetNamedParameterAnnotation cgGetNamedParameterAnnotation = NULL;
static _cgGetNamedSamplerStateAssignment cgGetNamedSamplerStateAssignment = NULL;
static _cgGetTextureStateAssignmentValue cgGetTextureStateAssignmentValue = NULL;
static _cgGetSamplerStateAssignmentValue cgGetSamplerStateAssignmentValue = NULL;
static _cgGetFloatAnnotationValues cgGetFloatAnnotationValues = NULL;
static _cgGetIntAnnotationValues cgGetIntAnnotationValues = NULL;
static _cgGetStringAnnotationValue cgGetStringAnnotationValue = NULL;
static _cgGetBoolAnnotationValues cgGetBoolAnnotationValues = NULL;
static _cgGetAnnotationName cgGetAnnotationName = NULL;
static _cgGetAnnotationType cgGetAnnotationType = NULL;
static _cgGetParameterValues cgGetParameterValues = NULL;
static _cgGetFirstParameterAnnotation cgGetFirstParameterAnnotation = NULL;
static _cgGetNextAnnotation cgGetNextAnnotation = NULL;
static _cgCreateEffect cgCreateEffect = NULL;
static _cgDestroyEffect cgDestroyEffect = NULL;
static _cgGetLastListing cgGetLastListing = NULL;
static _cgGetFirstTechnique cgGetFirstTechnique = NULL;
static _cgGetNextTechnique cgGetNextTechnique = NULL;
static _cgValidateTechnique cgValidateTechnique = NULL;
static _cgGetTechniqueName cgGetTechniqueName = NULL;
static _cgGetFirstPass cgGetFirstPass = NULL;
static _cgGetNextPass cgGetNextPass = NULL;
static _cgGetError cgGetError = NULL;
static _cgGetErrorString cgGetErrorString = NULL;
static _cgGetString cgGetString = NULL;
static _cgGetFirstLeafEffectParameter cgGetFirstLeafEffectParameter = NULL;
static _cgGetNextLeafParameter cgGetNextLeafParameter = NULL;
static _cgSetParameter1f cgSetParameter1f = NULL;
static _cgSetParameter2f cgSetParameter2f = NULL;
static _cgSetParameter1i cgSetParameter1i = NULL;
static _cgSetParameter1d cgSetParameter1d = NULL;
static _cgSetParameter2d cgSetParameter2d = NULL;
static _cgSetParameter3d cgSetParameter3d = NULL;
static _cgSetParameter4d cgSetParameter4d = NULL;
static _cgSetPassState cgSetPassState = NULL;
static _cgResetPassState cgResetPassState = NULL;

static _cgGLSetStateMatrixParameter cgGLSetStateMatrixParameter = NULL;
static _cgGLSetupSampler cgGLSetupSampler = NULL;
static _cgGLRegisterStates cgGLRegisterStates = NULL;
static _cgGLSetManageTextureParameters cgGLSetManageTextureParameters = NULL;


// Link Cg API dynamically so that it can compiled on systems that do not have Cg installed.
inline void loadCgFunctions(QLibrary & cgLibrary, QLibrary & cgGlLibrary)
{
	if (cgCreateContext != NULL)
	{
		return;
	}

	cgCreateContext = (_cgCreateContext) cgLibrary.resolve("cgCreateContext");
	cgDestroyContext = (_cgDestroyContext) cgLibrary.resolve("cgDestroyContext");
	cgGetParameterName = (_cgGetParameterName) cgLibrary.resolve("cgGetParameterName");
	cgGetParameterType = (_cgGetParameterType) cgLibrary.resolve("cgGetParameterType");
	cgGetParameterBaseType = (_cgGetParameterBaseType) cgLibrary.resolve("cgGetParameterBaseType");
	cgGetParameterClass = (_cgGetParameterClass) cgLibrary.resolve("cgGetParameterClass");
	cgGetParameterRows = (_cgGetParameterRows) cgLibrary.resolve("cgGetParameterRows");
	cgGetParameterColumns = (_cgGetParameterColumns) cgLibrary.resolve("cgGetParameterColumns");
	cgGetParameterSemantic = (_cgGetParameterSemantic) cgLibrary.resolve("cgGetParameterSemantic");
	cgGetParameterVariability = (_cgGetParameterVariability) cgLibrary.resolve("cgGetParameterVariability");
	cgIsParameterReferenced = (_cgIsParameterReferenced) cgLibrary.resolve("cgIsParameterReferenced");
	cgIsParameterUsed = (_cgIsParameterUsed) cgLibrary.resolve("cgIsParameterUsed");
	cgGetStringParameterValue = (_cgGetStringParameterValue) cgLibrary.resolve("cgGetStringParameterValue");
	cgGetNamedParameterAnnotation = (_cgGetNamedParameterAnnotation) cgLibrary.resolve("cgGetNamedParameterAnnotation");
	cgGetNamedSamplerStateAssignment = (_cgGetNamedSamplerStateAssignment) cgLibrary.resolve("cgGetNamedSamplerStateAssignment");
	cgGetTextureStateAssignmentValue = (_cgGetTextureStateAssignmentValue) cgLibrary.resolve("cgGetTextureStateAssignmentValue");
	cgGetSamplerStateAssignmentValue = (_cgGetSamplerStateAssignmentValue) cgLibrary.resolve("cgGetSamplerStateAssignmentValue");
	cgGetFloatAnnotationValues = (_cgGetFloatAnnotationValues) cgLibrary.resolve("cgGetFloatAnnotationValues");
	cgGetIntAnnotationValues = (_cgGetIntAnnotationValues) cgLibrary.resolve("cgGetIntAnnotationValues");
	cgGetStringAnnotationValue = (_cgGetStringAnnotationValue) cgLibrary.resolve("cgGetStringAnnotationValue");
	cgGetBoolAnnotationValues = (_cgGetBoolAnnotationValues) cgLibrary.resolve("cgGetBoolAnnotationValues");
	cgGetAnnotationName = (_cgGetAnnotationName) cgLibrary.resolve("cgGetAnnotationName");
	cgGetAnnotationType = (_cgGetAnnotationType) cgLibrary.resolve("cgGetAnnotationType");
	cgGetParameterValues = (_cgGetParameterValues) cgLibrary.resolve("cgGetParameterValues");
	cgGetFirstParameterAnnotation = (_cgGetFirstParameterAnnotation) cgLibrary.resolve("cgGetFirstParameterAnnotation");
	cgGetNextAnnotation = (_cgGetNextAnnotation) cgLibrary.resolve("cgGetNextAnnotation");
	cgCreateEffect = (_cgCreateEffect) cgLibrary.resolve("cgCreateEffect");
	cgDestroyEffect = (_cgDestroyEffect) cgLibrary.resolve("cgDestroyEffect");
	cgGetLastListing = (_cgGetLastListing) cgLibrary.resolve("cgGetLastListing");
	cgGetFirstTechnique = (_cgGetFirstTechnique) cgLibrary.resolve("cgGetFirstTechnique");
	cgGetNextTechnique = (_cgGetNextTechnique) cgLibrary.resolve("cgGetNextTechnique");
	cgValidateTechnique = (_cgValidateTechnique) cgLibrary.resolve("cgValidateTechnique");
	cgGetTechniqueName = (_cgGetTechniqueName) cgLibrary.resolve("cgGetTechniqueName");
	cgGetFirstPass = (_cgGetFirstPass) cgLibrary.resolve("cgGetFirstPass");
	cgGetNextPass = (_cgGetNextPass) cgLibrary.resolve("cgGetNextPass");
	cgGetError = (_cgGetError) cgLibrary.resolve("cgGetError");
	cgGetErrorString = (_cgGetErrorString) cgLibrary.resolve("cgGetErrorString");
	cgGetString = (_cgGetString) cgLibrary.resolve("cgGetString");
	cgGetFirstLeafEffectParameter = (_cgGetFirstLeafEffectParameter) cgLibrary.resolve("cgGetFirstLeafEffectParameter");
	cgGetNextLeafParameter = (_cgGetNextLeafParameter) cgLibrary.resolve("cgGetNextLeafParameter");
	cgSetParameter1f = (_cgSetParameter1f) cgLibrary.resolve("cgSetParameter1f");
	cgSetParameter2f = (_cgSetParameter2f) cgLibrary.resolve("cgSetParameter2f");
	cgSetParameter1i = (_cgSetParameter1i) cgLibrary.resolve("cgSetParameter1i");
	cgSetParameter1d = (_cgSetParameter1d) cgLibrary.resolve("cgSetParameter1d");
	cgSetParameter2d = (_cgSetParameter2d) cgLibrary.resolve("cgSetParameter2d");
	cgSetParameter3d = (_cgSetParameter3d) cgLibrary.resolve("cgSetParameter3d");
	cgSetParameter4d = (_cgSetParameter4d) cgLibrary.resolve("cgSetParameter4d");
	cgSetPassState = (_cgSetPassState) cgLibrary.resolve("cgSetPassState");
	cgResetPassState = (_cgResetPassState) cgLibrary.resolve("cgResetPassState");
	
	cgGLSetStateMatrixParameter = (_cgGLSetStateMatrixParameter) cgGlLibrary.resolve("cgGLSetStateMatrixParameter");
	cgGLSetupSampler = (_cgGLSetupSampler) cgGlLibrary.resolve("cgGLSetupSampler");
	cgGLRegisterStates = (_cgGLRegisterStates) cgGlLibrary.resolve("cgGLRegisterStates");
	cgGLSetManageTextureParameters = (_cgGLSetManageTextureParameters) cgGlLibrary.resolve("cgGLSetManageTextureParameters");
	
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // CGDYN_H
