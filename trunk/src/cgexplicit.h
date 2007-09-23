#ifndef CG_EXPLICIT_H
#define CG_EXPLICIT_H

#if defined(_qcg_h)
#	error("do not include 'qcg.h' directly.")
#endif
#define _qcg_h

#if defined(_qcggl_h)
#	error("do not include 'qcgGL.h' directly.")
#endif
#define _qcggl_h

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
v{
	CG_PARAMETERCLASS_UNKNOWN = 0,
	CG_PARAMETERCLASS_SCALAR,
	CG_PARAMETERCLASS_VECTOR,
	CG_PARAMETERCLASS_MATRIX,
	CG_PARAMETERCLASS_STRUCT,
	CG_PARAMETERCLASS_ARRAY,
	CG_PARAMETERCLASS_SAMPLER,
	CG_PARAMETERCLASS_OBJECT
} CGparameterclass;

// Cg domains.
typedef enum
{
    CG_UNKNOWN_DOMAIN = 0,
    CG_FIRST_DOMAIN   = 1,
    CG_VERTEX_DOMAIN  = 1,
    CG_FRAGMENT_DOMAIN,
    CG_GEOMETRY_DOMAIN,
    CG_NUMBER_OF_DOMAINS
} CGdomain;

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

typedef enum
{
	CG_UNDEFINED = 3256,
} CGresource;

typedef enum
{
	CG_PROFILE_START = 6144,
	CG_PROFILE_UNKNOWN,
	// ...
	CG_PROFILE_MAX = 7100
} CGprofile;

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
#define CGENTRY __cdecl
#else
#define CGAPI
#define CGENTRY
#endif

typedef CGbool (CGENTRY * CGstatecallback)(CGstateassignment);
typedef void (CGENTRY * CGerrorCallbackFunc)(void);
typedef void (CGENTRY * CGerrorHandlerFunc)(CGcontext ctx, CGerror err, void *data);
	
