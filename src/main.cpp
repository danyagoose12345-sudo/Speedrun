#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class SpeedrunMenuPopup : public geode::Popup<GJGameLevel*> {
protected:
    GJGameLevel* m_level;
    inline static int s_selectedMinutes = 1;

    bool setup(GJGameLevel* level) override {
        m_level = level;
        this->setTitle("Speedrun Menu");

        auto mainRow = CCMenu::create();
        mainRow->setLayout(
            AxisLayout::create()
                ->setAxis(Axis::Row)
                ->setGap(30.f)
                ->setAutoScale(false)
        );
        mainRow->setContentSize({360.f, 200.f});
        mainRow->setPosition(m_mainLayer->getContentSize() / 2 + CCPoint{0.f, -20.f});

        // ================= "TIME" COLUMN =================
        auto timeLeftColumn = CCMenu::create();
        timeLeftColumn->setLayout(
            AxisLayout::create()
                ->setAxis(Axis::Column)
                ->setGrowCrossAxis(true)
                ->setCrossAxisOverflow(false)
                ->setGap(5.f)
        );
        timeLeftColumn->setContentSize({180.f, 180.f});

        auto timeLabel = CCLabelBMFont::create("Time", "goldFont.fnt");
        timeLabel->setScale(0.7f);
        timeLeftColumn->addChild(timeLabel);

        std::vector<std::pair<std::string, int>> timeOptions = {
            {"1 min", 1}, {"2 min", 2}, {"3 min", 3}, {"4 min", 4}, {"5 min", 5},
            {"6 min", 6}, {"7 min", 7}, {"8 min", 8}, {"9 min", 9}, {"10 min", 10},
            {"1h", 60}, {"2h", 120}, {"3h", 180}, {"4h", 240}, {"5h", 300},
            {"6h", 360}, {"7h", 420}, {"8h", 480}
        };

        for (const auto& option : timeOptions) {
            const char* btnSpriteName = (s_selectedMinutes == option.second) ? "GJ_button_02.png" : "GJ_button_01.png";
            auto btnSpr = ButtonSprite::create(option.first.c_str(), "goldFont.fnt", btnSpriteName, .4f);
            btnSpr->setScale(0.7f);

            auto btn = CCMenuItemSpriteExtra::create(
                btnSpr,
                this,
                menu_selector(SpeedrunMenuPopup::onSelectTime)
            );
            btn->setTag(option.second);
            timeLeftColumn->addChild(btn);
        }
        timeLeftColumn->updateLayout();
        mainRow->addChild(timeLeftColumn);

        // ================= "GAME" COLUMN =================
        auto gameRightColumn = CCMenu::create();
        gameRightColumn->setLayout(
            AxisLayout::create()
                ->setAxis(Axis::Column)
                ->setGap(15.f)
        );
        gameRightColumn->setContentSize({120.f, 180.f});

        auto gameLabel = CCLabelBMFont::create("Game", "goldFont.fnt");
        gameLabel->setScale(0.7f);
        gameRightColumn->addChild(gameLabel);

        auto playSpr = ButtonSprite::create("Play", "goldFont.fnt", "GJ_button_01.png", .7f);
        auto playBtn = CCMenuItemSpriteExtra::create(
            playSpr,
            this,
            menu_selector(SpeedrunMenuPopup::onPlaySpeedrun)
        );
        gameRightColumn->addChild(playBtn);

        auto contSpr = ButtonSprite::create("Continue", "goldFont.fnt", "GJ_button_01.png", .7f);
        auto contBtn = CCMenuItemSpriteExtra::create(
            contSpr,
            this,
            menu_selector(SpeedrunMenuPopup::onContinueSpeedrun)
        );
        gameRightColumn->addChild(contBtn);

        gameRightColumn->updateLayout();
        mainRow->addChild(gameRightColumn);

        m_mainLayer->addChild(mainRow);
        mainRow->updateLayout();

        return true;
    }

    void onSelectTime(CCObject* sender) {
        s_selectedMinutes = sender->getTag();
        this->onClose(sender);
        SpeedrunMenuPopup::create(m_level)->show();
    }

    void onPlaySpeedrun(CCObject* sender) {
        std::string msg = "Starting a NEW speedrun for " + std::to_string(s_selectedMinutes) + " min!";
        FLAlertLayer::create("New Game", msg.c_str(), "Let's Go") -> show();
        this->onClose(sender);
    }

    void onContinueSpeedrun(CCObject* sender) {
        std::string msg = "Continuing current session (" + std::to_string(s_selectedMinutes) + " min total).";
        FLAlertLayer::create("Continue", msg.c_str(), "Fight!") -> show();
        this->onClose(sender);
    }

public:
    static SpeedrunMenuPopup* create(GJGameLevel* level) {
        auto ret = new SpeedrunMenuPopup();
        if (ret && ret->initAnchored(420.f, 260.f, level)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

class $modify(MyLevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;

        auto buttonMenu = this->getChildByID("left-side-menu");
        if (!buttonMenu) buttonMenu = m_fields->m_playBtnMenu; 

        auto buttonSprite = CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png");
        if (!buttonSprite) {
            buttonSprite = ButtonSprite::create("SR", "goldFont.fnt", "GJ_button_01.png", .8f);
        }

        auto speedrunBtn = CCMenuItemSpriteExtra::create(
            buttonSprite,
            this,
            menu_selector(MyLevelInfoLayer::onSpeedrun)
        );

        if (buttonMenu) {
            buttonMenu->addChild(speedrunBtn);
            buttonMenu->updateLayout();
        }

        return true;
    }

    void onSpeedrun(CCObject* sender) {
        SpeedrunMenuPopup::create(m_level)->show();
    }
};
