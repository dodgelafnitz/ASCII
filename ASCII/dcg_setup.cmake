###########################
# TODOS
###########################
# [X] asset moving
# [X] externals
# [X] external submodules
# [X] test files
# [ ] documentation
# [X] cmake file recreation disabling
# [X] file recreation disabling
# [X] file moving (update inlcudes in file)
# [X] autoplacing externals
# [X] testing as external
# [ ] edittable testing location/includes/name
# [ ] correct line feeds
# [X] file moving header guard updating
###########################

function(DCG_set_default_setting setting value)
  if("$ENV{${setting}}" STREQUAL "")
    set("ENV{${setting}}" "${value}")
  endif()
endfunction()

DCG_set_default_setting(DCG_ENABLE_TESTS FALSE)
DCG_set_default_setting(DCG_CPP_STANDARD 20)
DCG_set_default_setting(DCG_DEBUG_PRINT_STRUCTURE FALSE)
DCG_set_default_setting(DCG_COPYWRITE "")
DCG_set_default_setting(DCG_ENABLE_CMAKE_REGEN TRUE)

function(DCG_add_interface_source_group targetName)
  get_target_property(targetSourceDir "${targetName}" SOURCE_DIR)
  get_target_property(targetSources "${targetName}" INTERFACE_SOURCES)

  source_group(TREE "${targetSourceDir}/include"
    PREFIX "external\\${targetName}"
    FILES ${targetSources}
  )

  get_property(targetLinkLibs TARGET "${targetName}" PROPERTY INTERFACE_LINK_LIBRARIES)
  foreach(linkLibrary IN ITEMS ${targetLinkLibs})
    DCG_add_interface_source_group("${linkLibrary}")
  endforeach()
endfunction()

function(DCG_get_unique outVar)
  DCG_set_default_setting("UNIQUE_NUMBER_NEXT_VAL" 0)

  set("${outVar}" "UNIQUE_VAR_$ENV{UNIQUE_NUMBER_NEXT_VAL}" PARENT_SCOPE)

  math(EXPR nextVal "$ENV{UNIQUE_NUMBER_NEXT_VAL} + 1")
  set(ENV{UNIQUE_NUMBER_NEXT_VAL} "${nextVal}")
endfunction()

function(DCG_append_env_list envList element)
  set(editList "$ENV{${envList}}")
  list(APPEND editList "${element}")
  set("ENV{${envList}}" "${editList}")
endfunction()

function(DCG_PROJECT name type)
  set(argMode "")
  set(privateDependencies "")
  set(publicDependencies "")

  math(EXPR lastValue "${ARGC} - 1")
  foreach(value RANGE 2 "${lastValue}")
    set(argument "${ARGV${value}}")

    if("${argument}" STREQUAL "PRIVATE_DEPENDS" OR "${argument}" STREQUAL "PUBLIC_DEPENDS")
      set(argMode "${argument}")
    else()
      if("${argMode}" STREQUAL "PRIVATE_DEPENDS")
        list(APPEND privateDependencies "${argument}")
      elseif("${argMode}" STREQUAL "PUBLIC_DEPENDS")
        list(APPEND publicDependencies "${argument}")
      endif()
    endif()
  endforeach()

  DCG_append_env_list("DCG_SOLUTION_PROJECTS" "${name}")

  set("ENV{DCG_PROJECT_${name}_TYPE}" "${type}")
  set("ENV{DCG_PROJECT_${name}_PRIVATE_DEPENDENCIES}" "${privateDependencies}")
  set("ENV{DCG_PROJECT_${name}_PUBLIC_DEPENDENCIES}" "${publicDependencies}")
  set("ENV{DCG_PROJECT_${name}_ASSETS}" "")
  set("ENV{DCG_PROJECT_${name}_FOLDERS}" "")
  set("ENV{DCG_PROJECT_${name}_FILES}" "")

  set("ENV{DCG_CURRENT_PROJECT}" "${name}")
  set("ENV{DCG_CURRENT_FOLDER_PATH}" "")
endfunction()

function(DCG_PROJECT_EXE name)
  DCG_PROJECT("${name}" DCG_EXE ${ARGN})
endfunction()

function(DCG_PROJECT_LIB name)
  DCG_PROJECT("${name}" DCG_LIB ${ARGN})
endfunction()

function(DCG_PROJECT_DLL name)
  DCG_PROJECT("${name}" DCG_DLL ${ARGN})
endfunction()

function(DCG_PROJECT_INTERFACE name)
  DCG_PROJECT("${name}" DCG_INTERFACE ${ARGN})
endfunction()

