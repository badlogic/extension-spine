
#include <spine/AttachmentLoader.h>
#include <spine/extension.h>

#include <dmsdk/dlib/hash.h>
#include <dmsdk/dlib/log.h>
#include <dmsdk/gamesys/resources/res_textureset.h>

#include "spine_loader.h"

// Heavily borrowing from AtlasAttachmentLoader.c

// These functions are part of the api that the developer must fulfill
void _spAtlasPage_createTexture(spAtlasPage *self, const char *path) {
    self->rendererObject = 0;
    self->width = 2048;
    self->height = 2048;
}

void _spAtlasPage_disposeTexture(spAtlasPage *self) {
}

char *_spUtil_readFile(const char *path, int *length) {
    return 0;//_spReadFile(path, length);
}


namespace dmSpine
{
        // dmHashTable<dmhash_t, uint32_t>     m_AnimationIds;
        // dmGraphics::HTexture                m_Texture;
        // dmhash_t                            m_TexturePath;
        // dmGameSystemDDF::TextureSet*        m_TextureSet;

    // static spAtlasRegion* CreateRegionsFromGeometry(dmGameSystem::TextureSetResource* atlas)
    // {
    //     dmGameSystemDDF::TextureSet* texture_set_ddf = atlas->m_TextureSet;
    //     dmGameSystemDDF::TextureSetAnimation* animations = texture_set_ddf->m_Animations.m_Data;
    //     uint32_t    n_animations = texture_set_ddf->m_Animations.m_Count;
    //     uint32_t*   frame_indices = texture_set_ddf->m_FrameIndices.m_Data;

    //     spAtlasRegion* regions = new spAtlasRegion[n_animations];
    //     for (uint32_t i = 0; i < n_animations; ++i)
    //     {
    //         const dmGameSystemDDF::TextureSetAnimation* animation_ddf = &animations[i];
    //         uint32_t frame_index = frame_indices[animation_ddf->m_Start];

    //         uint32_t num_points = geometry->m_Vertices.m_Count / 2;

    //         const float* points = geometry->m_Vertices.m_Data;
    //         const float* uvs = geometry->m_Uvs.m_Data;

    //         // Depending on the sprite is flipped or not, we loop the vertices forward or backward
    //         // to respect face winding (and backface culling)
    //         int flipx = animation_ddf->m_FlipHorizontal ^ component->m_FlipHorizontal;
    //         int flipy = animation_ddf->m_FlipVertical ^ component->m_FlipVertical;
    //         int reverse = flipx ^ flipy;

    //         float scaleX = flipx ? -1 : 1;
    //         float scaleY = flipy ? -1 : 1;

    //         int step = reverse ? -2 : 2;
    //         points = reverse ? points + num_points*2 - 2 : points;
    //         uvs = reverse ? uvs + num_points*2 - 2 : uvs;

    //         for (uint32_t vert = 0; vert < num_points; ++vert, ++vertices, points += step, uvs += step)
    //         {
    //             float x = points[0] * scaleX; // range -0.5,+0.5
    //             float y = points[1] * scaleY;
    //             float u = uvs[0];
    //             float v = uvs[1];

    //             Vector4 p0 = w * Point3(x, y, 0.0f);
    //             vertices[0].x = ((float*)&p0)[0];
    //             vertices[0].y = ((float*)&p0)[1];
    //             vertices[0].z = ((float*)&p0)[2];
    //             vertices[0].u = u;
    //             vertices[0].v = v;
    //         }

    //         texture_set_ddf->m_Animations[i];
    //         dmhash_t h = dmHashString64(texture_set_ddf->m_Animations[i].m_Id);
    //         tile_set->m_AnimationIds.Put(h, i);
    //     }
    // }

    static spAtlasRegion* CreateRegionsFromQuads(dmGameSystem::TextureSetResource* atlas)
    {
        dmGameSystemDDF::TextureSet* texture_set_ddf = atlas->m_TextureSet;
        const float* tex_coords = (const float*) texture_set_ddf->m_TexCoords.m_Data;
        uint32_t n_animations = texture_set_ddf->m_Animations.m_Count;
        dmGameSystemDDF::TextureSetAnimation* animations = texture_set_ddf->m_Animations.m_Data;

        spAtlasRegion* regions = new spAtlasRegion[n_animations];
        for (uint32_t i = 0; i < n_animations; ++i)
        {
                dmGameSystemDDF::TextureSetAnimation* animation_ddf = &animations[i];

                dmLogWarning("Animation: %d %s", i, animation_ddf->m_Id);

    // required string id              = 1;
    // required uint32 width           = 2;
    // required uint32 height          = 3;
    // required uint32 start           = 4;
    // required uint32 end             = 5;
    // optional uint32 fps             = 6 [default = 30];
    // optional Playback playback      = 7 [default = PLAYBACK_ONCE_FORWARD];
    // optional uint32 flip_horizontal = 8 [default = 0];
    // optional uint32 flip_vertical   = 9 [default = 0];
    // optional uint32 is_animation    = 10 [default = 0]; // Deprecated


                uint32_t frame_index = animation_ddf->m_Start;
                const float* tc = &tex_coords[frame_index * 4 * 2];

                // From comp_sprite.cpp
                // vertices[0].u = tc[tex_lookup[0] * 2];
                // vertices[0].v = tc[tex_lookup[0] * 2 + 1];
                // vertices[1].u = tc[tex_lookup[1] * 2];
                // vertices[1].v = tc[tex_lookup[1] * 2 + 1];
                // vertices[2].u = tc[tex_lookup[2] * 2];
                // vertices[2].v = tc[tex_lookup[2] * 2 + 1];
                // vertices[3].u = tc[tex_lookup[4] * 2];
                // vertices[3].v = tc[tex_lookup[4] * 2 + 1];

                // Since this struct is only used as a placeholder to show which values are needed
                // we only set the ones we care about

                spAtlasRegion* region = &regions[i];
                memset(region, 0, sizeof(spAtlasRegion));

                region->u   = tc[0 * 2 + 0];
                region->v   = tc[0 * 2 + 1];
                region->u2  = tc[2 * 2 + 0];
                region->v2  = tc[2 * 2 + 1];

                region->degrees = 0; // The uv's are already rotated

                // We don't support packing yet
                region->offsetX = 0;
                region->offsetY = 0;
                region->width = region->originalWidth = animation_ddf->m_Width;
                region->height = region->originalHeight = animation_ddf->m_Height;
        }

        return regions;
    }

