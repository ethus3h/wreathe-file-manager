macro_optional_find_package(NepomukCore)
set_package_properties(NepomukCore PROPERTIES DESCRIPTION "Nepomuk Core libraries"
                       URL "http://www.kde.org"
                       TYPE OPTIONAL
                       PURPOSE "For adding desktop-wide tagging support to dolphin"
                      )

macro_optional_find_package(NepomukWidgets)
set_package_properties(NepomukWidgets PROPERTIES DESCRIPTION "Nepomuk Widgets"
                       URL "http://www.kde.org"
                       TYPE OPTIONAL
                       PURPOSE "For adding desktop-wide tagging support to dolphin"
                      )

if(NepomukCore_FOUND AND NepomukWidgets_FOUND)
    set(HAVE_NEPOMUK TRUE)
endif()

configure_file(config-nepomuk.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-nepomuk.h )

macro_bool_to_01(X11_Xrender_FOUND HAVE_XRENDER)
configure_file(config-X11.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-X11.h )

include_directories( ${KACTIVITIES_INCLUDE_DIRS} )

if(HAVE_NEPOMUK)
  find_package(Soprano 2.7.56)
  set_package_properties(Soprano PROPERTIES DESCRIPTION "Qt-based RDF storage and parsing solution"
                         URL "http://soprano.sourceforge.net"
                         TYPE REQUIRED
                         PURPOSE "Required for everything (storage and general data management)"
                        )

  include_directories( ${SOPRANO_INCLUDE_DIR} ${NEPOMUK_CORE_INCLUDE_DIR} ${NEPOMUK_WIDGETS_INCLUDE_DIR} )
endif()

add_subdirectory(tests)

########### next target ###############

set(dolphinprivate_LIB_SRCS
    kitemviews/kfileitemlistview.cpp
    kitemviews/kfileitemlistwidget.cpp
    kitemviews/kfileitemmodel.cpp
    kitemviews/kfileitemmodelrolesupdater.cpp
    kitemviews/kitemlistcontainer.cpp
    kitemviews/kitemlistcontroller.cpp
    kitemviews/kitemlistgroupheader.cpp
    kitemviews/kitemlistheader.cpp
    kitemviews/kitemlistselectionmanager.cpp
    kitemviews/kitemliststyleoption.cpp
    kitemviews/kitemlistview.cpp
    kitemviews/kitemlistviewaccessible.cpp
    kitemviews/kitemlistwidget.cpp
    kitemviews/kitemmodelbase.cpp
    kitemviews/kstandarditem.cpp
    kitemviews/kstandarditemlistgroupheader.cpp
    kitemviews/kstandarditemlistwidget.cpp
    kitemviews/kstandarditemlistview.cpp
    kitemviews/kstandarditemmodel.cpp
    kitemviews/private/kfileitemclipboard.cpp
    kitemviews/private/kfileitemmodeldirlister.cpp
    kitemviews/private/kfileitemmodelfilter.cpp
    kitemviews/private/kitemlistheaderwidget.cpp
    kitemviews/private/kitemlistkeyboardsearchmanager.cpp
    kitemviews/private/kitemlistroleeditor.cpp
    kitemviews/private/kitemlistrubberband.cpp
    kitemviews/private/kitemlistselectiontoggle.cpp
    kitemviews/private/kitemlistsizehintresolver.cpp
    kitemviews/private/kitemlistsmoothscroller.cpp
    kitemviews/private/kitemlistviewanimation.cpp
    kitemviews/private/kitemlistviewlayouter.cpp
    kitemviews/private/kpixmapmodifier.cpp
    settings/additionalinfodialog.cpp
    settings/applyviewpropsjob.cpp
    settings/viewmodes/viewmodesettings.cpp
    settings/viewpropertiesdialog.cpp
    settings/viewpropsprogressinfo.cpp
    views/dolphinfileitemlistwidget.cpp
    views/dolphinitemlistview.cpp
    views/dolphinnewfilemenuobserver.cpp
    views/dolphinremoteencoding.cpp
    views/dolphinview.cpp
    views/dolphinviewactionhandler.cpp
    views/draganddrophelper.cpp
    views/renamedialog.cpp
    views/tooltips/filemetadatatooltip.cpp
    views/tooltips/tooltipmanager.cpp
    views/versioncontrol/updateitemstatesthread.cpp
    views/versioncontrol/versioncontrolobserver.cpp
    views/viewmodecontroller.cpp
    views/viewproperties.cpp
    views/zoomlevelinfo.cpp
    dolphinremoveaction.cpp
)