function(DCG_END_PROJECT)
  set("ENV{DCG_CURRENT_PROJECT}" "")
endfunction()

function(DCG_push_folder folder)
  DCG_append_env_list("DCG_CURRENT_FOLDER_PATH" "${folder}")
endfunction()

function(DCG_pop_folder)
  set(editList "$ENV{DCG_CURRENT_FOLDER_PATH}")
  list(POP_BACK editList)
  set("ENV{DCG_CURRENT_FOLDER_PATH}" "${editList}")
endfunction()

function(DCG_get_folder_path outVar)
  string(REPLACE ";" "/" folderPath "$ENV{DCG_CURRENT_FOLDER_PATH}")
  set("${outVar}" "${folderPath}" PARENT_SCOPE)
endfunction()

function(DCG_FOLDER name)
  DCG_push_folder("${name}")
  DCG_get_folder_path(folderPath)
  DCG_append_env_list("DCG_PROJECT_$ENV{DCG_CURRENT_PROJECT}_FOLDERS" "${folderPath}")
endfunction()

function(DCG_END_FOLDER)
  DCG_pop_folder()
endfunction()

function(DCG_FILE name sourceType hasInterface hasSource hasTest previousLocation)
  DCG_get_folder_path(folderPath)
  DCG_get_unique(unqiueVar)

  set(fileVar "DCG_FILE_${unqiueVar}")

  DCG_append_env_list("DCG_PROJECT_$ENV{DCG_CURRENT_PROJECT}_FILES" "${fileVar}")
  DCG_append_env_list("DCG_FOLDER_${folderPath}_FILES" "${fileVar}")

  if("${folderPath}" STREQUAL "")
    set("ENV{${fileVar}_PATH}" "${name}")
  else()
    set("ENV{${fileVar}_PATH}" "${folderPath}/${name}")
  endif()

  set("ENV{${fileVar}_TYPE}" "${sourceType}")
  set("ENV{${fileVar}_HAS_INTERFACE}" "${hasInterface}")
  set("ENV{${fileVar}_HAS_SOURCE}" "${hasSource}")
  set("ENV{${fileVar}_HAS_TEST}" "${hasTest}")
  set("ENV{${fileVar}_PREVIOUS_LOCATION}" "${previousLocation}")
endfunction()

function(DCG_FILE_CPP name)
  DCG_FILE("${name}" DCG_CPP TRUE TRUE $ENV{DCG_ENABLE_TESTS} "")
endfunction()

function(DCG_FILE_C name)
  DCG_FILE("${name}" DCG_C TRUE TRUE $ENV{DCG_ENABLE_TESTS} "")
endfunction()

function(DCG_FILE_CPP_APPLICATION name)
  DCG_FILE("${name}" DCG_CPP FALSE TRUE FALSE "")
endfunction()

function(DCG_FILE_C_APPLICATION name)
  DCG_FILE("${name}" DCG_C FALSE TRUE FALSE "")
endfunction()

function(DCG_FILE_CPP_HEADER_ONLY name)
  DCG_FILE("${name}" DCG_CPP TRUE FALSE $ENV{DCG_ENABLE_TESTS} "")
endfunction()

function(DCG_FILE_C_HEADER_ONLY name)
  DCG_FILE("${name}" DCG_C TRUE FALSE $ENV{DCG_ENABLE_TESTS} "")
endfunction()

function(DCG_FILE_CPP_NO_TEST name)
  DCG_FILE("${name}" DCG_CPP TRUE TRUE FALSE "")
endfunction()

function(DCG_FILE_C_NO_TEST name)
  DCG_FILE("${name}" DCG_C TRUE TRUE FALSE "")
endfunction()

function(DCG_FILE_CPP_HEADER_ONLY_NO_TEST name)
  DCG_FILE("${name}" DCG_CPP TRUE FALSE FALSE "")
endfunction()

function(DCG_FILE_C_HEADER_ONLY_NO_TEST name)
  DCG_FILE("${name}" DCG_C TRUE FALSE FALSE "")
endfunction()

function(DCG_FILE_CPP_MOVED name previousLocation)
  DCG_FILE("${name}" DCG_CPP TRUE TRUE $ENV{DCG_ENABLE_TESTS} "${previousLocation}")
endfunction()

function(DCG_FILE_C_MOVED name previousLocation)
  DCG_FILE("${name}" DCG_C TRUE TRUE $ENV{DCG_ENABLE_TESTS} "${previousLocation}")
endfunction()

function(DCG_FILE_CPP_APPLICATION_MOVED name previousLocation)
  DCG_FILE("${name}" DCG_CPP FALSE TRUE FALSE "${previousLocation}")
