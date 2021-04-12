###########################
# TODOS
###########################
# [X] asset moving
# [X] externals
# [X] external submodules
# [ ] test files
# [ ] documentation
# [X] cmake file recreation disabling
# [X] file recreation disabling
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

  source_group(TREE "${targetSourceDir}"
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

function(DCG_FILE name type)
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
  set("ENV{${fileVar}_TYPE}" "${type}")
endfunction()

function(DCG_FILE_CPP name)
  DCG_FILE("${name}" DCG_CPP)
endfunction()

function(DCG_FILE_C name)
  DCG_FILE("${name}" DCG_C)
endfunction()

function(DCG_FILE_CPP_APPLICATION name)
  DCG_FILE("${name}" DCG_CPP_APPLICATION)
endfunction()

function(DCG_FILE_C_APPLICATION name)
  DCG_FILE("${name}" DCG_C_APPLICATION)
endfunction()

function(DCG_FILE_H name)
  DCG_FILE("${name}" DCG_H)
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

  if("${DCG_ENABLE_TESTS}")
    enable_testing()
  endif()
endfunction()

function(DCG_END)
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
        message("          Type : $ENV{${fileName}_Type}")
      endforeach()
    endforeach()

    message("  Externals : $ENV{DCG_SOLUTION_EXTERNALS}")

    foreach(externalName IN ITEMS $ENV{DCG_SOLUTION_EXTERNALS})
      message("    External : ${externalName}")
      message("      Type : $ENV{DCG_EXTERNAL_${externalName}_TYPE}")
      message("      Location : $ENV{DCG_EXTERNAL_${externalName}_LOCATION}")
    endforeach()

    message("")
  endif()

  DCG_create_files()
  DCG_create_cmake_files()
  DCG_build_projects()
endfunction()

function(DCG_create_files)
  set(sourceAreas "include;source")
  if("${DCG_ENABLE_TESTS}")
    list(APPEND sourceAreas "test")
  endif()

  foreach(projectName IN ITEMS $ENV{DCG_SOLUTION_PROJECTS})
    foreach(fileId IN ITEMS $ENV{DCG_PROJECT_${projectName}_FILES})
      foreach(sourceArea IN ITEMS ${sourceAreas})
        if(sourceArea STREQUAL "include")
          if("$ENV{${fileId}_TYPE}" STREQUAL "DCG_CPP_APPLICATION" OR "$ENV{${fileId}_TYPE}" STREQUAL "DCG_C_APPLICATION")
            continue()
          endif()
          set(fileType ".h")

          string(TOUPPER "${projectName}_$ENV{${fileId}_PATH}" fileDefine)
          string(REPLACE "/" "_" fileDefine "${fileDefine}")
          string(APPEND fileDefine "_H")

          set(fileContent "$ENV{DCG_COPYWRITE}#ifndef ${fileDefine}\n#define ${fileDefine}\n\n\n\n#endif // ${fileDefine}\n")
        elseif("$ENV{${fileId}_TYPE}" STREQUAL "DCG_H" OR "$ENV{PROJECT_${projectName}_TYPE}" STREQUAL "DCG_INTERFACE")
          continue()
        elseif("$ENV{${fileId}_TYPE}" STREQUAL "DCG_CPP")
          set(fileType ".cpp")
          set(fileContent "$ENV{DCG_COPYWRITE}#include \"$ENV{${fileId}_PATH}.h\"\n\n\n")
        elseif("$ENV{${fileId}_TYPE}" STREQUAL "DCG_C")
          set(fileType ".c")
          set(fileContent "$ENV{DCG_COPYWRITE}#include \"$ENV{${fileId}_PATH}.h\"\n\n\n")
        elseif("$ENV{${fileId}_TYPE}" STREQUAL "DCG_CPP_APPLICATION")
          set(fileType ".cpp")
          set(fileContent "$ENV{DCG_COPYWRITE}\n\n")
        elseif("$ENV{${fileId}_TYPE}" STREQUAL "DCG_C_APPLICATION")
          set(fileType ".c")
          set(fileContent "$ENV{DCG_COPYWRITE}\n\n")
        endif()

        set(filePath "${CMAKE_SOURCE_DIR}/projects/${projectName}/${sourceArea}/$ENV{${fileId}_PATH}${fileType}")
        if(NOT EXISTS "${filePath}")
          message("creating ${filePath}")
          file(WRITE "${filePath}" "${fileContent}")
        endif()
      endforeach()
    endforeach()
  endforeach()

  foreach(externalName IN ITEMS $ENV{DCG_SOLUTION_EXTERNALS})
    if(NOT EXISTS "./externals/${externalName}")
      if("$ENV{DCG_EXTERNAL_${externalName}_TYPE}" STREQUAL DCG_SUBMODULE)
        execute_process(COMMAND git submodule add "$ENV{DCG_EXTERNAL_${externalName}_LOCATION}" "./externals/${externalName}")
      else()
        file(WRITE "./externals/${externalName}/CMakeLists.txt" "")
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
    file(WRITE "${fileName}" ${fileContent})
  endif()
endfunction()

function(DCG_create_cmake_files)
  foreach(projectName IN ITEMS $ENV{DCG_SOLUTION_PROJECTS})
    set(fileContent "")

    set(projectGenerationMessage "")
    set(projectDeclarationSection "")
    set(projectPropertiesSection "")
    set(projectDependenciesSection "")
    set(projectAssetsSection "")
    set(projectIncludeDirsSection "")
    set(projectSourcesSection "")
    set(projectSourceGroupSection "")

    string(APPEND projectGenerationMessage "# CMakeLists.txt file generated with DCG.\n\n")

    if("$ENV{DCG_ENABLE_CMAKE_REGEN}")
      string(APPEND projectGenerationMessage "# To stop file regeneration, remove the following line.\n")
      string(APPEND projectGenerationMessage "#!DCG_REGENERATE_THIS_FILE\n\n")
    endif()

    string(APPEND projectDeclarationSection "project(${projectName} C CXX)\n\n")
    if("$ENV{DCG_PROJECT_${projectName}_TYPE}" STREQUAL DCG_LIB)
      string(APPEND projectDeclarationSection "add_library(${projectName} STATIC)\n\n")
    elseif("$ENV{DCG_PROJECT_${projectName}_TYPE}" STREQUAL DCG_DLL)
      string(APPEND projectDeclarationSection "add_library(${projectName} SHARED)\n\n")
    elseif("$ENV{DCG_PROJECT_${projectName}_TYPE}" STREQUAL DCG_INTERFACE)
      string(APPEND projectDeclarationSection "add_library(${projectName} INTERFACE)\n\n")
    else()
      string(APPEND projectDeclarationSection "add_executable(${projectName})\n\n")
    endif()

    if(NOT "$ENV{DCG_PROJECT_${projectName}_TYPE}" STREQUAL DCG_INTERFACE)
      string(APPEND projectPropertiesSection "set_property(TARGET ${projectName} PROPERTY CMAKE_CXX_STANDARD $ENV{DCG_CPP_STANDARD})\n")
      string(APPEND projectPropertiesSection "set_property(TARGET ${projectName} PROPERTY CXX_STANDARD_REQUIRED ON)\n")
    endif()

    if("$ENV{DCG_PROJECT_${projectName}_TYPE}" STREQUAL DCG_INTERFACE)
      string(APPEND projectPropertiesSection "target_compile_features(${projectName} INTERFACE cxx_std_$ENV{DCG_CPP_STANDARD})\n\n")
    else()
      string(APPEND projectPropertiesSection "target_compile_features(${projectName} PUBLIC cxx_std_$ENV{DCG_CPP_STANDARD})\n\n")
    endif()

    string(APPEND projectDependenciesSection "target_link_libraries(${projectName}\n")
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
    string(APPEND projectDependenciesSection ")\n\n")

    if(NOT "$ENV{DCG_PROJECT_${projectName}_ASSETS}" STREQUAL "")
      string(APPEND projectAssetsSection "add_custom_command(TARGET \"${projectName}\" POST_BUILD\n")
      foreach(assetFile IN ITEMS $ENV{DCG_PROJECT_${projectName}_ASSETS})
        #string(APPEND projectAssetsSection "  COMMAND \${CMAKE_COMMAND} -E copy \"\${CMAKE_SOURCE_DIR}/${assetFile}\" \"\${CMAKE_SOURCE_DIR}/build/projects/${projectName}/${assetFile}\"\n")
        string(APPEND projectAssetsSection "  COMMAND \${CMAKE_COMMAND} -E copy \"\${CMAKE_SOURCE_DIR}/${assetFile}\" \"$<TARGET_FILE_DIR:${projectName}>/${assetFile}\"\n")
      endforeach()
      string(APPEND projectAssetsSection ")\n\n")
    endif()

    if("$ENV{DCG_PROJECT_${projectName}_TYPE}" STREQUAL DCG_INTERFACE)
      string(APPEND projectIncludeDirsSection "target_include_directories(${projectName}\nINTERFACE\n  \"./include\"\n)\n\n")
    else()
      string(APPEND projectIncludeDirsSection "target_include_directories(${projectName}\nPUBLIC\n  \"./include\"\n)\n\n")
    endif()

    string(APPEND projectSourcesSection "set(inc\n")
    foreach(fileId IN ITEMS $ENV{DCG_PROJECT_${projectName}_FILES})
      if("$ENV{${fileId}_TYPE}" STREQUAL "DCG_CPP_APPLICATION" OR "$ENV{${fileId}_TYPE}" STREQUAL "DCG_C_APPLICATION")
        continue()
      endif()
      string(APPEND projectSourcesSection "  \"\${CMAKE_CURRENT_SOURCE_DIR}/include/$ENV{${fileId}_PATH}.h\"\n")
    endforeach()
    string(APPEND projectSourcesSection ")\n\n")

    if(NOT "$ENV{DCG_PROJECT_${projectName}_TYPE}" STREQUAL DCG_INTERFACE)
      string(APPEND projectSourcesSection "set(src\n")
      foreach(fileId IN ITEMS $ENV{DCG_PROJECT_${projectName}_FILES})
        string(APPEND projectSourcesSection "  \"\${CMAKE_CURRENT_SOURCE_DIR}/source/$ENV{${fileId}_PATH}")
        if("$ENV{${fileId}_TYPE}" STREQUAL DCG_CPP OR "$ENV{${fileId}_TYPE}" STREQUAL DCG_CPP_APPLICATION)
          string(APPEND projectSourcesSection ".cpp\"\n")
        elseif("$ENV{${fileId}_TYPE}" STREQUAL DCG_H)
          continue()
        else()
          string(APPEND projectSourcesSection ".c\"\n")
        endif()
      endforeach()
      string(APPEND projectSourcesSection ")\n\n")
    endif()

    string(APPEND projectSourcesSection "target_sources(${projectName}\n")
    if("$ENV{DCG_PROJECT_${projectName}_TYPE}" STREQUAL DCG_INTERFACE)
      string(APPEND projectSourcesSection "INTERFACE\n")
    else()
      string(APPEND projectSourcesSection "PUBLIC\n")
    endif()
    string(APPEND projectSourcesSection "  \${inc}\n")

    if(NOT "$ENV{DCG_PROJECT_${projectName}_TYPE}" STREQUAL DCG_INTERFACE)
      string(APPEND projectSourcesSection "PRIVATE\n")
      string(APPEND projectSourcesSection "  \${src}\n")
    endif()
    string(APPEND projectSourcesSection ")\n\n")

    string(APPEND projectSourceGroupSection "source_group(TREE \"\${CMAKE_CURRENT_SOURCE_DIR}/include\" PREFIX \"include\" FILES \${inc})\n")
    if(NOT "$ENV{DCG_PROJECT_${projectName}_TYPE}" STREQUAL DCG_INTERFACE)
      string(APPEND projectSourceGroupSection "source_group(TREE \"\${CMAKE_CURRENT_SOURCE_DIR}/source\" PREFIX \"source\" FILES \${src})\n")
    endif()

    string(APPEND projectSourceGroupSection "\n")

    foreach(dependency IN ITEMS $ENV{DCG_PROJECT_${projectName}_PRIVATE_DEPENDENCIES})
      string(APPEND projectSourceGroupSection "DCG_add_interface_source_group(${dependency})\n")
    endforeach()

    foreach(dependency IN ITEMS $ENV{DCG_PROJECT_${projectName}_PUBLIC_DEPENDENCIES})
      string(APPEND projectSourceGroupSection "DCG_add_interface_source_group(${dependency})\n")
    endforeach()

    string(APPEND fileContent "${projectGenerationMessage}")
    string(APPEND fileContent "${projectDeclarationSection}")
    string(APPEND fileContent "${projectPropertiesSection}")
    string(APPEND fileContent "${projectDependenciesSection}")
    string(APPEND fileContent "${projectAssetsSection}")
    string(APPEND fileContent "${projectIncludeDirsSection}")
    string(APPEND fileContent "${projectSourcesSection}")
    string(APPEND fileContent "${projectSourceGroupSection}")

    DCG_update_file_if_different("./projects/${projectName}/CMakeLists.txt" "${fileContent}")
  endforeach()
endfunction()

function(DCG_build_projects)
  foreach(externalName IN ITEMS $ENV{DCG_SOLUTION_EXTERNALS})
    add_subdirectory("./externals/${externalName}")
  endforeach()

  foreach(projectName IN ITEMS $ENV{DCG_SOLUTION_PROJECTS})
    add_subdirectory("./projects/${projectName}")
  endforeach()
endfunction()