if(HAVE_NEPOMUK)
    set(dolphinprivate_LIB_SRCS
        ${dolphinprivate_LIB_SRCS}
        kitemviews/private/knepomukrolesprovider.cpp
    )
endif()

kde4_add_kcfg_files(dolphinprivate_LIB_SRCS
    settings/dolphin_compactmodesettings.kcfgc
    settings/dolphin_directoryviewpropertysettings.kcfgc
    settings/dolphin_detailsmodesettings.kcfgc
    settings/dolphin_iconsmodesettings.kcfgc
    settings/dolphin_generalsettings.kcfgc
    settings/dolphin_versioncontrolsettings.kcfgc
)

kde4_add_library(dolphinprivate SHARED ${dolphinprivate_LIB_SRCS})

target_link_libraries(
    dolphinprivate
    ${KDE4_KFILE_LIBS}
    konq
    ${KDE4_KNEWSTUFF3_LIBS}
)

if(HAVE_NEPOMUK)
    target_link_libraries(
        dolphinprivate
        ${NEPOMUK_CORE_LIBRARY}
        ${NEPOMUK_WIDGETS_LIBRARY}
        ${SOPRANO_LIBRARIES}
    )
endif()

if(X11_Xrender_FOUND)
    target_link_libraries(dolphinprivate ${X11_Xrender_LIB})
endif(X11_Xrender_FOUND)

target_link_libraries(dolphinprivate ${KDE4_PLASMA_LIBS})

set_target_properties(dolphinprivate PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )
install(TARGETS dolphinprivate  ${INSTALL_TARGETS_DEFAULT_ARGS})

##########################################

set(dolphinpart_SRCS
   dolphinpart.cpp
)

# Add dolphinpart_ext.cpp conditionally, only with KDE > 4.9.1.
if (${KDE_VERSION} VERSION_GREATER "4.9.1")
set(dolphinpart_SRCS
    ${dolphinpart_SRCS}
    dolphinpart_ext.cpp)
endif (${KDE_VERSION} VERSION_GREATER "4.9.1")

kde4_add_plugin(dolphinpart ${dolphinpart_SRCS})

target_link_libraries(dolphinpart dolphinprivate konq ${KDE4_KPARTS_LIBS} ${KDE4_KFILE_LIBS})

install(TARGETS dolphinpart DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES dolphinpart.rc DESTINATION ${DATA_INSTALL_DIR}/dolphinpart)
install(FILES dolphinpart.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
install(FILES views/versioncontrol/fileviewversioncontrolplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})

##########################################

set(dolphin_SRCS
    dolphinapplication.cpp
    dolphindockwidget.cpp
    dolphinmainwindow.cpp
    dolphinnewfilemenu.cpp
    dolphinviewcontainer.cpp
    dolphincontextmenu.cpp
    filterbar/filterbar.cpp
    main.cpp
    panels/information/filemetadataconfigurationdialog.cpp
    panels/information/informationpanel.cpp
    panels/information/informationpanelcontent.cpp
    panels/information/pixmapviewer.cpp
    panels/information/phononwidget.cpp
    panels/places/placespanel.cpp
    panels/places/placesitem.cpp
    panels/places/placesitemeditdialog.cpp
    panels/places/placesitemlistgroupheader.cpp
    panels/places/placesitemlistwidget.cpp
    panels/places/placesitemmodel.cpp
    panels/places/placesitemsignalhandler.cpp
    panels/places/placesview.cpp
    panels/panel.cpp
    panels/folders/foldersitemlistwidget.cpp
    panels/folders/treeviewcontextmenu.cpp
    panels/folders/folderspanel.cpp
    search/dolphinfacetswidget.cpp
    search/dolphinsearchbox.cpp
    search/dolphinsearchinformation.cpp
    settings/general/behaviorsettingspage.cpp
    settings/general/configurepreviewplugindialog.cpp
    settings/general/confirmationssettingspage.cpp
    settings/general/generalsettingspage.cpp
    settings/general/previewssettingspage.cpp
    settings/general/statusbarsettingspage.cpp
    settings/dolphinsettingsdialog.cpp
    settings/navigation/navigationsettingspage.cpp
    settings/services/servicessettingspage.cpp
    settings/settingspagebase.cpp
    settings/serviceitemdelegate.cpp
    settings/servicemodel.cpp
    settings/startup/startupsettingspage.cpp
    settings/trash/trashsettingspage.cpp
    settings/viewmodes/dolphinfontrequester.cpp
    settings/viewmodes/viewsettingspage.cpp
    settings/viewmodes/viewmodesettings.cpp
    settings/viewmodes/viewsettingstab.cpp
    statusbar/dolphinstatusbar.cpp
    statusbar/statusbarspaceinfo.cpp
    views/zoomlevelinfo.cpp
)

