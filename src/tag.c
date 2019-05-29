#include "al2o3_platform/platform.h"
#include "al2o3_memory/memory.h"
#include "gfx_meshmod/cvector.h"
#include "gfx_meshmod/cdict.h"
#include "gfx_meshmod/tag.h"

typedef struct MeshMod_TagRegistryItem {
	size_t dataSize;
	char const *description;
} MeshMod_TagRegistryItem;

typedef struct MeshMod_TagRegistry {
	MeshMod_CVectorHandle registry;
	MeshMod_CDictU64Handle tagDictionary;
} MeshMod_TagRegistry;

AL2O3_EXTERN_C MeshMod_TagRegistryHandle MeshMod_TagRegistryCreate() {
	MeshMod_TagRegistry *reg = MEMORY_CALLOC(1, sizeof(MeshMod_TagRegistry));
	if (reg == NULL)
		return NULL;

	reg->registry = MeshMod_CVectorCreate(sizeof(MeshMod_TagRegistryItem));
	if (reg->registry == NULL) {
		MEMORY_FREE(reg);
		return NULL;
	}
	reg->tagDictionary = MeshMod_CDictU64Create();
	if (reg->tagDictionary == NULL) {
		MeshMod_CVectorDestroy(reg->registry);
		MEMORY_FREE(reg);
	}

	return reg;
}

AL2O3_EXTERN_C void MeshMod_TagRegistryDestroy(MeshMod_TagRegistryHandle handle) {
	ASSERT(handle);
	MeshMod_TagRegistry *reg = (MeshMod_TagRegistry *) handle;

	for (size_t i = 0; i < MeshMod_CVectorSizeFrom(reg->registry); ++i) {
		MeshMod_TagRegistryItem *item = MeshMod_CVectorElementFrom(reg->registry, i);
		MEMORY_FREE((void *) item->description);
	}

	MeshMod_CDictU64Destroy(reg->tagDictionary);
	MeshMod_CVectorDestroy(reg->registry);
	MEMORY_FREE(reg);
}

AL2O3_EXTERN_C void MeshMod_TagRegistryAddTag(MeshMod_TagRegistryHandle handle,
																							MeshMod_Tag tag,
																							size_t dataSize,
																							char const *description) {
	ASSERT(handle);
	MeshMod_TagRegistry *reg = (MeshMod_TagRegistry *) handle;

	char *str = (char *) MEMORY_MALLOC(strlen(description) + 1);
	strcpy(str, description);

	MeshMod_TagRegistryItem item = {
			dataSize,
			str
	};

	size_t index = MeshMod_CVectorPushElement(reg->registry, &item);
	bool okay = MeshMod_CDictU64Add(reg->tagDictionary, tag, (uint32_t) index);
	ASSERT(okay);
}
AL2O3_EXTERN_C bool MeshMod_TagRegistryTagExists(MeshMod_TagRegistryHandle handle, MeshMod_Tag tag) {
	ASSERT(handle);
	MeshMod_TagRegistry *reg = (MeshMod_TagRegistry *) handle;
	return MeshMod_CDictU64KeyExists(reg->tagDictionary, tag);
}

AL2O3_EXTERN_C size_t MeshMod_TagRegistryDataSize(MeshMod_TagRegistryHandle handle, MeshMod_Tag tag) {
	ASSERT(handle);
	MeshMod_TagRegistry *reg = (MeshMod_TagRegistry *) handle;
	ASSERT(MeshMod_CDictU64KeyExists(reg->tagDictionary, tag));

	uint64_t index = 0;
	bool okay = MeshMod_CDictU64Lookup(reg->tagDictionary, tag, &index);
	if (!okay)
		return 0;

	ASSERT(index < MeshMod_CVectorSizeFrom(reg->registry));
	MeshMod_TagRegistryItem *item = MeshMod_CVectorElementFrom(reg->registry, index);
	return item->dataSize;
}
AL2O3_EXTERN_C char const *MeshMod_TagRegistryDescription(MeshMod_TagRegistryHandle handle,
																													MeshMod_Tag tag) {
	ASSERT(handle);
	MeshMod_TagRegistry *reg = (MeshMod_TagRegistry *) handle;
	ASSERT(MeshMod_CDictU64KeyExists(reg->tagDictionary, tag));

	uint64_t index = 0;
	bool okay = MeshMod_CDictU64Lookup(reg->tagDictionary, tag, &index);
	if (!okay)
		return NULL;

	ASSERT(index < MeshMod_CVectorSizeFrom(reg->registry));
	MeshMod_TagRegistryItem *item = MeshMod_CVectorElementFrom(reg->registry, index);
	return item->description;
}
