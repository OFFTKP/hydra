#pragma once

#include "hydra/core.hxx"
#include <filesystem>
#include <memory>
#include <QWidget>

struct CheatMetadata
{
    bool enabled = true;
    std::string name{};
    std::string description{};
    std::string code{};
    uint32_t handle = hydra::BAD_CHEAT;
};

namespace hydra
{
    class EmulatorWrapper;
}

class CheatsWindow : public QWidget
{
    Q_OBJECT

public:
    CheatsWindow(std::shared_ptr<hydra::EmulatorWrapper> wrapper, bool& open,
                 const std::string& hash, QWidget* parent = nullptr);
    ~CheatsWindow();

private:
    bool& open_;
    std::vector<CheatMetadata> cheats_;
    std::shared_ptr<hydra::EmulatorWrapper> wrapper_;
    std::filesystem::path cheat_path_;
};