endfunction()

function(DCG_FILE_C_APPLICATION_MOVED name previousLocation)
  DCG_FILE("${name}" DCG_C FALSE TRUE FALSE "${previousLocation}")
endfunction()

function(DCG_FILE_CPP_HEADER_ONLY_MOVED name previousLocation)
  DCG_FILE("${name}" DCG_CPP TRUE FALSE $ENV{DCG_ENABLE_TESTS} "${previousLocation}")
endfunction()

function(DCG_FILE_C_HEADER_ONLY_MOVED name previousLocation)
  DCG_FILE("${name}" DCG_C TRUE FALSE $ENV{DCG_ENABLE_TESTS} "${previousLocation}")
endfunction()

function(DCG_FILE_CPP_NO_TEST_MOVED name previousLocation)
  DCG_FILE("${name}" DCG_CPP TRUE TRUE FALSE "${previousLocation}")
endfunction()

function(DCG_FILE_C_NO_TEST_MOVED name previousLocation)
  DCG_FILE("${name}" DCG_C TRUE TRUE FALSE "${previousLocation}")
endfunction()

function(DCG_FILE_CPP_HEADER_ONLY_NO_TEST_MOVED name previousLocation)
  DCG_FILE("${name}" DCG_CPP TRUE FALSE FALSE "${previousLocation}")
endfunction()

function(DCG_FILE_C_HEADER_ONLY_NO_TEST_MOVED name previousLocation)
  DCG_FILE("${name}" DCG_C TRUE FALSE FALSE "${previousLocation}")
endfunction()

function(DCG_ASSET name)
  DCG_append_env_list("DCG_PROJECT_$ENV{DCG_CURRENT_PROJECT}_ASSETS" "${name}")
endfunction()

function(DCG_EXTERNAL_FILES name)
  DCG_append_env_list("DCG_SOLUTION_EXTERNALS" "${name}")
  set("ENV{DCG_EXTERNAL_${name}_TYPE}" DCG_FILES)
endfunction()

function(DCG_EXTERNAL_SUBMODULE name address)
  DCG_append_env_list("DCG_SOLUTION_EXTERNALS" "${name}")
  set("ENV{DCG_EXTERNAL_${name}_TYPE}" DCG_SUBMODULE)
  set("ENV{DCG_EXTERNAL_${name}_LOCATION}" "${address}")
endfunction()

function(DCG_BEGIN)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  if("$ENV{DCG_ENABLE_TESTS}")
    enable_testing()
  endif()
endfunction()

function(DCG_END)
  if("$ENV{DCG_ENABLE_TESTS}")
    DCG_EXTERNAL_SUBMODULE("GoogleTest" "https://github.com/google/googletest.git")
  endif()

  DCG_create_files()
  DCG_create_cmake_files()
  DCG_build_projects()
  DCG_replace_strings()

  if("$ENV{DCG_DEBUG_PRINT_STRUCTURE}")
    message("Solution : ${PROJECT_NAME}")
    message("  Projects : $ENV{DCG_SOLUTION_PROJECTS}")

    foreach(projectName IN ITEMS $ENV{DCG_SOLUTION_PROJECTS})
      message("    Project: ${projectName}")
      message("      Type : $ENV{DCG_PROJECT_${projectName}_TYPE}")
      message("      Private Dependencies : $ENV{DCG_PROJECT_${projectName}_PRIVATE_DEPENDENCIES}")
      message("      Public Dependencies : $ENV{DCG_PROJECT_${projectName}_PUBLIC_DEPENDENCIES}")
      message("      Assets : $ENV{DCG_PROJECT_${projectName}_ASSETS}")
      message("      Folders : $ENV{DCG_PROJECT_${projectName}_FOLDERS}")

      foreach(folderName IN ITEMS $ENV{DCG_PROJECT_${projectName}_FOLDERS})
        message("        Folder : ${folderName}")
        message("          Files : $ENV{DCG_FOLDER_${folderName}_FILES}")
      endforeach()

      message("      Files : $ENV{DCG_PROJECT_${projectName}_FILES}")

      foreach(fileName IN ITEMS $ENV{DCG_PROJECT_${projectName}_FILES})
        message("        FileId : ${fileName}")
        message("          Path : $ENV{${fileName}_PATH}")
        message("          Type : $ENV{${fileName}_TYPE}")
        message("          Has Interface : $ENV{${fileName}_HAS_INTERFACE}")
        message("          Has Source : $ENV{${fileName}_HAS_SOURCE}")
        message("          Has Test : $ENV{${fileName}_HAS_TEST}")
        message("          Previous Location : $ENV{${fileName}_PREVIOUS_LOCATION}")
      endforeach()
    endforeach()

    message("  Externals : $ENV{DCG_SOLUTION_EXTERNALS}")

    foreach(externalName IN ITEMS $ENV{DCG_SOLUTION_EXTERNALS})
      message("    External : ${externalName}")
      message("      Type : $ENV{DCG_EXTERNAL_${externalName}_TYPE}")
      message("      Location : $ENV{DCG_EXTERNAL_${externalName}_LOCATION}")
    endforeach()

    message("  Replaces : $ENV{DCG_REPLACES}")

    foreach(replacement IN ITEMS $ENV{DCG_REPLACES})
      message("    Replacement : ${replacement}")
      message("      Match : $ENV{${replacement}_MATCH_STR}")
      message("      Replace : $ENV{${replacement}_REPLACE_STR}")
    endforeach()

    message("")
  endif()
