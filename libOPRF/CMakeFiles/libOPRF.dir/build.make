# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/i/npsi_my

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/i/npsi_my

# Include any dependencies generated for this target.
include libOPRF/CMakeFiles/libOPRF.dir/depend.make

# Include the progress variables for this target.
include libOPRF/CMakeFiles/libOPRF.dir/progress.make

# Include the compile flags for this target's objects.
include libOPRF/CMakeFiles/libOPRF.dir/flags.make

libOPRF/CMakeFiles/libOPRF.dir/Hashing/BaseOPRFs.cpp.o: libOPRF/CMakeFiles/libOPRF.dir/flags.make
libOPRF/CMakeFiles/libOPRF.dir/Hashing/BaseOPRFs.cpp.o: libOPRF/Hashing/BaseOPRFs.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/i/npsi_my/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object libOPRF/CMakeFiles/libOPRF.dir/Hashing/BaseOPRFs.cpp.o"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libOPRF.dir/Hashing/BaseOPRFs.cpp.o -c /mnt/i/npsi_my/libOPRF/Hashing/BaseOPRFs.cpp

libOPRF/CMakeFiles/libOPRF.dir/Hashing/BaseOPRFs.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libOPRF.dir/Hashing/BaseOPRFs.cpp.i"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/i/npsi_my/libOPRF/Hashing/BaseOPRFs.cpp > CMakeFiles/libOPRF.dir/Hashing/BaseOPRFs.cpp.i

libOPRF/CMakeFiles/libOPRF.dir/Hashing/BaseOPRFs.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libOPRF.dir/Hashing/BaseOPRFs.cpp.s"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/i/npsi_my/libOPRF/Hashing/BaseOPRFs.cpp -o CMakeFiles/libOPRF.dir/Hashing/BaseOPRFs.cpp.s

libOPRF/CMakeFiles/libOPRF.dir/Hashing/BaseOPRFs.cpp.o.requires:

.PHONY : libOPRF/CMakeFiles/libOPRF.dir/Hashing/BaseOPRFs.cpp.o.requires

libOPRF/CMakeFiles/libOPRF.dir/Hashing/BaseOPRFs.cpp.o.provides: libOPRF/CMakeFiles/libOPRF.dir/Hashing/BaseOPRFs.cpp.o.requires
	$(MAKE) -f libOPRF/CMakeFiles/libOPRF.dir/build.make libOPRF/CMakeFiles/libOPRF.dir/Hashing/BaseOPRFs.cpp.o.provides.build
.PHONY : libOPRF/CMakeFiles/libOPRF.dir/Hashing/BaseOPRFs.cpp.o.provides

libOPRF/CMakeFiles/libOPRF.dir/Hashing/BaseOPRFs.cpp.o.provides.build: libOPRF/CMakeFiles/libOPRF.dir/Hashing/BaseOPRFs.cpp.o


libOPRF/CMakeFiles/libOPRF.dir/Hashing/CuckooHash.cpp.o: libOPRF/CMakeFiles/libOPRF.dir/flags.make
libOPRF/CMakeFiles/libOPRF.dir/Hashing/CuckooHash.cpp.o: libOPRF/Hashing/CuckooHash.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/i/npsi_my/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object libOPRF/CMakeFiles/libOPRF.dir/Hashing/CuckooHash.cpp.o"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libOPRF.dir/Hashing/CuckooHash.cpp.o -c /mnt/i/npsi_my/libOPRF/Hashing/CuckooHash.cpp

libOPRF/CMakeFiles/libOPRF.dir/Hashing/CuckooHash.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libOPRF.dir/Hashing/CuckooHash.cpp.i"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/i/npsi_my/libOPRF/Hashing/CuckooHash.cpp > CMakeFiles/libOPRF.dir/Hashing/CuckooHash.cpp.i

