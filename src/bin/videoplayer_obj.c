/*
 * Enna Media Center.
 * Copyright (C) 2005-2013 Enna Team. All rights reserved.
 * This file is part of Enna.
 *
 * Enna is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Enna is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Enna; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <time.h>

#include <Evas.h>
#include <Emotion.h>
#include <Elementary.h>

#include "enna_config.h"
#include "metadata.h"
#include "mediaplayer.h"
#include "mediaplayer_obj.h"
#include "utils.h"
#include "logs.h"

/* variable and macros used for the eina_log module */
int _enna_log_dom_global;

#ifdef ENNA_DEFAULT_LOG_COLOR
# undef ENNA_DEFAULT_LOG_COLOR
#endif /* ifdef ENNA_DEFAULT_LOG_COLOR */
#define ENNA_DEFAULT_LOG_COLOR EINA_COLOR_LIGHTBLUE
#ifdef ERR
# undef ERR
#endif /* ifdef ERR */
#define ERR(...)  EINA_LOG_DOM_ERR(_enna_log_dom_global, __VA_ARGS__)
#ifdef DBG
# undef DBG
#endif /* ifdef DBG */
#define DBG(...)  EINA_LOG_DOM_DBG(_enna_log_dom_global, __VA_ARGS__)
#ifdef INF
# undef INF
#endif /* ifdef INF */
#define INF(...)  EINA_LOG_DOM_INFO(_enna_log_dom_global, __VA_ARGS__)
#ifdef WRN
# undef WRN
#endif /* ifdef WRN */
#define WRN(...)  EINA_LOG_DOM_WARN(_enna_log_dom_global, __VA_ARGS__)
#ifdef CRIT
# undef CRIT
#endif /* ifdef CRIT */
#define CRIT(...) EINA_LOG_DOM_CRIT(_enna_log_dom_global, __VA_ARGS__)


