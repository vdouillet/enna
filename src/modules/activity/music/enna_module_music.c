/* Interface */

#include "enna.h"
#include "smart_player.h"

#define ENNA_MODULE_NAME "music"

static void _create_menu();
static void _create_gui();
static void _create_mediaplayer_gui();
static void _browse(void *data, void *data2);
static void _activate();
static int _eos_cb(void *data, int type, void *event);
static void _next_song(void);
static void _prev_song(void);
static int _show_mediaplayer_cb(void *data);

static void _class_init(int dummy);
static void _class_shutdown(int dummy);
static void _class_show(int dummy);
static void _class_hide(int dummy);
static void _class_event(void *event_info);
static int em_init(Enna_Module *em);
static int em_shutdown(Enna_Module *em);


typedef struct _Enna_Module_Music Enna_Module_Music;

typedef enum _MUSIC_STATE MUSIC_STATE;

enum _MUSIC_STATE
{
    MENU_VIEW,
    BROWSER_VIEW,
    MEDIAPLAYER_VIEW
};

struct _Enna_Module_Music
{
    Evas_Object *o_edje;
    Evas_Object *o_list;
    Evas_Object *o_browser;
    Evas_Object *o_location;
    Evas_Object *o_mediaplayer;
    Ecore_Timer *timer;
    Enna_Module *em;
    MUSIC_STATE state;
    Ecore_Timer *timer_show_mediaplayer;
    Ecore_Event_Handler *eos_event_handler;
};

static Enna_Module_Music *mod;

Enna_Module_Api module_api =
{
    ENNA_MODULE_VERSION,
    ENNA_MODULE_ACTIVITY,
    "activity_music"
};

static Enna_Class_Activity class =
{
    "music",
    1,
    "music",
    NULL,
    "icon/music",
    {
	_class_init,
	_class_shutdown,
	_class_show,
	_class_hide,
	_class_event
    },
    NULL
};

static void
_class_init(int dummy)
{
    _create_gui();
    enna_content_append("music", mod->o_edje);
}

static void
_class_shutdown(int dummy)
{
}

static void
_class_show(int dummy)
{
    edje_object_signal_emit(mod->o_edje, "module,show", "enna");
    switch (mod->state)
    {
    case MENU_VIEW:
	edje_object_signal_emit(mod->o_edje, "content,show", "enna");
	edje_object_signal_emit(mod->o_edje, "mediaplayer,hide", "enna");
	break;
    case MEDIAPLAYER_VIEW:
	edje_object_signal_emit(mod->o_edje, "mediaplayer,show", "enna");
	edje_object_signal_emit(mod->o_edje, "content,hide", "enna");
	break;
    default:
	enna_log(ENNA_MSG_ERROR, ENNA_MODULE_NAME,
	    "Error State Unknown in music module\n");
    }

}

static void
_class_hide(int dummy)
{
    edje_object_signal_emit(mod->o_edje, "module,hide", "enna");
}

static void
_class_event(void *event_info)
{
    Evas_Event_Key_Down *ev = event_info;
    enna_key_t key = enna_get_key(ev);
    enna_log(ENNA_MSG_EVENT, ENNA_MODULE_NAME, "Key pressed music : %s\n",
	ev->key);
    switch (mod->state)
    {
    case MENU_VIEW:
	if (mod->o_mediaplayer)
	{
	    ENNA_TIMER_DEL(mod->timer_show_mediaplayer);
	    mod->timer_show_mediaplayer = ecore_timer_add(10,_show_mediaplayer_cb, NULL);
	}
	switch (key)
	{
	case ENNA_KEY_LEFT:
	case ENNA_KEY_CANCEL:
	    enna_content_hide();
	    enna_mainmenu_show(enna->o_mainmenu);
	    break;
	case ENNA_KEY_RIGHT:
	case ENNA_KEY_OK:
	case ENNA_KEY_SPACE:
	    _browse(enna_list_selected_data_get(mod->o_list), NULL);
	    break;
	default:
	    enna_list_event_key_down(mod->o_list, event_info);
	}
	break;
    case BROWSER_VIEW:
	if (mod->o_mediaplayer)
	{
	    ENNA_TIMER_DEL(mod->timer_show_mediaplayer);
	    mod->timer_show_mediaplayer = ecore_timer_add(10,_show_mediaplayer_cb, NULL);
	}
	enna_browser_event_feed(mod->o_browser, event_info);
	break;
    case MEDIAPLAYER_VIEW:
	switch (key)
	{
	case ENNA_KEY_OK:
	case ENNA_KEY_SPACE:
	    enna_mediaplayer_play();
	    break;
	case ENNA_KEY_RIGHT:
	    _next_song();
	    break;
	case ENNA_KEY_LEFT:
	    _prev_song();
	    break;
	case ENNA_KEY_CANCEL:
	    ENNA_TIMER_DEL(mod->timer_show_mediaplayer);
	    mod->timer_show_mediaplayer = ecore_timer_add(10,_show_mediaplayer_cb, NULL);
	    enna_log(ENNA_MSG_INFO, ENNA_MODULE_NAME, "Add timer 10s");
	    edje_object_signal_emit(mod->o_edje, "mediaplayer,hide","enna");
	    edje_object_signal_emit(mod->o_edje, "content,show", "enna");
	    if (mod->o_browser)
		mod->state = BROWSER_VIEW;
	    else
		mod->state = MENU_VIEW;
	    break;
	default:
	    break;
	}
	break;
    default:
	break;
    }
}

