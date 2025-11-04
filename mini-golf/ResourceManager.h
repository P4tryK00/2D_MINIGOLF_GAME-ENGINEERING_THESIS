//
// Created by patry on 04.11.2025.
//

#ifndef MINI_GOLF_RESOURCEMANAGER_H
#define MINI_GOLF_RESOURCEMANAGER_H

#include "TextureManager.h"
#include "FontManager.h"
#include "SoundManager.h"
#include "DEFINITIONS.h"

class ResourceManager {
    public:
    ResourceManager() = delete;
    static void loadAllResources();
    static void cleanUpAllResources();
};


#endif //MINI_GOLF_RESOURCEMANAGER_H