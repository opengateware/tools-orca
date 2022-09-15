################################################################################
# SPDX-License-Identifier: CC0-1.0
# SPDX-FileType: OTHER
# SPDX-FileCopyrightText: (c) 2022, OpenGateware authors and contributors
################################################################################
# Build Options
################################################################################
SHELL         = /bin/bash -o pipefail
COPYRIGHT     = (c) 2022, OpenGateware authors and contributors
BUG_REPORT    = sig-tools@lists.opengateware.org
################################################################################
# Base Directories
################################################################################
TOP_DIR         = .
SRC_DIR         = $(TOP_DIR)/src
LINUX_BUILD_DIR = $(TOP_DIR)/cmake-release-linux
MACOS_BUILD_DIR = $(TOP_DIR)/cmake-release-macos
MSYS_BUILD_DIR  = $(TOP_DIR)/cmake-release-msys
MINGW_BUILD_DIR = $(TOP_DIR)/cmake-release-mingw
MAKEFLAGS      += "-j $(shell nproc)"
################################################################################
## Makefile Options
################################################################################
.PHONY: default help mingw msys2 macos linux install clean release-linux release-macos release-windows release-mingw

default: help

linux:
	@cmake -G "Unix Makefiles" -B $(LINUX_BUILD_DIR) -DCMAKE_BUILD_TYPE=Release
	@cmake --build $(LINUX_BUILD_DIR) -- $(MAKEFLAGS)

macos:
	@cmake -G "Unix Makefiles" -B $(MACOS_BUILD_DIR) -DCMAKE_BUILD_TYPE=Release
	@cmake --build $(MACOS_BUILD_DIR) -- $(MAKEFLAGS)

msys2:
	@cmake -G "MinGW Makefiles" -B $(MSYS_BUILD_DIR) -DCMAKE_BUILD_TYPE=Release
	@cmake --build $(MSYS_BUILD_DIR) -- $(MAKEFLAGS)

mingw:
	@cmake -DCMAKE_TOOLCHAIN_FILE=./pkg/mingw-w64-x86_64.cmake -B $(MINGW_BUILD_DIR) -DCMAKE_BUILD_TYPE=Release
	@cmake --build $(MINGW_BUILD_DIR) -- $(MAKEFLAGS)

release-linux: clean-linux linux
	@cd $(LINUX_BUILD_DIR) && cpack
	@echo "Release Package for Linux Created"

release-macos: clean-linux macos
	@cd $(MACOS_BUILD_DIR) && cpack
	@echo "Release Package for macOS Created"

release-msys2: clean-msys msys2
	@cd $(MSYS_BUILD_DIR) && cpack
	@echo "Release Package for MSYS Created"

release-mingw: clean-mingw mingw
	@cd $(MINGW_BUILD_DIR) && cpack
	@echo "Release Package for MinGW Created"

install-linux: release-linux
	@sudo dpkg --install $(LINUX_BUILD_DIR)/*.deb

clean-linux:
	@echo -e "\033[0;33mCleaning Linux build...\033[0m"
	@rm -rf $(LINUX_BUILD_DIR)

clean-macos:
	@echo -e "\033[0;33mCleaning macOS build...\033[0m"
	@rm -rf $(MACOS_BUILD_DIR)

clean-msys:
	@echo -e "\033[0;33mCleaning MSYS build...\033[0m"
	@rm -rf $(MSYS_BUILD_DIR)

clean-mingw:
	@echo -e "\033[0;33mCleaning MinGW build...\033[0m"
	@rm -rf $(MINGW_BUILD_DIR)

clean-all:
	@echo -e "\033[0;33mCleaning all builds...\033[0m"
	@rm -rf $(LINUX_BUILD_DIR)
	@rm -rf $(MSYS_BUILD_DIR)
	@rm -rf $(MINGW_BUILD_DIR)
	@rm -rf $(MACOS_BUILD_DIR)

help:
	@echo "════════════════════════════════════════════════════════════════════════════════"
	@echo " OpenGateware.org - ORCA Help Menu "
	@echo "════════════════════════════════════════════════════════════════════════════════"
	@echo "1. Build:                                                                       "
	@echo "   1. linux             - build for debian/ubuntu distro                        "
	@echo "   2. msys2             - build for windows using MSYS2                         "
	@echo "   3. mingw             - build for windows using linux using mingw             "
	@echo "   4. macos             - build for macOS using                                 "
	@echo ""
	@echo "2. Release:                                                                     "
	@echo "   1. release-linux     - create and pack a release for linux (.deb/.zip/.tar.gz)"
	@echo "   2. release-msys2     - create and pack a release for windows [msys2] (.zip)  "
	@echo "   3. release-mingw     - create and pack a release for windows [mingw] (.zip)  "
	@echo "   4. release-macos     - create and pack a release for macOS (.zip/tar.gz)     "
	@echo ""
	@echo "3. Clean:                                                                       "
	@echo "   1. clean-linux       - clean linux build                                     "
	@echo "   2. clean-msys2       - clean msys2 build                                     "
	@echo "   3. clean-mingw       - clean mingw build                                     "
	@echo "   4. clean-macos       - clean macos build                                     "
	@echo "   5. clean-all         - clean all builds                                      "
