#pragma once

#include "screenwidget.hxx"
#include "settings.hxx"
#include <array>
#include <deque>
#include <hydra/core.hxx>
#include <memory>
#define MA_NO_DECODING
#define MA_NO_ENCODING
#include <core_loader.hxx>
#include <miniaudio.h>
#include <QFileDialog>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <thread>
#include <unordered_map>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    // Initialization functions
    void create_actions();
    void create_menus();

    // Menu bar actions
    void open_file();
    void open_file_impl(const std::string& file);
    void open_settings();
    void open_about();
    void open_shaders();
    void open_scripts();
    void open_terminal();
    void run_script(const std::string& script, bool safe_mode);
    void screenshot();
    void add_recent(const std::string& path);

    // Emulation functions
    void pause_emulator();
    void reset_emulator();
    void init_emulator();
    void stop_emulator();
    void enable_emulation_actions(bool should);
    void initialize_audio();
    void set_volume(int volume);
    void update_recent_files();
    void update_fbo(unsigned fbo);

    static void video_callback(void* data, hydra::Size size);
    static void audio_callback(void* data, size_t frames);
    static int32_t read_input_callback(uint32_t player, hydra::ButtonType button);

private slots:
    void emulator_frame();
    void on_mouse_move(QMouseEvent* event);

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void OpenFile(const std::string& file);
    QMenu* file_menu_;
    QMenu* emulation_menu_;
    QMenu* tools_menu_;
    QMenu* help_menu_;
    QAction* open_act_;
    QAction* pause_act_;
    QAction* reset_act_;
    QAction* mute_act_;
    QAction* about_act_;
    QAction* stop_act_;
    QAction* close_act_;
    QAction* settings_act_;
    QAction* open_settings_file_act_;
    QAction* screenshot_act_;
    QAction* shaders_act_;
    QAction* scripts_act_;
    QAction* terminal_act_;
    QAction* recent_act_;
    QTimer* emulator_timer_;
    ScreenWidget* screen_;
    ma_device sound_device_{};
    bool frontend_driven_ = false;
    std::unique_ptr<hydra::EmulatorWrapper> emulator_;
    std::unique_ptr<EmulatorInfo> info_;
    std::vector<int16_t> queued_audio_;
    bool settings_open_ = false;
    bool about_open_ = false;
    bool shaders_open_ = false;
    bool scripts_open_ = false;
    bool terminal_open_ = false;
    bool paused_ = false;
    std::mutex emulator_mutex_;
    std::mutex audio_mutex_;

    std::vector<uint8_t> video_buffer_;
    uint32_t video_width_ = 0;
    uint32_t video_height_ = 0;
    std::chrono::time_point<std::chrono::high_resolution_clock> last_emulation_second_time_;
    int frame_count_ = 0;
    int sleep_time_ = 0;

    // std::unordered_map<int, hc_input_e> current_mappings_{};
    // std::array<int8_t, hc_input_e::HC_INPUT_SIZE> input_state_{};
    std::deque<std::string> recent_files_;

    friend void hungry_for_more(ma_device*, void*, const void*, ma_uint32);
};
