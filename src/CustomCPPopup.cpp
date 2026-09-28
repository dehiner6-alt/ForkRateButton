#include "CustomCPPopup.hpp"
#include "utils/Utils.hpp"

bool CustomCPPopup::init(int levelID) {
    if (!FLAlertLayer::init(150))
        return false;

    m_levelID = levelID;
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    
    auto background = CCScale9Sprite::create("GJ_square01.png", {0, 0, 80, 80}, {10, 10, 60, 60});
    background->setContentSize({320.f, 200.f});
    background->setPosition(winSize.width / 2, winSize.height / 2);
    m_mainLayer->addChild(background);

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

    for (char c : text) {
        if (!std::isdigit(c)) {
            FLAlertLayer::create("Error", "Por favor ingresa un número válido.", "OK")->show();
            return;
        }
    }

    int amount = std::stoi(text);
    
    std::string serverURL = Mod::get()->getSettingValue<std::string>("server") + "/addcp?levelId=" + std::to_string(m_levelID) + "&cp=" + std::to_string(amount);
    std::string password = Mod::get()->getSettingValue<std::string>("password");

    // Quitamos Mod::get() de aquí dentro, ya que solo pide (url, callback)
    web::WebRequest()
        .header("Authorization", password)
        .post(serverURL, [this](web::WebResponse* response) {
            if (response->ok()) {
                FLAlertLayer::create("Éxito", "¡CPs aplicados al creador!", "OK")->show();
                this->removeFromParentAndCleanup(true);
            } else {
                FLAlertLayer::create("Error", "No se pudo conectar con el servidor.", "OK")->show();
            }
        });
}

CustomCPPopup* CustomCPPopup::create(int levelID) {
    auto ret = new CustomCPPopup();
    if (ret && ret->init(levelID)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
