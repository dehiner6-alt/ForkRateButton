#include <Geode/Geode.hpp>
#include <Geode/ui/TextInput.hpp>

using namespace geode::prelude;

class CustomCPPopup : public Popup<int> {
protected:
    TextInput* m_inputField;
    int m_levelID;

    bool setup(int levelID) override {
        m_levelID = levelID;
        this->setTitle("Dar CPs Personalizados");

        // Campo de texto para los CPs
        m_inputField = TextInput::create(150.f, "Cantidad", "chatFont.fnt");
        m_inputField->setFilter("0123456789-");
        m_inputField->setPosition(m_size / 2);
        m_mainLayer->addChild(m_inputField);

        // Botón de enviar
        auto submitBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Enviar", "goldBtn_01.png", 0.8f),
            this, menu_selector(CustomCPPopup::onSubmit)
        );
        submitBtn->setPosition({m_size.width / 2, 45.f});

        auto menu = CCMenu::create();
        menu->addChild(submitBtn);
        menu->setPosition({0, 0});
        m_mainLayer->addChild(menu);

        return true;
    }

    void onSubmit(CCObject*) {
        std::string cpValue = m_inputField->getString();
        if (cpValue.empty()) return;

        std::string serverURL = "https://choyhomero.ps.fhgdps.com/database/setCustomCP.php";
        std::string payload = fmt::format("levelID={}&cp={}", m_levelID, cpValue);

        // Capturamos una referencia segura para cerrar la ventana después
        auto weakSelf = Ref(this);

        web::WebRequest()
            .body(payload)
            .post(serverURL, [weakSelf, cpValue](web::WebResponse* response) {
                if (response->ok()) {
                    FLAlertLayer::create("Éxito", "¡CPs aplicados al creador!", "OK")->show();
                    if (weakSelf) {
                        weakSelf->removeFromParent();
                    }
                } else {
                    FLAlertLayer::create("Error", "No se pudo conectar con el servidor.", "OK")->show();
                }
            });
    }

public:
    static CustomCPPopup* create(int levelID) {
        auto ret = new CustomCPPopup();
        if (ret && ret->initAnchored(320.f, 200.f, levelID)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};