kde4_add_kcfg_files(dolphin_SRCS
    panels/folders/dolphin_folderspanelsettings.kcfgc
    panels/information/dolphin_informationpanelsettings.kcfgc
    panels/places/dolphin_placespanelsettings.kcfgc
    settings/dolphin_compactmodesettings.kcfgc
    settings/dolphin_detailsmodesettings.kcfgc
    settings/dolphin_generalsettings.kcfgc
    settings/dolphin_iconsmodesettings.kcfgc
    search/dolphin_searchsettings.kcfgc
    settings/dolphin_versioncontrolsettings.kcfgc
)

if(NOT WIN32)
   set(dolphin_SRCS ${dolphin_SRCS} panels/terminal/terminalpanel.cpp)
endif(NOT WIN32)

kde4_add_app_icon(dolphin_SRCS "${KDE4_ICON_INSTALL_DIR}/oxygen/*/apps/system-file-manager.png")

kde4_add_kdeinit_executable(dolphin ${dolphin_SRCS})

target_link_libraries(kdeinit_dolphin
    ${KDE4_KDEPRINT_LIBS}
    ${KDE4_KFILE_LIBS}
    ${KDE4_KPARTS_LIBS}
    ${KDE4_KCMUTILS_LIBRARY}
    konq
    dolphinprivate
    knewstuff3
    ${KDE4_SOLID_LIBS}
    ${KDE4_PHONON_LIBS}
)

if(HAVE_NEPOMUK)
    target_link_libraries(kdeinit_dolphin
        ${NEPOMUK_CORE_LIBRARY}
        ${NEPOMUK_WIDGETS_LIBRARY}
        ${SOPRANO_LIBRARIES}
    )
endif()

if (KActivities_FOUND)
    target_link_libraries(
        kdeinit_dolphin
        ${KACTIVITIES_LIBRARY}
    )
endif (KActivities_FOUND)

install(TARGETS kdeinit_dolphin ${INSTALL_TARGETS_DEFAULT_ARGS})
install(TARGETS dolphin ${INSTALL_TARGETS_DEFAULT_ARGS})

##########################################

set(kcm_dolphinviewmodes_PART_SRCS
    settings/kcm/kcmdolphinviewmodes.cpp
    settings/viewmodes/dolphinfontrequester.cpp
    settings/viewmodes/viewmodesettings.cpp
    settings/viewmodes/viewsettingstab.cpp
    views/zoomlevelinfo.cpp)

set(kcm_dolphinnavigation_PART_SRCS
    settings/kcm/kcmdolphinnavigation.cpp
    settings/navigation/navigationsettingspage.cpp
    settings/settingspagebase.cpp)

set(kcm_dolphinservices_PART_SRCS
    settings/kcm/kcmdolphinservices.cpp
    settings/services/servicessettingspage.cpp
    settings/settingspagebase.cpp
    settings/serviceitemdelegate.cpp
    settings/servicemodel.cpp)

set(kcm_dolphingeneral_PART_SRCS
    settings/kcm/kcmdolphingeneral.cpp
    settings/general/behaviorsettingspage.cpp
    settings/general/previewssettingspage.cpp
    settings/general/configurepreviewplugindialog.cpp
    settings/general/confirmationssettingspage.cpp
    settings/settingspagebase.cpp
    settings/serviceitemdelegate.cpp
    settings/servicemodel.cpp)

kde4_add_kcfg_files(kcm_dolphinviewmodes_PART_SRCS
    settings/dolphin_compactmodesettings.kcfgc
    settings/dolphin_directoryviewpropertysettings.kcfgc
    settings/dolphin_detailsmodesettings.kcfgc
    settings/dolphin_iconsmodesettings.kcfgc
    settings/dolphin_generalsettings.kcfgc
    settings/dolphin_versioncontrolsettings.kcfgc
)

kde4_add_kcfg_files(kcm_dolphinnavigation_PART_SRCS
    settings/dolphin_generalsettings.kcfgc)