endfunction()

function(DCG_create_files)
  set(sourceAreas "include;source")
  if("$ENV{DCG_ENABLE_TESTS}")
    list(APPEND sourceAreas "test")
  endif()

  foreach(projectName IN ITEMS $ENV{DCG_SOLUTION_PROJECTS})
    foreach(fileId IN ITEMS $ENV{DCG_PROJECT_${projectName}_FILES})
      foreach(sourceArea IN ITEMS ${sourceAreas})
        set(fileType ".c")
        set(fileSuffix "")

        if("$ENV{${fileId}_TYPE}" STREQUAL "DCG_CPP")
          set(fileType ".cpp")
        endif()

        if(sourceArea STREQUAL "include")
          if(NOT "$ENV{${fileId}_HAS_INTERFACE}")
            continue()
          endif()
          set(fileType ".h")

          string(TOUPPER "${projectName}_$ENV{${fileId}_PATH}_H" fileDefine)
          string(REPLACE "/" "_" fileDefine "${fileDefine}")

          set(fileContent "$ENV{DCG_COPYWRITE}#ifndef ${fileDefine}\n#define ${fileDefine}\n\n\n\n#endif // ${fileDefine}\n")
        elseif(sourceArea STREQUAL "source")
          if(NOT "$ENV{${fileId}_HAS_SOURCE}")
            continue()
          endif()

          if("$ENV{${fileId}_HAS_INTERFACE}")
            set(fileContent "$ENV{DCG_COPYWRITE}#include \"$ENV{${fileId}_PATH}.h\"\n\n\n")
          else()
            set(fileContent "$ENV{DCG_COPYWRITE}\n\n")
          endif()
        elseif(sourceArea STREQUAL "test")
          if(NOT "$ENV{${fileId}_HAS_TEST}")
            continue()
          endif()

          set(fileSuffix "Test")

          if("$ENV{${fileId}_HAS_INTERFACE}")
            set(fileContent "$ENV{DCG_COPYWRITE}#include \"$ENV{${fileId}_PATH}.h\"\n#include \"gtest/gtest.h\"\n\n\n")
          else()
            set(fileContent "$ENV{DCG_COPYWRITE}#include \"gtest/gtest.h\"\n\n\n")
          endif()
        endif()

        set(filePath "${CMAKE_SOURCE_DIR}/projects/${projectName}/${sourceArea}/$ENV{${fileId}_PATH}${fileSuffix}${fileType}")
        if(NOT EXISTS "${filePath}")
          set(fileIsPresent FALSE)

          if(NOT "$ENV{${fileId}_PREVIOUS_LOCATION}" STREQUAL "")
            set(previousFilePath "${CMAKE_SOURCE_DIR}/projects/${projectName}/${sourceArea}/$ENV{${fileId}_PREVIOUS_LOCATION}${fileSuffix}${fileType}")
            if(EXISTS "${previousFilePath}")
              message("moving ${previousFilePath} to ${filePath}")
              file(RENAME "${previousFilePath}" "${filePath}")
              set(fileIsPresent TRUE)

              set(oldInclude "#include \"$ENV{${fileId}_PREVIOUS_LOCATION}.h\"")
              set(newInclude "#include \"$ENV{${fileId}_PATH}.h\"")

              DCG_get_unique(uniqueVarPath)
              DCG_append_env_list("DCG_REPLACES" "DCG_REPLACE_${uniqueVarPath}")
              set("ENV{DCG_REPLACE_${uniqueVarPath}_MATCH_STR}" "${oldInclude}")
              set("ENV{DCG_REPLACE_${uniqueVarPath}_REPLACE_STR}" "${newInclude}")

              string(TOUPPER "${projectName}_$ENV{${fileId}_PREVIOUS_LOCATION}_H" oldFileDefine)
              string(REPLACE "/" "_" oldFileDefine "${oldFileDefine}")

              string(TOUPPER "${projectName}_$ENV{${fileId}_PATH}_H" newFileDefine)
              string(REPLACE "/" "_" newFileDefine "${newFileDefine}")

              DCG_get_unique(uniqueVarInclude)
              DCG_append_env_list("DCG_REPLACES" "DCG_REPLACE_${uniqueVarInclude}")
              set("ENV{DCG_REPLACE_${uniqueVarInclude}_MATCH_STR}" "${oldFileDefine}")
              set("ENV{DCG_REPLACE_${uniqueVarInclude}_REPLACE_STR}" "${newFileDefine}")
            endif()
          endif()

          if(NOT fileIsPresent)
            message("creating ${filePath}")
            file(WRITE "${filePath}" "${fileContent}")
          endif()
        elseif(NOT "$ENV{${fileId}_PREVIOUS_LOCATION}" STREQUAL "")
          message("${filePath} already exists. Please remove _MOVED suffix from DCG command")
        endif()
      endforeach()
    endforeach()
  endforeach()

  foreach(externalName IN ITEMS $ENV{DCG_SOLUTION_EXTERNALS})
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/externals/${externalName}")
      if("$ENV{DCG_EXTERNAL_${externalName}_TYPE}" STREQUAL DCG_SUBMODULE)
        execute_process(COMMAND git submodule add "$ENV{DCG_EXTERNAL_${externalName}_LOCATION}" "./externals/${externalName}")
      else()
        file(TOUCH "${CMAKE_SOURCE_DIR}/externals/${externalName}/CMakeLists.txt")
      endif()
    endif()
  endforeach()