libOPRF/CMakeFiles/libOPRF.dir/Hashing/CuckooHash.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libOPRF.dir/Hashing/CuckooHash.cpp.s"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/i/npsi_my/libOPRF/Hashing/CuckooHash.cpp -o CMakeFiles/libOPRF.dir/Hashing/CuckooHash.cpp.s

libOPRF/CMakeFiles/libOPRF.dir/Hashing/CuckooHash.cpp.o.requires:

.PHONY : libOPRF/CMakeFiles/libOPRF.dir/Hashing/CuckooHash.cpp.o.requires

libOPRF/CMakeFiles/libOPRF.dir/Hashing/CuckooHash.cpp.o.provides: libOPRF/CMakeFiles/libOPRF.dir/Hashing/CuckooHash.cpp.o.requires
	$(MAKE) -f libOPRF/CMakeFiles/libOPRF.dir/build.make libOPRF/CMakeFiles/libOPRF.dir/Hashing/CuckooHash.cpp.o.provides.build
.PHONY : libOPRF/CMakeFiles/libOPRF.dir/Hashing/CuckooHash.cpp.o.provides

libOPRF/CMakeFiles/libOPRF.dir/Hashing/CuckooHash.cpp.o.provides.build: libOPRF/CMakeFiles/libOPRF.dir/Hashing/CuckooHash.cpp.o


libOPRF/CMakeFiles/libOPRF.dir/Hashing/Hints.cpp.o: libOPRF/CMakeFiles/libOPRF.dir/flags.make
libOPRF/CMakeFiles/libOPRF.dir/Hashing/Hints.cpp.o: libOPRF/Hashing/Hints.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/i/npsi_my/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object libOPRF/CMakeFiles/libOPRF.dir/Hashing/Hints.cpp.o"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libOPRF.dir/Hashing/Hints.cpp.o -c /mnt/i/npsi_my/libOPRF/Hashing/Hints.cpp

libOPRF/CMakeFiles/libOPRF.dir/Hashing/Hints.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libOPRF.dir/Hashing/Hints.cpp.i"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/i/npsi_my/libOPRF/Hashing/Hints.cpp > CMakeFiles/libOPRF.dir/Hashing/Hints.cpp.i

libOPRF/CMakeFiles/libOPRF.dir/Hashing/Hints.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libOPRF.dir/Hashing/Hints.cpp.s"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/i/npsi_my/libOPRF/Hashing/Hints.cpp -o CMakeFiles/libOPRF.dir/Hashing/Hints.cpp.s

libOPRF/CMakeFiles/libOPRF.dir/Hashing/Hints.cpp.o.requires:

.PHONY : libOPRF/CMakeFiles/libOPRF.dir/Hashing/Hints.cpp.o.requires

libOPRF/CMakeFiles/libOPRF.dir/Hashing/Hints.cpp.o.provides: libOPRF/CMakeFiles/libOPRF.dir/Hashing/Hints.cpp.o.requires
	$(MAKE) -f libOPRF/CMakeFiles/libOPRF.dir/build.make libOPRF/CMakeFiles/libOPRF.dir/Hashing/Hints.cpp.o.provides.build
.PHONY : libOPRF/CMakeFiles/libOPRF.dir/Hashing/Hints.cpp.o.provides

libOPRF/CMakeFiles/libOPRF.dir/Hashing/Hints.cpp.o.provides.build: libOPRF/CMakeFiles/libOPRF.dir/Hashing/Hints.cpp.o


libOPRF/CMakeFiles/libOPRF.dir/Hashing/SimpleHash.cpp.o: libOPRF/CMakeFiles/libOPRF.dir/flags.make
libOPRF/CMakeFiles/libOPRF.dir/Hashing/SimpleHash.cpp.o: libOPRF/Hashing/SimpleHash.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/i/npsi_my/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object libOPRF/CMakeFiles/libOPRF.dir/Hashing/SimpleHash.cpp.o"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libOPRF.dir/Hashing/SimpleHash.cpp.o -c /mnt/i/npsi_my/libOPRF/Hashing/SimpleHash.cpp

