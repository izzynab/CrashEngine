#pragma once

#include "IconsFontAwesome5.h"

// widget
// filter combobox width
//#define FILTER_COMBO_WIDTH 120.0f
// button widget use for compose path
//#define IMGUI_PATH_BUTTON ImGui::Button
// standar button
//#define IMGUI_BUTTON ImGui::Button


//#define fileNameString "File Name : "
//#define buttonResetSearchString "Reset search"
//#define buttonDriveString "Drives"
//#define buttonResetPathString "Reset to current directory"
//#define buttonCreateDirString "Create Directory"

/*
Comment theses line if you not want to have customization, like icon font here
*/

#define USE_THUMBNAILS

// locales string
#define createDirButtonString ICON_FA_CALENDAR_PLUS
#define okButtonString ICON_FA_CHECK_SQUARE " OK"
#define cancelButtonString ICON_FA_WINDOW_CLOSE " Cancel"
#define dirEntryString ICON_FA_FOLDER
#define OverWriteDialogConfirmButtonString ICON_FA_CHECK_SQUARE " Confirm"
#define OverWriteDialogCancelButtonString ICON_FA_WINDOW_CLOSE " Cancel"

// see strftime functionin <ctime> for customize
// "%Y/%m/%d %H:%M:%S" give 2021:01:22 11:47:10
// "%Y/%m/%d %i:%M%p" give 2021:01:22 11:45PM
//#define DateTimeFormat "%Y/%m/%d %i:%M%p"

// theses icons will appear in table headers
#define USE_CUSTOM_SORTING_ICON
//#define tableHeaderAscendingIcon ICON_IGFD_CHEVRON_UP
//#define tableHeaderDescendingIcon ICON_IGFD_CHEVRON_DOWN
#define tableHeaderFileNameString " File name"
#define tableHeaderFileTypeString " Type"
#define tableHeaderFileSizeString " Size"
#define tableHeaderFileDateString " Date"

#define USE_BOOKMARK
//#define bookmarkPaneWith 150.0f
//#define IMGUI_TOGGLE_BUTTON ToggleButton
//#define bookmarksButtonString ICON_IGFD_BOOKMARK
//#define bookmarksButtonHelpString "bookmark"
#define addBookmarkButtonString ICON_FA_CALENDAR_PLUS
#define removeBookmarkButtonString ICON_FA_WINDOW_CLOSE
