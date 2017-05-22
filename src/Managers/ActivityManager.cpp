#include "ActivityManager.h"

#include <Activity.h>

ActivityManager::ActivityManager(WindowPtr window, TextureManagerPtr textureManager) :
        window_(window),
        active_(nullptr),
        textureManager_(textureManager)
{

}

void ActivityManager::Poll() {
    sf::Event sfmlEvent;
    while (window_->PollEvent(sfmlEvent)) {
        Event event = Event(sfmlEvent);
        if (active_ != nullptr) {
            active_->EventAction(event);
            UIPtr ui = active_->GetUI();
            if (ui != nullptr) {
               ui->EventAction(event.GetSFMLEvent());
            }
        }
    }
}

void ActivityManager::AddActivity(ActivityPtr activity) {
    activities_[activity->GetName()] = activity;
    activity->Prepare(shared_from_this());
}

void ActivityManager::RemoveActivity(std::string name) {
    std::map<std::string, ActivityPtr>::iterator it = activities_.find(name);
    if (it == activities_.end()) {
        std::cerr << "There's no such activity!" << std::endl;
        exit(1);
    }
    if (active_ != nullptr && active_->GetName() == it->second->GetName()) {
        active_ = nullptr;
    }
    it->second->End();
    activities_.erase(it);
}

void ActivityManager::Manage() {
    if (active_ != nullptr) {
        switch (active_->GetState()) {
            case ACTIVITY_STATE_INIT:
                active_->Init();
                active_->SetReady();
                break;

            case ACTIVITY_STATE_RUN:
                active_->Run();
                break;

            case ACTIVITY_STATE_END:
                active_->End();
                break;
        }
    }
}

bool ActivityManager::IsRunning() const {
    return active_ != nullptr;
}

void ActivityManager::SetAsActive(std::string name) {
    std::map<std::string, ActivityPtr>::iterator it = activities_.find(name);
    if (it == activities_.end()) {
        std::cerr << "There's no such activity!" << std::endl;
        exit(1);
    }
    active_ = it->second;
}

void ActivityManager::End() {
    /* Eventualy call End() on each activity. */
    active_ = nullptr;
}

RendererPtr ActivityManager::GetRenderer() const {
    return window_->GetRenderer();
}

TextureManagerPtr ActivityManager::GetTextureManager() const {
    return textureManager_;
}
