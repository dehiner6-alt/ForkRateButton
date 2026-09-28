#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class CustomCPPopup : public FLAlertLayer {
protected:
    int m_levelID;
    TextInput* m_inputField;

    bool init(int levelID);
    void onSend(CCObject*);

public:
    static CustomCPPopup* create(int levelID);
};