#define PRIV_DATA_GET(o, type, ptr) \
  type * ptr = evas_object_data_get(o, #type)

#define PRIV_GET_OR_RETURN(o, type, ptr)           \
  PRIV_DATA_GET(o, type, ptr);                     \
  if (!ptr)                                        \
    {                                              \
       CRIT("No private data for object %p (%s)!", \
               o, #type);                          \
       abort();                                    \
       return;                                     \
    }

#define PRIV_GET_OR_RETURN_VAL(o, type, ptr)       \
  DATA_GET(o, type, ptr);                          \
  if (!ptr)                                        \
    {                                              \
       CRIT("No private data for object %p (%s)!", \
               o, #type);                          \
       abort();                                    \
       return val;                                 \
    }

#define FREE_NULL(p) \
        if (p) { free(p); p = NULL; }

#define FREE_NULL_FUNC(fn, p) \
        if (p) { fn(p); p = NULL; }

#define ENNA_CONFIG_FILE "enna.conf"


/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

#define OSD_TIMER    4.0

typedef struct _Enna_View_Player_Video_Data Enna_View_Player_Video_Data;

struct _Enna_View_Player_Video_Data
{
    Evas_Object *video;
    Evas_Object *layout;
    Ecore_Timer *osd_timer;
    Evas_Object *cover;

    char *media;
    Eina_Bool on_hold;
};

typedef struct mediaplayer_cfg_s {
    char *engine;
} mediaplayer_cfg_t;

static mediaplayer_cfg_t mp_cfg;

static const struct {
    const char *name;
} map_player_type[] = {
    { "generic"                                  },
    { "gstreamer"                            },
    { "xine"                                 },
    { NULL                                   }
};

static void
cfg_mediaplayer_section_load (const char *section)
{
    const char *value = NULL;
    int i;

    enna_log(ENNA_MSG_INFO, NULL, "parameters:");

    value = enna_config_string_get(section, "type");
    if (value)
    {
        enna_log(ENNA_MSG_INFO, NULL, " * type: %s", value);

        for (i = 0; map_player_type[i].name; i++)
        {
            if (!strcmp(value, map_player_type[i].name))
            {
                mp_cfg.engine = strdup(map_player_type[i].name);
                break;
            }
        }
    }
}

static void
cfg_mediaplayer_section_save (const char *section)
{
    int i;

    /* Default type */
    for (i = 0; map_player_type[i].name; i++)
    {
        if (!strcmp(mp_cfg.engine ,map_player_type[i].name))
        {
            enna_config_string_set(section, "type",
                                   map_player_type[i].name);
            break;
        }
    }
}

static void
cfg_mediaplayer_free (void)
{
    free(mp_cfg.engine);
}

static void
cfg_mediaplayer_section_set_default (void)
{
    cfg_mediaplayer_free();

    mp_cfg.engine           = strdup("generic");
}

static Enna_Config_Section_Parser cfg_mediaplayer = {
    "mediaplayer",
    cfg_mediaplayer_section_load,
    cfg_mediaplayer_section_save,
    cfg_mediaplayer_section_set_default,
    cfg_mediaplayer_free,
};


static Eina_Bool
_osd_timer_cb(void *data)
{
   Enna_View_Player_Video_Data *priv = data;

   if (priv->on_hold)
   {
       return EINA_FALSE;
   }
   elm_object_signal_emit(priv->layout, "hide,osd", "enna");
   return EINA_TRUE;
}

static void
_set_osd_timer(Enna_View_Player_Video_Data *priv, double t)
{
   FREE_NULL_FUNC(ecore_timer_del, priv->osd_timer);

   if (t >= 0.0)
     priv->osd_timer = ecore_timer_add(t, _osd_timer_cb, priv);
   elm_object_signal_emit(priv->layout, "show,osd", "enna");
}

void
_update_time_part(Evas_Object *obj, const char *part, double t)
{
   Eina_Strbuf *str;
   double s;
   int h, m;

   s = t;
   h = (int)(s / 3600.0);
   s -= h * 3600;
   m = (int)(s / 60.0);
   s -= m * 60;

   str = eina_strbuf_new();
   eina_strbuf_append_printf(str, "%02d:%02d:%02d", h, m, (int)s);

   elm_object_part_text_set(obj, part, eina_strbuf_string_get(str));

   eina_strbuf_free(str);
}

static void
_slider_position_update_cb(void *data,
                           Evas_Object *obj EINA_UNUSED,
                           const char *emission EINA_UNUSED,
                           const char *source EINA_UNUSED)
{
    Enna_View_Player_Video_Data *priv = data;
    Evas_Object *emotion;
    Evas_Object *edje;
    double vx, vy;
    double pos;
    time_t timestamp;
    struct tm *t;
    Eina_Strbuf *str;

    emotion = elm_video_emotion_get(priv->video);

    edje = elm_layout_edje_get(priv->layout);
    edje_object_part_drag_value_get(edje, "time.slider", &vx, &vy);

    pos = vx *  emotion_object_play_length_get(emotion);

    _update_time_part(priv->layout, "time_current.text", pos);
    _update_time_part(priv->layout, "time_duration.text", emotion_object_play_length_get(emotion));

    timestamp = time(NULL);
    timestamp += emotion_object_play_length_get(emotion) - pos;
    t = localtime(&timestamp);
    str = eina_strbuf_new();
    eina_strbuf_append_printf(str, "End at %02dh%02d", t->tm_hour, t->tm_min);
    elm_object_part_text_set(priv->layout, "time_end_at.text", eina_strbuf_string_get(str));
    eina_strbuf_free(str);

    emotion_object_position_set(emotion, pos);
}

static void 
_emotion_position_update_cb(void *data, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   Enna_View_Player_Video_Data *priv = data;
   Evas_Object *emotion;
   Evas_Object *edje;
   double v;
   time_t timestamp;
   struct tm *t;
   Eina_Strbuf *str;

   emotion = elm_video_emotion_get(priv->video);

   _update_time_part(priv->layout, "time_current.text", 
                     emotion_object_position_get(emotion));
   _update_time_part(priv->layout, "time_duration.text", 
                     emotion_object_play_length_get(emotion));

   timestamp = time(NULL);
   timestamp += emotion_object_play_length_get(emotion) -
                emotion_object_position_get(emotion);
   t = localtime(&timestamp);
   str = eina_strbuf_new();
   eina_strbuf_append_printf(str, "End at %02dh%02d", t->tm_hour, t->tm_min);
   elm_object_part_text_set(priv->layout, "time_end_at.text", eina_strbuf_string_get(str));
   eina_strbuf_free(str);

   v = emotion_object_position_get(emotion) /
       emotion_object_play_length_get(emotion);

   edje = elm_layout_edje_get(priv->layout);
   edje_object_part_drag_value_set(edje, "time.slider", v, v);
}

#if 0
static void
_item_file_name_get_cb(void *data, Ems_Node *node EINA_UNUSED, const char *value)
{
   Enna_View_Player_Video_Data *priv = data;

   if (value && value[0])
     {
        elm_object_part_text_set(priv->layout, "title.text", value);
     }
   else
     {
        elm_object_part_text_set(priv->layout, "title.text", "Unknown");
        ERR("Cannot find a title for this file: :'(");
     }
}

static void
_item_name_get_cb(void *data, Ems_Node *node, const char *value)
{
   Enna_View_Player_Video_Data *priv = data;

   if (value && value[0])
     {
        elm_object_part_text_set(priv->layout, "title.text", value);
     }
   else
     {
        ems_node_media_info_get(node, priv->media, "clean_name", _item_file_name_get_cb,
                             NULL, NULL, priv);
     }
}

static void
_item_poster_get_cb(void *data, Ems_Node *node EINA_UNUSED, const char *value)
{
   Enna_View_Player_Video_Data *priv = data;

   if (value)
     {
        priv->cover = elm_icon_add(priv->layout);
        elm_image_file_set(priv->cover, value, NULL);
        elm_image_preload_disabled_set(priv->cover, EINA_FALSE);

        elm_object_part_content_set(priv->layout, "cover.swallow", priv->cover);
        elm_object_signal_emit(priv->layout, "show,cover", "enna"); 
     }
   else
     {
        elm_object_signal_emit(priv->layout, "hide,cover", "enna"); 
     }
}
#endif

static void
_emotion_open_done_cb(void *data, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   Enna_View_Player_Video_Data *priv = data;

   elm_object_signal_emit(priv->layout, "playing", "enna");
}

static void
_emotion_playback_started_cb(void *data, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   Enna_View_Player_Video_Data *priv = data;

   _set_osd_timer(priv, OSD_TIMER);
}

static void
_emotion_playback_finished_cb(void *data, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   Enna_View_Player_Video_Data *priv = data;
   
   evas_object_smart_callback_call(priv->layout, "playback_finished", NULL);
}

static void
_enna_view_del(void *data, Evas *e  EINA_UNUSED, Evas_Object *obj, void *event_info EINA_UNUSED)
{
   Enna_View_Player_Video_Data *priv = data;

   DBG("delete Enna_View_Player_Video_Data object (%p)", obj);

   FREE_NULL_FUNC(evas_object_del, priv->video);
   FREE_NULL_FUNC(evas_object_del, priv->cover);
   FREE_NULL_FUNC(ecore_timer_del, priv->osd_timer);
   FREE_NULL(priv->media);
}

static void
_mouse_move_cb(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
    Enna_View_Player_Video_Data *priv = data;

    _set_osd_timer(priv, OSD_TIMER);
}

static void
_mouse_down_cb(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
    Enna_View_Player_Video_Data *priv = data;

    priv->on_hold = EINA_TRUE;
    _set_osd_timer(priv, OSD_TIMER);
}

static void
_mouse_up_cb(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
    Enna_View_Player_Video_Data *priv = data;

    priv->on_hold = EINA_FALSE;
    _set_osd_timer(priv, OSD_TIMER);
}

static void
_drag_cb(void *data,
         Evas_Object *obj EINA_UNUSED,
         const char *emission,
         const char *source)
{
    Enna_View_Player_Video_Data *priv = data;
    Evas_Object *emotion;
    Evas_Object *edje;
    double v;
    double pos;
    time_t timestamp;
    struct tm *t;
    Eina_Strbuf *str;

    emotion = elm_video_emotion_get(priv->video);

    edje = elm_layout_edje_get(priv->layout);
    edje_object_part_drag_value_get(edje, "time.slider", &v, NULL);

    pos = v *  emotion_object_play_length_get(emotion);

    _update_time_part(priv->layout, "time_current.text", pos);
    _update_time_part(priv->layout, "time_duration.text", emotion_object_play_length_get(emotion));

}


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

Evas_Object *
enna_view_player_video_add(Evas_Object *parent)
{
   Evas_Object *layout;
   Enna_View_Player_Video_Data *priv;
   Evas_Object *emotion;

   priv = calloc(1, sizeof(Enna_View_Player_Video_Data));
   if (!priv)
       return NULL;

   layout = elm_layout_add(parent);
   elm_layout_file_set(layout, enna_config_theme_get(), "activity/layout/player/video");
   priv->layout = layout;

   priv->video = elm_video_add(parent);
   elm_object_part_content_set(layout, "video.swallow", priv->video);
   elm_object_style_set(priv->video, "enna");

   evas_object_data_set(layout, "Enna_View_Player_Video_Data", priv);

   emotion = elm_video_emotion_get(priv->video);
   evas_object_smart_callback_add(emotion, "position_update", _emotion_position_update_cb, priv);
   evas_object_smart_callback_add(emotion, "open_done", _emotion_open_done_cb, priv);
   evas_object_smart_callback_add(emotion, "playback_started", _emotion_playback_started_cb, priv);
   evas_object_smart_callback_add(emotion, "playback_finished", _emotion_playback_finished_cb, priv);
   evas_object_event_callback_add(emotion, EVAS_CALLBACK_MOUSE_MOVE, _mouse_move_cb, priv);
   evas_object_event_callback_add(emotion, EVAS_CALLBACK_MOUSE_DOWN, _mouse_down_cb, priv);
   evas_object_event_callback_add(emotion, EVAS_CALLBACK_MOUSE_UP, _mouse_up_cb, priv);

   elm_layout_signal_callback_add(layout, "drag", "*", _drag_cb, priv);
   elm_layout_signal_callback_add(layout, "drag,stop", "*", _slider_position_update_cb, priv);

   emotion_object_init(emotion, mp_cfg.engine);

   evas_object_event_callback_add(layout, EVAS_CALLBACK_DEL, _enna_view_del, priv);

   return layout;
}

void enna_view_player_video_uri_set(Evas_Object *o, Enna_File *f)
{
    const char *cover;
    const char *title;
    int prefix = 0;
    
   PRIV_GET_OR_RETURN(o, Enna_View_Player_Video_Data, priv);

   priv->media = strdup(f->mrl);

   DBG("Start video player with item: %s", f->mrl);

   if (!strncmp(f->mrl, "file://", 7))
       prefix = 7;
   elm_video_file_set(priv->video, f->mrl + prefix);

   title = enna_file_meta_get(f, "title");
   if (title)
   {
       elm_object_part_text_set(priv->layout, "title.text", title);
       eina_stringshare_del(title);
   }   
   else
   {
       title = ecore_file_file_get(f->mrl);
       elm_object_part_text_set(priv->layout, "title.text", title);
   }

   cover = enna_file_meta_get(f, "cover");
   if (cover)
   {
       priv->cover = elm_icon_add(priv->layout);
       printf("cover : %s\n", enna_file_meta_get(f, "cover"));
       
       elm_image_file_set(priv->cover, cover,  NULL);
       elm_image_preload_disabled_set(priv->cover, EINA_FALSE);
       
       elm_object_part_content_set(priv->layout, "cover.swallow", priv->cover);
       elm_object_signal_emit(priv->layout, "show,cover", "enna"); 
   }
   else
   {
       elm_object_signal_emit(priv->layout, "hide,cover", "enna"); 
   }
}

void enna_view_player_video_play(Evas_Object *o)
{
   PRIV_GET_OR_RETURN(o, Enna_View_Player_Video_Data, priv);

   if (!elm_video_is_playing_get(priv->video))
       elm_video_play(priv->video);
   else
       elm_video_pause(priv->video);
}

void enna_view_player_video_pause(Evas_Object *o)
{
   PRIV_GET_OR_RETURN(o, Enna_View_Player_Video_Data, priv);

   if (!elm_video_is_playing_get(priv->video))
       elm_video_play(priv->video);
   else
       elm_video_pause(priv->video);
}

void enna_view_player_video_stop(Evas_Object *o)
{
   PRIV_GET_OR_RETURN(o, Enna_View_Player_Video_Data, priv);

   elm_video_stop(priv->video);
}

void enna_videoplayer_obj_cfg_register(void)
{
    enna_config_section_parser_register(&cfg_mediaplayer);
}

void enna_view_video_player_show_osd(Evas_Object *o, Eina_Bool show)
{
    PRIV_GET_OR_RETURN(o, Enna_View_Player_Video_Data, priv);

    if (show)
        _set_osd_timer(priv, OSD_TIMER);
    else
        _set_osd_timer(priv, 0);
}

void enna_view_video_player_seek(Evas_Object *o, double time)
{ 
    PRIV_GET_OR_RETURN(o, Enna_View_Player_Video_Data, priv);
    Evas_Object *emotion;
    Evas_Object *edje;
    double v;
    double pos;
    time_t timestamp;
    struct tm *t;
    Eina_Strbuf *str;

    emotion = elm_video_emotion_get(priv->video);

    edje = elm_layout_edje_get(priv->layout);
    edje_object_part_drag_value_get(edje, "time.slider", &v, NULL);

    pos = v *  emotion_object_play_length_get(emotion) + time;

    _update_time_part(priv->layout, "time_current.text", pos);
    _update_time_part(priv->layout, "time_duration.text", emotion_object_play_length_get(emotion));

    v = pos / emotion_object_play_length_get(emotion);

   edje = elm_layout_edje_get(priv->layout);
   edje_object_part_drag_value_set(edje, "time.slider", v, v);

   emotion_object_position_set(emotion, pos);

}
