#pragma once
#include <Windows.h>
#include <filesystem>

std::filesystem::path getExecutableDir()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	return std::filesystem::path(buffer).parent_path();
}

std::string makeSourcePath(const std::string& relativePath) {
	return (getExecutableDir() / relativePath).string();
}