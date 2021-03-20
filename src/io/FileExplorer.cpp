#include "io/FileExplorer.hpp"

namespace fs = std::filesystem;

namespace
{
    std::string getFilename(const fs::path& path)
    {    
        auto path_string = std::string(path.u8string());
        const size_t last_slash_idx = path_string.find_last_of("\\/");
        if (std::string::npos != last_slash_idx)
            path_string.erase(0, last_slash_idx + 1);
        return path_string;
    }
    std::string getExtension(const fs::path& path)
    {    
        auto ext = getFilename(path);
        const size_t last_point_idx = ext.find_last_of(".");
        if (std::string::npos != last_point_idx)
            ext.erase(0, last_point_idx);
        return ext;
    }
};

FileExplorer::FileExplorer()
{
    currentPath = fs::current_path();
    updateItems();
}

void FileExplorer::setCurrentPath(const std::string& path)
{
    currentPath = path;
    if(!fs::is_directory(currentPath))
        currentPath = currentPath.parent_path();

    if(!fs::exists(currentPath))
        currentPath = fs::current_path();

    updateItems();
}

std::string FileExplorer::buildPath(const std::string& filename, const std::string& ext) const
{
    auto path = currentPath / filename;
    path.replace_extension(ext);
    return path.u8string();
}

bool FileExplorer::goToParentDir()
{
    if(currentPath.has_parent_path())
    {
        currentPath = currentPath.parent_path();
        updateItems();
        return true;
    }
    return false;
}

bool FileExplorer::goToChildDir(size_t index)
{
    if(index < items.size() && items[index].isDir)
    {
        currentPath = items[index].path;
        updateItems();
        return true;
    }
    return false;
}

std::string FileExplorer::getParent()
{
    return getFilename(currentPath.parent_path());
}

std::string FileExplorer::getCurrent()
{
    return getFilename(currentPath);
}

FileExplorer::FileInfo FileExplorer::getItem(size_t index) const
{
    if(index < items.size())
    {
        return items[index];
    }
    return FileInfo();
}

size_t FileExplorer::getItemCount() const
{
    return items.size();
}

void FileExplorer::updateItems()
{
    items.clear();
    for (const auto& entry : fs::directory_iterator(currentPath))
    {
        FileInfo info;
        info.path = entry.path();
        info.name = getFilename(entry.path());
        info.date = "unknow";
        info.isDir = fs::is_directory(entry.status());
        info.isRegular = fs::is_regular_file(entry.status());
        info.ext = getExtension(entry.path());
        info.size = 0;
        {
		    auto err = std::error_code{};
            auto filesize = fs::file_size(entry.path(), err);
            if (filesize != static_cast<uintmax_t>(-1)) info.size = filesize;
        }
        items.push_back(info);
    }
}