static int
_show_mediaplayer_cb(void *data)
{
    if (mod->o_mediaplayer)
    {
        mod->state = MEDIAPLAYER_VIEW;
        edje_object_signal_emit(mod->o_edje, "mediaplayer,show", "enna");
        edje_object_signal_emit(mod->o_edje, "content,hide", "enna");
        ENNA_TIMER_DEL(mod->timer_show_mediaplayer);
    }
    return 0;
}


static void
_browser_root_cb (void *data, Evas_Object *obj, void *event_info)
{
    printf("Root Selected\n");
    mod->state = MENU_VIEW;
    ENNA_OBJECT_DEL(mod->o_browser);
    mod->o_browser = NULL;
    _create_menu();
}

static void
_browser_selected_cb (void *data, Evas_Object *obj, void *event_info)
{
    int i = 0;
    Enna_Vfs_File *f;
    Eina_List *l;
    Browser_Selected_File_Data *ev = event_info;

    if (!ev || !ev->file) return;

    if (ev->file->is_directory)
    {
	enna_log(ENNA_MSG_EVENT, ENNA_MODULE_NAME, "Directory Selected %s\n", ev->file->uri);
	enna_location_append(mod->o_location, ev->file->label, NULL, NULL, NULL, NULL);
    }
    else
    {
	enna_log(ENNA_MSG_EVENT, ENNA_MODULE_NAME , "File Selected %s\n", ev->file->uri);
	/* File selected, create mediaplayer */
	EINA_LIST_FOREACH(ev->files, l, f)
	{
	    if (!f->is_directory)
	    {
		enna_mediaplayer_uri_append(f->uri, f->label);
		if (!strcmp(f->uri, ev->file->uri))
		{
		    enna_mediaplayer_select_nth(i);
	 	    enna_mediaplayer_play();
		}
		i++;
	    }
	}
	_create_mediaplayer_gui();
    }
    free(ev);
}

static void
_browse(void *data, void *data2)
{
    Enna_Class_Vfs *vfs = data;

    mod->o_browser = enna_browser_add(mod->em->evas);
    evas_object_smart_callback_add(mod->o_browser, "root", _browser_root_cb, NULL);
    evas_object_smart_callback_add(mod->o_browser, "selected", _browser_selected_cb, NULL);
    mod->state = BROWSER_VIEW;

    evas_object_show(mod->o_browser);
    edje_object_part_swallow(mod->o_edje, "enna.swallow.browser", mod->o_browser);
    enna_browser_root_set(mod->o_browser, vfs);
    evas_object_del(mod->o_list);
    mod->o_list = NULL;
}



static int
_update_position_timer(void *data)
{
    double pos;
    double length;

    length = enna_mediaplayer_length_get();
    pos = enna_mediaplayer_position_get();
    enna_smart_player_position_set(mod->o_mediaplayer, pos, length);
    return 1;
}

static void
_next_song()
{
    Enna_Metadata *metadata;
    if (!enna_mediaplayer_next())
    {
        metadata = enna_mediaplayer_metadata_get();
        enna_metadata_grab (metadata, ENNA_GRABBER_CAP_AUDIO);
        enna_smart_player_metadata_set(mod->o_mediaplayer, metadata);
    }
}

