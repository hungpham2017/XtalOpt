# Written by Patrick S. Avery - 2017

# Downloads the obabel executable if it doesn't already exist
macro(DownloadOBabel)
  # Let's set the name. Windows likes to add '.exe' at the end
  if(WIN32)
    set(OBABEL_NAME "obabel.exe")
  else(WIN32)
    set(OBABEL_NAME "obabel")
  endif(WIN32)

  # If it already exists, don't download it again
  if(NOT EXISTS ${CMAKE_BINARY_DIR}/bin/${OBABEL_NAME} AND NOT USE_SYSTEM_OBABEL)
     set(OBABEL_V "openbabel-2-4-1-static-obabel-executables")
    # Linux
    if(UNIX AND NOT APPLE)
      set(OBABEL_DOWNLOAD_LOCATION "https://github.com/psavery/openbabel/releases/download/${OBABEL_V}/linux64-obabel")
      set(MD5 "2e24b5d9816d9903b37a1258e0af27d6")

    # Apple
    elseif(APPLE)
      set(OBABEL_DOWNLOAD_LOCATION "https://github.com/psavery/openbabel/releases/download/${OBABEL_V}/osx64-obabel")
      set(MD5 "f74710141d13138de23f25463c812270")

    # Windows
    elseif(WIN32 AND NOT CYGWIN)
      set(OBABEL_DOWNLOAD_LOCATION "https://github.com/psavery/openbabel/releases/download/${OBABEL_V}/win64-obabel.exe")
      set(MD5 "bd7df731a0efc21dc53cc1f0b9fb152e")

    else()
      message(FATAL_ERROR "OBabel is not supported with the current OS type!")
    endif()

    message("-- Downloading OBabel executable from ${OBABEL_DOWNLOAD_LOCATION}")

    # Install to a temporary directory so we can copy and change file permissions
    file(DOWNLOAD ${OBABEL_DOWNLOAD_LOCATION}
         "${CMAKE_BINARY_DIR}/tmp/${OBABEL_NAME}"
         SHOW_PROGRESS
         EXPECTED_MD5 ${MD5})

    # We need to change the permissions
    file(COPY "${CMAKE_BINARY_DIR}/tmp/${OBABEL_NAME}"
         DESTINATION "${CMAKE_BINARY_DIR}/bin/"
         FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                          GROUP_READ GROUP_EXECUTE
                          WORLD_READ WORLD_EXECUTE)

    # Now remove the temporary directory
    file(REMOVE_RECURSE "${CMAKE_BINARY_DIR}/tmp")

  endif(NOT EXISTS ${CMAKE_BINARY_DIR}/bin/${OBABEL_NAME} AND NOT USE_SYSTEM_OBABEL)

  # Only install it if we are not using system obabel
  if(NOT USE_SYSTEM_OBABEL)
    install(FILES "${CMAKE_BINARY_DIR}/bin/${OBABEL_NAME}"
            DESTINATION bin
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                        GROUP_READ GROUP_EXECUTE
                        WORLD_READ WORLD_EXECUTE)
  endif(NOT USE_SYSTEM_OBABEL)

endmacro(DownloadOBabel)