typedef CGcontext (CGAPI *PFNCGCREATECONTEXTPROC)(void);
typedef void (CGAPI *PFNCGDESTROYCONTEXTPROC)(CGcontext);
typedef CGbool (CGAPI *PFNCGISCONTEXTPROC)(CGcontext);
typedef const char * (CGAPI *PFNCGGETLASTLISTINGPROC)(CGcontext);
typedef void (CGAPI *PFNCGSETLASTLISTINGPROC)(CGhandle, const char *);
typedef void (CGAPI *PFNCGSETAUTOCOMPILEPROC)(CGcontext, CGenum);
typedef CGenum (CGAPI *PFNCGGETAUTOCOMPILEPROC)(CGcontext);
typedef CGprogram (CGAPI *PFNCGCREATEPROGRAMPROC)(CGcontext, CGenum, const char *, CGprofile, const char *, const char **);
typedef CGprogram (CGAPI *PFNCGCREATEPROGRAMFROMFILEPROC)(CGcontext, CGenum, const char *, CGprofile, const char *, const char **);
typedef CGprogram (CGAPI *PFNCGCOPYPROGRAMPROC)(CGprogram);
typedef void (CGAPI *PFNCGDESTROYPROGRAMPROC)(CGprogram);
typedef CGprogram (CGAPI *PFNCGGETFIRSTPROGRAMPROC)(CGcontext);
typedef CGprogram (CGAPI *PFNCGGETNEXTPROGRAMPROC)(CGprogram);
typedef CGcontext (CGAPI *PFNCGGETPROGRAMCONTEXTPROC)(CGprogram);
typedef CGbool (CGAPI *PFNCGISPROGRAMPROC)(CGprogram);
typedef void (CGAPI *PFNCGCOMPILEPROGRAMPROC)(CGprogram);
typedef CGbool (CGAPI *PFNCGISPROGRAMCOMPILEDPROC)(CGprogram);
typedef const char * (CGAPI *PFNCGGETPROGRAMSTRINGPROC)(CGprogram, CGenum);
typedef CGprofile (CGAPI *PFNCGGETPROGRAMPROFILEPROC)(CGprogram);
typedef char const * const * (CGAPI *PFNCGGETPROGRAMOPTIONSPROC)(CGprogram);
typedef void (CGAPI *PFNCGSETPROGRAMPROFILEPROC)(CGprogram, CGprofile);
typedef void (CGAPI *PFNCGSETPASSPROGRAMPARAMETERSPROC)(CGprogram);
typedef CGparameter (CGAPI *PFNCGCREATEPARAMETERPROC)(CGcontext, CGtype);
typedef CGparameter (CGAPI *PFNCGCREATEPARAMETERARRAYPROC)(CGcontext, CGtype, int);
typedef CGparameter (CGAPI *PFNCGCREATEPARAMETERMULTIDIMARRAYPROC)(CGcontext, CGtype, int, const int *);
typedef void (CGAPI *PFNCGDESTROYPARAMETERPROC)(CGparameter);
typedef void (CGAPI *PFNCGCONNECTPARAMETERPROC)(CGparameter, CGparameter);
typedef void (CGAPI *PFNCGDISCONNECTPARAMETERPROC)(CGparameter);
typedef CGparameter (CGAPI *PFNCGGETCONNECTEDPARAMETERPROC)(CGparameter);
typedef int (CGAPI *PFNCGGETNUMCONNECTEDTOPARAMETERSPROC)(CGparameter);
typedef CGparameter (CGAPI *PFNCGGETCONNECTEDTOPARAMETERPROC)(CGparameter, int);
typedef CGparameter (CGAPI *PFNCGGETNAMEDPARAMETERPROC)(CGprogram, const char *);
typedef CGparameter (CGAPI *PFNCGGETNAMEDPROGRAMPARAMETERPROC)(CGprogram, CGenum, const char *);
typedef CGparameter (CGAPI *PFNCGGETFIRSTPARAMETERPROC)(CGprogram, CGenum);
typedef CGparameter (CGAPI *PFNCGGETNEXTPARAMETERPROC)(CGparameter);
typedef CGparameter (CGAPI *PFNCGGETFIRSTLEAFPARAMETERPROC)(CGprogram, CGenum);
typedef CGparameter (CGAPI *PFNCGGETNEXTLEAFPARAMETERPROC)(CGparameter);
typedef CGparameter (CGAPI *PFNCGGETFIRSTSTRUCTPARAMETERPROC)(CGparameter);
typedef CGparameter (CGAPI *PFNCGGETNAMEDSTRUCTPARAMETERPROC)(CGparameter, const char *);
typedef CGparameter (CGAPI *PFNCGGETFIRSTDEPENDENTPARAMETERPROC)(CGparameter);
typedef CGparameter (CGAPI *PFNCGGETARRAYPARAMETERPROC)(CGparameter, int);
typedef int (CGAPI *PFNCGGETARRAYDIMENSIONPROC)(CGparameter);
typedef CGtype (CGAPI *PFNCGGETARRAYTYPEPROC)(CGparameter);
typedef int (CGAPI *PFNCGGETARRAYSIZEPROC)(CGparameter, int);
typedef int (CGAPI *PFNCGGETARRAYTOTALSIZEPROC)(CGparameter);
typedef void (CGAPI *PFNCGSETARRAYSIZEPROC)(CGparameter, int);
typedef void (CGAPI *PFNCGSETMULTIDIMARRAYSIZEPROC)(CGparameter, const int *);
typedef CGprogram (CGAPI *PFNCGGETPARAMETERPROGRAMPROC)(CGparameter);
typedef CGcontext (CGAPI *PFNCGGETPARAMETERCONTEXTPROC)(CGparameter);
typedef CGbool (CGAPI *PFNCGISPARAMETERPROC)(CGparameter);
typedef const char * (CGAPI *PFNCGGETPARAMETERNAMEPROC)(CGparameter);
typedef CGtype (CGAPI *PFNCGGETPARAMETERTYPEPROC)(CGparameter);
typedef CGtype (CGAPI *PFNCGGETPARAMETERBASETYPEPROC)(CGparameter);
typedef CGparameterclass (CGAPI *PFNCGGETPARAMETERCLASSPROC)(CGparameter);
typedef int (CGAPI *PFNCGGETPARAMETERROWSPROC)(CGparameter);
typedef int (CGAPI *PFNCGGETPARAMETERCOLUMNSPROC)(CGparameter);
typedef CGtype (CGAPI *PFNCGGETPARAMETERNAMEDTYPEPROC)(CGparameter);
typedef const char * (CGAPI *PFNCGGETPARAMETERSEMANTICPROC)(CGparameter);
typedef CGresource (CGAPI *PFNCGGETPARAMETERRESOURCEPROC)(CGparameter);
typedef CGresource (CGAPI *PFNCGGETPARAMETERBASERESOURCEPROC)(CGparameter);
typedef unsigned long (CGAPI *PFNCGGETPARAMETERRESOURCEINDEXPROC)(CGparameter);
typedef CGenum (CGAPI *PFNCGGETPARAMETERVARIABILITYPROC)(CGparameter);
typedef CGenum (CGAPI *PFNCGGETPARAMETERDIRECTIONPROC)(CGparameter);
typedef CGbool (CGAPI *PFNCGISPARAMETERREFERENCEDPROC)(CGparameter);
typedef CGbool (CGAPI *PFNCGISPARAMETERUSEDPROC)(CGparameter, CGhandle);
typedef const double * (CGAPI *PFNCGGETPARAMETERVALUESPROC)(CGparameter, CGenum, int *);
typedef void (CGAPI *PFNCGSETPARAMETERVALUEDRPROC)(CGparameter, int, const double *);
typedef void (CGAPI *PFNCGSETPARAMETERVALUEDCPROC)(CGparameter, int, const double *);
typedef void (CGAPI *PFNCGSETPARAMETERVALUEFRPROC)(CGparameter, int, const float *);
typedef void (CGAPI *PFNCGSETPARAMETERVALUEFCPROC)(CGparameter, int, const float *);
typedef void (CGAPI *PFNCGSETPARAMETERVALUEIRPROC)(CGparameter, int, const int *);
typedef void (CGAPI *PFNCGSETPARAMETERVALUEICPROC)(CGparameter, int, const int *);
typedef int (CGAPI *PFNCGGETPARAMETERVALUEDRPROC)(CGparameter, int, double *);
typedef int (CGAPI *PFNCGGETPARAMETERVALUEDCPROC)(CGparameter, int, double *);
typedef int (CGAPI *PFNCGGETPARAMETERVALUEFRPROC)(CGparameter, int, float *);
typedef int (CGAPI *PFNCGGETPARAMETERVALUEFCPROC)(CGparameter, int, float *);
typedef int (CGAPI *PFNCGGETPARAMETERVALUEIRPROC)(CGparameter, int, int *);
typedef int (CGAPI *PFNCGGETPARAMETERVALUEICPROC)(CGparameter, int, int *);
typedef const char * (CGAPI *PFNCGGETSTRINGPARAMETERVALUEPROC)(CGparameter);
typedef void (CGAPI *PFNCGSETSTRINGPARAMETERVALUEPROC)(CGparameter, const char *);
typedef int (CGAPI *PFNCGGETPARAMETERORDINALNUMBERPROC)(CGparameter);
typedef CGbool (CGAPI *PFNCGISPARAMETERGLOBALPROC)(CGparameter);
typedef int (CGAPI *PFNCGGETPARAMETERINDEXPROC)(CGparameter);
typedef void (CGAPI *PFNCGSETPARAMETERVARIABILITYPROC)(CGparameter, CGenum);
typedef void (CGAPI *PFNCGSETPARAMETERSEMANTICPROC)(CGparameter, const char *);
typedef void (CGAPI *PFNCGSETPARAMETER1FPROC)(CGparameter, float);
typedef void (CGAPI *PFNCGSETPARAMETER2FPROC)(CGparameter, float, float);
typedef void (CGAPI *PFNCGSETPARAMETER3FPROC)(CGparameter, float, float, float);
typedef void (CGAPI *PFNCGSETPARAMETER4FPROC)(CGparameter, float, float, float, float);
typedef void (CGAPI *PFNCGSETPARAMETER1DPROC)(CGparameter, double);
typedef void (CGAPI *PFNCGSETPARAMETER2DPROC)(CGparameter, double, double);
typedef void (CGAPI *PFNCGSETPARAMETER3DPROC)(CGparameter, double, double, double);
typedef void (CGAPI *PFNCGSETPARAMETER4DPROC)(CGparameter, double, double, double, double);
typedef void (CGAPI *PFNCGSETPARAMETER1IPROC)(CGparameter, int);
typedef void (CGAPI *PFNCGSETPARAMETER2IPROC)(CGparameter, int, int);
typedef void (CGAPI *PFNCGSETPARAMETER3IPROC)(CGparameter, int, int, int);
typedef void (CGAPI *PFNCGSETPARAMETER4IPROC)(CGparameter, int, int, int, int);
typedef void (CGAPI *PFNCGSETPARAMETER1IVPROC)(CGparameter, const int *);
typedef void (CGAPI *PFNCGSETPARAMETER2IVPROC)(CGparameter, const int *);
typedef void (CGAPI *PFNCGSETPARAMETER3IVPROC)(CGparameter, const int *);
typedef void (CGAPI *PFNCGSETPARAMETER4IVPROC)(CGparameter, const int *);
typedef void (CGAPI *PFNCGSETPARAMETER1FVPROC)(CGparameter, const float *);
typedef void (CGAPI *PFNCGSETPARAMETER2FVPROC)(CGparameter, const float *);
typedef void (CGAPI *PFNCGSETPARAMETER3FVPROC)(CGparameter, const float *);
typedef void (CGAPI *PFNCGSETPARAMETER4FVPROC)(CGparameter, const float *);
typedef void (CGAPI *PFNCGSETPARAMETER1DVPROC)(CGparameter, const double *);
typedef void (CGAPI *PFNCGSETPARAMETER2DVPROC)(CGparameter, const double *);
typedef void (CGAPI *PFNCGSETPARAMETER3DVPROC)(CGparameter, const double *);
typedef void (CGAPI *PFNCGSETPARAMETER4DVPROC)(CGparameter, const double *);
typedef void (CGAPI *PFNCGSETMATRIXPARAMETERIRPROC)(CGparameter, const int *);
typedef void (CGAPI *PFNCGSETMATRIXPARAMETERDRPROC)(CGparameter, const double *);
typedef void (CGAPI *PFNCGSETMATRIXPARAMETERFRPROC)(CGparameter, const float *);
typedef void (CGAPI *PFNCGSETMATRIXPARAMETERICPROC)(CGparameter, const int *);
typedef void (CGAPI *PFNCGSETMATRIXPARAMETERDCPROC)(CGparameter, const double *);
typedef void (CGAPI *PFNCGSETMATRIXPARAMETERFCPROC)(CGparameter, const float *);
typedef void (CGAPI *PFNCGGETMATRIXPARAMETERIRPROC)(CGparameter, int *);
typedef void (CGAPI *PFNCGGETMATRIXPARAMETERDRPROC)(CGparameter, double *);
typedef void (CGAPI *PFNCGGETMATRIXPARAMETERFRPROC)(CGparameter, float *);
typedef void (CGAPI *PFNCGGETMATRIXPARAMETERICPROC)(CGparameter, int *);
typedef void (CGAPI *PFNCGGETMATRIXPARAMETERDCPROC)(CGparameter, double *);
typedef void (CGAPI *PFNCGGETMATRIXPARAMETERFCPROC)(CGparameter, float *);
typedef const char * (CGAPI *PFNCGGETTYPESTRINGPROC)(CGtype);
typedef CGtype (CGAPI *PFNCGGETTYPEPROC)(const char *);
typedef CGtype (CGAPI *PFNCGGETNAMEDUSERTYPEPROC)(CGhandle, const char *);
typedef int (CGAPI *PFNCGGETNUMUSERTYPESPROC)(CGhandle);
typedef CGtype (CGAPI *PFNCGGETUSERTYPEPROC)(CGhandle, int);
typedef int (CGAPI *PFNCGGETNUMPARENTTYPESPROC)(CGtype);
typedef CGtype (CGAPI *PFNCGGETPARENTTYPEPROC)(CGtype, int);
typedef CGbool (CGAPI *PFNCGISPARENTTYPEPROC)(CGtype, CGtype);
typedef CGbool (CGAPI *PFNCGISINTERFACETYPEPROC)(CGtype);
typedef const char * (CGAPI *PFNCGGETRESOURCESTRINGPROC)(CGresource);
typedef CGresource (CGAPI *PFNCGGETRESOURCEPROC)(const char *);
typedef const char * (CGAPI *PFNCGGETENUMSTRINGPROC)(CGenum);
typedef CGenum (CGAPI *PFNCGGETENUMPROC)(const char *);
typedef const char * (CGAPI *PFNCGGETPROFILESTRINGPROC)(CGprofile);
typedef CGprofile (CGAPI *PFNCGGETPROFILEPROC)(const char *);
typedef CGerror (CGAPI *PFNCGGETERRORPROC)(void);
typedef CGerror (CGAPI *PFNCGGETFIRSTERRORPROC)(void);
typedef const char * (CGAPI *PFNCGGETERRORSTRINGPROC)(CGerror);
typedef const char * (CGAPI *PFNCGGETLASTERRORSTRINGPROC)(CGerror *);
typedef void (CGAPI *PFNCGSETERRORCALLBACKPROC)(CGerrorCallbackFunc);
typedef CGerrorCallbackFunc (CGAPI *PFNCGGETERRORCALLBACKPROC)(void);
typedef void (CGAPI *PFNCGSETERRORHANDLERPROC)(CGerrorHandlerFunc, void *);
typedef CGerrorHandlerFunc (CGAPI *PFNCGGETERRORHANDLERPROC)(void **);
typedef const char * (CGAPI *PFNCGGETSTRINGPROC)(CGenum);
typedef CGeffect (CGAPI *PFNCGCREATEEFFECTPROC)(CGcontext, const char *, const char **);
typedef CGeffect (CGAPI *PFNCGCREATEEFFECTFROMFILEPROC)(CGcontext, const char *, const char **);
typedef void (CGAPI *PFNCGDESTROYEFFECTPROC)(CGeffect);
typedef CGcontext (CGAPI *PFNCGGETEFFECTCONTEXTPROC)(CGeffect);
typedef CGbool (CGAPI *PFNCGISEFFECTPROC)(CGeffect);
typedef CGeffect (CGAPI *PFNCGGETFIRSTEFFECTPROC)(CGcontext);
typedef CGeffect (CGAPI *PFNCGGETNEXTEFFECTPROC)(CGeffect);
typedef CGprogram (CGAPI *PFNCGCREATEPROGRAMFROMEFFECTPROC)(CGeffect, CGprofile, const char *, const char **);
typedef CGtechnique (CGAPI *PFNCGGETFIRSTTECHNIQUEPROC)(CGeffect);
typedef CGtechnique (CGAPI *PFNCGGETNEXTTECHNIQUEPROC)(CGtechnique);
typedef CGtechnique (CGAPI *PFNCGGETNAMEDTECHNIQUEPROC)(CGeffect, const char *);
typedef const char * (CGAPI *PFNCGGETTECHNIQUENAMEPROC)(CGtechnique);
typedef CGbool (CGAPI *PFNCGISTECHNIQUEPROC)(CGtechnique);
typedef CGbool (CGAPI *PFNCGVALIDATETECHNIQUEPROC)(CGtechnique);
typedef CGbool (CGAPI *PFNCGISTECHNIQUEVALIDATEDPROC)(CGtechnique);
typedef CGeffect (CGAPI *PFNCGGETTECHNIQUEEFFECTPROC)(CGtechnique);
typedef CGpass (CGAPI *PFNCGGETFIRSTPASSPROC)(CGtechnique);
typedef CGpass (CGAPI *PFNCGGETNAMEDPASSPROC)(CGtechnique, const char *);
typedef CGpass (CGAPI *PFNCGGETNEXTPASSPROC)(CGpass);
typedef CGbool (CGAPI *PFNCGISPASSPROC)(CGpass);
typedef const char * (CGAPI *PFNCGGETPASSNAMEPROC)(CGpass);
typedef CGtechnique (CGAPI *PFNCGGETPASSTECHNIQUEPROC)(CGpass);
typedef void (CGAPI *PFNCGSETPASSSTATEPROC)(CGpass);
typedef void (CGAPI *PFNCGRESETPASSSTATEPROC)(CGpass);
typedef CGstateassignment (CGAPI *PFNCGGETFIRSTSTATEASSIGNMENTPROC)(CGpass);
typedef CGstateassignment (CGAPI *PFNCGGETNAMEDSTATEASSIGNMENTPROC)(CGpass, const char *);
typedef CGstateassignment (CGAPI *PFNCGGETNEXTSTATEASSIGNMENTPROC)(CGstateassignment);
typedef CGbool (CGAPI *PFNCGISSTATEASSIGNMENTPROC)(CGstateassignment);
typedef CGbool (CGAPI *PFNCGCALLSTATESETCALLBACKPROC)(CGstateassignment);
typedef CGbool (CGAPI *PFNCGCALLSTATEVALIDATECALLBACKPROC)(CGstateassignment);
typedef CGbool (CGAPI *PFNCGCALLSTATERESETCALLBACKPROC)(CGstateassignment);
typedef CGpass (CGAPI *PFNCGGETSTATEASSIGNMENTPASSPROC)(CGstateassignment);
typedef CGparameter (CGAPI *PFNCGGETSAMPLERSTATEASSIGNMENTPARAMETERPROC)(CGstateassignment);
typedef const float * (CGAPI *PFNCGGETFLOATSTATEASSIGNMENTVALUESPROC)(CGstateassignment, int *);
typedef const int * (CGAPI *PFNCGGETINTSTATEASSIGNMENTVALUESPROC)(CGstateassignment, int *);
typedef const CGbool * (CGAPI *PFNCGGETBOOLSTATEASSIGNMENTVALUESPROC)(CGstateassignment, int *);
typedef const char * (CGAPI *PFNCGGETSTRINGSTATEASSIGNMENTVALUEPROC)(CGstateassignment);
typedef CGprogram (CGAPI *PFNCGGETPROGRAMSTATEASSIGNMENTVALUEPROC)(CGstateassignment);
typedef CGparameter (CGAPI *PFNCGGETTEXTURESTATEASSIGNMENTVALUEPROC)(CGstateassignment);
typedef CGparameter (CGAPI *PFNCGGETSAMPLERSTATEASSIGNMENTVALUEPROC)(CGstateassignment);
typedef int (CGAPI *PFNCGGETSTATEASSIGNMENTINDEXPROC)(CGstateassignment);
typedef int (CGAPI *PFNCGGETNUMDEPENDENTSTATEASSIGNMENTPARAMETERSPROC)(CGstateassignment);
typedef CGparameter (CGAPI *PFNCGGETDEPENDENTSTATEASSIGNMENTPARAMETERPROC)(CGstateassignment, int);
typedef CGstate (CGAPI *PFNCGGETSTATEASSIGNMENTSTATEPROC)(CGstateassignment);
typedef CGstate (CGAPI *PFNCGGETSAMPLERSTATEASSIGNMENTSTATEPROC)(CGstateassignment);
typedef CGstate (CGAPI *PFNCGCREATESTATEPROC)(CGcontext, const char *, CGtype);
typedef CGstate (CGAPI *PFNCGCREATEARRAYSTATEPROC)(CGcontext, const char *, CGtype, int);
typedef void (CGAPI *PFNCGSETSTATECALLBACKSPROC)(CGstate, CGstatecallback, CGstatecallback, CGstatecallback);
typedef CGstatecallback (CGAPI *PFNCGGETSTATESETCALLBACKPROC)(CGstate);
typedef CGstatecallback (CGAPI *PFNCGGETSTATERESETCALLBACKPROC)(CGstate);
typedef CGstatecallback (CGAPI *PFNCGGETSTATEVALIDATECALLBACKPROC)(CGstate);
typedef CGtype (CGAPI *PFNCGGETSTATETYPEPROC)(CGstate);
typedef const char * (CGAPI *PFNCGGETSTATENAMEPROC)(CGstate);
typedef CGcontext (CGAPI *PFNCGGETSTATECONTEXTPROC)(CGstate);
typedef CGstate (CGAPI *PFNCGGETNAMEDSTATEPROC)(CGcontext, const char *);
typedef CGstate (CGAPI *PFNCGGETFIRSTSTATEPROC)(CGcontext);
typedef CGstate (CGAPI *PFNCGGETNEXTSTATEPROC)(CGstate);
typedef CGbool (CGAPI *PFNCGISSTATEPROC)(CGstate);
typedef void (CGAPI *PFNCGADDSTATEENUMERANTPROC)(CGstate, const char *, int);
typedef CGstate (CGAPI *PFNCGCREATESAMPLERSTATEPROC)(CGcontext, const char *, CGtype);
typedef CGstate (CGAPI *PFNCGCREATEARRAYSAMPLERSTATEPROC)(CGcontext, const char *, CGtype, int);
typedef CGstate (CGAPI *PFNCGGETNAMEDSAMPLERSTATEPROC)(CGcontext, const char *);
typedef CGstate (CGAPI *PFNCGGETFIRSTSAMPLERSTATEPROC)(CGcontext);
typedef CGstateassignment (CGAPI *PFNCGGETFIRSTSAMPLERSTATEASSIGNMENTPROC)(CGparameter);
typedef CGstateassignment (CGAPI *PFNCGGETNAMEDSAMPLERSTATEASSIGNMENTPROC)(CGparameter, const char *);
typedef void (CGAPI *PFNCGSETSAMPLERSTATEPROC)(CGparameter);
typedef CGparameter (CGAPI *PFNCGGETNAMEDEFFECTPARAMETERPROC)(CGeffect, const char *);
typedef CGparameter (CGAPI *PFNCGGETFIRSTLEAFEFFECTPARAMETERPROC)(CGeffect);
typedef CGparameter (CGAPI *PFNCGGETFIRSTEFFECTPARAMETERPROC)(CGeffect);
typedef CGparameter (CGAPI *PFNCGGETEFFECTPARAMETERBYSEMANTICPROC)(CGeffect, const char *);
typedef CGannotation (CGAPI *PFNCGGETFIRSTTECHNIQUEANNOTATIONPROC)(CGtechnique);
typedef CGannotation (CGAPI *PFNCGGETFIRSTPASSANNOTATIONPROC)(CGpass);
typedef CGannotation (CGAPI *PFNCGGETFIRSTPARAMETERANNOTATIONPROC)(CGparameter);
typedef CGannotation (CGAPI *PFNCGGETFIRSTPROGRAMANNOTATIONPROC)(CGprogram);
typedef CGannotation (CGAPI *PFNCGGETFIRSTEFFECTANNOTATIONPROC)(CGeffect);
typedef CGannotation (CGAPI *PFNCGGETNEXTANNOTATIONPROC)(CGannotation);
typedef CGannotation (CGAPI *PFNCGGETNAMEDTECHNIQUEANNOTATIONPROC)(CGtechnique, const char *);
typedef CGannotation (CGAPI *PFNCGGETNAMEDPASSANNOTATIONPROC)(CGpass, const char *);
typedef CGannotation (CGAPI *PFNCGGETNAMEDPARAMETERANNOTATIONPROC)(CGparameter, const char *);
typedef CGannotation (CGAPI *PFNCGGETNAMEDPROGRAMANNOTATIONPROC)(CGprogram, const char *);
typedef CGannotation (CGAPI *PFNCGGETNAMEDEFFECTANNOTATIONPROC)(CGeffect, const char *);
typedef CGbool (CGAPI *PFNCGISANNOTATIONPROC)(CGannotation);
typedef const char * (CGAPI *PFNCGGETANNOTATIONNAMEPROC)(CGannotation);
typedef CGtype (CGAPI *PFNCGGETANNOTATIONTYPEPROC)(CGannotation);
typedef const float * (CGAPI *PFNCGGETFLOATANNOTATIONVALUESPROC)(CGannotation, int *);
typedef const int * (CGAPI *PFNCGGETINTANNOTATIONVALUESPROC)(CGannotation, int *);
typedef const char * (CGAPI *PFNCGGETSTRINGANNOTATIONVALUEPROC)(CGannotation);
typedef const int * (CGAPI *PFNCGGETBOOLEANANNOTATIONVALUESPROC)(CGannotation, int *);
typedef const CGbool * (CGAPI *PFNCGGETBOOLANNOTATIONVALUESPROC)(CGannotation, int *);
typedef int (CGAPI *PFNCGGETNUMDEPENDENTANNOTATIONPARAMETERSPROC)(CGannotation);
typedef CGparameter (CGAPI *PFNCGGETDEPENDENTANNOTATIONPARAMETERPROC)(CGannotation, int);
typedef void (CGAPI *PFNCGEVALUATEPROGRAMPROC)(CGprogram, float *, int, int, int, int);
typedef CGbool (CGAPI *PFNCGSETEFFECTNAMEPROC)(CGeffect, const char *);
typedef const char * (CGAPI *PFNCGGETEFFECTNAMEPROC)(CGeffect);
typedef CGeffect (CGAPI *PFNCGGETNAMEDEFFECTPROC)(CGcontext, const char *);
typedef CGparameter (CGAPI *PFNCGCREATEEFFECTPARAMETERPROC)(CGeffect, const char *, CGtype);
typedef CGtechnique (CGAPI *PFNCGCREATETECHNIQUEPROC)(CGeffect, const char *);
typedef CGpass (CGAPI *PFNCGCREATEPASSPROC)(CGtechnique, const char *);
typedef CGstateassignment (CGAPI *PFNCGCREATESTATEASSIGNMENTPROC)(CGpass, CGstate);
typedef CGstateassignment (CGAPI *PFNCGCREATESTATEASSIGNMENTINDEXPROC)(CGpass, CGstate, int);
typedef CGstateassignment (CGAPI *PFNCGCREATESAMPLERSTATEASSIGNMENTPROC)(CGparameter, CGstate);
typedef CGbool (CGAPI *PFNCGSETFLOATSTATEASSIGNMENTPROC)(CGstateassignment, float);
typedef CGbool (CGAPI *PFNCGSETINTSTATEASSIGNMENTPROC)(CGstateassignment, int);
typedef CGbool (CGAPI *PFNCGSETBOOLSTATEASSIGNMENTPROC)(CGstateassignment, CGbool);
typedef CGbool (CGAPI *PFNCGSETSTRINGSTATEASSIGNMENTPROC)(CGstateassignment, const char *);
typedef CGbool (CGAPI *PFNCGSETPROGRAMSTATEASSIGNMENTPROC)(CGstateassignment, CGprogram);
typedef CGbool (CGAPI *PFNCGSETSAMPLERSTATEASSIGNMENTPROC)(CGstateassignment, CGparameter);
typedef CGbool (CGAPI *PFNCGSETTEXTURESTATEASSIGNMENTPROC)(CGstateassignment, CGparameter);
typedef CGbool (CGAPI *PFNCGSETFLOATARRAYSTATEASSIGNMENTPROC)(CGstateassignment, const float *);
typedef CGbool (CGAPI *PFNCGSETINTARRAYSTATEASSIGNMENTPROC)(CGstateassignment, const int *);
typedef CGbool (CGAPI *PFNCGSETBOOLARRAYSTATEASSIGNMENTPROC)(CGstateassignment, const CGbool *);
typedef CGannotation (CGAPI *PFNCGCREATETECHNIQUEANNOTATIONPROC)(CGtechnique, const char *, CGtype);
typedef CGannotation (CGAPI *PFNCGCREATEPASSANNOTATIONPROC)(CGpass, const char *, CGtype);
typedef CGannotation (CGAPI *PFNCGCREATEPARAMETERANNOTATIONPROC)(CGparameter, const char *, CGtype);
typedef CGannotation (CGAPI *PFNCGCREATEPROGRAMANNOTATIONPROC)(CGprogram, const char *, CGtype);
typedef CGannotation (CGAPI *PFNCGCREATEEFFECTANNOTATIONPROC)(CGeffect, const char *, CGtype);
typedef CGbool (CGAPI *PFNCGSETINTANNOTATIONPROC)(CGannotation, int);
typedef CGbool (CGAPI *PFNCGSETFLOATANNOTATIONPROC)(CGannotation, float);
typedef CGbool (CGAPI *PFNCGSETBOOLANNOTATIONPROC)(CGannotation, CGbool);
typedef CGbool (CGAPI *PFNCGSETSTRINGANNOTATIONPROC)(CGannotation, const char *);
typedef const char* (CGAPI *PFNCGGETSTATEENUMERANTNAMEPROC)(CGstate, int);
typedef int (CGAPI *PFNCGGETSTATEENUMERANTVALUEPROC)(CGstate, const char *);
typedef CGeffect (CGAPI *PFNCGGETPARAMETEREFFECTPROC)(CGparameter);
typedef CGparameterclass (CGAPI *PFNCGGETTYPECLASSPROC)(CGtype);
typedef CGtype (CGAPI *PFNCGGETTYPEBASEPROC)(CGtype);
typedef CGbool (CGAPI *PFNCGGETTYPESIZESPROC)(CGtype, int *, int *);
typedef void (CGAPI *PFNCGGETMATRIXSIZEPROC)(CGtype, int *, int *);
typedef CGdomain (CGAPI *PFNCGGETPROFILEDOMAINPROC)(CGprofile);
typedef CGprogram (CGAPI *PFNCGCOMBINEPROGRAMSPROC)(int, const CGprogram *);
typedef CGprogram (CGAPI *PFNCGCOMBINEPROGRAMS2PROC)(CGprogram, CGprogram);
typedef CGprogram (CGAPI *PFNCGCOMBINEPROGRAMS3PROC)(CGprogram, CGprogram, CGprogram);
typedef int (CGAPI *PFNCGGETNUMPROGRAMDOMAINSPROC)(CGprogram);
typedef CGprofile (CGAPI *PFNCGGETPROGRAMDOMAINPROFILEPROC)(CGprogram, int);
typedef CGparameter (CGAPI *PFNCGCREATEEFFECTPARAMETERARRAYPROC)(CGeffect, const char *, CGtype, int);
typedef CGparameter (CGAPI *PFNCGCREATEEFFECTPARAMETERMULTIDIMARRAYPROC)(CGeffect, const char *, CGtype, int, const int *);
typedef CGparameter (CGAPI *PFNCGGETNAMEDSUBPARAMETERPROC)(CGparameter, const char *);
/*
typedef CGenum (CGAPI *PFNCGGETPROGRAMINPUTPROC)(CGprogram);
typedef CGenum (CGAPI *PFNCGGETPROGRAMOUTPUTPROC)(CGprogram);
typedef CGobj (CGAPI *PFNCGCREATEOBJPROC)(CGcontext, CGenum, const char *, CGprofile, const char **);
typedef CGobj (CGAPI *PFNCGCREATEOBJFROMFILEPROC)(CGcontext, CGenum, const char *, CGprofile, const char **);
typedef void (CGAPI *PFNCGDESTROYOBJPROC)(CGobj);
typedef void (CGAPI *PFNCGSETPARAMETERSETTINGMODEPROC)(CGcontext, CGenum);
typedef CGenum (CGAPI *PFNCGGETPARAMETERSETTINGMODEPROC)(CGcontext);
typedef void (CGAPI *PFNCGUPDATEPROGRAMPARAMETERSPROC)(CGprogram);
typedef CGenum (CGAPI *PFNCGSETLOCKINGPOLICYPROC)(CGenum);
typedef CGenum (CGAPI *PFNCGGETLOCKINGPOLICYPROC)(void);
typedef CGenum (CGAPI *PFNCGSETSEMANTICCASEPOLICYPROC)(CGenum);
typedef CGenum (CGAPI *PFNCGGETSEMANTICCASEPOLICYPROC)(void);
typedef CGparameter (CGAPI *PFNCGGETCONNECTEDSTATEASSIGNMENTPARAMETERPROC)(CGstateassignment);
typedef const char * const * (CGAPI *PFNCGGETSTRINGANNOTATIONVALUESPROC)(CGannotation, int *);
typedef void (CGAPI *PFNCGSETBUFFERDATAPROC)(CGbuffer, unsigned long, const void *);
typedef void (CGAPI *PFNCGSETBUFFERSUBDATAPROC)(CGbuffer, unsigned long, unsigned long, const void *);
typedef CGbuffer (CGAPI *PFNCGCREATEBUFFERPROC)(CGcontext, unsigned long);
typedef void (CGAPI *PFNCGDESTROYBUFFERPROC)(CGbuffer);
typedef unsigned long (CGAPI *PFNCGGETBUFFERSIZEPROC)(CGbuffer);
typedef int (CGAPI *PFNCGGETPARAMETERBUFFERINDEXPROC)(CGparameter);
typedef int (CGAPI *PFNCGGETPARAMETERBUFFEROFFSETPROC)(CGparameter);
typedef long (CGAPI *PFNCGGETPARAMETERRESOURCESIZEPROC)(CGparameter);
typedef CGbuffer (CGAPI *PFNCGGETPROGRAMBUFFERPROC)(CGprogram, int);
typedef void * (CGAPI *PFNCGMAPBUFFERPROC)(CGbuffer, CGbufferaccess);
typedef void (CGAPI *PFNCGSETPROGRAMBUFFERPROC)(CGprogram, int, CGbuffer);
typedef void (CGAPI *PFNCGUNMAPBUFFERPROC)(CGbuffer);
*/

