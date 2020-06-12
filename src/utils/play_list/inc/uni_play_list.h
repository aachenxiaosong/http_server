/**************************************************************************
 * Copyright (C) 2017-2017  Unisound
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 **************************************************************************
 *
 * Description : uni_play_list.h
 * Author      : wufangfang@unisound.com
 * Date        : 2019.05.21
 *
 **************************************************************************/
#ifndef UNI_PLAY_LIST_H_
#define UNI_PLAY_LIST_H_

#include "uni_iot.h"
#include "uni_json.h"
#include "uni_auto_string.h"

/* define page size */
#define MAX_PAGE_SONGS  30

typedef void * PLAY_LIST_HANDLE;

/* Play list source type */
typedef enum {
  PLAY_LIST_NEWS = 0,
  PLAY_LIST_MUSIC,
  PLAY_LIST_AUDIO,
  PLAY_LIST_POEM,
  PLAY_LIST_BROADCAST,
  PLAY_LIST_TYPE_INVALID
} PlayListType;


/* Play mode for next song */
typedef enum {
  PLAY_SINGLE_LOOP = 0,
  PLAY_LIST_ORDER,
  PLAY_LIST_LOOP,
  PLAY_LIST_SHUFFLED,
  PLAY_LIST_MODE_INVALID
} PlayListMode;

/* Play list intent, used to load page list */
/* Need not send to server if the string is NULL */
/* http://wiki.it.yzs.io:8090/pages/viewpage.action?pageId=24733671 */
typedef struct {
  STRING_HANDLE   song;
  STRING_HANDLE   artist;
  STRING_HANDLE   genre;
  STRING_HANDLE   tag;
  STRING_HANDLE   mood;
  STRING_HANDLE   scene;
  STRING_HANDLE   billboard;
  STRING_HANDLE   language;
  STRING_HANDLE   lyrics;
  STRING_HANDLE   songlist;
  STRING_HANDLE   keyword;
  STRING_HANDLE   album;
  STRING_HANDLE   artistType;
  STRING_HANDLE   ktvMode;
  STRING_HANDLE   artistId;
}PlayListIntent;

/* Song's info, it's a item of play list */
/* http://wiki.it.yzs.io:8090/pages/viewpage.action?pageId=30014018 */
typedef struct {
  STRING_HANDLE   musicId;
  STRING_HANDLE   name;
  STRING_HANDLE   artist;
  STRING_HANDLE   artistId;
  STRING_HANDLE   album;
  STRING_HANDLE   albumId;
  int             duration;
  int             online;
  STRING_HANDLE   format;
  STRING_HANDLE   pic;
  STRING_HANDLE   pic140;
  STRING_HANDLE   title;
  STRING_HANDLE   url;
  STRING_HANDLE   channelId;
  STRING_HANDLE   linkUrl;
  int             linkFlag;
}PlayListItemInfo;

typedef enum {
  SONG_SWITCH_NEXT = 0,
  SONG_SWITCH_PREV,
} SongSwitch;

/* callback function that will be called when need next page */
typedef cJSON* (*_cb_load_new_page)(cJSON *search, PlayListType type);
/* callback function that will be called when xiamalay switch next/prev song */
typedef cJSON* (*_cb_switch_song)(PlayListType type, SongSwitch ctl);
/* callback function that will be called when play list was refreshed */
typedef Result (*_cb_play_list_refreshed)(void);
/* callback function that will be called when switch to previous song but current song is first one */
typedef Result (*_cb_no_more_prv_song)(void);
/* callback function that will be called when switch to next song but current song is last one */
typedef Result (*_cb_no_more_next_song)(void);

/**
 * Usage:   Creat a play list and initialize it
 * Params:  p_cb_load:          callback function
 *          p_cb_refreshed:     callback function
 *          p_cb_no_prv_song:   callback function
 *          p_cb_no_next_song:  callback function
 * Return:  Handle of play list
 */
PLAY_LIST_HANDLE PlayListCreat(_cb_load_new_page p_cb_load,
                                  _cb_switch_song p_cb_switch_song,
                                  _cb_play_list_refreshed p_cb_refreshed,
                                  _cb_no_more_prv_song p_cb_no_prv_song,
                                  _cb_no_more_next_song p_cb_no_next_song);

/**
 * Usage:   Destory the play list
 * Params:  handle:   handle of play list
 * Return:  None
 */
void PlayListDestory(PLAY_LIST_HANDLE handle);

/**
 * Usage:   Set play mode
 * Params:  handle:   handle of play list
 *          mode:     play mode
 * Return:  Result of set
 */
Result PlayListSetMode(PLAY_LIST_HANDLE handle, PlayListMode mode);

/**
 * Usage:   Get play mode
 * Params:  handle:   handle of play list
 * Return:  Play mode
 */
PlayListMode PlayListGetMode(PLAY_LIST_HANDLE handle);

