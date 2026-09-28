#include "CustomCPPopup.hpp"
#include "utils/Utils.hpp"

CustomCPPopup* CustomCPPopup::create(int levelID) {
    auto ret = new CustomCPPopup();
    // initAnchored crea la ventana con el tamaño exacto (ancho, alto)
    if (ret && ret->initAnchored(350.f, 200.f, levelID)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool CustomCPPopup::setup(int levelID) {
    m_levelID = levelID;
    
    // Popup ya crea el fondo y la capa principal automáticamente
    setTitle("Dar CPs Personalizados");

    m_inputField = TextInput::create(150.f, "Cantidad", "chatFont.fnt");
    m_inputField->setPosition({m_size.width / 2, m_size.height / 2 + 10.f});
    m_mainLayer->addChild(m_inputField);

    auto submitBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Enviar", "goldFont.fnt", "goldBtn_01.png"),
        this,
        menu_selector(CustomCPPopup::onSend)
    );
    
    auto menu = CCMenu::create();
    menu->addChild(submitBtn);
    menu->setPosition({m_size.width / 2, m_size.height / 2 - 45.f});
    m_mainLayer->addChild(menu);

    return true;
}

void CustomCPPopup::onSend(CCObject*) {
    std::string text = m_inputField->getString();
    if (text.empty()) {
        FLAlertLayer::create("Error", "Por favor ingresa una cantidad de CPs.", "OK")->show();
        return;
    }

    for (char c : text) {
        if (!std::isdigit(c)) {
            FLAlertLayer::create("Error", "Por favor ingresa un número válido.", "OK")->show();
            return;
        }
    }

    int amount = std::stoi(text);
    
    std::string serverURL = Mod::get()->getSettingValue<std::string>("server") + "/addcp?levelId=" + std::to_string(m_levelID) + "&cp=" + std::to_string(amount);
    std::string password = Mod::get()->getSettingValue<std::string>("password");

    auto req = web::WebRequest();
    req.header("Authorization", password);

    m_listener.bind([this](Task<web::WebResponse>::Event* event) {
        if (event->isFinished()) {
            if (auto response = event->getValue()) {
                if (response->ok()) {
                    FLAlertLayer::create("Éxito", "¡CPs aplicados al creador!", "OK")->show();
                    this->onClose(nullptr); // Cierra el Popup de forma limpia
                } else {
                    FLAlertLayer::create("Error", "No se pudo conectar con el servidor.", "OK")->show();
                }
            } else {
                FLAlertLayer::create("Error", "Error de red con el servidor.", "OK")->show();
            }
        }
    });

    m_listener.setFilter(req.post(serverURL));
}