typedef CGbool (CGAPI *PFNCGGLISPROFILESUPPORTEDPROC)(CGprofile);
typedef void (CGAPI *PFNCGGLENABLEPROFILEPROC)(CGprofile);
typedef void (CGAPI *PFNCGGLDISABLEPROFILEPROC)(CGprofile);
typedef CGprofile (CGAPI *PFNCGGLGETLATESTPROFILEPROC)(CGGLenum);
typedef void (CGAPI *PFNCGGLSETOPTIMALOPTIONSPROC)(CGprofile);
typedef void (CGAPI *PFNCGGLLOADPROGRAMPROC)(CGprogram);
typedef CGbool (CGAPI *PFNCGGLISPROGRAMLOADEDPROC)(CGprogram);
typedef void (CGAPI *PFNCGGLBINDPROGRAMPROC)(CGprogram);
typedef void (CGAPI *PFNCGGLUNBINDPROGRAMPROC)(CGprofile);
typedef GLuint (CGAPI *PFNCGGLGETPROGRAMIDPROC)(CGprogram);
typedef void (CGAPI *PFNCGGLSETPARAMETER1FPROC)(CGparameter, float);
typedef void (CGAPI *PFNCGGLSETPARAMETER2FPROC)(CGparameter, float, float);
typedef void (CGAPI *PFNCGGLSETPARAMETER3FPROC)(CGparameter, float, float, float);
typedef void (CGAPI *PFNCGGLSETPARAMETER4FPROC)(CGparameter, float, float, float, float);
typedef void (CGAPI *PFNCGGLSETPARAMETER1FVPROC)(CGparameter, const float *);
typedef void (CGAPI *PFNCGGLSETPARAMETER2FVPROC)(CGparameter, const float *);
typedef void (CGAPI *PFNCGGLSETPARAMETER3FVPROC)(CGparameter, const float *);
typedef void (CGAPI *PFNCGGLSETPARAMETER4FVPROC)(CGparameter, const float *);
typedef void (CGAPI *PFNCGGLSETPARAMETER1DPROC)(CGparameter, double);
typedef void (CGAPI *PFNCGGLSETPARAMETER2DPROC)(CGparameter, double, double);
typedef void (CGAPI *PFNCGGLSETPARAMETER3DPROC)(CGparameter, double, double, double);
typedef void (CGAPI *PFNCGGLSETPARAMETER4DPROC)(CGparameter, double, double, double, double);
typedef void (CGAPI *PFNCGGLSETPARAMETER1DVPROC)(CGparameter, const double *);
typedef void (CGAPI *PFNCGGLSETPARAMETER2DVPROC)(CGparameter, const double *);
typedef void (CGAPI *PFNCGGLSETPARAMETER3DVPROC)(CGparameter, const double *);
typedef void (CGAPI *PFNCGGLSETPARAMETER4DVPROC)(CGparameter, const double *);
typedef void (CGAPI *PFNCGGLGETPARAMETER1FPROC)(CGparameter, float *);
typedef void (CGAPI *PFNCGGLGETPARAMETER2FPROC)(CGparameter, float *);
typedef void (CGAPI *PFNCGGLGETPARAMETER3FPROC)(CGparameter, float *);
typedef void (CGAPI *PFNCGGLGETPARAMETER4FPROC)(CGparameter, float *);
typedef void (CGAPI *PFNCGGLGETPARAMETER1DPROC)(CGparameter, double *);
typedef void (CGAPI *PFNCGGLGETPARAMETER2DPROC)(CGparameter, double *);
typedef void (CGAPI *PFNCGGLGETPARAMETER3DPROC)(CGparameter, double *);
typedef void (CGAPI *PFNCGGLGETPARAMETER4DPROC)(CGparameter, double *);
typedef void (CGAPI *PFNCGGLSETPARAMETERARRAY1FPROC)(CGparameter, long, long, const float *);
typedef void (CGAPI *PFNCGGLSETPARAMETERARRAY2FPROC)(CGparameter, long, long, const float *);
typedef void (CGAPI *PFNCGGLSETPARAMETERARRAY3FPROC)(CGparameter, long, long, const float *);
typedef void (CGAPI *PFNCGGLSETPARAMETERARRAY4FPROC)(CGparameter, long, long, const float *);
typedef void (CGAPI *PFNCGGLSETPARAMETERARRAY1DPROC)(CGparameter, long, long, const double *);
typedef void (CGAPI *PFNCGGLSETPARAMETERARRAY2DPROC)(CGparameter, long, long, const double *);
typedef void (CGAPI *PFNCGGLSETPARAMETERARRAY3DPROC)(CGparameter, long, long, const double *);
typedef void (CGAPI *PFNCGGLSETPARAMETERARRAY4DPROC)(CGparameter, long, long, const double *);
typedef void (CGAPI *PFNCGGLGETPARAMETERARRAY1FPROC)(CGparameter, long, long, float *);
typedef void (CGAPI *PFNCGGLGETPARAMETERARRAY2FPROC)(CGparameter, long, long, float *);
typedef void (CGAPI *PFNCGGLGETPARAMETERARRAY3FPROC)(CGparameter, long, long, float *);
typedef void (CGAPI *PFNCGGLGETPARAMETERARRAY4FPROC)(CGparameter, long, long, float *);
typedef void (CGAPI *PFNCGGLGETPARAMETERARRAY1DPROC)(CGparameter, long, long, double *);
typedef void (CGAPI *PFNCGGLGETPARAMETERARRAY2DPROC)(CGparameter, long, long, double *);
typedef void (CGAPI *PFNCGGLGETPARAMETERARRAY3DPROC)(CGparameter, long, long, double *);
typedef void (CGAPI *PFNCGGLGETPARAMETERARRAY4DPROC)(CGparameter, long, long, double *);
typedef void (CGAPI *PFNCGGLSETPARAMETERPOINTERPROC)(CGparameter, GLint, GLenum, GLsizei, const GLvoid *);
typedef void (CGAPI *PFNCGGLENABLECLIENTSTATEPROC)(CGparameter);
typedef void (CGAPI *PFNCGGLDISABLECLIENTSTATEPROC)(CGparameter);
typedef void (CGAPI *PFNCGGLSETMATRIXPARAMETERDRPROC)(CGparameter, const double *);
typedef void (CGAPI *PFNCGGLSETMATRIXPARAMETERFRPROC)(CGparameter, const float *);
typedef void (CGAPI *PFNCGGLSETMATRIXPARAMETERDCPROC)(CGparameter, const double *);
typedef void (CGAPI *PFNCGGLSETMATRIXPARAMETERFCPROC)(CGparameter, const float *);
typedef void (CGAPI *PFNCGGLGETMATRIXPARAMETERDRPROC)(CGparameter, double *);
typedef void (CGAPI *PFNCGGLGETMATRIXPARAMETERFRPROC)(CGparameter, float *);
typedef void (CGAPI *PFNCGGLGETMATRIXPARAMETERDCPROC)(CGparameter, double *);
typedef void (CGAPI *PFNCGGLGETMATRIXPARAMETERFCPROC)(CGparameter, float *);
typedef void (CGAPI *PFNCGGLSETSTATEMATRIXPARAMETERPROC)(CGparameter, CGGLenum, CGGLenum);
typedef void (CGAPI *PFNCGGLSETMATRIXPARAMETERARRAYFCPROC)(CGparameter, long, long, const float *);
typedef void (CGAPI *PFNCGGLSETMATRIXPARAMETERARRAYFRPROC)(CGparameter, long, long, const float *);
typedef void (CGAPI *PFNCGGLSETMATRIXPARAMETERARRAYDCPROC)(CGparameter, long, long, const double *);
typedef void (CGAPI *PFNCGGLSETMATRIXPARAMETERARRAYDRPROC)(CGparameter, long, long, const double *);
typedef void (CGAPI *PFNCGGLGETMATRIXPARAMETERARRAYFCPROC)(CGparameter, long, long, float *);
typedef void (CGAPI *PFNCGGLGETMATRIXPARAMETERARRAYFRPROC)(CGparameter, long, long, float *);
typedef void (CGAPI *PFNCGGLGETMATRIXPARAMETERARRAYDCPROC)(CGparameter, long, long, double *);
typedef void (CGAPI *PFNCGGLGETMATRIXPARAMETERARRAYDRPROC)(CGparameter, long, long, double *);
typedef void (CGAPI *PFNCGGLSETTEXTUREPARAMETERPROC)(CGparameter, GLuint);
typedef GLuint (CGAPI *PFNCGGLGETTEXTUREPARAMETERPROC)(CGparameter);
typedef void (CGAPI *PFNCGGLENABLETEXTUREPARAMETERPROC)(CGparameter);
typedef void (CGAPI *PFNCGGLDISABLETEXTUREPARAMETERPROC)(CGparameter);
typedef GLenum (CGAPI *PFNCGGLGETTEXTUREENUMPROC)(CGparameter);
typedef void (CGAPI *PFNCGGLSETMANAGETEXTUREPARAMETERSPROC)(CGcontext, CGbool);
typedef CGbool (CGAPI *PFNCGGLGETMANAGETEXTUREPARAMETERSPROC)(CGcontext);
typedef void (CGAPI *PFNCGGLSETUPSAMPLERPROC)(CGparameter, GLuint);
typedef void (CGAPI *PFNCGGLREGISTERSTATESPROC)(CGcontext);
typedef void (CGAPI *PFNCGGLENABLEPROGRAMPROFILESPROC)(CGprogram);
typedef void (CGAPI *PFNCGGLDISABLEPROGRAMPROFILESPROC)(CGprogram);
typedef void (CGAPI *PFNCGGLSETDEBUGMODEPROC)(CGbool);
/*
typedef CGbuffer (CGAPI *PFNCGGLCREATEBUFFERPROC)(CGcontext, unsigned long, GLenum);
typedef GLuint (CGAPI *PFNCGGLGETBUFFEROBJECTPROC)(CGbuffer);
*/