libOPRF/CMakeFiles/libOPRF.dir/Hashing/SimpleHash.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libOPRF.dir/Hashing/SimpleHash.cpp.i"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/i/npsi_my/libOPRF/Hashing/SimpleHash.cpp > CMakeFiles/libOPRF.dir/Hashing/SimpleHash.cpp.i

libOPRF/CMakeFiles/libOPRF.dir/Hashing/SimpleHash.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libOPRF.dir/Hashing/SimpleHash.cpp.s"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/i/npsi_my/libOPRF/Hashing/SimpleHash.cpp -o CMakeFiles/libOPRF.dir/Hashing/SimpleHash.cpp.s

libOPRF/CMakeFiles/libOPRF.dir/Hashing/SimpleHash.cpp.o.requires:

.PHONY : libOPRF/CMakeFiles/libOPRF.dir/Hashing/SimpleHash.cpp.o.requires

libOPRF/CMakeFiles/libOPRF.dir/Hashing/SimpleHash.cpp.o.provides: libOPRF/CMakeFiles/libOPRF.dir/Hashing/SimpleHash.cpp.o.requires
	$(MAKE) -f libOPRF/CMakeFiles/libOPRF.dir/build.make libOPRF/CMakeFiles/libOPRF.dir/Hashing/SimpleHash.cpp.o.provides.build
.PHONY : libOPRF/CMakeFiles/libOPRF.dir/Hashing/SimpleHash.cpp.o.provides

libOPRF/CMakeFiles/libOPRF.dir/Hashing/SimpleHash.cpp.o.provides.build: libOPRF/CMakeFiles/libOPRF.dir/Hashing/SimpleHash.cpp.o


libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfReceiver.cpp.o: libOPRF/CMakeFiles/libOPRF.dir/flags.make
libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfReceiver.cpp.o: libOPRF/OPPRF/BfOpprfReceiver.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/i/npsi_my/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfReceiver.cpp.o"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libOPRF.dir/OPPRF/BfOpprfReceiver.cpp.o -c /mnt/i/npsi_my/libOPRF/OPPRF/BfOpprfReceiver.cpp

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfReceiver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libOPRF.dir/OPPRF/BfOpprfReceiver.cpp.i"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/i/npsi_my/libOPRF/OPPRF/BfOpprfReceiver.cpp > CMakeFiles/libOPRF.dir/OPPRF/BfOpprfReceiver.cpp.i

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfReceiver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libOPRF.dir/OPPRF/BfOpprfReceiver.cpp.s"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/i/npsi_my/libOPRF/OPPRF/BfOpprfReceiver.cpp -o CMakeFiles/libOPRF.dir/OPPRF/BfOpprfReceiver.cpp.s

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfReceiver.cpp.o.requires:

.PHONY : libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfReceiver.cpp.o.requires

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfReceiver.cpp.o.provides: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfReceiver.cpp.o.requires
	$(MAKE) -f libOPRF/CMakeFiles/libOPRF.dir/build.make libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfReceiver.cpp.o.provides.build
.PHONY : libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfReceiver.cpp.o.provides

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfReceiver.cpp.o.provides.build: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfReceiver.cpp.o


libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfSender.cpp.o: libOPRF/CMakeFiles/libOPRF.dir/flags.make
libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfSender.cpp.o: libOPRF/OPPRF/BfOpprfSender.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/i/npsi_my/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfSender.cpp.o"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libOPRF.dir/OPPRF/BfOpprfSender.cpp.o -c /mnt/i/npsi_my/libOPRF/OPPRF/BfOpprfSender.cpp

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfSender.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libOPRF.dir/OPPRF/BfOpprfSender.cpp.i"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/i/npsi_my/libOPRF/OPPRF/BfOpprfSender.cpp > CMakeFiles/libOPRF.dir/OPPRF/BfOpprfSender.cpp.i

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfSender.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libOPRF.dir/OPPRF/BfOpprfSender.cpp.s"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/i/npsi_my/libOPRF/OPPRF/BfOpprfSender.cpp -o CMakeFiles/libOPRF.dir/OPPRF/BfOpprfSender.cpp.s

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfSender.cpp.o.requires:

