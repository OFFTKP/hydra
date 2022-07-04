#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <filesystem>
#include <iostream>
#include <fstream>
#include <include/emulator.h>
#include <lib/md5.h>
#include <GL/glew.h>
#include <include/error_factory.hxx>
#include <lib/str_hash.h>

namespace {
	std::ifstream::pos_type filesize(const char* filename) {
        std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
        return in.tellg(); 
    }
}

namespace TKPEmu {
    void Emulator::HandleKeyDown(SDL_Keycode keycode) { 
        std::cout << "Warning: Key " << SDL_GetKeyName(keycode) << " was pressed but\n"
        	"emulator.HandleKeyDown was not implemented" << std::endl;
    }
    void Emulator::HandleKeyUp(SDL_Keycode keycode) { 
        std::cout << "Warning: Key " << SDL_GetKeyName(keycode) << " was released but\n"
        	"emulator.HandleKeyUp was not implemented" << std::endl;
    }
    void Emulator::Screenshot(std::string filename, std::string directory) { 
		std::lock_guard<std::mutex> lg(DrawMutex);
    }
    void* Emulator::GetScreenData() { 
        throw ErrorFactory::generate_exception(__func__, __LINE__, "GetScreenData was not implemented for this emulator");
    }
    void Emulator::Start() { 
		start();
    }
	void Emulator::start() { 
		throw ErrorFactory::generate_exception(__func__, __LINE__, "start was not implemented for this emulator");
    }
	void Emulator::reset() { 
		throw ErrorFactory::generate_exception(__func__, __LINE__, "reset was not implemented for this emulator");
    }
	bool Emulator::load_file(std::string) { 
		throw ErrorFactory::generate_exception(__func__, __LINE__, "load_file was not implemented for this emulator");
    }
	bool Emulator::LoadFromFile(std::string path) {
		return load_file(path);
	}
    void Emulator::Reset() {
        reset();
    }
	void Emulator::CloseAndWait() {
		Step.store(true);
        Paused.store(false);
        Stopped.store(true);
		v_extra_close();
        Step.notify_all();
		std::lock_guard<std::mutex> lguard(ThreadStartedMutex);
	}
    bool Emulator::poll_request(const Request& request) {
        auto cur = str_hash(request.c_str());
        switch (cur) {
            case str_hash("pause"): {
                Paused.store(true);
                Step.store(true);
                Step.notify_all();
                Response response {
                    .Recipient = TKPMQ_RESPONSE_MAIN,
                    .Type = "paused",
                };
                MessageQueue->PushResponse(response);
                return true;
            }
            default: return poll_uncommon_request(request);
        }
    }
}