/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <sys/types.h>

#include <cstdint>
#include <string>

#include <processgroup/cgroup_controller.h>
#include <processgroup/util.h>

// Convenient wrapper of a CgroupController pointer.
class CgroupControllerWrapper {
  public:
    // Does not own controller
    explicit CgroupControllerWrapper(const CgroupController* controller)
        : controller_(controller) {}

    uint32_t version() const;
    const char* name() const;
    const char* path() const;

    bool HasValue() const;
    bool IsUsable();

    std::string GetTasksFilePath(const std::string& path) const;
    std::string GetProcsFilePath(const std::string& path, uid_t uid, pid_t pid) const;
    bool GetTaskGroup(pid_t tid, std::string* group) const;

  private:
    enum ControllerState {
        UNKNOWN = 0,
        USABLE = 1,
        MISSING = 2,
    };

    const CgroupController* controller_ = nullptr; // CgroupMap owns the object behind this pointer
    ControllerState state_ = ControllerState::UNKNOWN;
};

class CgroupMap {
  public:
    static CgroupMap& GetInstance();
    CgroupControllerWrapper FindController(const std::string& name) const;
    CgroupControllerWrapper FindControllerByPath(const std::string& path) const;
    bool ActivateControllers(const std::string& path) const;

  private:
    bool loaded_ = false;
    CgroupDescriptorMap descriptors_;
    CgroupMap();
    bool LoadDescriptors();
    void Print() const;
};