endfunction()

function(DCG_update_file_if_different fileName fileContent)
  if(NOT "$ENV{DCG_ENABLE_CMAKE_REGEN}")
    return()
  endif()

  set(oldContent "")
  if(EXISTS "${fileName}")
    file(READ "${fileName}" oldContent)
    string(FIND "${oldContent}" "#!DCG_REGENERATE_THIS_FILE" foundVar)
    if("${foundVar}" EQUAL "-1")
      return()
    endif()
  endif()

  if(NOT "${oldContent}" STREQUAL "${fileContent}")
    file(WRITE "${fileName}" "${fileContent}")
  endif()
endfunction()

function(DCG_create_cmake_file projectName isTestProject)
  set(projectType "$ENV{DCG_PROJECT_${projectName}_TYPE}")
  set(projectTarget "${projectName}")

  if(${isTestProject})
    string(APPEND projectTarget "Test")
  endif()

  set(projectPublicCategory "PUBLIC")
  if(${projectType} STREQUAL DCG_INTERFACE)
    set(projectPublicCategory "INTERFACE")
  endif()

  set(projectInterfaceFiles "")
  set(projectSourceFiles "")
  set(projectTestFiles "")
  foreach(fileId IN ITEMS $ENV{DCG_PROJECT_${projectName}_FILES})
    set(fileType ".c")
    if("$ENV{${fileId}_TYPE}" STREQUAL "DCG_CPP")
      set(fileType ".cpp")
    endif()

    if("$ENV{${fileId}_HAS_INTERFACE}")
      list(APPEND projectInterfaceFiles "\${CMAKE_CURRENT_SOURCE_DIR}/include/$ENV{${fileId}_PATH}.h")
    endif()

    if("$ENV{${fileId}_HAS_SOURCE}")
      list(APPEND projectSourceFiles "\${CMAKE_CURRENT_SOURCE_DIR}/source/$ENV{${fileId}_PATH}${fileType}")
    endif()

    if("$ENV{${fileId}_HAS_TEST}")
      list(APPEND projectTestFiles "\${CMAKE_CURRENT_SOURCE_DIR}/$ENV{${fileId}_PATH}Test${fileType}")
    endif()
  endforeach()

  set(fileContent "")

  set(projectGenerationMessage "")
  set(projectDeclarationSection "")
  set(projectPropertiesSection "")
  set(projectDependenciesSection "")
  set(projectAssetsSection "")
  set(projectIncludeDirsSection "")
  set(projectSourcesSection "")
  set(projectSourceGroupSection "")
  set(projectIncludeTestSection "")

  string(APPEND projectGenerationMessage "# CMakeLists.txt file generated with DCG.\n\n")

  if("$ENV{DCG_ENABLE_CMAKE_REGEN}")
    string(APPEND projectGenerationMessage "# To stop file regeneration, remove the following line.\n")
    string(APPEND projectGenerationMessage "#!DCG_REGENERATE_THIS_FILE\n\n")
  endif()

  if(NOT ${isTestProject})
    string(APPEND projectDeclarationSection "project(${projectTarget} C CXX)\n\n")
  endif()

  if(${isTestProject})
    string(APPEND projectDeclarationSection "add_executable(${projectTarget})\n\n")
    string(APPEND projectDeclarationSection "set_property(TARGET ${projectTarget} PROPERTY FOLDER tests)\n\n")
  else()
    if("$ENV{DCG_PROJECT_${projectName}_TYPE}" STREQUAL DCG_LIB)
      string(APPEND projectDeclarationSection "add_library(${projectTarget} STATIC)\n\n")
      string(APPEND projectDeclarationSection "set_property(TARGET ${projectTarget} PROPERTY FOLDER libraries)\n\n")
    elseif("$ENV{DCG_PROJECT_${projectName}_TYPE}" STREQUAL DCG_DLL)
      string(APPEND projectDeclarationSection "add_library(${projectTarget} SHARED)\n\n")
      string(APPEND projectDeclarationSection "set_property(TARGET ${projectTarget} PROPERTY FOLDER libraries)\n\n")
    elseif("$ENV{DCG_PROJECT_${projectName}_TYPE}" STREQUAL DCG_INTERFACE)
      string(APPEND projectDeclarationSection "add_library(${projectTarget} INTERFACE)\n\n")
      string(APPEND projectDeclarationSection "set_property(TARGET ${projectTarget} PROPERTY FOLDER libraries)\n\n")
    else()
      string(APPEND projectDeclarationSection "add_executable(${projectTarget})\n\n")
      string(APPEND projectDeclarationSection "set_property(TARGET ${projectTarget} PROPERTY FOLDER executables)\n\n")
    endif()
  endif()

  if(NOT "$ENV{DCG_PROJECT_${projectName}_TYPE}" STREQUAL DCG_INTERFACE)
    string(APPEND projectPropertiesSection "set_property(TARGET ${projectTarget} PROPERTY CMAKE_CXX_STANDARD $ENV{DCG_CPP_STANDARD})\n")
    string(APPEND projectPropertiesSection "set_property(TARGET ${projectTarget} PROPERTY CXX_STANDARD_REQUIRED ON)\n")
  endif()

  string(APPEND projectPropertiesSection "target_compile_features(${projectTarget} ${projectPublicCategory} cxx_std_$ENV{DCG_CPP_STANDARD})\n\n")

  string(APPEND projectDependenciesSection "target_link_libraries(${projectTarget}\n")
  if(${isTestProject})
    string(APPEND projectDependenciesSection "PRIVATE\n")
    string(APPEND projectDependenciesSection "  ${projectName}\n")
    string(APPEND projectDependenciesSection "  gtest_main\n")
  else()
    if(NOT "$ENV{DCG_PROJECT_${projectName}_PRIVATE_DEPENDENCIES}" STREQUAL "")
      string(APPEND projectDependenciesSection "PRIVATE\n")
      foreach(dependency IN ITEMS $ENV{DCG_PROJECT_${projectName}_PRIVATE_DEPENDENCIES})
        string(APPEND projectDependenciesSection "  ${dependency}\n")
      endforeach()
    endif()
    if(NOT "$ENV{DCG_PROJECT_${projectName}_PUBLIC_DEPENDENCIES}" STREQUAL "")
      string(APPEND projectDependenciesSection "PUBLIC\n")
      foreach(dependency IN ITEMS $ENV{DCG_PROJECT_${projectName}_PUBLIC_DEPENDENCIES})
        string(APPEND projectDependenciesSection "  ${dependency}\n")
      endforeach()
    endif()
  endif()
  string(APPEND projectDependenciesSection ")\n\n")

  if(NOT ${isTestProject} AND NOT "$ENV{DCG_PROJECT_${projectName}_ASSETS}" STREQUAL "")
    string(APPEND projectAssetsSection "add_custom_command(TARGET ${projectTarget} POST_BUILD\n")
    foreach(assetFile IN ITEMS $ENV{DCG_PROJECT_${projectName}_ASSETS})
      string(APPEND projectAssetsSection "  COMMAND \${CMAKE_COMMAND} -E copy \"\${CMAKE_SOURCE_DIR}/${assetFile}\" \"\$<TARGET_FILE_DIR:${projectTarget}>/${assetFile}\"\n")
    endforeach()
    string(APPEND projectAssetsSection ")\n\n")
  endif()

  if(NOT ${isTestProject})
    string(APPEND projectIncludeDirsSection "target_include_directories(${projectTarget}\n${projectPublicCategory}\n  \"\${CMAKE_CURRENT_SOURCE_DIR}/include\"\n)\n\n")
  endif()

  if(${isTestProject})
    if(NOT "${projectTestFiles}" STREQUAL "")
      string(APPEND projectSourcesSection "set(tst\n")
      foreach(filePath IN ITEMS ${projectTestFiles})
        string(APPEND projectSourcesSection "  \"${filePath}\"\n")
      endforeach()
      string(APPEND projectSourcesSection ")\n\n")
    endif()
  else()
    if(NOT "${projectInterfaceFiles}" STREQUAL "")
      string(APPEND projectSourcesSection "set(inc\n")
      foreach(filePath IN ITEMS ${projectInterfaceFiles})
        string(APPEND projectSourcesSection "  \"${filePath}\"\n")
      endforeach()
      string(APPEND projectSourcesSection ")\n\n")
    endif()

    if(NOT "${projectSourceFiles}" STREQUAL "")
      string(APPEND projectSourcesSection "set(src\n")
      foreach(filePath IN ITEMS ${projectSourceFiles})
        string(APPEND projectSourcesSection "  \"${filePath}\"\n")
      endforeach()
      string(APPEND projectSourcesSection ")\n\n")
    endif()
  endif()

  if(${isTestProject})
    if(NOT "${projectTestFiles}" STREQUAL "")
      string(APPEND projectSourcesSection "target_sources(${projectTarget}\nPRIVATE\n  \${tst}\n)\n\n")
    endif()
  else()
    if(NOT "${projectInterfaceFiles}${projectSourceFiles}" STREQUAL "")
      string(APPEND projectSourcesSection "target_sources(${projectTarget}\n")
      if(NOT "${projectInterfaceFiles}" STREQUAL "")
        string(APPEND projectSourcesSection "${projectPublicCategory}\n")
        string(APPEND projectSourcesSection "  \${inc}\n")
      endif()
      if(NOT "${projectSourceFiles}" STREQUAL "")
        string(APPEND projectSourcesSection "PRIVATE\n")
        string(APPEND projectSourcesSection "  \${src}\n")
      endif()
      string(APPEND projectSourcesSection ")\n\n")
    endif()
  endif()

  if(${isTestProject})
    if(NOT "${projectTestFiles}" STREQUAL "")
      string(APPEND projectSourceGroupSection "source_group(TREE \"\${CMAKE_CURRENT_SOURCE_DIR}\" PREFIX \"test\" FILES \${tst})\n\n")
      string(APPEND projectSourceGroupSection "DCG_add_interface_source_group(${projectName})\n")
    endif()
  else()
    if(NOT "${projectInterfaceFiles}" STREQUAL "")
      string(APPEND projectSourceGroupSection "source_group(TREE \"\${CMAKE_CURRENT_SOURCE_DIR}/include\" PREFIX \"include\" FILES \${inc})\n")
    endif()
    if(NOT "${projectSourceFiles}" STREQUAL "")
      string(APPEND projectSourceGroupSection "source_group(TREE \"\${CMAKE_CURRENT_SOURCE_DIR}/source\" PREFIX \"source\" FILES \${src})\n")
    endif()

    if(NOT "$ENV{DCG_PROJECT_${projectName}_PRIVATE_DEPENDENCIES}$ENV{DCG_PROJECT_${projectName}_PUBLIC_DEPENDENCIES}" STREQUAL "")
      string(APPEND projectSourceGroupSection "\n")
    endif()

    foreach(dependency IN ITEMS $ENV{DCG_PROJECT_${projectName}_PRIVATE_DEPENDENCIES})
      string(APPEND projectSourceGroupSection "DCG_add_interface_source_group(${dependency})\n")
    endforeach()

    foreach(dependency IN ITEMS $ENV{DCG_PROJECT_${projectName}_PUBLIC_DEPENDENCIES})
      string(APPEND projectSourceGroupSection "DCG_add_interface_source_group(${dependency})\n")
    endforeach()
  endif()

  if($ENV{DCG_ENABLE_TESTS} AND (NOT ${isTestProject}) AND (NOT "${projectTestFiles}" STREQUAL ""))
    string(APPEND projectIncludeTestSection "\nadd_subdirectory(\"\${CMAKE_SOURCE_DIR}/projects/${projectName}/test\")\n")
    DCG_create_cmake_file(${projectName} TRUE)
  endif()

  string(APPEND fileContent "${projectGenerationMessage}")
  string(APPEND fileContent "${projectDeclarationSection}")
  string(APPEND fileContent "${projectPropertiesSection}")
  string(APPEND fileContent "${projectDependenciesSection}")
  string(APPEND fileContent "${projectAssetsSection}")
  string(APPEND fileContent "${projectIncludeDirsSection}")
  string(APPEND fileContent "${projectSourcesSection}")
  string(APPEND fileContent "${projectSourceGroupSection}")
  string(APPEND fileContent "${projectIncludeTestSection}")

  if(${isTestProject})
    DCG_update_file_if_different("${CMAKE_SOURCE_DIR}/projects/${projectName}/test/CMakeLists.txt" "${fileContent}")
  else()
    DCG_update_file_if_different("${CMAKE_SOURCE_DIR}/projects/${projectName}/CMakeLists.txt" "${fileContent}")
  endif()
