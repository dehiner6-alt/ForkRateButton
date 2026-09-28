#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class CustomCPPopup : public Popup<int> {
protected:
    int m_levelID;
    TextInput* m_inputField;

    bool setup(int levelID) override;
    void onSend(CCObject*);

public:
    static CustomCPPopup* create(int levelID);
};