.PHONY : libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfSender.cpp.o.requires

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfSender.cpp.o.provides: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfSender.cpp.o.requires
	$(MAKE) -f libOPRF/CMakeFiles/libOPRF.dir/build.make libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfSender.cpp.o.provides.build
.PHONY : libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfSender.cpp.o.provides

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfSender.cpp.o.provides.build: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfSender.cpp.o


libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFReceiver.cpp.o: libOPRF/CMakeFiles/libOPRF.dir/flags.make
libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFReceiver.cpp.o: libOPRF/OPPRF/OPPRFReceiver.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/i/npsi_my/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFReceiver.cpp.o"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libOPRF.dir/OPPRF/OPPRFReceiver.cpp.o -c /mnt/i/npsi_my/libOPRF/OPPRF/OPPRFReceiver.cpp

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFReceiver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libOPRF.dir/OPPRF/OPPRFReceiver.cpp.i"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/i/npsi_my/libOPRF/OPPRF/OPPRFReceiver.cpp > CMakeFiles/libOPRF.dir/OPPRF/OPPRFReceiver.cpp.i

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFReceiver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libOPRF.dir/OPPRF/OPPRFReceiver.cpp.s"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/i/npsi_my/libOPRF/OPPRF/OPPRFReceiver.cpp -o CMakeFiles/libOPRF.dir/OPPRF/OPPRFReceiver.cpp.s

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFReceiver.cpp.o.requires:

.PHONY : libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFReceiver.cpp.o.requires

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFReceiver.cpp.o.provides: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFReceiver.cpp.o.requires
	$(MAKE) -f libOPRF/CMakeFiles/libOPRF.dir/build.make libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFReceiver.cpp.o.provides.build
.PHONY : libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFReceiver.cpp.o.provides

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFReceiver.cpp.o.provides.build: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFReceiver.cpp.o


libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFSender.cpp.o: libOPRF/CMakeFiles/libOPRF.dir/flags.make
libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFSender.cpp.o: libOPRF/OPPRF/OPPRFSender.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/i/npsi_my/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFSender.cpp.o"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libOPRF.dir/OPPRF/OPPRFSender.cpp.o -c /mnt/i/npsi_my/libOPRF/OPPRF/OPPRFSender.cpp

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFSender.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libOPRF.dir/OPPRF/OPPRFSender.cpp.i"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/i/npsi_my/libOPRF/OPPRF/OPPRFSender.cpp > CMakeFiles/libOPRF.dir/OPPRF/OPPRFSender.cpp.i

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFSender.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libOPRF.dir/OPPRF/OPPRFSender.cpp.s"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/i/npsi_my/libOPRF/OPPRF/OPPRFSender.cpp -o CMakeFiles/libOPRF.dir/OPPRF/OPPRFSender.cpp.s

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFSender.cpp.o.requires:

.PHONY : libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFSender.cpp.o.requires

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFSender.cpp.o.provides: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFSender.cpp.o.requires
	$(MAKE) -f libOPRF/CMakeFiles/libOPRF.dir/build.make libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFSender.cpp.o.provides.build
.PHONY : libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFSender.cpp.o.provides

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFSender.cpp.o.provides.build: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFSender.cpp.o


libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfReceiver.cpp.o: libOPRF/CMakeFiles/libOPRF.dir/flags.make
libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfReceiver.cpp.o: libOPRF/OPPRF/PlainOprfReceiver.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/i/npsi_my/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfReceiver.cpp.o"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libOPRF.dir/OPPRF/PlainOprfReceiver.cpp.o -c /mnt/i/npsi_my/libOPRF/OPPRF/PlainOprfReceiver.cpp

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfReceiver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libOPRF.dir/OPPRF/PlainOprfReceiver.cpp.i"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/i/npsi_my/libOPRF/OPPRF/PlainOprfReceiver.cpp > CMakeFiles/libOPRF.dir/OPPRF/PlainOprfReceiver.cpp.i

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfReceiver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libOPRF.dir/OPPRF/PlainOprfReceiver.cpp.s"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/i/npsi_my/libOPRF/OPPRF/PlainOprfReceiver.cpp -o CMakeFiles/libOPRF.dir/OPPRF/PlainOprfReceiver.cpp.s

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfReceiver.cpp.o.requires:

.PHONY : libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfReceiver.cpp.o.requires

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfReceiver.cpp.o.provides: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfReceiver.cpp.o.requires
	$(MAKE) -f libOPRF/CMakeFiles/libOPRF.dir/build.make libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfReceiver.cpp.o.provides.build
.PHONY : libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfReceiver.cpp.o.provides

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfReceiver.cpp.o.provides.build: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfReceiver.cpp.o


libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfSender.cpp.o: libOPRF/CMakeFiles/libOPRF.dir/flags.make
libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfSender.cpp.o: libOPRF/OPPRF/PlainOprfSender.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/i/npsi_my/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfSender.cpp.o"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libOPRF.dir/OPPRF/PlainOprfSender.cpp.o -c /mnt/i/npsi_my/libOPRF/OPPRF/PlainOprfSender.cpp

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfSender.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libOPRF.dir/OPPRF/PlainOprfSender.cpp.i"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/i/npsi_my/libOPRF/OPPRF/PlainOprfSender.cpp > CMakeFiles/libOPRF.dir/OPPRF/PlainOprfSender.cpp.i

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfSender.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libOPRF.dir/OPPRF/PlainOprfSender.cpp.s"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/i/npsi_my/libOPRF/OPPRF/PlainOprfSender.cpp -o CMakeFiles/libOPRF.dir/OPPRF/PlainOprfSender.cpp.s

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfSender.cpp.o.requires:

.PHONY : libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfSender.cpp.o.requires

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfSender.cpp.o.provides: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfSender.cpp.o.requires
	$(MAKE) -f libOPRF/CMakeFiles/libOPRF.dir/build.make libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfSender.cpp.o.provides.build
.PHONY : libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfSender.cpp.o.provides

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfSender.cpp.o.provides.build: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfSender.cpp.o


libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfReceiver.cpp.o: libOPRF/CMakeFiles/libOPRF.dir/flags.make
libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfReceiver.cpp.o: libOPRF/OPPRF/PolyOpprfReceiver.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/i/npsi_my/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfReceiver.cpp.o"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfReceiver.cpp.o -c /mnt/i/npsi_my/libOPRF/OPPRF/PolyOpprfReceiver.cpp

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfReceiver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfReceiver.cpp.i"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/i/npsi_my/libOPRF/OPPRF/PolyOpprfReceiver.cpp > CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfReceiver.cpp.i

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfReceiver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfReceiver.cpp.s"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/i/npsi_my/libOPRF/OPPRF/PolyOpprfReceiver.cpp -o CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfReceiver.cpp.s

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfReceiver.cpp.o.requires:

.PHONY : libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfReceiver.cpp.o.requires

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfReceiver.cpp.o.provides: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfReceiver.cpp.o.requires
	$(MAKE) -f libOPRF/CMakeFiles/libOPRF.dir/build.make libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfReceiver.cpp.o.provides.build
.PHONY : libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfReceiver.cpp.o.provides

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfReceiver.cpp.o.provides.build: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfReceiver.cpp.o


libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfSender.cpp.o: libOPRF/CMakeFiles/libOPRF.dir/flags.make
libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfSender.cpp.o: libOPRF/OPPRF/PolyOpprfSender.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/i/npsi_my/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfSender.cpp.o"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfSender.cpp.o -c /mnt/i/npsi_my/libOPRF/OPPRF/PolyOpprfSender.cpp

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfSender.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfSender.cpp.i"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/i/npsi_my/libOPRF/OPPRF/PolyOpprfSender.cpp > CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfSender.cpp.i

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfSender.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfSender.cpp.s"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/i/npsi_my/libOPRF/OPPRF/PolyOpprfSender.cpp -o CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfSender.cpp.s

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfSender.cpp.o.requires:

.PHONY : libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfSender.cpp.o.requires

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfSender.cpp.o.provides: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfSender.cpp.o.requires
	$(MAKE) -f libOPRF/CMakeFiles/libOPRF.dir/build.make libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfSender.cpp.o.provides.build
.PHONY : libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfSender.cpp.o.provides

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfSender.cpp.o.provides.build: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfSender.cpp.o


libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfReceiver.cpp.o: libOPRF/CMakeFiles/libOPRF.dir/flags.make
libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfReceiver.cpp.o: libOPRF/OPPRF/TableOpprfReceiver.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/i/npsi_my/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfReceiver.cpp.o"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libOPRF.dir/OPPRF/TableOpprfReceiver.cpp.o -c /mnt/i/npsi_my/libOPRF/OPPRF/TableOpprfReceiver.cpp

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfReceiver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libOPRF.dir/OPPRF/TableOpprfReceiver.cpp.i"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/i/npsi_my/libOPRF/OPPRF/TableOpprfReceiver.cpp > CMakeFiles/libOPRF.dir/OPPRF/TableOpprfReceiver.cpp.i

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfReceiver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libOPRF.dir/OPPRF/TableOpprfReceiver.cpp.s"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/i/npsi_my/libOPRF/OPPRF/TableOpprfReceiver.cpp -o CMakeFiles/libOPRF.dir/OPPRF/TableOpprfReceiver.cpp.s

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfReceiver.cpp.o.requires:

.PHONY : libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfReceiver.cpp.o.requires

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfReceiver.cpp.o.provides: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfReceiver.cpp.o.requires
	$(MAKE) -f libOPRF/CMakeFiles/libOPRF.dir/build.make libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfReceiver.cpp.o.provides.build
.PHONY : libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfReceiver.cpp.o.provides

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfReceiver.cpp.o.provides.build: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfReceiver.cpp.o


libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfSender.cpp.o: libOPRF/CMakeFiles/libOPRF.dir/flags.make
libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfSender.cpp.o: libOPRF/OPPRF/TableOpprfSender.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/i/npsi_my/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfSender.cpp.o"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libOPRF.dir/OPPRF/TableOpprfSender.cpp.o -c /mnt/i/npsi_my/libOPRF/OPPRF/TableOpprfSender.cpp

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfSender.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libOPRF.dir/OPPRF/TableOpprfSender.cpp.i"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/i/npsi_my/libOPRF/OPPRF/TableOpprfSender.cpp > CMakeFiles/libOPRF.dir/OPPRF/TableOpprfSender.cpp.i

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfSender.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libOPRF.dir/OPPRF/TableOpprfSender.cpp.s"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/i/npsi_my/libOPRF/OPPRF/TableOpprfSender.cpp -o CMakeFiles/libOPRF.dir/OPPRF/TableOpprfSender.cpp.s

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfSender.cpp.o.requires:

.PHONY : libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfSender.cpp.o.requires

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfSender.cpp.o.provides: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfSender.cpp.o.requires
	$(MAKE) -f libOPRF/CMakeFiles/libOPRF.dir/build.make libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfSender.cpp.o.provides.build
.PHONY : libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfSender.cpp.o.provides

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfSender.cpp.o.provides.build: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfSender.cpp.o


