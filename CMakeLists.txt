cmake_minimum_required(VERSION 2.6)
project(codablecash)


add_subdirectory(src_ext)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	message(STATUS "Debug Build")
		
	set(CMAKE_CXX_FLAGS "-std=c++0x -D__cplusplus=201103L -D__DEBUG__ -D__USE_LARGEFILE64 -fmessage-length=0 -g -O0 -ftest-coverage -fprofile-arcs")
	#set(CMAKE_CXX_FLAGS "-std=c++0x -D__cplusplus=201103L -D__DEBUG__ -fmessage-length=0 -O3 -march=native -MMD -MP -MF")
	
	#set(CMAKE_C_FLAGS "-g -O0 -coverage -fprofile-arcs -ftest-coverage")
	#set(CMAKE_EXE_LINKER_FLAGS="-fprofile-arcs -ftest-coverage")
	
	set(CMAKE_C_FLAGS "-g -O0 -coverage")
	set(CMAKE_EXE_LINKER_FLAGS=" -coverage")
	
	set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
	set(CMAKE_C_OUTPUT_EXTENSION_REPLACE ON)
	
	message(STATUS "Debug Build" ${CMAKE_CXX_FLAGS})
elseif(CMAKE_BUILD_TYPE STREQUAL "ReleaseTest")
	set(CMAKE_CXX_FLAGS "-std=c++0x -D__cplusplus=201103L -D__DEBUG__ -D__USE_LARGEFILE64 -fmessage-length=0 -g -march=native -MMD -MP -MF")
else()
	set(CMAKE_CXX_FLAGS "-std=c++0x -D__cplusplus=201103L -DNDEBUG -D__USE_LARGEFILE64 -Wall -fmessage-length=0 -O3 -march=native -MMD -MP -MF")
endif()


if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "ReleaseTest")
	enable_testing()
	
	add_definitions(-D__TEST_CPP_UNIT__=1)
	
	# Report
	set(infofiles coverage/test.info)
	add_custom_target(report)
	add_custom_command(TARGET report
		COMMAND genhtml --no-function-coverage --title "CodableCash Test Report" -s -p ${PROJECT_SOURCE_DIR} -o html_report ${infofiles}
		
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		)
		
	set(covdir coverage/)
	set(workdir ${CMAKE_BINARY_DIR}/${covdir})
	
	
	add_custom_target(lcov
	    COMMAND mkdir -p ${covdir}
	    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	    )
	add_custom_command(TARGET lcov
		COMMAND echo "Merging LCOV...."
		COMMAND ${CMAKE_COMMAND} -DCMAKE_BINARY_DIR=${CMAKE_BINARY_DIR} -P ${PROJECT_SOURCE_DIR}/CMakeReport.cmake
		
		WORKING_DIRECTORY ${workdir}
		)
	add_dependencies(report lcov)
	
endif()

#[[ =========================================================================================== ]]#

macro(outgcov target_name gnodir srclist)
	set(infoname ${target_name}.info)
	set(covdir coverage/${target_name})
	set(workdir ${CMAKE_BINARY_DIR}/${covdir})
	
	add_custom_target(${target_name}
	    COMMAND mkdir -p ${covdir}
	    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	    )
	
	add_custom_command(TARGET ${target_name}
	    COMMAND echo "Making GCOV and LCOV of ${target_name} ...."
	    COMMAND gcov -b ${srclist} -o ${gnodir} >> gcov.log
		COMMAND lcov -c -d ${gnodir} -o ${infoname} >> lcov.log
		WORKING_DIRECTORY ${workdir}
	)
	add_dependencies(lcov ${target_name})
endmacro(outgcov)

macro(set_parentv srclist prefix var_name)
	set(__tmptestsrc "")
	foreach(loop_var ${srclist})
		list(APPEND __tmptestsrc ${prefix}/${loop_var})
	endforeach()
	set(${var_name} ${__tmptestsrc} PARENT_SCOPE)
endmacro(set_parentv)