static void
_prev_song()
{
    Enna_Metadata *metadata;
    if (!enna_mediaplayer_prev())
    {
        metadata = enna_mediaplayer_metadata_get();
        enna_metadata_grab (metadata, ENNA_GRABBER_CAP_AUDIO);
        enna_smart_player_metadata_set(mod->o_mediaplayer, metadata);
    }
}

static int
_eos_cb(void *data, int type, void *event)
{
    /* EOS received, update metadata */
    _next_song();
    return 1;
}

static void
_create_mediaplayer_gui()
{
    Evas_Object *o;
    Enna_Metadata *metadata;

    mod->state = MEDIAPLAYER_VIEW;

    if (mod->o_mediaplayer)
    {
        ENNA_TIMER_DEL(mod->timer);
        ecore_event_handler_del(mod->eos_event_handler);
        evas_object_del(mod->o_mediaplayer);
    }

    mod->eos_event_handler = ecore_event_handler_add(
            ENNA_EVENT_MEDIAPLAYER_EOS, _eos_cb, NULL);

    o = enna_smart_player_add(mod->em->evas);
    edje_object_part_swallow(mod->o_edje, "enna.swallow.mediaplayer", o);
    evas_object_show(o);

    metadata = enna_mediaplayer_metadata_get();
    enna_metadata_grab (metadata, ENNA_GRABBER_CAP_AUDIO);
    enna_smart_player_metadata_set(o, metadata);

    mod->o_mediaplayer = o;
    mod->timer = ecore_timer_add(1, _update_position_timer, NULL);

    edje_object_signal_emit(mod->o_edje, "mediaplayer,show", "enna");
    edje_object_signal_emit(mod->o_edje, "content,hide", "enna");
}

static void
_create_menu()
{
    Evas_Object *o;
    Eina_List *l, *categories;

    /* Create List */
    o = enna_list_add(mod->em->evas);
    edje_object_signal_emit(mod->o_edje, "list,right,now", "enna");

    categories = enna_vfs_get(ENNA_CAPS_MUSIC);
    for (l = categories; l; l = l->next)
    {
        Evas_Object *item;
        Enna_Class_Vfs *cat;
	Evas_Object *icon;

        cat = l->data;
        icon = edje_object_add(mod->em->evas);
        edje_object_file_set(icon, enna_config_theme_get(), cat->icon);
        item = enna_listitem_add(mod->em->evas);
        enna_listitem_create_simple(item, icon, cat->label);
        enna_list_append(o, item, _browse, NULL, cat, NULL);
    }

    enna_list_selected_set(o, 0);
    mod->o_list = o;
    edje_object_part_swallow(mod->o_edje, "enna.swallow.list", o);
    edje_object_signal_emit(mod->o_edje, "list,default", "enna");
}

static void
_create_gui()
{
    Evas_Object *o;
    Evas_Object *icon;

    /* Set default state */
    mod->state = MENU_VIEW;

    /* Create main edje object */
    o = edje_object_add(mod->em->evas);
    edje_object_file_set(o, enna_config_theme_get(), "module/music");
    mod->o_edje = o;

    _create_menu();

    /* Create Location bar */
    o = enna_location_add(mod->em->evas);
    edje_object_part_swallow(mod->o_edje, "enna.swallow.location", o);

    icon = edje_object_add(mod->em->evas);
    edje_object_file_set(icon, enna_config_theme_get(), "icon/music_mini");
    enna_location_append(o, "Music", icon, NULL, NULL, NULL);
    mod->o_location = o;
}

/* Module interface */

static int
em_init(Enna_Module *em)
{
    mod = calloc(1, sizeof(Enna_Module_Music));
    mod->em = em;
    em->mod = mod;

    enna_activity_add(&class);

    return 1;
}

static int
em_shutdown(Enna_Module *em)
{
    ENNA_OBJECT_DEL(mod->o_edje);
    ENNA_OBJECT_DEL(mod->o_list);
    ENNA_OBJECT_DEL(mod->o_browser);
    ENNA_OBJECT_DEL(mod->o_location);
    ENNA_TIMER_DEL(mod->timer_show_mediaplayer);
    ENNA_TIMER_DEL(mod->timer);
    ENNA_OBJECT_DEL(mod->o_mediaplayer);
    free(mod);
    return 1;
}

void module_init(Enna_Module *em)
{
    if (!em)
        return;

    if (!em_init(em))
        return;
}

void module_shutdown(Enna_Module *em)
{
    em_shutdown(em);
}