    // Create an array or regions given the atlas. Maps 1:1 with the animation count
    spAtlasRegion* CreateRegions(dmGameSystem::TextureSetResource* atlas)
    {
        return CreateRegionsFromQuads(atlas);
    }

    static spAtlasRegion* FindAtlasRegion(dmGameSystem::TextureSetResource* atlas, spAtlasRegion* regions, const char* name)
    {
        dmhash_t name_hash = dmHashString64(name);
        uint32_t* anim_index = atlas->m_AnimationIds.Get(name_hash);
        if (!anim_index)
            return 0;
        return &regions[*anim_index];
    }

    static spAttachment *spDefoldAtlasAttachmentLoader_createAttachment(spAttachmentLoader *loader, spSkin *skin, spAttachmentType type,
                                                                  const char *name, const char *path)
    {
        spDefoldAtlasAttachmentLoader *self = SUB_CAST(spDefoldAtlasAttachmentLoader, loader);
        switch (type) {
            case SP_ATTACHMENT_REGION: {
                spRegionAttachment* attachment;
                spAtlasRegion *region = FindAtlasRegion(self->atlas, self->regions, path);
                if (!region) {
                    _spAttachmentLoader_setError(loader, "Region not found: ", path);
                    return 0;
                }
                attachment = spRegionAttachment_create(name);
                attachment->rendererObject = region;
                spRegionAttachment_setUVs(attachment, region->u, region->v, region->u2, region->v2, region->degrees);
                attachment->regionOffsetX = region->offsetX;
                attachment->regionOffsetY = region->offsetY;
                attachment->regionWidth = region->width;
                attachment->regionHeight = region->height;
                attachment->regionOriginalWidth = region->originalWidth;
                attachment->regionOriginalHeight = region->originalHeight;
                return SUPER(attachment);
            }
            case SP_ATTACHMENT_MESH:
            case SP_ATTACHMENT_LINKED_MESH: {
                spMeshAttachment *attachment;
                spAtlasRegion *region = FindAtlasRegion(self->atlas, self->regions, path);
                if (!region) {
                    _spAttachmentLoader_setError(loader, "Region not found: ", path);
                    return 0;
                }
                attachment = spMeshAttachment_create(name);
                attachment->rendererObject = region;
                attachment->regionU = region->u;
                attachment->regionV = region->v;
                attachment->regionU2 = region->u2;
                attachment->regionV2 = region->v2;
                attachment->regionDegrees = region->degrees;
                attachment->regionOffsetX = region->offsetX;
                attachment->regionOffsetY = region->offsetY;
                attachment->regionWidth = region->width;
                attachment->regionHeight = region->height;
                attachment->regionOriginalWidth = region->originalWidth;
                attachment->regionOriginalHeight = region->originalHeight;
                return SUPER(SUPER(attachment));
            }
            case SP_ATTACHMENT_BOUNDING_BOX:
                return SUPER(SUPER(spBoundingBoxAttachment_create(name)));
            case SP_ATTACHMENT_PATH:
                return SUPER(SUPER(spPathAttachment_create(name)));
            case SP_ATTACHMENT_POINT:
                return SUPER(spPointAttachment_create(name));
            case SP_ATTACHMENT_CLIPPING:
                return SUPER(SUPER(spClippingAttachment_create(name)));
            default:
                _spAttachmentLoader_setUnknownTypeError(loader, type);
                return 0;
        }

        UNUSED(skin);
    }

    spDefoldAtlasAttachmentLoader* CreateAttachmentLoader(dmGameSystem::TextureSetResource* atlas, spAtlasRegion* regions)
    {
        spDefoldAtlasAttachmentLoader* self = NEW(spDefoldAtlasAttachmentLoader);
        _spAttachmentLoader_init(SUPER(self), _spAttachmentLoader_deinit, spDefoldAtlasAttachmentLoader_createAttachment, 0, 0);
        self->atlas = atlas;
        self->regions = regions;
        return self;
    }

    void Dispose(spDefoldAtlasAttachmentLoader* loader)
    {
        spAttachmentLoader_dispose((spAttachmentLoader*)loader);
    }

} // namespace
