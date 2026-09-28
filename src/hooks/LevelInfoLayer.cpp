#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include "../DevRateStarsPopup.hpp"
#include "../managers/SessionManager.hpp"
#include "../utils/Utils.hpp"

// Declaramos tu popup personalizado por si está en otro archivo
class CustomCPPopup : public geode::prelude::Popup<int> {
public:
    static CustomCPPopup* create(int levelID);
};

using namespace geode::prelude;

class $modify(MyLevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel *level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge))
            return false;

        if (!Utils::isOnGdps())
            return true;

        if (auto *leftMenu = static_cast<CCMenu *>(this->getChildByID("left-side-menu"))) {
            // 1. Botón original de Dev Rate
            auto *devRateButton = CCMenuItemSpriteExtra::create(
                CCSprite::createWithSpriteFrameName("GJ_starBtnMod_001.png"),
                this,
                menu_selector(MyLevelInfoLayer::onDevRateButton)
            );
            devRateButton->setColor({ 74, 242, 250 });
            devRateButton->setID("dev-rate-button"_spr);

            leftMenu->addChild(devRateButton);

            // 2. Tu botón de CPs personalizados con el icono de creador oficial
            auto *customCPButton = CCMenuItemSpriteExtra::create(
                CircleButtonSprite::createWithSpriteName("GJ_creatorIcon_001.png", 1.0f, CircleBaseColor::Green),
                this,
                menu_selector(MyLevelInfoLayer::onCustomCPButton)
            );
            customCPButton->setID("custom-cp-button"_spr);

            leftMenu->addChild(customCPButton);

            // Actualizamos la posición de los botones en el menú lateral
            leftMenu->updateLayout();
        }
        
        return true;
    }

    void onDevRateButton(CCObject *) {
        if (!SessionManager::ShownRateButtonWarning) {
            geode::createQuickPopup(
                "Rate Button Warning",
                "This mod <co>only works</c> if you have <cy>permissions to rate levels</c> on a GDPS!\n"
                "Using it <co>without the mentioned permissions</c> will result in <cr>nothing happening</c> when you click Submit.\n\n"
                "<cy>Do you acknowledge this?</c>\n<co>This will not be shown again.</c>",
                "No", "Yes",
                [levelID = static_cast<int>(m_level->m_levelID)](auto, bool btn2) {
                    if (btn2) {
                        Mod::get()->setSavedValue("shown-rate-button-warning", true);
                        SessionManager::ShownRateButtonWarning = true;

                        DevRateStarsPopup::create(levelID)->show();
                    }
                }
            );
        } else {
            DevRateStarsPopup::create(m_level->m_levelID)->show();
        }
    }

    // La función que abre tu ventana de CPs personalizados
    void onCustomCPButton(CCObject *) {
        int levelID = static_cast<int>(m_level->m_levelID);
        CustomCPPopup::create(levelID)->show();
    }
};
