#pragma once
#include <Windows.h>
#include <TlHelp32.h>


#include <cstdint>
#include <memory>
#include <optional>
#include <string_view>

class Memory {
 private:
  DWORD processId = 0;
  HANDLE processHandle = nullptr;

  std::optional<DWORD> FindProcessId(
      const std::string_view processName) const noexcept {
    PROCESSENTRY32 entry = {};
    entry.dwSize = sizeof(PROCESSENTRY32);

    const HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snapshot == INVALID_HANDLE_VALUE) {
      return std::nullopt;
    }

    std::optional<DWORD> result = std::nullopt;

    if (Process32First(snapshot, &entry)) {
      do {
        if (processName == entry.szExeFile) {
          result = entry.th32ProcessID;
          break;
        }
      } while (Process32Next(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return result;
  }

 public:
  Memory(const std::string_view processName) noexcept {
    processId = *FindProcessId(processName);
    processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
  }

  ~Memory() {
    if (processHandle) {
      CloseHandle(processHandle);
    }
  }

  const std::uintptr_t GetModuleAddress(
      const std::string_view moduleName) const noexcept {
    MODULEENTRY32 entry = {};
    entry.dwSize = sizeof(MODULEENTRY32);

    const HANDLE snapshot = CreateToolhelp32Snapshot(
        TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);

    if (snapshot == INVALID_HANDLE_VALUE) {
      return 0;
    }

    std::uintptr_t result = 0;

    if (Module32First(snapshot, &entry)) {
      do {
        if (moduleName == entry.szModule) {
          result = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
          break;
        }
      } while (Module32Next(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return result;
  }

  template <typename T>
  const T Read(const std::uintptr_t address) const noexcept {
    T value = {};
    SIZE_T bytesRead = 0;
    ReadProcessMemory(processHandle, reinterpret_cast<const void*>(address),
                      &value, sizeof(T), &bytesRead);
    return value;
  }

  template <typename T>
  void Write(const std::uintptr_t address, const T& value) const noexcept {
    SIZE_T bytesWritten = 0;
    WriteProcessMemory(processHandle, reinterpret_cast<void*>(address), &value,
                       sizeof(T), &bytesWritten);
  }

  std::string ReadString(const std::uintptr_t address,
                         const std::size_t size) const {
    std::string value(size, '\0');
    SIZE_T bytesRead = 0;
    ReadProcessMemory(processHandle, reinterpret_cast<const void*>(address),
                      &value[0], size, &bytesRead);
    value.resize(bytesRead);
    return value;
  }

  template <typename Function>
  Function call_vfunc(PVOID Base, DWORD Index) {
    PDWORD* VTablePointer = (PDWORD*)Base;
    PDWORD VTableFunctionBase = *VTablePointer;
    DWORD dwAddress = VTableFunctionBase[Index];
    return (Function)(dwAddress);
  }

  HANDLE GetHandle() const noexcept { return processHandle; }

  Memory() {}
};

inline Memory mem{"csgo.exe"};