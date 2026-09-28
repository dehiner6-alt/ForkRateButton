#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class CustomCPPopup : public Popup<int> {
protected:
    int m_levelID;
    TextInput* m_inputField;
    EventListener<Task<web::WebResponse>> m_listener;

    bool setup(int levelID) override;
    void onSend(CCObject*);

public:
    static CustomCPPopup* create(int levelID);
};
