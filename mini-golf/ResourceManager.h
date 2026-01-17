#ifndef MINI_GOLF_RESOURCEMANAGER_H
#define MINI_GOLF_RESOURCEMANAGER_H

#include "TextureManager.h"
#include "FontManager.h"
#include "SoundManager.h"
#include "DEFINITIONS.h"

/**
 * @class ResourceManager
 * @brief Fasada zarządzająca ładowaniem wszystkich zasobów aplikacji.
 *
 * Klasa statyczna, która agreguje wywołania do poszczególnych menedżerów
 * (Texture, Font, Sound) w celu jednorazowego załadowania lub zwolnienia
 * wszystkich assetów gry.
 */
class ResourceManager {
public:
    ResourceManager() = delete;

    /**
     * @brief Ładuje wszystkie tekstury, czcionki i dźwięki zdefiniowane w DEFINITIONS.h.
     * Powinna być wywołana na początku funkcji main().
     */
    static void loadAllResources();

    /**
     * @brief Czyści pamięć wszystkich menedżerów zasobów.
     */
    static void cleanUpAllResources();
};

#endif //MINI_GOLF_RESOURCEMANAGER_H