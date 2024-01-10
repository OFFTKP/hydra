#pragma once

#include <cstdint>
#include <filesystem>
#include <log.hxx>
#include <span>
#include <sstream>
#include <string>
#include <vector>

#ifdef HYDRA_WEB
#include <emscripten.h>
#endif

static void sync_fs()
{
#ifdef HYDRA_WEB
    EM_ASM(
        FS.syncfs(false, function (err) {
            if (err) {
                console.log("Failed to sync FS: " + err);
            }
        });
    );
#endif
}

namespace hydra
{
    inline std::vector<std::string> split(const std::string& s, char delimiter)
    {
        std::vector<std::string> splits;
        std::string split;
        std::istringstream ss(s);
        while (std::getline(ss, split, delimiter))
        {
            splits.push_back(split);
        }
        return splits;
    }

    inline std::vector<uint8_t> hex_to_bytes(const std::string& cheat)
    {
        std::vector<uint8_t> bytes;
        for (size_t i = 0; i < cheat.size(); i += 2)
        {
            std::string hex = cheat.substr(i, 2);
            bytes.push_back((uint8_t)std::stoul(hex, nullptr, 16));
        }
        return bytes;
    }

    // Function for hashing a string in compile time in order to be used in a switch statement
    // https://stackoverflow.com/a/46711735
    // If there's a collision between two strings, we will know
    // at compile time since the cases can't use the same number twice
    constexpr uint32_t str_hash(std::string_view data) noexcept
    {
        uint32_t hash = 5381;
        const size_t size = data.size();
        for (size_t i = 0; i < size; i++)
            hash = ((hash << 5) + hash) + (unsigned char)data[i];

        return hash;
    }

    inline std::string fread(const std::filesystem::path& path)
    {
        FILE* file = std::fopen(path.string().c_str(), "rb");
        if (!file)
        {
            file = std::fopen(path.string().c_str(), "wb");
            if (!file)
            {
                hydra::log("Failed to open file: %s\n", path.string().c_str());
                return "";
            }
            std::fclose(file);
            sync_fs();
            return "";
        }

        std::string contents;
        std::fseek(file, 0, SEEK_END);
        contents.resize(std::ftell(file));
        std::rewind(file);
        std::fread(&contents[0], 1, contents.size(), file);
        std::fclose(file);
        return contents;
    }

    inline void fwrite(const std::filesystem::path& path, std::span<const uint8_t> contents)
    {
        FILE* file = std::fopen(path.string().c_str(), "wb");
        if (!file)
        {
            hydra::log("Failed to open file: %s\n", path.string().c_str());
            return;
        }
        std::fwrite(contents.data(), 1, contents.size(), file);
        std::fclose(file);
        sync_fs();
    }

    inline std::string file_picker()
    {
        return "";
    }

} // namespace hydra
