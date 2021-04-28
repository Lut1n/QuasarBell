#ifndef IO_FILE_EXPLORER_H
#define IO_FILE_EXPLORER_H

#include <filesystem>
#include <vector>

struct FileExplorer
{
    using Path = std::filesystem::path;

    struct FileInfo
    {
        Path path;
        std::string name;
        std::string date;
        size_t size = 0;
        bool isDir = false;
        bool isRegular = false;
        std::string ext;
    };

    FileExplorer();

    void setCurrentPath(const std::string& path);

    std::string buildPath(const std::string& filename, const std::string& ext) const;

    bool goToParentDir();
    bool goToChildDir(size_t index);

    std::string getParent();
    std::string getCurrent();
    FileInfo getItem(size_t index) const;
    size_t getItemCount() const;

private:

    void updateItems();
    
    Path currentPath;
    std::vector<FileInfo> items;
};

#endif // IO_FILE_EXPLORER_H