extern PFNCGCREATECONTEXTPROC qcgCreateContext;
extern PFNCGDESTROYCONTEXTPROC qcgDestroyContext;
extern PFNCGISCONTEXTPROC qcgIsContext;
extern PFNCGGETLASTLISTINGPROC qcgGetLastListing;
extern PFNCGSETLASTLISTINGPROC qcgSetLastListing;
extern PFNCGSETAUTOCOMPILEPROC qcgSetAutoCompile;
extern PFNCGGETAUTOCOMPILEPROC qcgGetAutoCompile;
extern PFNCGCREATEPROGRAMPROC qcgCreateProgram;
extern PFNCGCREATEPROGRAMFROMFILEPROC qcgCreateProgramFromFile;
extern PFNCGCOPYPROGRAMPROC qcgCopyProgram;
extern PFNCGDESTROYPROGRAMPROC qcgDestroyProgram;
extern PFNCGGETFIRSTPROGRAMPROC qcgGetFirstProgram;
extern PFNCGGETNEXTPROGRAMPROC qcgGetNextProgram;
extern PFNCGGETPROGRAMCONTEXTPROC qcgGetProgramContext;
extern PFNCGISPROGRAMPROC qcgIsProgram;
extern PFNCGCOMPILEPROGRAMPROC qcgCompileProgram;
extern PFNCGISPROGRAMCOMPILEDPROC qcgIsProgramCompiled;
extern PFNCGGETPROGRAMSTRINGPROC qcgGetProgramString;
extern PFNCGGETPROGRAMPROFILEPROC qcgGetProgramProfile;
extern PFNCGGETPROGRAMOPTIONSPROC qcgGetProgramOptions;
extern PFNCGSETPROGRAMPROFILEPROC qcgSetProgramProfile;
extern PFNCGSETPASSPROGRAMPARAMETERSPROC qcgSetPassProgramParameters;
extern PFNCGCREATEPARAMETERPROC qcgCreateParameter;
extern PFNCGCREATEPARAMETERARRAYPROC qcgCreateParameterArray;
extern PFNCGCREATEPARAMETERMULTIDIMARRAYPROC qcgCreateParameterMultiDimArray;
extern PFNCGDESTROYPARAMETERPROC qcgDestroyParameter;
extern PFNCGCONNECTPARAMETERPROC qcgConnectParameter;
extern PFNCGDISCONNECTPARAMETERPROC qcgDisconnectParameter;
extern PFNCGGETCONNECTEDPARAMETERPROC qcgGetConnectedParameter;
extern PFNCGGETNUMCONNECTEDTOPARAMETERSPROC qcgGetNumConnectedToParameters;
extern PFNCGGETCONNECTEDTOPARAMETERPROC qcgGetConnectedToParameter;
extern PFNCGGETNAMEDPARAMETERPROC qcgGetNamedParameter;
extern PFNCGGETNAMEDPROGRAMPARAMETERPROC qcgGetNamedProgramParameter;
extern PFNCGGETFIRSTPARAMETERPROC qcgGetFirstParameter;
extern PFNCGGETNEXTPARAMETERPROC qcgGetNextParameter;
extern PFNCGGETFIRSTLEAFPARAMETERPROC qcgGetFirstLeafParameter;
extern PFNCGGETNEXTLEAFPARAMETERPROC qcgGetNextLeafParameter;
extern PFNCGGETFIRSTSTRUCTPARAMETERPROC qcgGetFirstStructParameter;
extern PFNCGGETNAMEDSTRUCTPARAMETERPROC qcgGetNamedStructParameter;
extern PFNCGGETFIRSTDEPENDENTPARAMETERPROC qcgGetFirstDependentParameter;
extern PFNCGGETARRAYPARAMETERPROC qcgGetArrayParameter;
extern PFNCGGETARRAYDIMENSIONPROC qcgGetArrayDimension;
extern PFNCGGETARRAYTYPEPROC qcgGetArrayType;
extern PFNCGGETARRAYSIZEPROC qcgGetArraySize;
extern PFNCGGETARRAYTOTALSIZEPROC qcgGetArrayTotalSize;
extern PFNCGSETARRAYSIZEPROC qcgSetArraySize;
extern PFNCGSETMULTIDIMARRAYSIZEPROC qcgSetMultiDimArraySize;
extern PFNCGGETPARAMETERPROGRAMPROC qcgGetParameterProgram;
extern PFNCGGETPARAMETERCONTEXTPROC qcgGetParameterContext;
extern PFNCGISPARAMETERPROC qcgIsParameter;
extern PFNCGGETPARAMETERNAMEPROC qcgGetParameterName;
extern PFNCGGETPARAMETERTYPEPROC qcgGetParameterType;
extern PFNCGGETPARAMETERBASETYPEPROC qcgGetParameterBaseType;
extern PFNCGGETPARAMETERCLASSPROC qcgGetParameterClass;
extern PFNCGGETPARAMETERROWSPROC qcgGetParameterRows;
extern PFNCGGETPARAMETERCOLUMNSPROC qcgGetParameterColumns;
extern PFNCGGETPARAMETERNAMEDTYPEPROC qcgGetParameterNamedType;
extern PFNCGGETPARAMETERSEMANTICPROC qcgGetParameterSemantic;
extern PFNCGGETPARAMETERRESOURCEPROC qcgGetParameterResource;
extern PFNCGGETPARAMETERBASERESOURCEPROC qcgGetParameterBaseResource;
extern PFNCGGETPARAMETERRESOURCEINDEXPROC qcgGetParameterResourceIndex;
extern PFNCGGETPARAMETERVARIABILITYPROC qcgGetParameterVariability;
extern PFNCGGETPARAMETERDIRECTIONPROC qcgGetParameterDirection;
extern PFNCGISPARAMETERREFERENCEDPROC qcgIsParameterReferenced;
extern PFNCGISPARAMETERUSEDPROC qcgIsParameterUsed;
extern PFNCGGETPARAMETERVALUESPROC qcgGetParameterValues;
extern PFNCGSETPARAMETERVALUEDRPROC qcgSetParameterValuedr;
extern PFNCGSETPARAMETERVALUEDCPROC qcgSetParameterValuedc;
extern PFNCGSETPARAMETERVALUEFRPROC qcgSetParameterValuefr;
extern PFNCGSETPARAMETERVALUEFCPROC qcgSetParameterValuefc;
extern PFNCGSETPARAMETERVALUEIRPROC qcgSetParameterValueir;
extern PFNCGSETPARAMETERVALUEICPROC qcgSetParameterValueic;
extern PFNCGGETPARAMETERVALUEDRPROC qcgGetParameterValuedr;
extern PFNCGGETPARAMETERVALUEDCPROC qcgGetParameterValuedc;
extern PFNCGGETPARAMETERVALUEFRPROC qcgGetParameterValuefr;
extern PFNCGGETPARAMETERVALUEFCPROC qcgGetParameterValuefc;
extern PFNCGGETPARAMETERVALUEIRPROC qcgGetParameterValueir;
extern PFNCGGETPARAMETERVALUEICPROC qcgGetParameterValueic;
extern PFNCGGETSTRINGPARAMETERVALUEPROC qcgGetStringParameterValue;
extern PFNCGSETSTRINGPARAMETERVALUEPROC qcgSetStringParameterValue;
extern PFNCGGETPARAMETERORDINALNUMBERPROC qcgGetParameterOrdinalNumber;
extern PFNCGISPARAMETERGLOBALPROC qcgIsParameterGlobal;
extern PFNCGGETPARAMETERINDEXPROC qcgGetParameterIndex;
extern PFNCGSETPARAMETERVARIABILITYPROC qcgSetParameterVariability;
extern PFNCGSETPARAMETERSEMANTICPROC qcgSetParameterSemantic;
extern PFNCGSETPARAMETER1FPROC qcgSetParameter1f;
extern PFNCGSETPARAMETER2FPROC qcgSetParameter2f;
extern PFNCGSETPARAMETER3FPROC qcgSetParameter3f;
extern PFNCGSETPARAMETER4FPROC qcgSetParameter4f;
extern PFNCGSETPARAMETER1DPROC qcgSetParameter1d;
extern PFNCGSETPARAMETER2DPROC qcgSetParameter2d;
extern PFNCGSETPARAMETER3DPROC qcgSetParameter3d;
extern PFNCGSETPARAMETER4DPROC qcgSetParameter4d;
extern PFNCGSETPARAMETER1IPROC qcgSetParameter1i;
extern PFNCGSETPARAMETER2IPROC qcgSetParameter2i;
extern PFNCGSETPARAMETER3IPROC qcgSetParameter3i;
extern PFNCGSETPARAMETER4IPROC qcgSetParameter4i;
extern PFNCGSETPARAMETER1IVPROC qcgSetParameter1iv;
extern PFNCGSETPARAMETER2IVPROC qcgSetParameter2iv;
extern PFNCGSETPARAMETER3IVPROC qcgSetParameter3iv;
extern PFNCGSETPARAMETER4IVPROC qcgSetParameter4iv;
extern PFNCGSETPARAMETER1FVPROC qcgSetParameter1fv;
extern PFNCGSETPARAMETER2FVPROC qcgSetParameter2fv;
extern PFNCGSETPARAMETER3FVPROC qcgSetParameter3fv;
extern PFNCGSETPARAMETER4FVPROC qcgSetParameter4fv;
extern PFNCGSETPARAMETER1DVPROC qcgSetParameter1dv;
extern PFNCGSETPARAMETER2DVPROC qcgSetParameter2dv;
extern PFNCGSETPARAMETER3DVPROC qcgSetParameter3dv;
extern PFNCGSETPARAMETER4DVPROC qcgSetParameter4dv;
extern PFNCGSETMATRIXPARAMETERIRPROC qcgSetMatrixParameterir;
extern PFNCGSETMATRIXPARAMETERDRPROC qcgSetMatrixParameterdr;
extern PFNCGSETMATRIXPARAMETERFRPROC qcgSetMatrixParameterfr;
extern PFNCGSETMATRIXPARAMETERICPROC qcgSetMatrixParameteric;
extern PFNCGSETMATRIXPARAMETERDCPROC qcgSetMatrixParameterdc;
extern PFNCGSETMATRIXPARAMETERFCPROC qcgSetMatrixParameterfc;
extern PFNCGGETMATRIXPARAMETERIRPROC qcgGetMatrixParameterir;
extern PFNCGGETMATRIXPARAMETERDRPROC qcgGetMatrixParameterdr;
extern PFNCGGETMATRIXPARAMETERFRPROC qcgGetMatrixParameterfr;
extern PFNCGGETMATRIXPARAMETERICPROC qcgGetMatrixParameteric;
extern PFNCGGETMATRIXPARAMETERDCPROC qcgGetMatrixParameterdc;
extern PFNCGGETMATRIXPARAMETERFCPROC qcgGetMatrixParameterfc;
extern PFNCGGETTYPESTRINGPROC qcgGetTypeString;
extern PFNCGGETTYPEPROC qcgGetType;
extern PFNCGGETNAMEDUSERTYPEPROC qcgGetNamedUserType;
extern PFNCGGETNUMUSERTYPESPROC qcgGetNumUserTypes;
extern PFNCGGETUSERTYPEPROC qcgGetUserType;
extern PFNCGGETNUMPARENTTYPESPROC qcgGetNumParentTypes;
extern PFNCGGETPARENTTYPEPROC qcgGetParentType;
extern PFNCGISPARENTTYPEPROC qcgIsParentType;
extern PFNCGISINTERFACETYPEPROC qcgIsInterfaceType;
extern PFNCGGETRESOURCESTRINGPROC qcgGetResourceString;
extern PFNCGGETRESOURCEPROC qcgGetResource;
extern PFNCGGETENUMSTRINGPROC qcgGetEnumString;
extern PFNCGGETENUMPROC qcgGetEnum;
extern PFNCGGETPROFILESTRINGPROC qcgGetProfileString;
extern PFNCGGETPROFILEPROC qcgGetProfile;
extern PFNCGGETERRORPROC qcgGetError;
extern PFNCGGETFIRSTERRORPROC qcgGetFirstError;
extern PFNCGGETERRORSTRINGPROC qcgGetErrorString;
extern PFNCGGETLASTERRORSTRINGPROC qcgGetLastErrorString;
extern PFNCGSETERRORCALLBACKPROC qcgSetErrorCallback;
extern PFNCGGETERRORCALLBACKPROC qcgGetErrorCallback;
extern PFNCGSETERRORHANDLERPROC qcgSetErrorHandler;
extern PFNCGGETERRORHANDLERPROC qcgGetErrorHandler;
extern PFNCGGETSTRINGPROC qcgGetString;
extern PFNCGCREATEEFFECTPROC qcgCreateEffect;
extern PFNCGCREATEEFFECTFROMFILEPROC qcgCreateEffectFromFile;
extern PFNCGDESTROYEFFECTPROC qcgDestroyEffect;
extern PFNCGGETEFFECTCONTEXTPROC qcgGetEffectContext;
extern PFNCGISEFFECTPROC qcgIsEffect;
extern PFNCGGETFIRSTEFFECTPROC qcgGetFirstEffect;
extern PFNCGGETNEXTEFFECTPROC qcgGetNextEffect;
extern PFNCGCREATEPROGRAMFROMEFFECTPROC qcgCreateProgramFromEffect;
extern PFNCGGETFIRSTTECHNIQUEPROC qcgGetFirstTechnique;
extern PFNCGGETNEXTTECHNIQUEPROC qcgGetNextTechnique;
extern PFNCGGETNAMEDTECHNIQUEPROC qcgGetNamedTechnique;
extern PFNCGGETTECHNIQUENAMEPROC qcgGetTechniqueName;
extern PFNCGISTECHNIQUEPROC qcgIsTechnique;
extern PFNCGVALIDATETECHNIQUEPROC qcgValidateTechnique;
extern PFNCGISTECHNIQUEVALIDATEDPROC qcgIsTechniqueValidated;
extern PFNCGGETTECHNIQUEEFFECTPROC qcgGetTechniqueEffect;
extern PFNCGGETFIRSTPASSPROC qcgGetFirstPass;
extern PFNCGGETNAMEDPASSPROC qcgGetNamedPass;
extern PFNCGGETNEXTPASSPROC qcgGetNextPass;
extern PFNCGISPASSPROC qcgIsPass;
extern PFNCGGETPASSNAMEPROC qcgGetPassName;
extern PFNCGGETPASSTECHNIQUEPROC qcgGetPassTechnique;
extern PFNCGSETPASSSTATEPROC qcgSetPassState;
extern PFNCGRESETPASSSTATEPROC qcgResetPassState;
extern PFNCGGETFIRSTSTATEASSIGNMENTPROC qcgGetFirstStateAssignment;
extern PFNCGGETNAMEDSTATEASSIGNMENTPROC qcgGetNamedStateAssignment;
extern PFNCGGETNEXTSTATEASSIGNMENTPROC qcgGetNextStateAssignment;
extern PFNCGISSTATEASSIGNMENTPROC qcgIsStateAssignment;
extern PFNCGCALLSTATESETCALLBACKPROC qcgCallStateSetCallback;
extern PFNCGCALLSTATEVALIDATECALLBACKPROC qcgCallStateValidateCallback;
extern PFNCGCALLSTATERESETCALLBACKPROC qcgCallStateResetCallback;
extern PFNCGGETSTATEASSIGNMENTPASSPROC qcgGetStateAssignmentPass;
extern PFNCGGETSAMPLERSTATEASSIGNMENTPARAMETERPROC qcgGetSamplerStateAssignmentParameter;
extern PFNCGGETFLOATSTATEASSIGNMENTVALUESPROC qcgGetFloatStateAssignmentValues;
extern PFNCGGETINTSTATEASSIGNMENTVALUESPROC qcgGetIntStateAssignmentValues;
extern PFNCGGETBOOLSTATEASSIGNMENTVALUESPROC qcgGetBoolStateAssignmentValues;
extern PFNCGGETSTRINGSTATEASSIGNMENTVALUEPROC qcgGetStringStateAssignmentValue;
extern PFNCGGETPROGRAMSTATEASSIGNMENTVALUEPROC qcgGetProgramStateAssignmentValue;
extern PFNCGGETTEXTURESTATEASSIGNMENTVALUEPROC qcgGetTextureStateAssignmentValue;
extern PFNCGGETSAMPLERSTATEASSIGNMENTVALUEPROC qcgGetSamplerStateAssignmentValue;
extern PFNCGGETSTATEASSIGNMENTINDEXPROC qcgGetStateAssignmentIndex;
extern PFNCGGETNUMDEPENDENTSTATEASSIGNMENTPARAMETERSPROC qcgGetNumDependentStateAssignmentParameters;
extern PFNCGGETDEPENDENTSTATEASSIGNMENTPARAMETERPROC qcgGetDependentStateAssignmentParameter;
extern PFNCGGETSTATEASSIGNMENTSTATEPROC qcgGetStateAssignmentState;
extern PFNCGGETSAMPLERSTATEASSIGNMENTSTATEPROC qcgGetSamplerStateAssignmentState;
extern PFNCGCREATESTATEPROC qcgCreateState;
extern PFNCGCREATEARRAYSTATEPROC qcgCreateArrayState;
extern PFNCGSETSTATECALLBACKSPROC qcgSetStateCallbacks;
extern PFNCGGETSTATESETCALLBACKPROC qcgGetStateSetCallback;
extern PFNCGGETSTATERESETCALLBACKPROC qcgGetStateResetCallback;
extern PFNCGGETSTATEVALIDATECALLBACKPROC qcgGetStateValidateCallback;
extern PFNCGGETSTATETYPEPROC qcgGetStateType;
extern PFNCGGETSTATENAMEPROC qcgGetStateName;
extern PFNCGGETSTATECONTEXTPROC qcgGetStateContext;
extern PFNCGGETNAMEDSTATEPROC qcgGetNamedState;
extern PFNCGGETFIRSTSTATEPROC qcgGetFirstState;
extern PFNCGGETNEXTSTATEPROC qcgGetNextState;
extern PFNCGISSTATEPROC qcgIsState;
extern PFNCGADDSTATEENUMERANTPROC qcgAddStateEnumerant;
extern PFNCGCREATESAMPLERSTATEPROC qcgCreateSamplerState;
extern PFNCGCREATEARRAYSAMPLERSTATEPROC qcgCreateArraySamplerState;
extern PFNCGGETNAMEDSAMPLERSTATEPROC qcgGetNamedSamplerState;
extern PFNCGGETFIRSTSAMPLERSTATEPROC qcgGetFirstSamplerState;
extern PFNCGGETFIRSTSAMPLERSTATEASSIGNMENTPROC qcgGetFirstSamplerStateAssignment;
extern PFNCGGETNAMEDSAMPLERSTATEASSIGNMENTPROC qcgGetNamedSamplerStateAssignment;
extern PFNCGSETSAMPLERSTATEPROC qcgSetSamplerState;
extern PFNCGGETNAMEDEFFECTPARAMETERPROC qcgGetNamedEffectParameter;
extern PFNCGGETFIRSTLEAFEFFECTPARAMETERPROC qcgGetFirstLeafEffectParameter;
extern PFNCGGETFIRSTEFFECTPARAMETERPROC qcgGetFirstEffectParameter;
extern PFNCGGETEFFECTPARAMETERBYSEMANTICPROC qcgGetEffectParameterBySemantic;
extern PFNCGGETFIRSTTECHNIQUEANNOTATIONPROC qcgGetFirstTechniqueAnnotation;
extern PFNCGGETFIRSTPASSANNOTATIONPROC qcgGetFirstPassAnnotation;
extern PFNCGGETFIRSTPARAMETERANNOTATIONPROC qcgGetFirstParameterAnnotation;
extern PFNCGGETFIRSTPROGRAMANNOTATIONPROC qcgGetFirstProgramAnnotation;
extern PFNCGGETFIRSTEFFECTANNOTATIONPROC qcgGetFirstEffectAnnotation;
extern PFNCGGETNEXTANNOTATIONPROC qcgGetNextAnnotation;
extern PFNCGGETNAMEDTECHNIQUEANNOTATIONPROC qcgGetNamedTechniqueAnnotation;
extern PFNCGGETNAMEDPASSANNOTATIONPROC qcgGetNamedPassAnnotation;
extern PFNCGGETNAMEDPARAMETERANNOTATIONPROC qcgGetNamedParameterAnnotation;
extern PFNCGGETNAMEDPROGRAMANNOTATIONPROC qcgGetNamedProgramAnnotation;
extern PFNCGGETNAMEDEFFECTANNOTATIONPROC qcgGetNamedEffectAnnotation;
extern PFNCGISANNOTATIONPROC qcgIsAnnotation;
extern PFNCGGETANNOTATIONNAMEPROC qcgGetAnnotationName;
extern PFNCGGETANNOTATIONTYPEPROC qcgGetAnnotationType;
extern PFNCGGETFLOATANNOTATIONVALUESPROC qcgGetFloatAnnotationValues;
extern PFNCGGETINTANNOTATIONVALUESPROC qcgGetIntAnnotationValues;
extern PFNCGGETSTRINGANNOTATIONVALUEPROC qcgGetStringAnnotationValue;
extern PFNCGGETBOOLEANANNOTATIONVALUESPROC qcgGetBooleanAnnotationValues;
extern PFNCGGETBOOLANNOTATIONVALUESPROC qcgGetBoolAnnotationValues;
extern PFNCGGETNUMDEPENDENTANNOTATIONPARAMETERSPROC qcgGetNumDependentAnnotationParameters;
extern PFNCGGETDEPENDENTANNOTATIONPARAMETERPROC qcgGetDependentAnnotationParameter;
extern PFNCGEVALUATEPROGRAMPROC qcgEvaluateProgram;
extern PFNCGSETEFFECTNAMEPROC qcgSetEffectName;
extern PFNCGGETEFFECTNAMEPROC qcgGetEffectName;
extern PFNCGGETNAMEDEFFECTPROC qcgGetNamedEffect;
extern PFNCGCREATEEFFECTPARAMETERPROC qcgCreateEffectParameter;
extern PFNCGCREATETECHNIQUEPROC qcgCreateTechnique;
extern PFNCGCREATEPASSPROC qcgCreatePass;
extern PFNCGCREATESTATEASSIGNMENTPROC qcgCreateStateAssignment;
extern PFNCGCREATESTATEASSIGNMENTINDEXPROC qcgCreateStateAssignmentIndex;
extern PFNCGCREATESAMPLERSTATEASSIGNMENTPROC qcgCreateSamplerStateAssignment;
extern PFNCGSETFLOATSTATEASSIGNMENTPROC qcgSetFloatStateAssignment;
extern PFNCGSETINTSTATEASSIGNMENTPROC qcgSetIntStateAssignment;
extern PFNCGSETBOOLSTATEASSIGNMENTPROC qcgSetBoolStateAssignment;
extern PFNCGSETSTRINGSTATEASSIGNMENTPROC qcgSetStringStateAssignment;
extern PFNCGSETPROGRAMSTATEASSIGNMENTPROC qcgSetProgramStateAssignment;
extern PFNCGSETSAMPLERSTATEASSIGNMENTPROC qcgSetSamplerStateAssignment;
extern PFNCGSETTEXTURESTATEASSIGNMENTPROC qcgSetTextureStateAssignment;
extern PFNCGSETFLOATARRAYSTATEASSIGNMENTPROC qcgSetFloatArrayStateAssignment;
extern PFNCGSETINTARRAYSTATEASSIGNMENTPROC qcgSetIntArrayStateAssignment;
extern PFNCGSETBOOLARRAYSTATEASSIGNMENTPROC qcgSetBoolArrayStateAssignment;
extern PFNCGCREATETECHNIQUEANNOTATIONPROC qcgCreateTechniqueAnnotation;
extern PFNCGCREATEPASSANNOTATIONPROC qcgCreatePassAnnotation;
extern PFNCGCREATEPARAMETERANNOTATIONPROC qcgCreateParameterAnnotation;
extern PFNCGCREATEPROGRAMANNOTATIONPROC qcgCreateProgramAnnotation;
extern PFNCGCREATEEFFECTANNOTATIONPROC qcgCreateEffectAnnotation;
extern PFNCGSETINTANNOTATIONPROC qcgSetIntAnnotation;
extern PFNCGSETFLOATANNOTATIONPROC qcgSetFloatAnnotation;
extern PFNCGSETBOOLANNOTATIONPROC qcgSetBoolAnnotation;
extern PFNCGSETSTRINGANNOTATIONPROC qcgSetStringAnnotation;
extern PFNCGGETSTATEENUMERANTNAMEPROC qcgGetStateEnumerantName;
extern PFNCGGETSTATEENUMERANTVALUEPROC qcgGetStateEnumerantValue;
extern PFNCGGETPARAMETEREFFECTPROC qcgGetParameterEffect;
extern PFNCGGETTYPECLASSPROC qcgGetTypeClass;
extern PFNCGGETTYPEBASEPROC qcgGetTypeBase;
extern PFNCGGETTYPESIZESPROC qcgGetTypeSizes;
extern PFNCGGETMATRIXSIZEPROC qcgGetMatrixSize;
extern PFNCGGETPROFILEDOMAINPROC qcgGetProfileDomain;
extern PFNCGCOMBINEPROGRAMSPROC qcgCombinePrograms;
extern PFNCGCOMBINEPROGRAMS2PROC qcgCombinePrograms2;
extern PFNCGCOMBINEPROGRAMS3PROC qcgCombinePrograms3;
extern PFNCGGETNUMPROGRAMDOMAINSPROC qcgGetNumProgramDomains;
extern PFNCGGETPROGRAMDOMAINPROFILEPROC qcgGetProgramDomainProfile;
extern PFNCGCREATEEFFECTPARAMETERARRAYPROC qcgCreateEffectParameterArray;
extern PFNCGCREATEEFFECTPARAMETERMULTIDIMARRAYPROC qcgCreateEffectParameterMultiDimArray;
extern PFNCGGETNAMEDSUBPARAMETERPROC qcgGetNamedSubParameter;
/*
extern PFNCGGETPROGRAMINPUTPROC qcgGetProgramInput;
extern PFNCGGETPROGRAMOUTPUTPROC qcgGetProgramOutput;
extern PFNCGCREATEOBJPROC qcgCreateObj;
extern PFNCGCREATEOBJFROMFILEPROC qcgCreateObjFromFile;
extern PFNCGDESTROYOBJPROC qcgDestroyObj;
extern PFNCGSETPARAMETERSETTINGMODEPROC qcgSetParameterSettingMode;
extern PFNCGGETPARAMETERSETTINGMODEPROC qcgGetParameterSettingMode;
extern PFNCGUPDATEPROGRAMPARAMETERSPROC qcgUpdateProgramParameters;
extern PFNCGSETLOCKINGPOLICYPROC qcgSetLockingPolicy;
extern PFNCGGETLOCKINGPOLICYPROC qcgGetLockingPolicy;
extern PFNCGSETSEMANTICCASEPOLICYPROC qcgSetSemanticCasePolicy;
extern PFNCGGETSEMANTICCASEPOLICYPROC qcgGetSemanticCasePolicy;
extern PFNCGGETCONNECTEDSTATEASSIGNMENTPARAMETERPROC qcgGetConnectedStateAssignmentParameter;
extern PFNCGGETSTRINGANNOTATIONVALUESPROC qcgGetStringAnnotationValues;
extern PFNCGSETBUFFERDATAPROC qcgSetBufferData;
extern PFNCGSETBUFFERSUBDATAPROC qcgSetBufferSubData;
extern PFNCGCREATEBUFFERPROC qcgCreateBuffer;
extern PFNCGDESTROYBUFFERPROC qcgDestroyBuffer;
extern PFNCGGETBUFFERSIZEPROC qcgGetBufferSize;
extern PFNCGGETPARAMETERBUFFERINDEXPROC qcgGetParameterBufferIndex;
extern PFNCGGETPARAMETERBUFFEROFFSETPROC qcgGetParameterBufferOffset;
extern PFNCGGETPARAMETERRESOURCESIZEPROC qcgGetParameterResourceSize;
extern PFNCGGETPROGRAMBUFFERPROC qcgGetProgramBuffer;
extern PFNCGMAPBUFFERPROC qcgMapBuffer;
extern PFNCGSETPROGRAMBUFFERPROC qcgSetProgramBuffer;
extern PFNCGUNMAPBUFFERPROC qcgUnmapBuffer;
*/