libOPRF/CMakeFiles/libOPRF.dir/OPPRF/binSet.cpp.o: libOPRF/CMakeFiles/libOPRF.dir/flags.make
libOPRF/CMakeFiles/libOPRF.dir/OPPRF/binSet.cpp.o: libOPRF/OPPRF/binSet.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/i/npsi_my/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object libOPRF/CMakeFiles/libOPRF.dir/OPPRF/binSet.cpp.o"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libOPRF.dir/OPPRF/binSet.cpp.o -c /mnt/i/npsi_my/libOPRF/OPPRF/binSet.cpp

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/binSet.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libOPRF.dir/OPPRF/binSet.cpp.i"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/i/npsi_my/libOPRF/OPPRF/binSet.cpp > CMakeFiles/libOPRF.dir/OPPRF/binSet.cpp.i

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/binSet.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libOPRF.dir/OPPRF/binSet.cpp.s"
	cd /mnt/i/npsi_my/libOPRF && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/i/npsi_my/libOPRF/OPPRF/binSet.cpp -o CMakeFiles/libOPRF.dir/OPPRF/binSet.cpp.s

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/binSet.cpp.o.requires:

.PHONY : libOPRF/CMakeFiles/libOPRF.dir/OPPRF/binSet.cpp.o.requires

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/binSet.cpp.o.provides: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/binSet.cpp.o.requires
	$(MAKE) -f libOPRF/CMakeFiles/libOPRF.dir/build.make libOPRF/CMakeFiles/libOPRF.dir/OPPRF/binSet.cpp.o.provides.build
.PHONY : libOPRF/CMakeFiles/libOPRF.dir/OPPRF/binSet.cpp.o.provides

libOPRF/CMakeFiles/libOPRF.dir/OPPRF/binSet.cpp.o.provides.build: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/binSet.cpp.o


# Object files for target libOPRF
libOPRF_OBJECTS = \
"CMakeFiles/libOPRF.dir/Hashing/BaseOPRFs.cpp.o" \
"CMakeFiles/libOPRF.dir/Hashing/CuckooHash.cpp.o" \
"CMakeFiles/libOPRF.dir/Hashing/Hints.cpp.o" \
"CMakeFiles/libOPRF.dir/Hashing/SimpleHash.cpp.o" \
"CMakeFiles/libOPRF.dir/OPPRF/BfOpprfReceiver.cpp.o" \
"CMakeFiles/libOPRF.dir/OPPRF/BfOpprfSender.cpp.o" \
"CMakeFiles/libOPRF.dir/OPPRF/OPPRFReceiver.cpp.o" \
"CMakeFiles/libOPRF.dir/OPPRF/OPPRFSender.cpp.o" \
"CMakeFiles/libOPRF.dir/OPPRF/PlainOprfReceiver.cpp.o" \
"CMakeFiles/libOPRF.dir/OPPRF/PlainOprfSender.cpp.o" \
"CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfReceiver.cpp.o" \
"CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfSender.cpp.o" \
"CMakeFiles/libOPRF.dir/OPPRF/TableOpprfReceiver.cpp.o" \
"CMakeFiles/libOPRF.dir/OPPRF/TableOpprfSender.cpp.o" \
"CMakeFiles/libOPRF.dir/OPPRF/binSet.cpp.o"

# External object files for target libOPRF
libOPRF_EXTERNAL_OBJECTS =

