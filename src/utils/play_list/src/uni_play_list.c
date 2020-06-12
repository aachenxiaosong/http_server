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
 * Description : uni_play_list.c
 * Author      : wufangfang@unisound.com
 * Date        : 2019.05.21
 *
 **************************************************************************/

#include "uni_play_list.h"
#include "uni_log.h"
#include "uni_string.h"

#define PLAY_LIST_TAG "play_list"

/* weather PLAY_LIST_ORDER deffrent with PLAY_LIST_LOOP */
/* Confirm with the product manager that the sequential
   playback is consistent with the list loop */
#define IS_LIST_ORDER_LOOP_DIFF 0

/* max parse string length */
#define MAX_URL_LENGTH        (1024)
#define MAX_TITLE_LENGTH      (128)
#define MAX_ARTIST_LENGTH     (128)
#define MAX_ALBUM_LENGTH      (128)
#define MAX_ID_LENGTH         (128)

/* suffle arry used for shuffled play mode */
typedef struct {
  int                     *arry;      // rand arry for shuffled play mode
  int                     arry_size;  // arry size
  int                     total_num;  // there is how many songs
  int                     index;      // current song index
}SuffleArry;

typedef struct {
  PlayListType            type;                 // source type of list
  PlayListMode            mode;                 // play mode for next song
  PlayListItemInfo        list[MAX_PAGE_SONGS]; // data of play list
  int                     list_len;             // total number of songs in list
  int                     cur_index;            // current play song index of list
  SuffleArry              suff_arry;            // suffle arry used for shuffled play mode
  int                     total_songs;          // total number of all songs in server
  int                     page_index;           // current page index of all songs pages
  int                     page_size;            // total number of songs on one page
  PlayListIntent          intent;               // intent fo play list
  STRING_HANDLE           service;              // service of new play list
  STRING_HANDLE           opcode;               // opration code of new play list
  STRING_HANDLE           text;                 // come from TR result
  _cb_load_new_page       cb_load;              // callback function for load new page
  _cb_switch_song         cb_switch_song;        // callback function for load new song
  _cb_play_list_refreshed cb_refreshed;         // callback function for push play list
  _cb_no_more_prv_song    cb_no_prv_song;       // callback function for notify no more songs
  _cb_no_more_next_song   cb_no_next_song;      // callback function for notify no more songs
}PlayListContext;

static void _creat_intent(PlayListContext *p_context){
  if (p_context) {
    p_context->intent.album = StringCreat("");
    p_context->intent.artist = StringCreat("");
    p_context->intent.artistType = StringCreat("");
    p_context->intent.billboard = StringCreat("");
    p_context->intent.genre = StringCreat("");
    p_context->intent.keyword = StringCreat("");
    p_context->intent.ktvMode = StringCreat("");
    p_context->intent.language = StringCreat("");
    p_context->intent.lyrics = StringCreat("");
    p_context->intent.mood = StringCreat("");
    p_context->intent.scene = StringCreat("");
    p_context->intent.song = StringCreat("");
    p_context->intent.songlist = StringCreat("");
    p_context->intent.tag = StringCreat("");
    p_context->intent.artistId = StringCreat("");
  }
}

static void _clear_intent(PlayListContext *p_context) {
  if (p_context) {
    StringAssignment(p_context->intent.album, "");
    StringAssignment(p_context->intent.artist, "");
    StringAssignment(p_context->intent.artistType, "");
    StringAssignment(p_context->intent.billboard, "");
    StringAssignment(p_context->intent.genre, "");
    StringAssignment(p_context->intent.keyword, "");
    StringAssignment(p_context->intent.ktvMode, "");
    StringAssignment(p_context->intent.language, "");
    StringAssignment(p_context->intent.lyrics, "");
    StringAssignment(p_context->intent.mood, "");
    StringAssignment(p_context->intent.scene, "");
    StringAssignment(p_context->intent.song, "");
    StringAssignment(p_context->intent.songlist, "");
    StringAssignment(p_context->intent.tag, "");
    StringAssignment(p_context->intent.artistId, "");
  }
}

static void _destory_intent(PlayListContext *p_context) {
  if (p_context) {
    StringDestroy(p_context->intent.album);
    StringDestroy(p_context->intent.artist);
    StringDestroy(p_context->intent.artistType);
    StringDestroy(p_context->intent.billboard);
    StringDestroy(p_context->intent.genre);
    StringDestroy(p_context->intent.keyword);
    StringDestroy(p_context->intent.ktvMode);
    StringDestroy(p_context->intent.language);
    StringDestroy(p_context->intent.lyrics);
    StringDestroy(p_context->intent.mood);
    StringDestroy(p_context->intent.scene);
    StringDestroy(p_context->intent.song);
    StringDestroy(p_context->intent.songlist);
    StringDestroy(p_context->intent.tag);
    StringDestroy(p_context->intent.artistId);
  }
}

static void _creat_play_list(PlayListContext *p_context) {
  int i = 0;
  if (p_context) {
    for (i=0; i<MAX_PAGE_SONGS; i++) {
      p_context->list[i].album = StringCreat("");
      p_context->list[i].albumId = StringCreat("");
      p_context->list[i].artist = StringCreat("");
      p_context->list[i].artistId = StringCreat("");
      p_context->list[i].format = StringCreat("");
      p_context->list[i].musicId = StringCreat("");
      p_context->list[i].name = StringCreat("");
      p_context->list[i].pic = StringCreat("");
      p_context->list[i].pic140 = StringCreat("");
      p_context->list[i].title = StringCreat("");
      p_context->list[i].url = StringCreat("");
      p_context->list[i].channelId = StringCreat("");
      p_context->list[i].linkUrl = StringCreat("");
      p_context->list[i].linkFlag = 0;
      p_context->list[i].duration = 0;
      p_context->list[i].online = 0;
    }
  }
}

static void _clear_play_list(PlayListContext *p_context) {
  int i = 0;
  if (p_context) {
    for (i=0; i<MAX_PAGE_SONGS; i++) {
      StringAssignment(p_context->list[i].album, "");
      StringAssignment(p_context->list[i].albumId, "");
      StringAssignment(p_context->list[i].artist, "");
      StringAssignment(p_context->list[i].artistId, "");
      StringAssignment(p_context->list[i].format, "");
      StringAssignment(p_context->list[i].musicId, "");
      StringAssignment(p_context->list[i].name, "");
      StringAssignment(p_context->list[i].pic, "");
      StringAssignment(p_context->list[i].pic140, "");
      StringAssignment(p_context->list[i].title, "");
      StringAssignment(p_context->list[i].url, "");
      StringAssignment(p_context->list[i].channelId, "");
      StringAssignment(p_context->list[i].linkUrl, "");
      p_context->list[i].linkFlag = 0;
      p_context->list[i].duration = 0;
      p_context->list[i].online = 0;
    }
  }
}

static void _destory_play_list(PlayListContext *p_context) {
  int i = 0;
  if (p_context) {
    for (i=0; i<MAX_PAGE_SONGS; i++) {
      StringDestroy(p_context->list[i].album);
      StringDestroy(p_context->list[i].albumId);
      StringDestroy(p_context->list[i].artist);
      StringDestroy(p_context->list[i].artistId);
      StringDestroy(p_context->list[i].format);
      StringDestroy(p_context->list[i].musicId);
      StringDestroy(p_context->list[i].name);
      StringDestroy(p_context->list[i].pic);
      StringDestroy(p_context->list[i].pic140);
      StringDestroy(p_context->list[i].title);
      StringDestroy(p_context->list[i].url);
      StringDestroy(p_context->list[i].channelId);
      StringDestroy(p_context->list[i].linkUrl);
      p_context->list[i].linkFlag = 0;
      p_context->list[i].duration = 0;
      p_context->list[i].online = 0;
    }
  }
}

static void _init_suffle_arry(PlayListContext *p_context){
  if (p_context) {
    if (NULL == p_context->suff_arry.arry) {
      srand(time(NULL));
      p_context->suff_arry.arry = (int *)uni_malloc(sizeof(int) * MAX_PAGE_SONGS);
      if (p_context->suff_arry.arry) {
        p_context->suff_arry.arry_size = MAX_PAGE_SONGS;
        p_context->suff_arry.index = -1;
        p_context->suff_arry.total_num = 0;
      } else {
        p_context->suff_arry.arry_size = 0;
        p_context->suff_arry.index = -1;
        p_context->suff_arry.total_num = 0;
      }
    }
  }
}