macro(handle_sub target subsrclist subPrefix subName)
	set(__tmptestsrc "")
	foreach(loop_var ${subsrclist})
		list(APPEND __tmptestsrc "${subName}/${loop_var}")
	endforeach()
	set(tmpsrc ${__tmptestsrc})
	
	## subprefix
	if(${subPrefix} STREQUAL "null")
		set(src_out_dir "src")
	else()
		set(src_out_dir src_${subPrefix})
	endif()
	
	#report name
	if(${subPrefix} STREQUAL "null")
		string(REPLACE "/" "_" subName2 ${subName})
		set(report_name "report_${subName2}")
	else()
		string(REPLACE "/" "_" subName2 ${subName})
		set(report_name "report_${subPrefix}_${subName2}")
	endif()
		
	set(gnodir "${CMAKE_BINARY_DIR}/CMakeFiles/${target}.dir/${src_out_dir}/${subName}")
	outgcov("${report_name}" ${gnodir} "${subsrclist}")
	
	list(LENGTH __dirsrc list_length)
	if(${list_length} EQUAL 0)
		set(__dirsrc "${tmpsrc}" PARENT_SCOPE)
	else()
		set(__dirsrc "${__dirsrc};${tmpsrc}" PARENT_SCOPE)
	endif()	
endmacro(handle_sub)

#[[ =========================================================================================== ]]#

set(srcmain
	AlinousStore.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/src/)
include_directories(${PROJECT_SOURCE_DIR}/src_blockchain/)
include_directories(${PROJECT_SOURCE_DIR}/src_blockchain_p2p/)
include_directories(${PROJECT_SOURCE_DIR}/src_db/)
include_directories(${PROJECT_SOURCE_DIR}/src_gen/)
include_directories(${PROJECT_SOURCE_DIR}/src_ext/)
include_directories(${PROJECT_SOURCE_DIR}/src_smartcontract/)
include_directories(${PROJECT_SOURCE_DIR}/src_smartcontract_db/)
include_directories(${PROJECT_SOURCE_DIR}/src_smartcontract_vm/)
include_directories(${PROJECT_SOURCE_DIR}/src_test/)

add_subdirectory(src)
add_subdirectory(src_blockchain)
add_subdirectory(src_blockchain_p2p)
add_subdirectory(src_db)
add_subdirectory(src_gen)
add_subdirectory(src_smartcontract)
add_subdirectory(src_smartcontract_db)
add_subdirectory(src_smartcontract_vm)


add_library(codablecashlib STATIC ${top_src} ${top_src_blockchain} ${top_src_blockchain_p2p} ${top_src_db} ${top_src_gen} ${top_src_smartcontract} ${top_src_smartcontract_db} ${top_src_smartcontract_vm})
target_link_libraries(codablecashlib extlib pthread gmp)

add_executable(codablecash ${srcmain})
target_link_libraries(codablecash codablecashlib extlib pthread gmp)

if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "ReleaseTest")
	#
	# Test Codes
	#
	add_subdirectory(src_test)
	
	set(testtarget testall)
	add_executable(${testtarget} src_test/testallmain.cpp ${top_testsrc})
	target_link_libraries(${testtarget} codablecashlib extlib pthread gmp)
	
	add_test(${testtarget} ${testtarget})
endif()


#[[ =========================================================================================== ]]#
#[[
get_cmake_property(_variableNames VARIABLES)
foreach(_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
endforeach()
]]#

#--cppcheck-build-dir=${CMAKE_BINARY_DIR}/cppcheck/

add_custom_target(cppcheck
    COMMAND cppcheck --enable=all --std=c++11 --language=c++ --suppressions-list=${PROJECT_SOURCE_DIR}/cppcheck/Suppressions.txt
    	--includes-file=${PROJECT_SOURCE_DIR}/cppcheck/includes.txt --cppcheck-build-dir=${CMAKE_BINARY_DIR}/cppcheck/
    	-i/usr/include/c++/7/
    	-D__TEST_CPP_UNIT__=1 --xml --xml-version=2 --output-file=${CMAKE_BINARY_DIR}/cppcheck.xml
    	./src/ ./src_db/ ./src_test/
    	
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )

add_custom_target(valgrind
	COMMAND valgrind --leak-check=full
		--xml=yes --xml-file=testall.%p.valgrind.xml --main-stacksize=83886080
		./testall
	
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	)

add_custom_target(valgrindp
	COMMAND valgrind --leak-check=full
		--xml=yes --xml-file=testall.%p.valgrind.xml --main-stacksize=83886080
		./testall -g TestVoteAndFinalizeTicketGroup
	
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	)
	
add_custom_target(helgrindp
	COMMAND valgrind --tool=helgrind --xml=yes --xml-file=testall.%p.helgrind.xml ./testall -g TestPoWManagerGroup
	
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	)