lib/liblibOPRF.a: libOPRF/CMakeFiles/libOPRF.dir/Hashing/BaseOPRFs.cpp.o
lib/liblibOPRF.a: libOPRF/CMakeFiles/libOPRF.dir/Hashing/CuckooHash.cpp.o
lib/liblibOPRF.a: libOPRF/CMakeFiles/libOPRF.dir/Hashing/Hints.cpp.o
lib/liblibOPRF.a: libOPRF/CMakeFiles/libOPRF.dir/Hashing/SimpleHash.cpp.o
lib/liblibOPRF.a: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfReceiver.cpp.o
lib/liblibOPRF.a: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfSender.cpp.o
lib/liblibOPRF.a: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFReceiver.cpp.o
lib/liblibOPRF.a: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFSender.cpp.o
lib/liblibOPRF.a: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfReceiver.cpp.o
lib/liblibOPRF.a: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfSender.cpp.o
lib/liblibOPRF.a: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfReceiver.cpp.o
lib/liblibOPRF.a: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfSender.cpp.o
lib/liblibOPRF.a: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfReceiver.cpp.o
lib/liblibOPRF.a: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfSender.cpp.o
lib/liblibOPRF.a: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/binSet.cpp.o
lib/liblibOPRF.a: libOPRF/CMakeFiles/libOPRF.dir/build.make
lib/liblibOPRF.a: libOPRF/CMakeFiles/libOPRF.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/i/npsi_my/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Linking CXX static library ../lib/liblibOPRF.a"
	cd /mnt/i/npsi_my/libOPRF && $(CMAKE_COMMAND) -P CMakeFiles/libOPRF.dir/cmake_clean_target.cmake
	cd /mnt/i/npsi_my/libOPRF && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/libOPRF.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libOPRF/CMakeFiles/libOPRF.dir/build: lib/liblibOPRF.a

.PHONY : libOPRF/CMakeFiles/libOPRF.dir/build

libOPRF/CMakeFiles/libOPRF.dir/requires: libOPRF/CMakeFiles/libOPRF.dir/Hashing/BaseOPRFs.cpp.o.requires
libOPRF/CMakeFiles/libOPRF.dir/requires: libOPRF/CMakeFiles/libOPRF.dir/Hashing/CuckooHash.cpp.o.requires
libOPRF/CMakeFiles/libOPRF.dir/requires: libOPRF/CMakeFiles/libOPRF.dir/Hashing/Hints.cpp.o.requires
libOPRF/CMakeFiles/libOPRF.dir/requires: libOPRF/CMakeFiles/libOPRF.dir/Hashing/SimpleHash.cpp.o.requires
libOPRF/CMakeFiles/libOPRF.dir/requires: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfReceiver.cpp.o.requires
libOPRF/CMakeFiles/libOPRF.dir/requires: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/BfOpprfSender.cpp.o.requires
libOPRF/CMakeFiles/libOPRF.dir/requires: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFReceiver.cpp.o.requires
libOPRF/CMakeFiles/libOPRF.dir/requires: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/OPPRFSender.cpp.o.requires
libOPRF/CMakeFiles/libOPRF.dir/requires: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfReceiver.cpp.o.requires
libOPRF/CMakeFiles/libOPRF.dir/requires: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PlainOprfSender.cpp.o.requires
libOPRF/CMakeFiles/libOPRF.dir/requires: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfReceiver.cpp.o.requires
libOPRF/CMakeFiles/libOPRF.dir/requires: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/PolyOpprfSender.cpp.o.requires
libOPRF/CMakeFiles/libOPRF.dir/requires: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfReceiver.cpp.o.requires
libOPRF/CMakeFiles/libOPRF.dir/requires: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/TableOpprfSender.cpp.o.requires
libOPRF/CMakeFiles/libOPRF.dir/requires: libOPRF/CMakeFiles/libOPRF.dir/OPPRF/binSet.cpp.o.requires

.PHONY : libOPRF/CMakeFiles/libOPRF.dir/requires

libOPRF/CMakeFiles/libOPRF.dir/clean:
	cd /mnt/i/npsi_my/libOPRF && $(CMAKE_COMMAND) -P CMakeFiles/libOPRF.dir/cmake_clean.cmake
.PHONY : libOPRF/CMakeFiles/libOPRF.dir/clean

libOPRF/CMakeFiles/libOPRF.dir/depend:
	cd /mnt/i/npsi_my && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/i/npsi_my /mnt/i/npsi_my/libOPRF /mnt/i/npsi_my /mnt/i/npsi_my/libOPRF /mnt/i/npsi_my/libOPRF/CMakeFiles/libOPRF.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libOPRF/CMakeFiles/libOPRF.dir/depend
