#include "CustomCPPopup.hpp"
#include "managers/SessionManager.hpp"
#include "utils/Utils.hpp"

bool CustomCPPopup::setup(int levelID) {
    m_levelID = levelID;
    
    auto winSize = m_size;
    
    auto title = CCLabelBMFont::create("Dar CPs Personalizados", "goldFont.fnt");
    title->setPosition({winSize.width / 2, winSize.height / 2 + 60.f});
    title->setScale(0.8f);
    m_mainLayer->addChild(title);

    m_inputField = TextInput::create(150.f, "Cantidad", "chatFont.fnt");
    m_inputField->setPosition({winSize.width / 2, winSize.height / 2 + 10.f});
    m_mainLayer->addChild(m_inputField);

    auto submitBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Enviar", "goldFont.fnt", "goldBtn_01.png"),
        this,
        menu_selector(CustomCPPopup::onSend)
    );
    
    auto menu = CCMenu::create();
    menu->addChild(submitBtn);
    menu->setPosition({winSize.width / 2, winSize.height / 2 - 45.f});
    m_mainLayer->addChild(menu);

    return true;
}

void CustomCPPopup::onSend(CCObject*) {
    std::string text = m_inputField->getString();
    if (text.empty()) {
        FLAlertLayer::create("Error", "Por favor ingresa una cantidad de CPs.", "OK")->show();
        return;
    }

    // Validar que sean solo números
    for (char c : text) {
        if (!std::isdigit(c)) {
            FLAlertLayer::create("Error", "Por favor ingresa un número válido.", "OK")->show();
            return;
        }
    }

    int amount = std::stoi(text);
    
    // Usamos la misma lógica de URL que el resto del mod
    std::string serverURL = SessionManager::getServerURL() + "/addcp?levelId=" + std::to_string(m_levelID) + "&cp=" + std::to_string(amount);

    web::WebRequest()
        .header("Authorization", SessionManager::getPassword())
        .post(serverURL, Mod::get(), [this](web::WebResponse* response) {
            if (response->ok()) {
                FLAlertLayer::create("Éxito", "¡CPs aplicados al creador!", "OK")->show();
                this->onClose(nullptr);
            } else {
                FLAlertLayer::create("Error", "No se pudo conectar con el servidor.", "OK")->show();
            }
        });
}

CustomCPPopup* CustomCPPopup::create(int levelID) {
    auto ret = new CustomCPPopup();
    if (ret && ret->initAnchored(320.f, 200.f, levelID)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