extern PFNCGGLISPROFILESUPPORTEDPROC qcgGLIsProfileSupported;
extern PFNCGGLENABLEPROFILEPROC qcgGLEnableProfile;
extern PFNCGGLDISABLEPROFILEPROC qcgGLDisableProfile;
extern PFNCGGLGETLATESTPROFILEPROC qcgGLGetLatestProfile;
extern PFNCGGLSETOPTIMALOPTIONSPROC qcgGLSetOptimalOptions;
extern PFNCGGLLOADPROGRAMPROC qcgGLLoadProgram;
extern PFNCGGLISPROGRAMLOADEDPROC qcgGLIsProgramLoaded;
extern PFNCGGLBINDPROGRAMPROC qcgGLBindProgram;
extern PFNCGGLUNBINDPROGRAMPROC qcgGLUnbindProgram;
extern PFNCGGLGETPROGRAMIDPROC qcgGLGetProgramID;
extern PFNCGGLSETPARAMETER1FPROC qcgGLSetParameter1f;
extern PFNCGGLSETPARAMETER2FPROC qcgGLSetParameter2f;
extern PFNCGGLSETPARAMETER3FPROC qcgGLSetParameter3f;
extern PFNCGGLSETPARAMETER4FPROC qcgGLSetParameter4f;
extern PFNCGGLSETPARAMETER1FVPROC qcgGLSetParameter1fv;
extern PFNCGGLSETPARAMETER2FVPROC qcgGLSetParameter2fv;
extern PFNCGGLSETPARAMETER3FVPROC qcgGLSetParameter3fv;
extern PFNCGGLSETPARAMETER4FVPROC qcgGLSetParameter4fv;
extern PFNCGGLSETPARAMETER1DPROC qcgGLSetParameter1d;
extern PFNCGGLSETPARAMETER2DPROC qcgGLSetParameter2d;
extern PFNCGGLSETPARAMETER3DPROC qcgGLSetParameter3d;
extern PFNCGGLSETPARAMETER4DPROC qcgGLSetParameter4d;
extern PFNCGGLSETPARAMETER1DVPROC qcgGLSetParameter1dv;
extern PFNCGGLSETPARAMETER2DVPROC qcgGLSetParameter2dv;
extern PFNCGGLSETPARAMETER3DVPROC qcgGLSetParameter3dv;
extern PFNCGGLSETPARAMETER4DVPROC qcgGLSetParameter4dv;
extern PFNCGGLGETPARAMETER1FPROC qcgGLGetParameter1f;
extern PFNCGGLGETPARAMETER2FPROC qcgGLGetParameter2f;
extern PFNCGGLGETPARAMETER3FPROC qcgGLGetParameter3f;
extern PFNCGGLGETPARAMETER4FPROC qcgGLGetParameter4f;
extern PFNCGGLGETPARAMETER1DPROC qcgGLGetParameter1d;
extern PFNCGGLGETPARAMETER2DPROC qcgGLGetParameter2d;
extern PFNCGGLGETPARAMETER3DPROC qcgGLGetParameter3d;
extern PFNCGGLGETPARAMETER4DPROC qcgGLGetParameter4d;
extern PFNCGGLSETPARAMETERARRAY1FPROC qcgGLSetParameterArray1f;
extern PFNCGGLSETPARAMETERARRAY2FPROC qcgGLSetParameterArray2f;
extern PFNCGGLSETPARAMETERARRAY3FPROC qcgGLSetParameterArray3f;
extern PFNCGGLSETPARAMETERARRAY4FPROC qcgGLSetParameterArray4f;
extern PFNCGGLSETPARAMETERARRAY1DPROC qcgGLSetParameterArray1d;
extern PFNCGGLSETPARAMETERARRAY2DPROC qcgGLSetParameterArray2d;
extern PFNCGGLSETPARAMETERARRAY3DPROC qcgGLSetParameterArray3d;
extern PFNCGGLSETPARAMETERARRAY4DPROC qcgGLSetParameterArray4d;
extern PFNCGGLGETPARAMETERARRAY1FPROC qcgGLGetParameterArray1f;
extern PFNCGGLGETPARAMETERARRAY2FPROC qcgGLGetParameterArray2f;
extern PFNCGGLGETPARAMETERARRAY3FPROC qcgGLGetParameterArray3f;
extern PFNCGGLGETPARAMETERARRAY4FPROC qcgGLGetParameterArray4f;
extern PFNCGGLGETPARAMETERARRAY1DPROC qcgGLGetParameterArray1d;
extern PFNCGGLGETPARAMETERARRAY2DPROC qcgGLGetParameterArray2d;
extern PFNCGGLGETPARAMETERARRAY3DPROC qcgGLGetParameterArray3d;
extern PFNCGGLGETPARAMETERARRAY4DPROC qcgGLGetParameterArray4d;
extern PFNCGGLSETPARAMETERPOINTERPROC qcgGLSetParameterPointer;
extern PFNCGGLENABLECLIENTSTATEPROC qcgGLEnableClientState;
extern PFNCGGLDISABLECLIENTSTATEPROC qcgGLDisableClientState;
extern PFNCGGLSETMATRIXPARAMETERDRPROC qcgGLSetMatrixParameterdr;
extern PFNCGGLSETMATRIXPARAMETERFRPROC qcgGLSetMatrixParameterfr;
extern PFNCGGLSETMATRIXPARAMETERDCPROC qcgGLSetMatrixParameterdc;
extern PFNCGGLSETMATRIXPARAMETERFCPROC qcgGLSetMatrixParameterfc;
extern PFNCGGLGETMATRIXPARAMETERDRPROC qcgGLGetMatrixParameterdr;
extern PFNCGGLGETMATRIXPARAMETERFRPROC qcgGLGetMatrixParameterfr;
extern PFNCGGLGETMATRIXPARAMETERDCPROC qcgGLGetMatrixParameterdc;
extern PFNCGGLGETMATRIXPARAMETERFCPROC qcgGLGetMatrixParameterfc;
extern PFNCGGLSETSTATEMATRIXPARAMETERPROC qcgGLSetStateMatrixParameter;
extern PFNCGGLSETMATRIXPARAMETERARRAYFCPROC qcgGLSetMatrixParameterArrayfc;
extern PFNCGGLSETMATRIXPARAMETERARRAYFRPROC qcgGLSetMatrixParameterArrayfr;
extern PFNCGGLSETMATRIXPARAMETERARRAYDCPROC qcgGLSetMatrixParameterArraydc;
extern PFNCGGLSETMATRIXPARAMETERARRAYDRPROC qcgGLSetMatrixParameterArraydr;
extern PFNCGGLGETMATRIXPARAMETERARRAYFCPROC qcgGLGetMatrixParameterArrayfc;
extern PFNCGGLGETMATRIXPARAMETERARRAYFRPROC qcgGLGetMatrixParameterArrayfr;
extern PFNCGGLGETMATRIXPARAMETERARRAYDCPROC qcgGLGetMatrixParameterArraydc;
extern PFNCGGLGETMATRIXPARAMETERARRAYDRPROC qcgGLGetMatrixParameterArraydr;
extern PFNCGGLSETTEXTUREPARAMETERPROC qcgGLSetTextureParameter;
extern PFNCGGLGETTEXTUREPARAMETERPROC qcgGLGetTextureParameter;
extern PFNCGGLENABLETEXTUREPARAMETERPROC qcgGLEnableTextureParameter;
extern PFNCGGLDISABLETEXTUREPARAMETERPROC qcgGLDisableTextureParameter;
extern PFNCGGLGETTEXTUREENUMPROC qcgGLGetTextureEnum;
extern PFNCGGLSETMANAGETEXTUREPARAMETERSPROC qcgGLSetManageTextureParameters;
extern PFNCGGLGETMANAGETEXTUREPARAMETERSPROC qcgGLGetManageTextureParameters;
extern PFNCGGLSETUPSAMPLERPROC qcgGLSetupSampler;
extern PFNCGGLREGISTERSTATESPROC qcgGLRegisterStates;
extern PFNCGGLENABLEPROGRAMPROFILESPROC qcgGLEnableProgramProfiles;
extern PFNCGGLDISABLEPROGRAMPROFILESPROC qcgGLDisableProgramProfiles;
extern PFNCGGLSETDEBUGMODEPROC qcgGLSetDebugMode;
/*
extern PFNCGGLCREATEBUFFERPROC qcgGLCreateBuffer;
extern PFNCGGLGETBUFFEROBJECTPROC qcgGLGetBufferObject;
*/

CGbool cgLoadLibrary();
CGbool cgGLLoadLibrary();
	
#ifdef __cplusplus
} // extern "C"
#endif

#endif // CG_EXPLICIT_H