static void _swop_number(int *a, int *b){
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

static void _suffle_arry(int *arry, int total_num, int page_len){
  int i = 0;
  int index = 0;
  if (arry) {
    /* init sequence number */
    for (i=0; i<total_num; i++) {
      arry[i] = i;
    }
    /* the first number must be in the first page to play immediately */
    index = rand() % page_len;
    _swop_number(&arry[0], &arry[index]);
    /* shuffle other number */
    for (i=1; i<total_num; i++) {
      index = rand() % (total_num - i) + i;
      _swop_number(&arry[i], &arry[index]);
    }
  }
}

static void _refresh_suffle_arry(PlayListContext *p_context,
                                        int total_num, int page_len){
  if (p_context) {
    if (total_num > p_context->suff_arry.arry_size) {
      if (p_context->suff_arry.arry) {
        uni_free(p_context->suff_arry.arry);
      }
      p_context->suff_arry.arry = (int *)uni_malloc(sizeof(int) * total_num);
      if (p_context->suff_arry.arry) {
        p_context->suff_arry.arry_size = total_num;
        p_context->suff_arry.index = 0;
        p_context->suff_arry.total_num = total_num;
        _suffle_arry(p_context->suff_arry.arry, total_num, page_len);
      }
    } else {
      if (p_context->suff_arry.arry) {
        p_context->suff_arry.index = 0;
        p_context->suff_arry.total_num = total_num;
        _suffle_arry(p_context->suff_arry.arry, total_num, page_len);
      }
    }
  }
}

static void _final_suffle_arry(PlayListContext *p_context){
  if (p_context) {
    if (p_context->suff_arry.arry) {
      uni_free(p_context->suff_arry.arry);
      p_context->suff_arry.arry = NULL;
      p_context->suff_arry.arry_size = 0;
      p_context->suff_arry.index = -1;
      p_context->suff_arry.total_num = 0;
    }
  }
}

PLAY_LIST_HANDLE PlayListCreat(_cb_load_new_page p_cb_load,
                                  _cb_switch_song p_cb_switch_song,
                                  _cb_play_list_refreshed p_cb_refreshed,
                                  _cb_no_more_prv_song p_cb_no_prv_song,
                                  _cb_no_more_next_song p_cb_no_next_song){
  PlayListContext *p_context = NULL;

  p_context = (PlayListContext *)uni_malloc(sizeof(PlayListContext));
  if (p_context) {
    uni_memset(p_context, 0, sizeof(PlayListContext));
    p_context->cb_load = p_cb_load;
    p_context->cb_switch_song = p_cb_switch_song;
    p_context->cb_refreshed = p_cb_refreshed;
    p_context->cb_no_next_song = p_cb_no_next_song;
    p_context->cb_no_prv_song = p_cb_no_prv_song;
    p_context->cur_index = -1;
    p_context->list_len = 0;
    p_context->mode = PLAY_LIST_MODE_INVALID;
    p_context->page_index = -1;
    p_context->page_size = MAX_PAGE_SONGS;
    p_context->total_songs = 0;
    p_context->type = PLAY_LIST_TYPE_INVALID;
    p_context->service = StringCreat("");
    p_context->opcode = StringCreat("");
    p_context->text = StringCreat("");
    _creat_intent(p_context);
    _creat_play_list(p_context);
    _init_suffle_arry(p_context);
  }
  return (PLAY_LIST_HANDLE)p_context;
}

void PlayListDestory(PLAY_LIST_HANDLE handle){
  PlayListContext *p_context = (PlayListContext *)handle;
  if (p_context) {
    StringDestroy(p_context->text);
    StringDestroy(p_context->service);
    StringDestroy(p_context->opcode);
    _destory_intent(p_context);
    _destory_play_list(p_context);
    _final_suffle_arry(p_context);
    uni_free(p_context);
    p_context = NULL;
  }
}

Result PlayListSetMode(PLAY_LIST_HANDLE handle, PlayListMode mode){
  PlayListContext *p_context = (PlayListContext *)handle;
  if (p_context) {
    if (mode <= PLAY_LIST_MODE_INVALID) {
      p_context->mode = mode;
    }
    return E_OK;
  }
  return E_FAILED;
}

PlayListMode PlayListGetMode(PLAY_LIST_HANDLE handle){
  PlayListContext *p_context = (PlayListContext *)handle;
  if (p_context) {
    return p_context->mode;
  }
  return PLAY_LIST_MODE_INVALID;
}

static Result _parse_news_item(PlayListItemInfo *p_item, cJSON *item) {
  cJSON *audioUrl = NULL;
  cJSON *title = NULL;
  cJSON *artist = NULL;
  char tmp_str[MAX_URL_LENGTH] = {0};

  if (p_item) {
    audioUrl = cJSON_GetObjectItem(item, CJSON_OBJECT_NAME(audioUrl));
    title = cJSON_GetObjectItem(item, CJSON_OBJECT_NAME(title));
    artist = cJSON_GetObjectItem(item, CJSON_OBJECT_NAME(artist));
    if (audioUrl && audioUrl->valuestring) {
      uni_memset(tmp_str, 0, MAX_URL_LENGTH);
      uni_snprintf(tmp_str, MAX_URL_LENGTH, "http%s",
                  uni_strstr(audioUrl->valuestring, "://"));
      StringAssignment(p_item->url, tmp_str);
    }
    if (title && title->valuestring) {
      StringAssignment(p_item->title, title->valuestring);
    }
    if (artist && artist->valuestring) {
      StringAssignment(p_item->artist, artist->valuestring);
    }
    return E_OK;
  }
  return E_FAILED;
}

static Result _parse_tr_result_news(PlayListContext *p_context,
                                          cJSON *result) {
  cJSON *news = NULL;
  cJSON *item = NULL;
  int i = 0, size = 0;

  if (p_context) {
    if (NULL == (news = cJSON_GetObjectItem(result, CJSON_OBJECT_NAME(news)))) {
      LOGE(PLAY_LIST_TAG, "result not exist");
      return E_FAILED;
    }
    size = cJSON_GetArraySize(news);
    if (0 == size) {
      LOGW(PLAY_LIST_TAG, "new list empty, return");
      return E_FAILED;
    }
    p_context->list_len = size;
    p_context->page_index = 0;
    p_context->total_songs = size;

    /* clear play list */
    _clear_play_list(p_context);
    /* parse play list info */
    for (i = 0; i < p_context->list_len; i++) {
      item = cJSON_GetArrayItem(news, i);
      _parse_news_item(&p_context->list[i], item);
    }
    return E_OK;
  }
  return E_FAILED;
}

static const char* _select_song_format(const char *arry) {
  if (arry) {
    if (uni_strstr(arry, "128kmp3")) {
      return "128kmp3";
    } else {
      return NULL;
    }
  }
  return NULL;
}

static Result _parse_music_item(PlayListItemInfo *p_item, cJSON *item) {
  cJSON *temp = NULL;
  char tmp_str[MAX_URL_LENGTH] = {0};

  if (p_item) {
    temp = cJSON_GetObjectItem(item, CJSON_OBJECT_NAME(id));
    if (temp) {
      LOGT(PLAY_LIST_TAG, "get music id:%lld",temp->valueint); 
      uni_memset(tmp_str, 0, MAX_URL_LENGTH);
      uni_snprintf(tmp_str, MAX_URL_LENGTH,"%lld",temp->valueint);
      StringAssignment(p_item->musicId, tmp_str);
    }
    
    temp = cJSON_GetObjectItem(item, CJSON_OBJECT_NAME(name));
    if (temp && temp->valuestring) {
      StringAssignment(p_item->name, temp->valuestring);
    }
    temp = cJSON_GetObjectItem(item, CJSON_OBJECT_NAME(artist));
    if (temp && temp->valuestring) {
      StringAssignment(p_item->artist, temp->valuestring);
    }
    temp = cJSON_GetObjectItem(item, CJSON_OBJECT_NAME(artistId));
    if (temp) {
      uni_memset(tmp_str, 0, MAX_URL_LENGTH);
      uni_snprintf(tmp_str, MAX_URL_LENGTH,"%lld",temp->valueint);
      StringAssignment(p_item->artistId, tmp_str);
    }
    temp = cJSON_GetObjectItem(item, CJSON_OBJECT_NAME(album));
    if (temp && temp->valuestring) {
      StringAssignment(p_item->album, temp->valuestring);
    }
    temp = cJSON_GetObjectItem(item, CJSON_OBJECT_NAME(albumId));
    if (temp) {
      uni_memset(tmp_str, 0, MAX_URL_LENGTH);
      uni_snprintf(tmp_str, MAX_URL_LENGTH,"%lld",temp->valueint);
      StringAssignment(p_item->albumId, tmp_str);
    }
    temp = cJSON_GetObjectItem(item, CJSON_OBJECT_NAME(duration));
    if (temp) {
      p_item->duration = temp->valueint;
    }
    temp = cJSON_GetObjectItem(item, CJSON_OBJECT_NAME(online));
    if (temp) {
      p_item->online = temp->valueint;
    }
    temp = cJSON_GetObjectItem(item, CJSON_OBJECT_NAME(format));
    if (temp && temp->valuestring) {
      StringAssignment(p_item->format, _select_song_format(temp->valuestring));
    }
    temp = cJSON_GetObjectItem(item, CJSON_OBJECT_NAME(pic300));
    if (temp && temp->valuestring) {
      uni_memset(tmp_str, 0, MAX_URL_LENGTH);
      uni_snprintf(tmp_str, MAX_URL_LENGTH, "http%s",
                  uni_strstr(temp->valuestring, "://"));
      StringAssignment(p_item->pic, tmp_str);
    }
    temp = cJSON_GetObjectItem(item, CJSON_OBJECT_NAME(pic500));
    if (temp && temp->valuestring) {
      uni_memset(tmp_str, 0, MAX_URL_LENGTH);
      uni_snprintf(tmp_str, MAX_URL_LENGTH, "http%s",
                  uni_strstr(temp->valuestring, "://"));
      StringAssignment(p_item->pic140, tmp_str);
    }
    return E_OK;
  }
  return E_FAILED;
}

static Result _parse_data_number(PlayListContext *p_context,
                                              cJSON *data){
  int size = 0;

  if (p_context) {
    size = cJSON_GetArraySize(data);
    if (0 == size) {
      LOGW(PLAY_LIST_TAG, "audio list empty, return");
      return E_FAILED;
    }
    if (size > MAX_PAGE_SONGS) {
      size = MAX_PAGE_SONGS;
    }
    p_context->total_songs = size;
    p_context->list_len = size;
    p_context->page_index = 1;
    LOGW(PLAY_LIST_TAG, "data num,total:%d len:%d,index:%d",
                        p_context->total_songs,
                        p_context->list_len,
                        p_context->page_index);
    return E_OK;
  }
  return E_FAILED;
}

static Result _parse_tr_result_music(PlayListContext *p_context,
                                            cJSON *data) {
  cJSON *items = NULL;
  cJSON *item = NULL;
  int i = 0;


  if (p_context) {
    if (NULL == data) {
      LOGE(PLAY_LIST_TAG, "data not exist");
      return E_FAILED;
    }

    if (E_OK != _parse_data_number(p_context, data)) {
      LOGE(PLAY_LIST_TAG, "data parse failed");
      return E_FAILED;
    }
    /* parse play list info */
    items = data;
    /* clear play list */
    _clear_play_list(p_context);
    for (i = 0; i < p_context->list_len; i++) {
      item = cJSON_GetArrayItem(items, i);
      if(item == NULL) {
         LOGE(PLAY_LIST_TAG, "item not exist");
      }
      _parse_music_item(&p_context->list[i], item);
    }
    return E_OK;
  }
  return E_FAILED;
}

static Result _parse_audio_item(PlayListItemInfo *p_item, cJSON *item) {
  cJSON *title = NULL;
  cJSON *id = NULL;
  cJSON *cover = NULL;
  cJSON *artist =NULL;
  cJSON *albumId = NULL;
  cJSON *albumTitle = NULL;
  cJSON *url = NULL;

  if (p_item) {
    title = cJSON_GetObjectItem(item, CJSON_OBJECT_NAME(title));
    id = cJSON_GetObjectItem(item, CJSON_OBJECT_NAME(id));
    cover = cJSON_GetObjectItem(item, CJSON_OBJECT_NAME(cover));
    artist = cJSON_GetObjectItem(item, CJSON_OBJECT_NAME(artist));
    albumId = cJSON_GetObjectItem(item, CJSON_OBJECT_NAME(albumId));
    albumTitle = cJSON_GetObjectItem(item, CJSON_OBJECT_NAME(albumTitle));
    url = cJSON_GetObjectItem(item, CJSON_OBJECT_NAME(url));
    if (url && url->valuestring) {
      StringAssignment(p_item->url, url->valuestring);
    } else {
      LOGE(PLAY_LIST_TAG,"not find url");
      goto L_ERROR;
    }
    if (title && title->valuestring) {
      LOGT(PLAY_LIST_TAG, "audio title:%s", title->valuestring);
      StringAssignment(p_item->name, title->valuestring);
    }
    if (id && id->valuestring) {
      StringAssignment(p_item->musicId, id->valuestring);
      LOGT(PLAY_LIST_TAG, "id:%s", id->valuestring);
    }
    if (cover && cover->valuestring) {
      StringAssignment(p_item->pic, cover->valuestring);
    }
    if (artist && artist->valuestring) {
      StringAssignment(p_item->artist, artist->valuestring);
    }
    if (albumId && albumId->valuestring) {
      StringAssignment(p_item->albumId, albumId->valuestring);
    }
    if (albumTitle && albumTitle->valuestring) {
      StringAssignment(p_item->album, albumTitle->valuestring);
    }
    return E_OK;
  }
L_ERROR:
  return E_FAILED;
}

static Result _parse_tr_result_audio(PlayListContext *p_context,
                                            cJSON *result) {
  cJSON *playlist = NULL;
  cJSON *item = NULL;
  int i = 0, size = 0;

  if (p_context) {
    playlist = cJSON_GetObjectItem(result, CJSON_OBJECT_NAME(playlist));
    if (NULL == playlist) {
      LOGE(PLAY_LIST_TAG, "playlist not exist");
      return E_FAILED;
    }
    size = cJSON_GetArraySize(playlist);
    if (0 == size) {
      LOGW(PLAY_LIST_TAG, "audio list empty, return");
      return E_FAILED;
    }
    p_context->list_len = size;
    p_context->page_index = 0;
    p_context->total_songs = size;

    /* clear play list */
    _clear_play_list(p_context);
    /* parse play list info */
    for (i = 0; i < p_context->list_len; i++) {
      item = cJSON_GetArrayItem(playlist, i);
      _parse_audio_item(&p_context->list[i], item);
    }
    return E_OK;
  }
  return E_FAILED;
}

static Result _parse_tr_result_poem(PlayListContext *p_context,
                                          cJSON *content) {
  cJSON *general = NULL;
  cJSON *style = NULL;
  cJSON *audio = NULL;

  if (p_context) {
    general = cJSON_GetObjectItem(content, CJSON_OBJECT_NAME(general));
    if (NULL == general) {
      LOGE(PLAY_LIST_TAG, "general not exist");
      return E_FAILED;
    }
    style = cJSON_GetObjectItem(general, CJSON_OBJECT_NAME(style));
    if (NULL == style || NULL == style->valuestring) {
      LOGE(PLAY_LIST_TAG, "style not exist");
      return E_FAILED;
    }
    if (0 != uni_strcmp(style->valuestring, "poem")) {
      LOGE(PLAY_LIST_TAG, "style[%s] not support", style->valuestring);
      return E_FAILED;
    }
    audio = cJSON_GetObjectItem(general, CJSON_OBJECT_NAME(audio));
    if (NULL == audio || NULL == audio->valuestring) {
      LOGE(PLAY_LIST_TAG, "poem audio not exist");
      return E_FAILED;
    }
    /* clear play list */
    _clear_play_list(p_context);
    /* parse data */
    StringAssignment(p_context->list[0].url, audio->valuestring);
    p_context->list_len = 1;
    p_context->page_index = 0;
    p_context->total_songs = 1;
    return E_OK;
  }
  return E_FAILED;
}

static Result _parse_intent(PlayListContext *p_context, cJSON *intent){
  cJSON *temp = NULL;
  if (p_context) {
    /* clear intent */
    _clear_intent(p_context);
    /* parse intent */
    temp = cJSON_GetObjectItem(intent, CJSON_OBJECT_NAME(song));
    if (temp && temp->valuestring) {
      StringAssignment(p_context->intent.song, temp->valuestring);
    }
    temp = cJSON_GetObjectItem(intent, CJSON_OBJECT_NAME(artist));
    if (temp && temp->valuestring) {
      StringAssignment(p_context->intent.artist, temp->valuestring);
    }
    temp = cJSON_GetObjectItem(intent, CJSON_OBJECT_NAME(genre));
    if (temp && temp->valuestring) {
      StringAssignment(p_context->intent.genre, temp->valuestring);
    }
    temp = cJSON_GetObjectItem(intent, CJSON_OBJECT_NAME(tag));
    if (temp && temp->valuestring) {
      StringAssignment(p_context->intent.tag, temp->valuestring);
    }
    temp = cJSON_GetObjectItem(intent, CJSON_OBJECT_NAME(mood));
    if (temp && temp->valuestring) {
      StringAssignment(p_context->intent.mood, temp->valuestring);
    }
    temp = cJSON_GetObjectItem(intent, CJSON_OBJECT_NAME(scene));
    if (temp && temp->valuestring) {
      StringAssignment(p_context->intent.scene, temp->valuestring);
    }
    temp = cJSON_GetObjectItem(intent, CJSON_OBJECT_NAME(billboard));
    if (temp && temp->valuestring) {
      StringAssignment(p_context->intent.billboard, temp->valuestring);
    }
    temp = cJSON_GetObjectItem(intent, CJSON_OBJECT_NAME(language));
    if (temp && temp->valuestring) {
      StringAssignment(p_context->intent.language, temp->valuestring);
    }
    temp = cJSON_GetObjectItem(intent, CJSON_OBJECT_NAME(lyrics));
    if (temp && temp->valuestring) {
      StringAssignment(p_context->intent.lyrics, temp->valuestring);
    }
    temp = cJSON_GetObjectItem(intent, CJSON_OBJECT_NAME(songlist));
    if (temp && temp->valuestring) {
      StringAssignment(p_context->intent.songlist, temp->valuestring);
    }
    temp = cJSON_GetObjectItem(intent, CJSON_OBJECT_NAME(keyword));
    if (temp && temp->valuestring) {
      StringAssignment(p_context->intent.keyword, temp->valuestring);
    }
    temp = cJSON_GetObjectItem(intent, CJSON_OBJECT_NAME(album));
    if (temp && temp->valuestring) {
      StringAssignment(p_context->intent.album, temp->valuestring);
    }
    temp = cJSON_GetObjectItem(intent, CJSON_OBJECT_NAME(artistType));
    if (temp && temp->valuestring) {
      StringAssignment(p_context->intent.artistType, temp->valuestring);
    }
    temp = cJSON_GetObjectItem(intent, CJSON_OBJECT_NAME(ktvMode));
    if (temp && temp->valuestring) {
      StringAssignment(p_context->intent.ktvMode, temp->valuestring);
    }
    temp = cJSON_GetObjectItem(intent, CJSON_OBJECT_NAME(artistId));
    if (temp && temp->valuestring) {
      StringAssignment(p_context->intent.artistId, temp->valuestring);
    }
    return E_OK;
  }
  return E_FAILED;
}

static inline void _set_string_value(STRING_HANDLE *key, const char *value,
                                     const char *default_value) {
  const char *val = value;
  if (key) {
    if (!val && default_value) {
      val = default_value;
    }
    if (val) {
      StringAssignment(key, val);
    }
  }
}

static Result _skill_parse_stream(PlayListItemInfo* item, cJSON *stream) {
  cJSON *temp = NULL;
  char *url = NULL;
  char *url_used = NULL;
  int int_temp = 0;
  if ((NULL == item) || (NULL == stream)) {
    LOGE(PLAY_LIST_TAG, "NULL param %p,%p", item, stream);
    return E_FAILED;
  }
  temp = cJSON_GetObjectItem(stream, CJSON_OBJECT_NAME(author));
  if (temp) {
    _set_string_value(item->artist, temp->valuestring, "");
  } else {
    StringAssignment(item->artist, "");
  }

  temp = cJSON_GetObjectItem(stream, CJSON_OBJECT_NAME(token));
  if (temp) {
    _set_string_value(item->musicId, temp->valuestring, "");
  } else {
    StringAssignment(item->musicId, "");
    LOGE(PLAY_LIST_TAG, "Not contain musicId(token)");
  }

  temp = cJSON_GetObjectItem(stream, CJSON_OBJECT_NAME(title));
  if (temp) {
    _set_string_value(item->title, temp->valuestring, "");
    _set_string_value(item->name, temp->valuestring, "");
  } else {
    StringAssignment(item->title, "");
    StringAssignment(item->name, "");
    LOGE(PLAY_LIST_TAG, "Not contain title");
  }
  if (JsonReadItemInt(stream, "stream.linkFlag", &int_temp) == 0) {
    item->linkFlag = int_temp;
  } else {
    item->linkFlag = 0;
  }
  if (JsonReadItemInt(stream, "stream.totalContentPlayTime", &int_temp) == 0) {
    item->duration = int_temp;
  }
  JsonReadItemString(stream, "stream.url", &url);
  if (url) {
#if UNI_HTTPS_ENABLE
    url_used = url;
#else
    if (0 == uni_strncmp(url, "https", 5)) {
      url[1] = 'h';
      url[2] = 't';
      url[3] = 't';
      url[4] = 'p';
       url_used = url+1;
    } else {
      url_used = url;
    }
#endif
    if (item->linkFlag) {
      _set_string_value(item->linkUrl, url_used, "");
    } else {
      _set_string_value(item->url, url_used, "");
    }
    uni_free(url);
  }
  return E_OK;
}

static void _dummy_page_data(PlayListContext *p_context, int total,
                             int page_size, int page_index) {
  if (p_context) {
     //skill support next page
    p_context->total_songs = total + 1;
    p_context->list_len = page_size;
    p_context->page_index = page_index;
    p_context->page_size = total;
    LOGT(PLAY_LIST_TAG, "list size %d", total);
  }
}

static Result _skill_news_parser(PlayListContext *p_context, cJSON *content) {
  cJSON* streams = NULL;
  char *device_type = NULL;
  const char *widget_audio = "Widget.AudioPlayer";

  if (JsonReadItemString(content, "semantic.intent.operations[0].deviceType",
                         &device_type) == 0 &&
      NULL != device_type &&
      uni_strncmp(widget_audio, device_type, uni_strlen(widget_audio)) == 0) {
    JsonReadItemObject(content, "data.data", &streams);
    uni_free(device_type);
  }

  if (streams) {
    int item_size = uni_min(cJSON_GetArraySize(streams), MAX_PAGE_SONGS);
    _dummy_page_data(p_context, item_size, item_size, 0);
    for (; item_size > 0; item_size--) {
      _skill_parse_stream(&(p_context->list[item_size - 1]),
                          cJSON_GetArrayItem(streams, item_size - 1));
    }
    cJSON_Delete(streams);
  } else {
    LOGT(PLAY_LIST_TAG, "News not contain steams");
    return E_FAILED;
  }

  return E_OK;
}

static Result _music_parse_content(PlayListContext *p_context,
                                        cJSON *content) {
  cJSON *code = NULL;
  cJSON *semantic = NULL;
  cJSON *intent = NULL;
  cJSON *data = NULL;

  if (NULL == p_context) {
    LOGE(PLAY_LIST_TAG, "play list not init");
    return E_FAILED;
  }
  /* parse opration code */
  code = cJSON_GetObjectItem(content, CJSON_OBJECT_NAME(code));
  if (NULL == code) {
    LOGE(PLAY_LIST_TAG, "code not exist");
    return E_FAILED;
  }
  LOGT(PLAY_LIST_TAG, "code:%s.", code->valuestring);
  StringAssignment(p_context->opcode, code->valuestring);
  /* parse intent */
  semantic = cJSON_GetObjectItem(content, CJSON_OBJECT_NAME(semantic));
  if (semantic) {
    intent = cJSON_GetObjectItem(semantic, CJSON_OBJECT_NAME(intent));
    if (intent) {
      _parse_intent(p_context, intent);
    }
  }
  /* parse result */
  data = cJSON_GetObjectItem(content, CJSON_OBJECT_NAME(data));
  if (NULL == data) {
    LOGE(PLAY_LIST_TAG, "data not exist");
    return E_FAILED;
  }

  return _parse_tr_result_music(p_context, data);
}

static Result _audio_parse_content(PlayListContext *p_context,
                                        cJSON *content) {
  cJSON *data = NULL;
  cJSON *result = NULL;

  if (NULL == p_context) {
    LOGE(PLAY_LIST_TAG, "play list not init");
    return E_FAILED;
  }
  /* parse result */
  data = cJSON_GetObjectItem(content, CJSON_OBJECT_NAME(data));
  if (NULL == data) {
    LOGE(PLAY_LIST_TAG, "data not exist");
    return E_FAILED;
  }
  result = cJSON_GetObjectItem(data, CJSON_OBJECT_NAME(result));
  if (NULL == result) {
    LOGE(PLAY_LIST_TAG, "result not exist");
    return E_FAILED;
  }
  return _parse_tr_result_audio(p_context, result);
}

static Result _news_parse_content(PlayListContext *p_context,
                                        cJSON *content) {
  cJSON *data = NULL;
  cJSON *result = NULL;

  if (NULL == p_context) {
    LOGE(PLAY_LIST_TAG, "play list not init");
    return E_FAILED;
  }
  data = cJSON_GetObjectItem(content, CJSON_OBJECT_NAME(data));
  if (NULL == data) {
    LOGE(PLAY_LIST_TAG, "data not exist");
    return E_FAILED;
  }
  result = cJSON_GetObjectItem(data, CJSON_OBJECT_NAME(result));
  if (NULL == result) {
    LOGE(PLAY_LIST_TAG, "result not exist");
    return E_FAILED;
  }
  return _parse_tr_result_news(p_context, result);
}

static Result _chat_parse_content(PlayListContext *p_context,
                                        cJSON *content) {
  if (NULL == p_context) {
    LOGE(PLAY_LIST_TAG, "play list not init");
    return E_FAILED;
  }
  return _parse_tr_result_poem(p_context, content);
}

static Result _favorit_parse_content(PlayListContext *p_context,
                                            cJSON *content){
  cJSON *semantic = NULL;
  cJSON *intent = NULL;
  cJSON *operations = NULL;
  cJSON *item = NULL;
  cJSON *operands = NULL;
  int size = 0;
  int i = 0;

  if (NULL == p_context) {
    LOGE(PLAY_LIST_TAG, "play list not init");
    return E_FAILED;
  }
  semantic = cJSON_GetObjectItem(content, CJSON_OBJECT_NAME(semantic));
  if (semantic) {
    intent = cJSON_GetObjectItem(semantic, CJSON_OBJECT_NAME(intent));
    if (intent) {
      operations = cJSON_GetObjectItem(intent, CJSON_OBJECT_NAME(operations));
      if (operations) {
        size = cJSON_GetArraySize(operations);
        for (i=0; i<size; i++) {
          item = cJSON_GetArrayItem(operations, i);
          if (item) {
            operands = cJSON_GetObjectItem(item, CJSON_OBJECT_NAME(operands));
            if (operands 
                && (uni_strncmp(operands->valuestring, "OBJ_FAV_MUSIC_LIST",
                                uni_strlen("OBJ_FAV_MUSIC_LIST")) == 0)) {
              p_context->type = PLAY_LIST_MUSIC;
              return _music_parse_content(p_context, content);
            }
          }
        }
      }
    }
  }
  return E_FAILED;
}

Result PlayListParseTrResult(PLAY_LIST_HANDLE handle, cJSON *content){
  PlayListContext *p_context = (PlayListContext *)handle;
  cJSON *service = NULL;
  cJSON *text = NULL;
  Result ret = E_FAILED;
  uni_bool is_favorit_song = FALSE;

  if (NULL == p_context) {
    LOGE(PLAY_LIST_TAG, "play list not init");
    return E_FAILED;
  }
  if (NULL == content) {
    LOGE(PLAY_LIST_TAG, "content is NULL");
    return E_FAILED;
  }
  service = cJSON_GetObjectItem(content, CJSON_OBJECT_NAME(service));
  if (NULL == service || NULL == service->valuestring) {
    LOGE(PLAY_LIST_TAG, "service not exist");
    return E_FAILED;
  }
  StringAssignment(p_context->service, service->valuestring);
  /* parse text */
  text = cJSON_GetObjectItem(content, CJSON_OBJECT_NAME(text));
  if (text) {
    StringAssignment(p_context->text, text->valuestring);
  }
  if (NULL != uni_strstr(service->valuestring, "unios.usk.unisound.news")) {
    LOGE(PLAY_LIST_TAG, "parsing news skill event content");
    p_context->type = PLAY_LIST_NEWS;
    ret = _skill_news_parser(p_context, content);
  } else if (NULL != uni_strstr(service->valuestring, "music")) {
    p_context->type = PLAY_LIST_MUSIC;
    ret = _music_parse_content(p_context, content);
  } else if (NULL != uni_strstr(service->valuestring, "audio")) {
    p_context->type = PLAY_LIST_AUDIO;
    ret = _audio_parse_content(p_context, content);
  } else if (NULL != uni_strstr(service->valuestring, "news")) {
    p_context->type = PLAY_LIST_NEWS;
    ret = _news_parse_content(p_context, content);
  } else if (NULL != uni_strstr(service->valuestring, "chat")) {
    p_context->type = PLAY_LIST_POEM;
    ret = _chat_parse_content(p_context, content);
  } else if (NULL != uni_strstr(service->valuestring, "setting.mp")) {
    is_favorit_song = TRUE;
    ret = _favorit_parse_content(p_context, content);
  } else if (NULL != uni_strstr(service->valuestring, "broadcast")) {
    p_context->type = PLAY_LIST_BROADCAST;
    return E_FAILED;
  }
  /* the first song random always */
  if (E_OK == ret) {
    _refresh_suffle_arry(p_context, p_context->total_songs,
                        p_context->list_len);
    if (is_favorit_song && (PLAY_LIST_SHUFFLED != p_context->mode)) {
      /* favorit songs play from first */
      p_context->cur_index = 0;
    } else {
      p_context->cur_index = p_context->suff_arry.arry[0];
    }
    /* upload play list */
    if (p_context->cb_refreshed) {
      p_context->cb_refreshed();
    }
    LOGT(PLAY_LIST_TAG, "music sum=%d, page=%d, index=%d",
                        p_context->list_len, p_context->page_index,
                        p_context->page_index);
  } else {
    p_context->type = PLAY_LIST_TYPE_INVALID;
    LOGE(PLAY_LIST_TAG, "invalid play type[%s]", service->valuestring);
  }
  return ret;
}

static Result _parse_pull_result_music(PlayListContext *p_context,
                                              cJSON *data){
  cJSON *items = NULL;
  cJSON *item = NULL;
  cJSON *temp = NULL;
  int i = 0;

  if (NULL == p_context) {
    LOGE(PLAY_LIST_TAG, "play list not init");
    return E_FAILED;
  }
  if (E_OK != _parse_data_number(p_context, data)) {
    LOGE(PLAY_LIST_TAG, "data parse failed");
    return E_FAILED;
  }
  /* parse play list info */
  items = cJSON_GetObjectItem(data, CJSON_OBJECT_NAME(items));
  if (NULL == items) {
    LOGE(PLAY_LIST_TAG, "item not exist");
    return E_FAILED;
  }
  /* clear play list */
  _clear_play_list(p_context);
  for (i = 0; i < p_context->list_len; i++) {
    item = cJSON_GetArrayItem(items, i);
    _parse_music_item(&p_context->list[i], item);
    /* get selected key */
    temp = cJSON_GetObjectItem(item, CJSON_OBJECT_NAME(selected));
    if (temp) {
      if (1 == temp->valueint) {
        p_context->cur_index = i;
      }
    }
  }
  return E_OK;
}

static Result _parse_pull_result_audio(PlayListContext *p_context,
                                              cJSON *data){
  cJSON *items = NULL;
  cJSON *item = NULL;
  cJSON *temp = NULL;
  int size = 0;
  int i = 0;

  if (NULL == p_context) {
    LOGE(PLAY_LIST_TAG, "play list not init");
    return E_FAILED;
  }
  /* audio list use old format now */
  items = cJSON_GetObjectItem(data, CJSON_OBJECT_NAME(result));
  if (NULL == items) {
    LOGE(PLAY_LIST_TAG, "item not exist");
    return E_FAILED;
  }
  size = cJSON_GetArraySize(items);
  if (0 == size) {
    LOGW(PLAY_LIST_TAG, "audio list empty, return");
    return E_FAILED;
  }
  p_context->total_songs = size;
  p_context->list_len = size;
  p_context->page_index = 0;

  /* clear play list */
  _clear_play_list(p_context);
  /* parse play list info */
  for (i = 0; i < p_context->list_len; i++) {
    item = cJSON_GetArrayItem(items, i);
    _parse_audio_item(&p_context->list[i], item);
    /* get selected key */
    temp = cJSON_GetObjectItem(item, CJSON_OBJECT_NAME(selected));
    if (temp) {
      if (1 == temp->valueint) {
        p_context->cur_index = i;
      }
    }
  }
  return E_OK;
}

Result PlayListParsePullResult(PLAY_LIST_HANDLE handle, cJSON *content,
                                      PlayListType type){
  PlayListContext *p_context = (PlayListContext *)handle;
  cJSON *data = NULL;
  Result ret = E_FAILED;

  if (NULL == p_context) {
    LOGE(PLAY_LIST_TAG, "play list not init");
    return E_FAILED;
  }
  switch(type) {
    case PLAY_LIST_MUSIC:
      /* parse data */
      data = cJSON_GetObjectItem(content, CJSON_OBJECT_NAME(data));
      if (NULL == data) {
        LOGE(PLAY_LIST_TAG, "data not exist");
        return E_FAILED;
      }
      p_context->type = PLAY_LIST_MUSIC;
      ret = _parse_pull_result_music(p_context, data);
      break;
    case PLAY_LIST_AUDIO:
      /* parse controlInfo */
      data = cJSON_GetObjectItem(content, CJSON_OBJECT_NAME(controlInfo));
      if (NULL == data) {
        LOGE(PLAY_LIST_TAG, "controlInfo not exist");
        return E_FAILED;
      }
      p_context->type = PLAY_LIST_AUDIO;
      ret = _parse_pull_result_audio(p_context, data);
      break;
    default:
      LOGE(PLAY_LIST_TAG, "unsupport data type[%d]", type);
      p_context->type = PLAY_LIST_TYPE_INVALID;
      ret = E_FAILED;
      break;
  }
  if (E_OK == ret) {
    _refresh_suffle_arry(p_context, p_context->total_songs,
                        p_context->list_len);
    LOGT(PLAY_LIST_TAG, "music sum=%d, page=%d, index=%d",
                        p_context->list_len, p_context->page_index,
                        p_context->page_index);
  }
  return ret;
}

Result PlayListSetSearchCondition(PLAY_LIST_HANDLE handle, cJSON *cond){
  PlayListContext *p_context = (PlayListContext *)handle;
  cJSON *temp = NULL;
  cJSON *page = NULL;
  if (p_context) {
    /* parse text */
    temp = cJSON_GetObjectItem(cond, CJSON_OBJECT_NAME(text));
    if (temp) {
      StringAssignment(p_context->text, temp->valuestring);
    }
    /* parse service */
    temp = cJSON_GetObjectItem(cond, CJSON_OBJECT_NAME(service));
    if (NULL == temp) {
      LOGE(PLAY_LIST_TAG, "service not exist");
      return E_FAILED;
    }
    StringAssignment(p_context->service, temp->valuestring);
    /* parse code */
    temp = cJSON_GetObjectItem(cond, CJSON_OBJECT_NAME(code));
    if (NULL == temp) {
      LOGE(PLAY_LIST_TAG, "code not exist");
      return E_FAILED;
    }
    StringAssignment(p_context->opcode, temp->valuestring);
    /* parse intent */
    temp = cJSON_GetObjectItem(cond, CJSON_OBJECT_NAME(intent));
    if (NULL == temp) {
      LOGE(PLAY_LIST_TAG, "intent not exist");
      return E_FAILED;
    }
    /* parse page index */
    page = cJSON_GetObjectItem(temp, CJSON_OBJECT_NAME(page));
    if (NULL == page) {
      LOGE(PLAY_LIST_TAG, "page not exist");
      return E_FAILED;
    }
    if (page->valueint < 1) {
      LOGE(PLAY_LIST_TAG, "page index invalid : %d", page->valueint);
      return E_FAILED;
    }
    p_context->page_index = page->valueint - 1;
    return _parse_intent(p_context, temp);
  }
  return E_FAILED;
}

Result PlayListClear(PLAY_LIST_HANDLE handle){
  PlayListContext *p_context = (PlayListContext *)handle;
  if (p_context) {
    p_context->cur_index = -1;
    p_context->list_len = 0;
    p_context->page_index = -1;
    p_context->total_songs = 0;
    p_context->type = PLAY_LIST_TYPE_INVALID;
    _clear_play_list(p_context);
    _clear_intent(p_context);
    return E_OK;
  }
  return E_FAILED;
}

PlayListType PlayListGetType(PLAY_LIST_HANDLE handle){
  PlayListContext *p_context = (PlayListContext *)handle;
  if (p_context) {
    return p_context->type;
  }
  return PLAY_LIST_TYPE_INVALID;
}

int PlayListGetTotalNumber(PLAY_LIST_HANDLE handle){
  PlayListContext *p_context = (PlayListContext *)handle;
  if (p_context) {
    return p_context->total_songs;
  }
  return -1;
}

char* PlayListGetService(PLAY_LIST_HANDLE handle){
  PlayListContext *p_context = (PlayListContext *)handle;
  if (p_context) {
    return SringGetString(p_context->service);
  }
  return NULL;
}

char* PlayListGetOpcode(PLAY_LIST_HANDLE handle){
  PlayListContext *p_context = (PlayListContext *)handle;
  if (p_context) {
    return SringGetString(p_context->opcode);
  }
  return NULL;
}

int PlayListGetPageIndex(PLAY_LIST_HANDLE handle){
  PlayListContext *p_context = (PlayListContext *)handle;
  if (p_context) {
    return p_context->page_index;
  }
  return -1;
}

int PlayListGetPageSize(PLAY_LIST_HANDLE handle){
  PlayListContext *p_context = (PlayListContext *)handle;
  if (p_context) {
    return p_context->page_size;
  }
  return -1;
}

int PlayListGetSongsNum(PLAY_LIST_HANDLE handle){
  PlayListContext *p_context = (PlayListContext *)handle;
  if (p_context) {
    return p_context->list_len;
  }
  return -1;
}

static cJSON* _creat_song_item_json(PlayListItemInfo *item){
  cJSON *root = NULL;
  cJSON *temp = NULL;

  if (item) {
    root = cJSON_CreateObject();
    if (NULL == root) {
      LOGE(PLAY_LIST_TAG, "creat JSON node failed");
      return NULL;
    }
    if (SringGetLength(item->musicId) > 0) {
      temp = cJSON_CreateString(SringGetString(item->musicId));
      cJSON_AddItemToObject(root, "musicId", temp);
    }
    if (SringGetLength(item->name) > 0) {
      temp = cJSON_CreateString(SringGetString(item->name));
      cJSON_AddItemToObject(root, "name", temp);
    }
    if (SringGetLength(item->artist) > 0) {
      /*TODO: remove it later */
      if (SringGetLength(item->artist) > 255) {
        temp = cJSON_CreateString("群星");
        cJSON_AddItemToObject(root, "artist", temp);
      } else {
        temp = cJSON_CreateString(SringGetString(item->artist));
        cJSON_AddItemToObject(root, "artist", temp);
      }
    }
    if (SringGetLength(item->artistId) > 0) {
      temp = cJSON_CreateString(SringGetString(item->artistId));
      cJSON_AddItemToObject(root, "artistId", temp);
    }
    if (SringGetLength(item->album) > 0) {
      temp = cJSON_CreateString(SringGetString(item->album));
      cJSON_AddItemToObject(root, "album", temp);
    }
    if (SringGetLength(item->albumId) > 0) {
      temp = cJSON_CreateString(SringGetString(item->albumId));
      cJSON_AddItemToObject(root, "albumId", temp);
    }
    temp = cJSON_CreateNumber(item->duration);
    cJSON_AddItemToObject(root, "duration", temp);
    temp = cJSON_CreateNumber(item->online);
    cJSON_AddItemToObject(root, "online", temp);
    if (SringGetLength(item->format) > 0) {
      temp = cJSON_CreateString(SringGetString(item->format));
      cJSON_AddItemToObject(root, "format", temp);
    }
    if (SringGetLength(item->pic) > 0) {
      temp = cJSON_CreateString(SringGetString(item->pic));
      cJSON_AddItemToObject(root, "pic", temp);
    }
    if (SringGetLength(item->pic140) > 0) {
      temp = cJSON_CreateString(SringGetString(item->pic140));
      cJSON_AddItemToObject(root, "pic140", temp);
    }
    if (SringGetLength(item->title) > 0) {
      temp = cJSON_CreateString(SringGetString(item->title));
      cJSON_AddItemToObject(root, "title", temp);
    }
    if (SringGetLength(item->channelId) > 0) {
      temp = cJSON_CreateString(SringGetString(item->channelId));
      cJSON_AddItemToObject(root, "channelId", temp);
    }
    return root;
  }
  return NULL;
}

cJSON* PlayListCreatSongJson(PlayListItemInfo *item){
  if (NULL == item) {
    LOGE(PLAY_LIST_TAG, "invalid param");
    return NULL;
  }
  return _creat_song_item_json(item);
}

cJSON* PlayListCreatListJson(PLAY_LIST_HANDLE handle){
  PlayListContext *p_context = (PlayListContext *)handle;
  cJSON* root = NULL;
  cJSON* jlist = NULL;
  cJSON* jtemp = NULL;
  cJSON* jitem = NULL;
  int i = 0;

  if (p_context) {
    root = cJSON_CreateObject();
    if (NULL == root) {
      LOGE(PLAY_LIST_TAG, "creat JSON node failed");
      return NULL;
    }
    /* total number of all */
    jtemp = cJSON_CreateNumber(p_context->total_songs);
    cJSON_AddItemToObject(root, "total", jtemp);

    /* page index and size */
    jtemp = cJSON_CreateNumber(p_context->page_index + 1);
    cJSON_AddItemToObject(root, "page", jtemp);
    jtemp = cJSON_CreateNumber(p_context->page_size);
    cJSON_AddItemToObject(root, "size", jtemp);

    /* play list */
    jlist = cJSON_CreateArray();
    for (i=0; i< p_context->list_len; i++) {
      jitem = _creat_song_item_json(&p_context->list[i]);
      if (i == p_context->cur_index) {
        jtemp = cJSON_CreateNumber(TRUE);
      } else {
        jtemp = cJSON_CreateNumber(FALSE);
      }
      cJSON_AddItemToObject(jitem, "selected", jtemp);
      cJSON_AddItemToArray(jlist, jitem);
    }
    cJSON_AddItemToObject(root, "items", jlist);
    return root;
  }
  return NULL;
}

cJSON* _creat_audio_item_json(PlayListItemInfo *item){
  cJSON *root = NULL;
  cJSON *temp = NULL;

  if (item) {
    root = cJSON_CreateObject();
    if (NULL == root) {
      LOGE(PLAY_LIST_TAG, "creat JSON node failed");
      return NULL;
    }
    temp = cJSON_CreateNumber(uni_strtol(SringGetString(item->musicId),
                                        NULL, 10));
    cJSON_AddItemToObject(root, "id", temp);
    temp = cJSON_CreateNumber(uni_strtol(SringGetString(item->channelId),
                                        NULL, 10));
    cJSON_AddItemToObject(root, "channelId", temp);
    if (SringGetLength(item->title) > 0) {
      temp = cJSON_CreateString(SringGetString(item->title));
      cJSON_AddItemToObject(root, "title", temp);
    }
    return root;
  }
  return NULL;
}

cJSON* PlayListCreatAudioListJson(PLAY_LIST_HANDLE handle){
  PlayListContext *p_context = (PlayListContext *)handle;
  cJSON* jlist = NULL;
  cJSON* jtemp = NULL;
  cJSON* jitem = NULL;
  int i = 0;

  if (p_context) {
    jlist = cJSON_CreateArray();
    if (NULL == jlist) {
      LOGE(PLAY_LIST_TAG, "creat JSON node failed");
      return NULL;
    }
    for (i=0; i<p_context->list_len; i++) {
      jitem = _creat_audio_item_json(&p_context->list[i]);
      if (i == p_context->cur_index) {
        jtemp = cJSON_CreateNumber(TRUE);
      } else {
        jtemp = cJSON_CreateNumber(FALSE);
      }
      cJSON_AddItemToObject(jitem, "selected", jtemp);
      jtemp = cJSON_CreateNumber(0);
      cJSON_AddItemToObject(jitem, "collected", jtemp);
      cJSON_AddItemToArray(jlist, jitem);
    }
    return jlist;
  }
  return NULL;
}

static cJSON* _creat_intent_json(PlayListContext *p_context,
                                      int page_index){
  cJSON* root = NULL;
  cJSON *temp = NULL;

  if (p_context) {
    root = cJSON_CreateObject();
    if (NULL == root) {
      LOGE(PLAY_LIST_TAG, "creat JSON node failed");
      return NULL;
    }
    /* page index and size */
    temp = cJSON_CreateNumber(page_index + 1);
    cJSON_AddItemToObject(root, "page", temp);
    temp = cJSON_CreateNumber(p_context->page_size);
    cJSON_AddItemToObject(root, "size", temp);

    /* intent */
    if (SringGetLength(p_context->intent.song) > 0) {
      temp = cJSON_CreateString(SringGetString(p_context->intent.song));
      cJSON_AddItemToObject(root, "song", temp);
    }
    if (SringGetLength(p_context->intent.artist) > 0) {
      temp = cJSON_CreateString(SringGetString(p_context->intent.artist));
      cJSON_AddItemToObject(root, "artist", temp);
    }
    if (SringGetLength(p_context->intent.genre) > 0) {
      temp = cJSON_CreateString(SringGetString(p_context->intent.genre));
      cJSON_AddItemToObject(root, "genre", temp);
    }
    if (SringGetLength(p_context->intent.tag) > 0) {
      temp = cJSON_CreateString(SringGetString(p_context->intent.tag));
      cJSON_AddItemToObject(root, "tag", temp);
    }
    if (SringGetLength(p_context->intent.mood) > 0) {
      temp = cJSON_CreateString(SringGetString(p_context->intent.mood));
      cJSON_AddItemToObject(root, "mood", temp);
    }
    if (SringGetLength(p_context->intent.scene) > 0) {
      temp = cJSON_CreateString(SringGetString(p_context->intent.scene));
      cJSON_AddItemToObject(root, "scene", temp);
    }
    if (SringGetLength(p_context->intent.billboard) > 0) {
      temp = cJSON_CreateString(SringGetString(p_context->intent.billboard));
      cJSON_AddItemToObject(root, "billboard", temp);
    }
    if (SringGetLength(p_context->intent.language) > 0) {
      temp = cJSON_CreateString(SringGetString(p_context->intent.language));
      cJSON_AddItemToObject(root, "language", temp);
    }
    if (SringGetLength(p_context->intent.lyrics) > 0) {
      temp = cJSON_CreateString(SringGetString(p_context->intent.lyrics));
      cJSON_AddItemToObject(root, "lyrics", temp);
    }
    if (SringGetLength(p_context->intent.songlist) > 0) {
      temp = cJSON_CreateString(SringGetString(p_context->intent.songlist));
      cJSON_AddItemToObject(root, "songlist", temp);
    }
    if (SringGetLength(p_context->intent.keyword) > 0) {
      temp = cJSON_CreateString(SringGetString(p_context->intent.keyword));
      cJSON_AddItemToObject(root, "keyword", temp);
    }
    if (SringGetLength(p_context->intent.album) > 0) {
      temp = cJSON_CreateString(SringGetString(p_context->intent.album));
      cJSON_AddItemToObject(root, "album", temp);
    }
    if (SringGetLength(p_context->intent.artistType) > 0) {
      temp = cJSON_CreateString(SringGetString(p_context->intent.artistType));
      cJSON_AddItemToObject(root, "artistType", temp);
    }
    if (SringGetLength(p_context->intent.ktvMode) > 0) {
      temp = cJSON_CreateString(SringGetString(p_context->intent.ktvMode));
      cJSON_AddItemToObject(root, "ktvMode", temp);
    }
    if (SringGetLength(p_context->intent.artistId) > 0) {
      temp = cJSON_CreateString(SringGetString(p_context->intent.artistId));
      cJSON_AddItemToObject(root, "artistId", temp);
    }
    return root;
  }
  return NULL;
}

static cJSON *_creat_search_json(PlayListContext *p_context,
                                      int page_index){
  cJSON* root = NULL;
  cJSON* jtemp = NULL;

  if (p_context) {
    root = cJSON_CreateObject();
    if (NULL == root) {
      LOGE(PLAY_LIST_TAG, "creat JSON node failed");
      return NULL;
    }
    /* text */
    if (SringGetLength(p_context->text) > 0) {
      jtemp = cJSON_CreateString(SringGetString(p_context->text));
      cJSON_AddItemToObject(root, "text", jtemp);
    }
    /* code */
    if (SringGetLength(p_context->opcode) <= 0) {
      LOGE(PLAY_LIST_TAG, "code is empty");
      cJSON_Delete(root);
      return NULL;
    }
    jtemp = cJSON_CreateString(SringGetString(p_context->opcode));
    cJSON_AddItemToObject(root, "code", jtemp);
    /* service */
    if (SringGetLength(p_context->service) <= 0) {
      LOGE(PLAY_LIST_TAG, "service is empty");
      cJSON_Delete(root);
      return NULL;
    }
    jtemp = cJSON_CreateString(SringGetString(p_context->service));
    cJSON_AddItemToObject(root, "service", jtemp);
    /* intent */
    jtemp = _creat_intent_json(p_context, page_index);
    if (NULL == jtemp) {
      LOGE(PLAY_LIST_TAG, "creat intent failed");
      cJSON_Delete(root);
      return NULL;
    }
    cJSON_AddItemToObject(root, "intent", jtemp);
    return root;
  }
  return NULL;
}

cJSON* PlayListCreatSearchJson(PLAY_LIST_HANDLE handle){
  PlayListContext *p_context = (PlayListContext *)handle;
  if (p_context) {
    return _creat_search_json(p_context, p_context->page_index);
  }
  return NULL;
}

static uni_bool _is_last_page_of_all(PlayListContext *p_context){
  int total_pages = 0;

  if (p_context) {
    total_pages = p_context->total_songs / p_context->page_size;
    if ((p_context->total_songs % p_context->page_size) > 0) {
      total_pages += 1;
    }
    if ((p_context->page_index+1) >= total_pages) {
      return TRUE;
    }
  }
  return FALSE;
}

static uni_bool _is_last_song_of_all(PlayListContext *p_context){
  if (p_context) {
    if (_is_last_page_of_all(p_context)) {
      if ((p_context->cur_index+1) >= p_context->list_len) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

static uni_bool _is_first_song_of_all(PlayListContext *p_context){
  if (p_context) {
    if (p_context->page_index == 0) {
      if (p_context->cur_index == 0) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

static Result _parse_serach_result(PlayListContext *p_context,
                                        cJSON *reslut, PlayListType type){
  Result ret = E_OK;
  if (NULL == p_context) {
    LOGE(PLAY_LIST_TAG, "play list not init");
    return E_FAILED;
  }
  switch(type) {
    case PLAY_LIST_MUSIC:
      ret = _parse_tr_result_music(p_context, reslut);
      break;
    case PLAY_LIST_AUDIO:
      ret = _parse_tr_result_audio(p_context, reslut);
      break;
    case PLAY_LIST_NEWS:
      LOGT(PLAY_LIST_TAG, "PLAY_LIST_NEWS search");
      ret = _skill_news_parser(p_context, reslut);
      break;
    default:
      LOGE(PLAY_LIST_TAG, "unsupport data type[%d]", type);
      ret = E_FAILED;
      break;
  }
  return ret;
}

static Result _load_page_songs(PlayListContext *p_context, int page_index){
  cJSON *reslut = NULL;
  cJSON *search = NULL;
  Result ret = E_OK;

  if (p_context) {
    if (p_context->page_index == page_index) {
      /* donnot need to load new page */
      return E_OK;
    }
    if (p_context->cb_load) {
      /* try to get prv page songs */
      search = _creat_search_json(p_context, page_index);
      reslut = p_context->cb_load(search, p_context->type);
      if (reslut) {
        ret = _parse_serach_result(p_context, reslut, p_context->type);
      } else {
        LOGE(PLAY_LIST_TAG, "cannot load new page");
        ret = E_FAILED;
      }
    }
  }
  if (reslut) {
    cJSON_Delete(reslut);
  }
  if (search) {
    cJSON_Delete(search);
  }
  return ret;
}

#if IS_LIST_ORDER_LOOP_DIFF
static int _switch_prv_song_order(PlayListContext *p_context){
  if (p_context) {
    LOGT(PLAY_LIST_TAG, "page_index=%d, cur_index=%d, list_len=%d, total_songs=%d",
                        p_context->page_index, p_context->cur_index,
                        p_context->list_len, p_context->total_songs);
    if (_is_first_song_of_all(p_context)) {
      /* notify no more songs */
      if (p_context->cb_no_prv_song) {
        p_context->cb_no_prv_song();
      }
    } else {
      if (p_context->cur_index == 0) {
        /* try to load new page songs if it's first song in current page */
        if (_load_page_songs(p_context, p_context->page_index-1) == E_OK) {
          /* point to last song of new page */
          p_context->cur_index= p_context->list_len - 1;
          /* upload play list */
          if (p_context->cb_refreshed) {
            p_context->cb_refreshed();
          }
        } else {
          /* no more songs */
          if (p_context->cb_no_prv_song) {
            p_context->cb_no_prv_song();
          }
        }
      } else {
        /* play prv song */
        p_context->cur_index--;
      }
    }
    return p_context->cur_index;
  }
  return -1;
}

static int _switch_next_song_order(PlayListContext *p_context){
  if (p_context) {
    LOGT(PLAY_LIST_TAG, "page_index=%d, cur_index=%d, list_len=%d, total_songs=%d",
                        p_context->page_index, p_context->cur_index,
                        p_context->list_len, p_context->total_songs);
    if (_is_last_song_of_all(p_context)) {
      /* notify no more songs */
      if (p_context->cb_no_next_song) {
        p_context->cb_no_next_song();
      }
    } else {
      if (p_context->cur_index >= (p_context->list_len-1)) {
        /* try to load new page songs if it's last song in current page */
        if (_load_page_songs(p_context, p_context->page_index+1) == E_OK) {
          /* point to first song of new page */
          p_context->cur_index= 0;
          /* upload play list */
          if (p_context->cb_refreshed) {
            p_context->cb_refreshed();
          }
        } else {
          /* no more songs */
          if (p_context->cb_no_next_song) {
            p_context->cb_no_next_song();
          }
        }
      } else {
        /* play next song */
        p_context->cur_index++;
      }
    }
    return p_context->cur_index;
  }
  return -1;
}
#endif

static int _switch_prv_song_loop(PlayListContext *p_context){
  int total_pages = 0;

  if (p_context) {
    LOGT(PLAY_LIST_TAG, "page_index=%d, cur_index=%d, list_len=%d, total_songs=%d",
                        p_context->page_index, p_context->cur_index,
                        p_context->list_len, p_context->total_songs);
    if (_is_first_song_of_all(p_context)) {
      /* try to load last page songs */
      total_pages = p_context->total_songs / p_context->page_size;
      if ((p_context->total_songs % p_context->page_size) > 0) {
        total_pages += 1;
      }
      if (_load_page_songs(p_context, total_pages-1) == E_OK) {
        /* point to last song of new page */
        p_context->cur_index= p_context->list_len - 1;
        /* upload play list */
        if (p_context->cb_refreshed) {
          p_context->cb_refreshed();
        }
      } else {
        /* no more songs */
        if (p_context->cb_no_prv_song) {
          p_context->cb_no_prv_song();
        }
        /* switch to last song */
        p_context->cur_index= p_context->list_len - 1;
      }
    } else {
      if (p_context->cur_index == 0) {
        /* try to load new page songs if it's first song in current page */
        if (_load_page_songs(p_context, p_context->page_index-1) == E_OK) {
          /* point to last song of new page */
          p_context->cur_index= p_context->list_len - 1;
          /* upload play list */
          if (p_context->cb_refreshed) {
            p_context->cb_refreshed();
          }
        } else {
          /* no more songs */
          if (p_context->cb_no_prv_song) {
            p_context->cb_no_prv_song();
          }
          /* switch to last song */
          p_context->cur_index= p_context->list_len - 1;
        }
      } else {
        /* play prv song */
        p_context->cur_index--;
      }
    }
    return p_context->cur_index;
  }
  return -1;
}

static int _switch_next_song_loop(PlayListContext *p_context){
  if (p_context) {
    LOGT(PLAY_LIST_TAG, "page_index=%d, cur_index=%d, list_len=%d, total_songs=%d",
                        p_context->page_index, p_context->cur_index,
                        p_context->list_len, p_context->total_songs);
    if (_is_last_song_of_all(p_context)) {
      /* try to load first page songs */
      if (_load_page_songs(p_context, 0) == E_OK) {
        /* point to first song of new page */
        p_context->cur_index= 0;
        /* upload play list */
        if (p_context->cb_refreshed) {
          p_context->cb_refreshed();
        }
      } else {
        /* no more songs */
        if (p_context->cb_no_next_song) {
          p_context->cb_no_next_song();
        }
        /* switch to first song */
        p_context->cur_index= 0;
      }
    } else {
      if (p_context->cur_index >= (p_context->list_len-1)) {
        /* try to load new page songs if it's last song in current page */
        if (_load_page_songs(p_context, p_context->page_index+1) == E_OK) {
          /* point to first song of new page */
          p_context->cur_index= 0;
          /* upload play list */
          if (p_context->cb_refreshed) {
            p_context->cb_refreshed();
          }
        } else {
          /* no more songs */
          if (p_context->cb_no_next_song) {
            p_context->cb_no_next_song();
          }
          /* switch to first song */
          p_context->cur_index= 0;
        }
      } else {
        /* play next song */
        p_context->cur_index++;
      }
    }
    return p_context->cur_index;
  }
  return -1;
}

static int _get_prv_rand_song(PlayListContext *p_context){
  int song_index = 0;
  if (p_context) {
    if (p_context->suff_arry.total_num > 0) {
      p_context->suff_arry.index--;
      if (p_context->suff_arry.index < 0) {
        /* go to last song in the suffle arry */
        p_context->suff_arry.index = p_context->suff_arry.total_num - 1;
      }
      song_index = p_context->suff_arry.arry[p_context->suff_arry.index];
      return song_index;
    }
  }
  return -1;
}

static int _get_next_rand_song(PlayListContext *p_context){
  int song_index = 0;
  if (p_context) {
    if (p_context->suff_arry.total_num > 0) {
      p_context->suff_arry.index++;
      if (p_context->suff_arry.index >= p_context->suff_arry.total_num) {
        /* go to first song in the suffle arry */
        p_context->suff_arry.index = 0;
      }
      song_index = p_context->suff_arry.arry[p_context->suff_arry.index];
      return song_index;
    }
  }
  return -1;
}

static int _get_page_index_of_song(PlayListContext *p_context, int num){
  int page_index = 0;
  if (p_context) {
    /* git page index include number of song */
    page_index = num / p_context->page_size;
    return page_index;
  }
  return -1;
}

static int _switch_prv_song_shuffled(PlayListContext *p_context){
  int song_index = 0;
  int page_index = 0;
  int list_index = 0;

  if (p_context) {
    LOGT(PLAY_LIST_TAG, "page_index=%d, cur_index=%d, list_len=%d, total_songs=%d",
                        p_context->page_index, p_context->cur_index,
                        p_context->list_len, p_context->total_songs);
    song_index = _get_prv_rand_song(p_context);
    LOGT(PLAY_LIST_TAG, "suff_arry.total_num=%d, suff_arry.index=%d, num=%d",
                        p_context->suff_arry.total_num, p_context->suff_arry.index,
                        p_context->suff_arry.arry[p_context->suff_arry.index]);
    if (song_index < 0) {
      /* no more songs */
      if (p_context->cb_no_prv_song) {
        p_context->cb_no_prv_song();
      }
    } else {
      page_index = _get_page_index_of_song(p_context, song_index);
      /* load new page if this song not inclued in current page */
      if (_load_page_songs(p_context, page_index) == E_OK) {
        /* point to specified song in new page */
        list_index = song_index % p_context->page_size;
        p_context->cur_index = list_index;
        /* select a song if cur_index is invalid */
        if (p_context->cur_index >= p_context->list_len) {
          p_context->cur_index = 0;
        }
        /* upload play list */
        if (p_context->cb_refreshed) {
          p_context->cb_refreshed();
        }
      } else {
        /* no more songs */
        if (p_context->cb_no_prv_song) {
          p_context->cb_no_prv_song();
        }
        /* play a song in current page */
        p_context->cur_index = rand() % p_context->list_len;
      }
    }
    return p_context->cur_index;
  }
  return -1;
}

static int _switch_next_song_shuffled(PlayListContext *p_context){
  int song_index = 0;
  int page_index = 0;
  int list_index = 0;

  if (p_context) {
    LOGT(PLAY_LIST_TAG, "page_index=%d, cur_index=%d, list_len=%d, total_songs=%d",
                        p_context->page_index, p_context->cur_index,
                        p_context->list_len, p_context->total_songs);
    song_index = _get_next_rand_song(p_context);
    LOGT(PLAY_LIST_TAG, "suff_arry.total_num=%d, suff_arry.index=%d, num=%d",
                        p_context->suff_arry.total_num, p_context->suff_arry.index,
                        p_context->suff_arry.arry[p_context->suff_arry.index]);
    if (song_index < 0) {
      /* no more songs */
      if (p_context->cb_no_next_song) {
        p_context->cb_no_next_song();
      }
    } else {
      page_index = _get_page_index_of_song(p_context, song_index);
      /* load new page if this song not inclued in current page */
      if (_load_page_songs(p_context, page_index) == E_OK) {
        /* point to specified song in new page */
        list_index = song_index % p_context->page_size;
        p_context->cur_index = list_index;
        /* select a song if cur_index is invalid */
        if (p_context->cur_index >= p_context->list_len) {
          p_context->cur_index = 0;
        }
        /* upload play list */
        if (p_context->cb_refreshed) {
          p_context->cb_refreshed();
        }
      } else {
        /* no more songs */
        if (p_context->cb_no_next_song) {
          p_context->cb_no_next_song();
        }
        /* play a song in current page */
        p_context->cur_index = rand() % p_context->list_len;
      }
    }
    return p_context->cur_index;
  }
  return -1;
}

static int _audio_switch_prev(PlayListContext *context) {
  cJSON *root =NULL;
  int ret =-1;
  root = context->cb_switch_song(context->type,SONG_SWITCH_PREV);
  if(root) {
    if(E_OK == _parse_tr_result_audio(context,root)) {
      context->cur_index =0;
      if (context->cb_refreshed) {
        context->cb_refreshed();
      }
      ret = context->cur_index;
    }
  }
  if(root) {
    cJSON_Delete(root);
  }
  return ret;
}

static int _audio_switch_next(PlayListContext *context) {
  cJSON *root =NULL;
  int ret =-1;
  root = context->cb_switch_song(context->type,SONG_SWITCH_NEXT);
  if(root) {
    if(E_OK == _parse_tr_result_audio(context,root)) {
      context->cur_index =0;
      if (context->cb_refreshed) {
        context->cb_refreshed();
      }
      ret = context->cur_index;
    }
  }
  if(root) {
    cJSON_Delete(root);
  }
  return ret;
}

int PlayListSwitchNext(PLAY_LIST_HANDLE handle){
  PlayListContext *p_context = (PlayListContext *)handle;

  if (p_context) {
    if ((p_context->cur_index < 0) || (p_context->list_len <= 0)) {
      /* play list invalid */
      return -1;
    }
    if(PLAY_LIST_AUDIO == p_context->type) {
      return _audio_switch_next(p_context);
    }
    switch (p_context->mode) {
      case PLAY_SINGLE_LOOP:
        /* user can switch next song and even if PLAY_SINGLE_LOOP mode */
      case PLAY_LIST_ORDER:
#if IS_LIST_ORDER_LOOP_DIFF
        _switch_next_song_order(p_context);
        break;
#endif
      case PLAY_LIST_LOOP:
        _switch_next_song_loop(p_context);
        break;
      case PLAY_LIST_SHUFFLED:
        _switch_next_song_shuffled(p_context);
        break;
      default:
        break;
    }
    return p_context->cur_index;
  }
  return -1;
}

int PlayListAutoNext(PLAY_LIST_HANDLE handle) {
  PlayListContext *p_context = (PlayListContext *)handle;

  if (p_context) {
    if ((p_context->cur_index < 0) || (p_context->list_len <= 0)) {
      /* play list invalid */
      return -1;
    }
    #if 0
    if(PLAY_LIST_AUDIO == p_context->type) {
      if(PLAY_SINGLE_LOOP == p_context->mode) {
        return p_context->cur_index;
      } else {
        return _audio_switch_next(p_context);
      }
    }
    #endif
    switch (p_context->mode) {
      case PLAY_SINGLE_LOOP:
        /* replay current song if PLAY_SINGLE_LOOP mode */
        break;
      case PLAY_LIST_ORDER:
#if IS_LIST_ORDER_LOOP_DIFF
        _switch_next_song_order(p_context);
        break;
#endif
      case PLAY_LIST_LOOP:
        _switch_next_song_loop(p_context);
        break;
      case PLAY_LIST_SHUFFLED:
        _switch_next_song_shuffled(p_context);
        break;
      default:
        break;
    }
    return p_context->cur_index;
  }
  return -1;
}

int PlayListSwitchPrev(PLAY_LIST_HANDLE handle){
  PlayListContext *p_context = (PlayListContext *)handle;

  if (p_context) {
    if ((p_context->cur_index < 0) || (p_context->list_len <= 0)) {
      /* play list invalid */
      return -1;
    }
     if(PLAY_LIST_AUDIO == p_context->type) {
      return _audio_switch_prev(p_context);
    }
    switch (p_context->mode) {
      case PLAY_SINGLE_LOOP:
        /* user can switch next song and even if PLAY_SINGLE_LOOP mode */
      case PLAY_LIST_ORDER:
#if IS_LIST_ORDER_LOOP_DIFF
        _switch_prv_song_order(p_context);
        break;
#endif
      case PLAY_LIST_LOOP:
        _switch_prv_song_loop(p_context);
        break;
      case PLAY_LIST_SHUFFLED:
        _switch_prv_song_shuffled(p_context);
        break;
      default:
        break;
    }
    return p_context->cur_index;
  }
  return -1;
}

int PlayListSwitchSelect(PLAY_LIST_HANDLE handle, char *id){
  PlayListContext *p_context = (PlayListContext *)handle;
  int i = 0;
  if (p_context) {
    for (i=0; i<p_context->list_len; i++) {
      if (0 == SringCmp(p_context->list[i].musicId, id)) {
        p_context->cur_index = i;
        return p_context->cur_index;
      }
    }
  }
  return -1;
}

PlayListItemInfo* PlayListGetCurSong(PLAY_LIST_HANDLE handle){
  PlayListContext *p_context = (PlayListContext *)handle;
  if (p_context) {
    if ((p_context->cur_index >= 0) && (p_context->list_len > 0)
        && (p_context->cur_index < p_context->list_len)) {
      return &p_context->list[p_context->cur_index];
    }
  }
  return NULL;
}

Result PlayListMusicLoadRandomPage(PLAY_LIST_HANDLE handle) {
  Result ret = E_FAILED;
  cJSON *result;
  cJSON *search;
  PlayListContext *p_context = (PlayListContext *)handle;
  if (p_context && p_context->cb_load) {
    _clear_intent(p_context);
    p_context->page_index = 0;
    p_context->type = PLAY_LIST_MUSIC;
    StringAssignment(p_context->text, "我要听歌");
    StringAssignment(p_context->opcode, "SEARCH_RANDOM");
    StringAssignment(p_context->service, "cn.yunzhisheng.music");
    search = _creat_search_json(p_context, p_context->page_index);
    result = p_context->cb_load(search, p_context->type);
    if (NULL != result) {
      ret = _parse_serach_result(p_context, result, p_context->type);
      if (E_OK == ret) {
        p_context->cur_index = 0;
      }
    }
    if (search) {
      cJSON_Delete(search);
    }
    
  } 
  return ret;
}