kde4_add_kcfg_files(kcm_dolphinservices_PART_SRCS
    settings/dolphin_generalsettings.kcfgc
    settings/dolphin_versioncontrolsettings.kcfgc)

kde4_add_kcfg_files(kcm_dolphingeneral_PART_SRCS
    settings/dolphin_generalsettings.kcfgc)

kde4_add_plugin(kcm_dolphinviewmodes ${kcm_dolphinviewmodes_PART_SRCS})
kde4_add_plugin(kcm_dolphinnavigation ${kcm_dolphinnavigation_PART_SRCS})
kde4_add_plugin(kcm_dolphinservices ${kcm_dolphinservices_PART_SRCS})
kde4_add_plugin(kcm_dolphingeneral ${kcm_dolphingeneral_PART_SRCS})

target_link_libraries(kcm_dolphinviewmodes  ${KDE4_KDEUI_LIBS} ${KDE4_KFILE_LIBS} dolphinprivate)
target_link_libraries(kcm_dolphinnavigation  ${KDE4_KDEUI_LIBS} ${KDE4_KFILE_LIBS} dolphinprivate)
target_link_libraries(kcm_dolphinservices  ${KDE4_KDEUI_LIBS} ${KDE4_KFILE_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KNEWSTUFF3_LIBRARY} dolphinprivate)
target_link_libraries(kcm_dolphingeneral ${KDE4_KDEUI_LIBS} ${KDE4_KFILE_LIBS} ${KDE4_KIO_LIBS} dolphinprivate)
if(HAVE_NEPOMUK)
  target_link_libraries(kcm_dolphinviewmodes ${NEPOMUK_CORE_LIBRARY} ${NEPOMUK_WIDGETS_LIBRARY})
  target_link_libraries(kcm_dolphinnavigation ${NEPOMUK_CORE_LIBRARY} ${NEPOMUK_WIDGETS_LIBRARY})
  target_link_libraries(kcm_dolphinservices ${NEPOMUK_CORE_LIBRARY} ${NEPOMUK_WIDGETS_LIBRARY})
  target_link_libraries(kcm_dolphingeneral ${NEPOMUK_CORE_LIBRARY} ${NEPOMUK_WIDGETS_LIBRARY})
endif()

install(TARGETS kcm_dolphinviewmodes DESTINATION ${PLUGIN_INSTALL_DIR} )
install(TARGETS kcm_dolphinnavigation DESTINATION ${PLUGIN_INSTALL_DIR} )
install(TARGETS kcm_dolphinservices DESTINATION ${PLUGIN_INSTALL_DIR} )
install(TARGETS kcm_dolphingeneral DESTINATION ${PLUGIN_INSTALL_DIR} )

#########################################

set(kio_search_PART_SRCS
    search/filenamesearchprotocol.cpp)
kde4_add_plugin(kio_filenamesearch ${kio_search_PART_SRCS})
target_link_libraries(kio_filenamesearch ${KDE4_KIO_LIBS})
install(TARGETS kio_filenamesearch DESTINATION ${PLUGIN_INSTALL_DIR})

########### install files ###############

install( PROGRAMS dolphin.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES settings/dolphin_directoryviewpropertysettings.kcfg
               settings/dolphin_generalsettings.kcfg
               settings/dolphin_compactmodesettings.kcfg
               settings/dolphin_iconsmodesettings.kcfg
               settings/dolphin_detailsmodesettings.kcfg
               settings/dolphin_versioncontrolsettings.kcfg
         DESTINATION ${KCFG_INSTALL_DIR} )
install( FILES dolphinui.rc DESTINATION ${DATA_INSTALL_DIR}/dolphin )
install( FILES search/filenamesearch.protocol DESTINATION ${SERVICES_INSTALL_DIR} )
install( FILES settings/kcm/kcmdolphinviewmodes.desktop DESTINATION
${SERVICES_INSTALL_DIR} )
install( FILES settings/kcm/kcmdolphinnavigation.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
install( FILES settings/kcm/kcmdolphinservices.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
install( FILES settings/kcm/kcmdolphingeneral.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
install( FILES settings/services/servicemenu.knsrc DESTINATION ${CONFIG_INSTALL_DIR} )
install( PROGRAMS settings/services/servicemenuinstallation DESTINATION ${BIN_INSTALL_DIR} )
install( PROGRAMS settings/services/servicemenudeinstallation DESTINATION ${BIN_INSTALL_DIR} )