endfunction()

function(DCG_create_cmake_files)
  foreach(projectName IN ITEMS $ENV{DCG_SOLUTION_PROJECTS})
    DCG_create_cmake_file("${projectName}" FALSE)
  endforeach()
endfunction()

function(DCG_get_all_subdirectories outVar directory)
  set(resultDirs "${directory}")

  get_property(subdirectories DIRECTORY ${directory} PROPERTY SUBDIRECTORIES)
  foreach(subdirectory IN ITEMS ${subdirectories})
    DCG_get_all_subdirectories(internalDirectories "${subdirectory}")
    list(APPEND resultDirs ${internalDirectories})
  endforeach()

  set("${outVar}" "${resultDirs}" PARENT_SCOPE)
endfunction()

function(DCG_get_relative_directory outVar targetDirectory sourceDirectory)
  string(FIND "${targetDirectory}" "${sourceDirectory}" prefixFind)
  if("${prefixFind}" EQUAL 0)
    string(REPLACE "${sourceDirectory}/" "" relDir "${targetDirectory}")
    set("${outVar}" "${relDir}" PARENT_SCOPE)
  endif()
endfunction()

function(DCG_set_external_folders)
  DCG_get_all_subdirectories(subdirectories "${CMAKE_SOURCE_DIR}")

  foreach(directory IN ITEMS ${subdirectories})
    get_property(targets DIRECTORY "${directory}" PROPERTY BUILDSYSTEM_TARGETS)
    DCG_get_relative_directory(relDir "${directory}" "${CMAKE_SOURCE_DIR}/externals")

    if(NOT "${relDir}" STREQUAL "")
      foreach(target IN ITEMS ${targets})
        set_property(TARGET ${target} PROPERTY FOLDER "externals/${relDir}")
      endforeach()
    endif()
  endforeach()
