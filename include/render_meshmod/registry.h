#pragma once

#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"

typedef void const *(*MeshMod_RegistryDefaultDataFunc)();
typedef char const *(*MeshMod_RegistryDescriptionFunc)();
typedef bool (*MeshMod_RegistryEqualFunc)(void const* a, void const* b, float const epsilon);
typedef void (*MeshMod_RegistryDestroyFunc)(void* element);
typedef void (*MeshMod_RegistryCloneFunc)(void* src, void* dst);
typedef double (*MeshMod_RegistryDistanceFunc)();

typedef void (*MeshMod_RegistryVertexInterpolate1DFunc)(void const* a, void const* b, void* r, float const t);
typedef void (*MeshMod_RegistryVertexInterpolate2DFunc)(void const* a, void const* b, void const* c, void* r, float const u, float const v);

typedef struct MeshMod_RegistryCommonFunctionTable {
	MeshMod_RegistryDefaultDataFunc defaultDataFunc;
	MeshMod_RegistryEqualFunc equalFunc;
	MeshMod_RegistryDestroyFunc destroyFunc; // can be null only set if really needs a destructor
	MeshMod_RegistryDescriptionFunc descriptionFunc; // can be null
	MeshMod_RegistryDistanceFunc distanceFunc;
	MeshMod_RegistryCloneFunc cloneFunc; // can be null only set if really needs a destructor
} MeshMod_RegistryCommonFunctionTable;

typedef struct MeshMod_RegistryVertexFunctionTable {
	MeshMod_RegistryVertexInterpolate1DFunc interpolate1DFunc; // can be null meaning not interpolatable
	MeshMod_RegistryVertexInterpolate2DFunc interpolate2DFunc; // can be null meaning not interpolatable
} MeshMod_RegistryVertexFunctionTable;

typedef struct MeshMod_RegistryEdgeFunctionTable {
	int placeholder; // because C requires 1 member
} MeshMod_RegistryEdgeFunctionTable;

typedef struct MeshMod_RegistryPolygonFunctionTable {
	int placeholder; // because C requires 1 member
} MeshMod_RegistryPolygonFunctionTable;

AL2O3_EXTERN_C MeshMod_RegistryHandle MeshMod_RegistryCreate();
AL2O3_EXTERN_C MeshMod_RegistryHandle MeshMod_RegistryCreateWithDefaults();
AL2O3_EXTERN_C void MeshMod_RegistryDestroy(MeshMod_RegistryHandle handle);
AL2O3_EXTERN_C MeshMod_RegistryHandle MeshMod_RegistryClone(MeshMod_RegistryHandle handle);

AL2O3_EXTERN_C void MeshMod_RegistryAddType(
	MeshMod_RegistryHandle handle,
	MeshMod_Tag tag,
	size_t elementSize,
	MeshMod_RegistryCommonFunctionTable* commonFunctionTable,
	void* functionTable);


AL2O3_EXTERN_C bool MeshMod_RegistryExists(MeshMod_RegistryHandle handle, MeshMod_Tag tag);
AL2O3_EXTERN_C size_t MeshMod_RegistryElementSize(MeshMod_RegistryHandle handle, MeshMod_Tag tag);
AL2O3_EXTERN_C MeshMod_Type MeshMod_RegistryType(MeshMod_RegistryHandle handle, MeshMod_Tag tag);
AL2O3_EXTERN_C char const* MeshMod_RegistryDescription(MeshMod_RegistryHandle handle, MeshMod_Tag tag);
AL2O3_EXTERN_C void const* MeshMod_RegistryDefaultData(MeshMod_RegistryHandle handle, MeshMod_Tag tag);
AL2O3_EXTERN_C bool MeshMod_RegistryIsDefaultData(MeshMod_RegistryHandle handle, MeshMod_Tag tag, void const* testData);
AL2O3_EXTERN_C bool MeshMod_RegistryHasDestroy(MeshMod_RegistryHandle handle, MeshMod_Tag tag);
AL2O3_EXTERN_C bool MeshMod_RegistryHasClone(MeshMod_RegistryHandle handle, MeshMod_Tag tag);

AL2O3_EXTERN_C MeshMod_RegistryCommonFunctionTable* MeshMod_RegistryGetCommonFunctionTable(MeshMod_RegistryHandle handle, MeshMod_Tag tag);

AL2O3_EXTERN_C void* MeshMod_RegistryFunctionTable(MeshMod_RegistryHandle handle, MeshMod_Tag tag, MeshMod_Type type);