/**
 * Usage:   Parse content info that from TR service
 * Params:  handle:     handle of play list
 *          content:    result of speech recognition
 * Return:  Result of inject
 */
Result PlayListParseTrResult(PLAY_LIST_HANDLE handle, cJSON *content);

/**
 * Usage:   Parse content info that APP push
 * Params:  handle:     handle of play list
 *          content:    play list data that APP push
 *          type:       type if play list data
 * Return:  Result of inject
 */
Result PlayListParsePullResult(PLAY_LIST_HANDLE handle, cJSON *content,
                                      PlayListType type);

/**
 * Usage:   Set play list search condition (used for play ctrl from APP)
 * Params:  handle:   handle of play list
 *          cond:     search condition, come from APP
 * Return:  Result of set
 */
Result PlayListSetSearchCondition(PLAY_LIST_HANDLE handle, cJSON *cond);

/**
 * Usage:   Clear the play list
 * Params:  handle:   handle of play list
 * Return:  Result of clear
 */
Result PlayListClear(PLAY_LIST_HANDLE handle);

/**
 * Usage:   Get data type of play list
 * Params:  handle:   handle of play list
 * Return:  Data type
 */
PlayListType PlayListGetType(PLAY_LIST_HANDLE handle);

/**
 * Usage:   Get total number of play list
 * Params:  handle:   handle of play list
 * Return:  Total number of play list
 */
int PlayListGetTotalNumber(PLAY_LIST_HANDLE handle);

/**
 * Usage:   Get service of play list
 * Params:  handle:   handle of play list
 * Return:  Service domain
 */
char* PlayListGetService(PLAY_LIST_HANDLE handle);

/**
 * Usage:   Get opration code of play list
 * Params:  handle:   handle of play list
 * Return:  Opration code
 */
char* PlayListGetOpcode(PLAY_LIST_HANDLE handle);

/**
 * Usage:   Get page index of play list
 * Params:  handle:   handle of play list
 * Return:  Page index of play list
 */
int PlayListGetPageIndex(PLAY_LIST_HANDLE handle);

/**
 * Usage:   Get page size of play list
 * Params:  handle:   handle of play list
 * Return:  Page size of play list
 */
int PlayListGetPageSize(PLAY_LIST_HANDLE handle);

/**
 * Usage:   Get number of play list
 * Params:  handle:   handle of play list
 * Return:  Number of play list
 */
int PlayListGetSongsNum(PLAY_LIST_HANDLE handle);

/**
 * Usage:   Creat song info with JSON type
 * Params:  item: info of song item
 * Return:  JSON node of list, you need to free it when you use it done
 */
cJSON* PlayListCreatSongJson(PlayListItemInfo *item);

/**
 * Usage:   Creat play list info with JSON type
 * Params:  handle:   handle of play list
 * Return:  JSON node of list, you need to free it when you use it done
 */
cJSON* PlayListCreatListJson(PLAY_LIST_HANDLE handle);

/**
 * Usage:   Creat audio list info with JSON type (will be abandoned in the future)
 * Params:  handle:   handle of play list
 * Return:  JSON node of list, you need to free it when you use it done
 */
cJSON* PlayListCreatAudioListJson(PLAY_LIST_HANDLE handle);

/**
 * Usage:   Creat search info with JSON type for APP play ctrl
 * Params:  handle:   handle of play list
 * Return:  JSON node of search, you need to free it when you use it done
 */
cJSON* PlayListCreatSearchJson(PLAY_LIST_HANDLE handle);

/**
 * Usage:   Switch to next song
 * Params:  handle:   handle of play list
 * Return:  Number of new song in list, -1 means no more songs
 */
int PlayListSwitchNext(PLAY_LIST_HANDLE handle);

/**
 * Usage:   Switch to next song when play end
 * Params:  handle:   handle of play list
 * Return:  Number of new song in list, -1 means no more songs
 */
int PlayListAutoNext(PLAY_LIST_HANDLE handle);

/**
 * Usage:   Switch to prev song
 * Params:  handle:   handle of play list
 * Return:  Number of new song in list, -1 means no more songs
 */
int PlayListSwitchPrev(PLAY_LIST_HANDLE handle);

/**
 * Usage:   Switch to specified sonng
 * Params:  handle:   handle of play list
 *          id:       id of specified sonng
 * Return:  Number of new song in list, -1 means cannot found it in list
 */
int PlayListSwitchSelect(PLAY_LIST_HANDLE handle, char *id);

/**
 * Usage:   Get current song content
 * Params:  handle:   handle of play list
 * Return:  Content of song, not include URL info, NULL means failed
 *          Don't free it, you can get songID from it and modify URL info of it
 */
PlayListItemInfo* PlayListGetCurSong(PLAY_LIST_HANDLE handle);

Result PlayListMusicLoadRandomPage(PLAY_LIST_HANDLE handle);

#endif  //  UNI_PLAY_LIST_H_