endfunction()

function(DCG_build_projects)
  foreach(externalName IN ITEMS $ENV{DCG_SOLUTION_EXTERNALS})
    add_subdirectory("${CMAKE_SOURCE_DIR}/externals/${externalName}")
  endforeach()

  DCG_set_external_folders()

  foreach(projectName IN ITEMS $ENV{DCG_SOLUTION_PROJECTS})
    add_subdirectory("${CMAKE_SOURCE_DIR}/projects/${projectName}")
  endforeach()
endfunction()

function(DCG_replace_strings)
  set(files "")

  foreach(projectName IN ITEMS $ENV{DCG_SOLUTION_PROJECTS})
    foreach(fileId IN ITEMS $ENV{DCG_PROJECT_${projectName}_FILES})
      set(fileType ".c")
      if("$ENV{${fileId}_TYPE}" STREQUAL "DCG_CPP")
        set(fileType ".cpp")
      endif()

      if("$ENV{${fileId}_HAS_INTERFACE}")
        list(APPEND files "projects/${projectName}/include/$ENV{${fileId}_PATH}.h")
      endif()

      if("$ENV{${fileId}_HAS_SOURCE}")
        list(APPEND files "projects/${projectName}/source/$ENV{${fileId}_PATH}${fileType}")
      endif()

      if("$ENV{${fileId}_HAS_TEST}")
        list(APPEND files "projects/${projectName}/test/$ENV{${fileId}_PATH}Test${fileType}")
      endif()
    endforeach()
  endforeach()

  foreach(fileName IN ITEMS ${files})
    file(READ "${fileName}" oldFileContent)
    set(fileContent "${oldFileContent}")

    foreach(replaceValue IN ITEMS $ENV{DCG_REPLACES})
      string(REPLACE "$ENV{${replaceValue}_MATCH_STR}" "$ENV{${replaceValue}_REPLACE_STR}" fileContent "${fileContent}")
    endforeach()

    if(NOT "${fileContent}" STREQUAL "${oldFileContent}")
      file(WRITE "${fileName}" "${fileContent}")
    endif()
  endforeach()
endfunction()
